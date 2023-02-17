
#include "av_samples_buffer.h"

#include <utility>
#include <ios>
#include <sstream>
#include <iostream>

#include <cpaf_libs/math/base/utils.hpp>
#include <cpaf_libs/time/cpaf_time.h>

using namespace std;
using namespace cpaf;
using namespace std::chrono;
using namespace std::chrono_literals;

namespace cpaf::video {

std::array<char, 0xFFFF> null_audio_data = []() constexpr -> auto { auto a = std::array<char, 0xFFFF>(); a.fill(0u); return a;}();


av_samples_buffer av_samples_buffer::create_flush_sample()
{
    av_samples_buffer buf;
    buf.is_flush_sample_set();
    return buf;
}

// -------------------------
// --- Constructors etc. ---
// -------------------------



av_samples_buffer::~av_samples_buffer()

{

    if (resampled_data_) {

        av_freep(&resampled_data_[0]);

    }

    av_freep(&resampled_data_);

}



void av_samples_buffer::swap(av_samples_buffer& src) noexcept

{

    std::swap(presentation_time_, src.presentation_time_);

    std::swap(resampled_data_, src.resampled_data_);

    std::swap(buffer_size_, src.buffer_size_);

    std::swap(buffer_pos_, src.buffer_pos_);

    std::swap(bytes_per_microsecond_, src.bytes_per_microsecond_);

    std::swap(bytes_per_sample_, src.bytes_per_sample_);
    std::swap(pipeline_control_, src.pipeline_control_);
    std::swap(pipeline_index_, src.pipeline_index_);
    std::swap(is_flush_sample_, src.is_flush_sample_);
}


int av_samples_buffer::samples_alloc_array_and_samples(int* linesize, int nb_channels, int nb_samples, AVSampleFormat sample_format, int align)
{
    if (resampled_data_ && resampled_data_[0]) {
        av_freep(resampled_data_[0]);
    }
    const auto alloc_ret_val = av_samples_alloc_array_and_samples(
                &resampled_data_,
                linesize,
                nb_channels,
                nb_samples,
                sample_format,
                align
                );
    return alloc_ret_val;
}

int av_samples_buffer::samples_alloc(int* linesize, int nb_channels, int nb_samples, AVSampleFormat sample_format, int align)
{
    if (resampled_data_ && resampled_data_[0]) {
        av_freep(resampled_data_[0]);
    }
    const auto alloc_ret_val = av_samples_alloc(
                resampled_data_,
                linesize,
                nb_channels,
                nb_samples,
                sample_format,
                align
                );
    return alloc_ret_val;
}

int32_t av_samples_buffer::skip_samples(int32_t bytes_to_skip)
{
    bytes_to_skip = cpaf::math::round_down(bytes_to_skip, bytes_per_sample());
    const auto new_buf_pos = buffer_pos_ + bytes_to_skip;
    const bool ok = new_buf_pos <= buffer_size_;
    if (!ok) {
        bytes_to_skip = cpaf::math::round_down(size(), bytes_per_sample());
    }
    buffer_pos_ += bytes_to_skip;
    return bytes_to_skip;
}

int32_t av_samples_buffer::skip_samples(const std::chrono::microseconds& us_to_skip)
{
    const int32_t bytes_to_skip = us_to_skip.count() * bytes_per_microsecond();
    return skip_samples(bytes_to_skip);
}


int32_t av_samples_buffer::add_samples (int32_t bytes_to_add)
{
    bytes_to_add = cpaf::math::round_down(bytes_to_add, bytes_per_sample());
    const auto new_buf_pos = buffer_pos_ - bytes_to_add;
    const bool ok = new_buf_pos >= 0;
    if (!ok) {
        bytes_to_add = buffer_pos_;
    }
    buffer_pos_ -= bytes_to_add;
    return bytes_to_add;
}

int32_t av_samples_buffer::add_samples (const std::chrono::microseconds& us_to_add)
{
    const int32_t bytes_to_add = us_to_add.count() * bytes_per_microsecond();
    return add_samples(bytes_to_add);
}

int32_t av_samples_buffer::copy_out(uint8_t* dest_buf)
{
    const uint8_t* src_ptr = out_buffer();
    const auto bytes_to_copy = size();
    memcpy(dest_buf, src_ptr, static_cast<uint32_t>(bytes_to_copy));
    buffer_pos_ = buffer_size_;
    return bytes_to_copy;
}

/** Copy at most @p max_bytes_to_copy from the samples buffer to the destination
    buffer.
    @return The number of bytes actually copied. */
int32_t av_samples_buffer::copy_out(uint8_t* dest_buf, int32_t max_bytes_to_copy)
{
    if (max_bytes_to_copy >= size()) {
        return copy_out(dest_buf);
    }
    else {
        const uint8_t* src_ptr = out_buffer();
        memcpy(dest_buf, src_ptr, static_cast<uint32_t>(max_bytes_to_copy));
        buffer_pos_ += max_bytes_to_copy;
        return max_bytes_to_copy;
    }
}

int32_t av_samples_buffer::copy_out(uint8_t* dest_buf, const std::chrono::microseconds& max_duration_to_copy)
{
    const int32_t max_bytes_to_copy = max_duration_to_copy.count() * bytes_per_microsecond();
    const int32_t bytes_copied = copy_out(dest_buf, max_bytes_to_copy);
    return bytes_copied;
}

microseconds av_samples_buffer::bytes_to_duration(int32_t bytes) const
{
    const auto bytes_per_us = bytes_per_microsecond();
    const auto remaining_us =  std::chrono::microseconds(static_cast<int64_t>(bytes / bytes_per_us));
    return remaining_us;
}

// -----------------------
// --- Debug functions ---
// -----------------------



std::string av_samples_buffer::dbg_string_short() const
{
    std::stringstream ss;
//    ss << "[" << create_number() << "]"
//       << " media_type: '" << to_string(media_type_get())
//       << "', pts_stream_base: " << pts_stream_base()
//       ;
    return ss.str();
}

// ---------------------
// --- Samples queue ---
// ---------------------

av_samples_queue_t::av_samples_queue_t(size_t queue_size)
    : fifo_(queue_size)
{

}

av_samples_buffer& av_samples_queue_t::front ()
{
    return fifo_.front();
}

const av_samples_buffer& av_samples_queue_t::front () const
{
    return fifo_.front();
}

void av_samples_queue_t::pop()
{
    fifo_.pop();
}

av_samples_buffer av_samples_queue_t::pop_front ()
{
    if (empty()) {
        return av_samples_buffer();
    }

    const std::lock_guard<std::mutex> lock(fifo_mutex_);
    if (empty()) {
        return av_samples_buffer();
    }
    auto buffer = std::move(front());
    pop();
    return buffer;
}

bool av_samples_queue_t::push (av_samples_buffer buffer)
{
    return fifo_.push(std::move(buffer));
}

void av_samples_queue_t::flush ()
{
    fifo_.flush();
}


/** Copy a number of bytes from the samples queue to the destination address.
    @return The number of bytes actually copied */
int32_t copy_audio_samples(
        uint8_t* dest_buf,
        av_samples_queue_t& samples_queue,
        int32_t bytes_to_copy,
        const samples_queue_callback_t& queue_pop_callback)
{
    auto bytes_left = bytes_to_copy;
    while (!samples_queue.empty() && (bytes_left > 0)) {
        if (queue_pop_callback) {
            queue_pop_callback(samples_queue.front());
        }
        av_samples_buffer& front = samples_queue.front();
        const auto bytes_copied = front.copy_out(dest_buf, bytes_left);
        dest_buf += bytes_copied;
        bytes_left -= bytes_copied;
        if (front.empty()) {
            samples_queue.pop();
        }
    }

    const auto bytes_actually_copied = bytes_to_copy - bytes_left;
    return bytes_actually_copied;
}

bool more_than_a_buffer_behind(av_samples_queue_t& samples_queue, const std::chrono::microseconds& sync_to_media_time)
{
    return (sync_to_media_time - samples_queue.front().presentation_time()) > samples_queue.front().duration();
}

//bool more_than_a_buffer_ahead(av_samples_queue_t& samples_queue, const std::chrono::microseconds& sync_to_media_time)
//{
//    return (sync_to_media_time - samples_queue.front().presentation_time()) < -samples_queue.front().duration();
//}

void skip_audio_samples_helper(av_samples_queue_t& samples_queue, const std::chrono::microseconds& sync_to_media_time)
{
    while (!samples_queue.empty()
           && ( (sync_to_media_time - samples_queue.front().presentation_time()) > samples_queue.front().duration()) ) {
        samples_queue.pop();
    }
}


/** Copy bytes from samples queue while trying to synch with 'current media time'.
    @return The number of bytes actually copied */
int32_t copy_audio_samples(
        uint8_t* dest_buf,
        av_samples_queue_t& samples_queue,
        int32_t bytes_to_copy,
        const std::chrono::microseconds& sync_to_media_time,
        const std::chrono::microseconds& sync_ok_interval,
        const samples_queue_callback_t& queue_pop_callback)
{
    if (samples_queue.empty()) return 0;
    auto sync_err_difference      = sync_to_media_time - samples_queue.front().presentation_time();
    const auto sync_err_difference_abs  = time::abs(sync_err_difference);
    if (sync_err_difference_abs <= sync_ok_interval) {
        return copy_audio_samples(dest_buf, samples_queue, bytes_to_copy, queue_pop_callback);
    }


    if ( sync_err_difference > 0ms ) {
        if (more_than_a_buffer_behind(samples_queue, sync_to_media_time)) {
            skip_audio_samples_helper(samples_queue, sync_to_media_time);
        }
        if (samples_queue.empty()) return 0;

        sync_err_difference = sync_to_media_time - samples_queue.front().presentation_time();
        samples_queue.front().skip_samples(sync_err_difference);
        return copy_audio_samples(dest_buf, samples_queue, bytes_to_copy, queue_pop_callback);
    }
    else if ( sync_err_difference < 0ms ) {
        auto bytes_copied = copy_audio_samples(dest_buf, samples_queue, bytes_to_copy, queue_pop_callback);

        if (samples_queue.empty()) return bytes_copied;
        samples_queue.front().add_samples(5ms);

        return bytes_copied;
    }

    return copy_audio_samples(dest_buf, samples_queue, bytes_to_copy, queue_pop_callback);
}


} //END namespace cpaf::video

//----- SAVE ME TEMPORARY ---
//        std::cerr << "BEFORE SYNC SAMPLES sync_to_media_time: " << duration_cast<milliseconds>(sync_to_media_time).count() << " ms"
//                  << " queue front time: " << samples_queue.front().presentation_time_ms().count() << " ms"
//                  << " sync_err_difference: " << duration_cast<milliseconds>(sync_err_difference).count() << " ms"
//                  << "\n";
//        sync_err_difference = next_sync_to_media_time - samples_queue.front().presentation_time();
//        std::cerr << "FIXMENM AUDIO SYNC AHEAD BEFORE:"
//                  << ", next_sync_to_media_time: " << duration_cast<milliseconds>(next_sync_to_media_time).count() << " ms"
//                  << ", queue front time: " << samples_queue.front().presentation_time_ms().count() << " ms"
//                  << ", front duration: " << samples_queue.front().duration_ms().count() << " ms"
//                  << ", sync_err_difference: " << duration_cast<milliseconds>(sync_err_difference).count() << " ms"
//                  << "\n";
//        samples_queue.front().add_samples(100us);
//        sync_err_difference = next_sync_to_media_time - samples_queue.front().presentation_time();
//        std::cerr << "FIXMENM AUDIO SYNC AHEAD AFTER:"
//                  << ", next_sync_to_media_time: " << duration_cast<milliseconds>(next_sync_to_media_time).count() << " ms"
//                  << ", queue front time: " << samples_queue.front().presentation_time_ms().count() << " ms"
//                  << ", front duration: " << samples_queue.front().duration_ms().count() << " ms"
//                  << ", sync_err_difference: " << duration_cast<milliseconds>(sync_err_difference).count() << " ms"
//                  << "\n";
