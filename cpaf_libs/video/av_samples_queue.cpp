
#include "av_samples_queue.h"

#include <utility>
#include <ios>
#include <sstream>
#include <iostream>
#include <cpaf_libs/time/cpaf_time.h>

using namespace std;
using namespace cpaf;
using namespace std::chrono;
using namespace std::chrono_literals;

namespace cpaf::video {

// ---------------------
// --- Samples queue ---
// ---------------------

av_samples_queue::av_samples_queue(size_t queue_size)
    : fifo_(queue_size)
{

}

av_samples_buffer& av_samples_queue::front ()
{
    return fifo_.front();
}

const av_samples_buffer& av_samples_queue::front () const
{
    return fifo_.front();
}

void av_samples_queue::pop()
{
    fifo_.pop();
}

av_samples_buffer av_samples_queue::pop_front ()
{
    if (fifo_.empty()) {
        return av_samples_buffer();
    }

    std::lock_guard<std::mutex> lock(fifo_mutex_);
    if (fifo_.empty()) {
        return av_samples_buffer();
    }
    auto buffer = std::move(fifo_.front());
    fifo_.pop();
    return buffer;
}

bool av_samples_queue::push (av_samples_buffer buffer)
{
    std::lock_guard<std::mutex> lock(fifo_mutex_);
    return fifo_.push(std::move(buffer));
}

void av_samples_queue::flush ()
{
    std::lock_guard<std::mutex> lock(fifo_mutex_);
    fifo_.flush();
}


/** Copy a number of bytes from the samples queue to the destination address.
    @return The number of bytes actually copied */
int32_t av_samples_queue::copy_audio_samples(
        uint8_t* dest_buf,
        int32_t bytes_to_copy,
        const samples_queue_callback_t& queue_pop_callback)
{
    auto bytes_left = bytes_to_copy;
    while (!empty() && (bytes_left > 0)) {
        av_samples_buffer& frnt = front();
        if (queue_pop_callback) {
            queue_pop_callback(frnt);
        }
        const auto bytes_copied = frnt.copy_out(dest_buf, bytes_left);
        dest_buf += bytes_copied;
        bytes_left -= bytes_copied;
        if (frnt.empty()) {
            pop();
        }
    }

    const auto bytes_actually_copied = bytes_to_copy - bytes_left;
    return bytes_actually_copied;
}

bool more_than_a_buffer_behind(av_samples_queue& samples_queue, const std::chrono::microseconds& sync_to_media_time)
{
    return (sync_to_media_time - samples_queue.front().presentation_time()) > samples_queue.front().duration();
}

//bool more_than_a_buffer_ahead(av_samples_queue_t& samples_queue, const std::chrono::microseconds& sync_to_media_time)
//{
//    return (sync_to_media_time - samples_queue.front().presentation_time()) < -samples_queue.front().duration();
//}

void skip_audio_samples_helper(av_samples_queue& samples_queue, const std::chrono::microseconds& sync_to_media_time)
{
    while (!samples_queue.empty()
           && ( (sync_to_media_time - samples_queue.front().presentation_time()) > samples_queue.front().duration()) ) {
        samples_queue.pop();
    }
}


/** Copy bytes from samples queue while trying to synch with 'current media time'.
    @return The number of bytes actually copied */
int32_t av_samples_queue::copy_audio_samples(
        uint8_t* dest_buf,
        int32_t bytes_to_copy,
        const std::chrono::microseconds& sync_to_media_time,
        const std::chrono::microseconds& sync_ok_interval,
        const samples_queue_callback_t& queue_pop_callback)
{
    if (empty()) return 0;
    auto sync_err_difference      = sync_to_media_time - front().presentation_time();
    const auto sync_err_difference_abs  = cpaf::time::abs(sync_err_difference);
    if (sync_err_difference_abs <= sync_ok_interval) {
        return copy_audio_samples(dest_buf, bytes_to_copy, queue_pop_callback);
    }


    if ( sync_err_difference > 0ms ) {
        if (more_than_a_buffer_behind(sync_to_media_time)) {
            skip_audio_samples_helper(sync_to_media_time);
        }
        if (empty()) return 0;

        sync_err_difference = sync_to_media_time - front().presentation_time();
        front().skip_samples(sync_err_difference);
        return copy_audio_samples(dest_buf, bytes_to_copy, queue_pop_callback);
    }
    else if ( sync_err_difference < 0ms ) {
        auto bytes_copied = copy_audio_samples(dest_buf, bytes_to_copy, queue_pop_callback);

        if (empty()) return bytes_copied;
        front().add_samples(5ms);

        return bytes_copied;
    }

    return copy_audio_samples(dest_buf, bytes_to_copy, queue_pop_callback);
}


} //END namespace cpaf::video
