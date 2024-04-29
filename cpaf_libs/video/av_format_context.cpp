
#include "av_format_context.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/avstring.h>
#include <libavutil/time.h>
#include <libavutil/opt.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}


#include <utility>
#include <ios>
#include <thread>
#include <algorithm>
#include <fmt/format.h>
#include <cpaf_libs/locale/language_codes.h>
#include <cpaf_libs/time/cpaf_time.h>
#include <cpaf_libs/video/io/custom_io_base.h>

using namespace std;
using namespace std::chrono;

namespace cpaf::video {

// -------------------------
// --- Constructors etc. ---
// -------------------------

av_format_context::av_format_context(get_torrents_fn get_torrents_function,
                                     std::atomic<stream_state_t>* stream_state_ptr)
    : get_torrents_function_(get_torrents_function),
      global_stream_state_ptr_(stream_state_ptr),
      custom_io_ptr_(nullptr)
{
}

av_format_context::~av_format_context()
{
    close();
}

bool av_format_context::open(const std::string& resource_path)
{

    resource_path_ = resource_path;

    const string protocol_name = protocol_from_uri(resource_path);
    custom_io_ptr_ = custom_io_base::create(protocol_name, get_torrents_function_);
    if (custom_io_ptr_) {
        if (!custom_io_ptr_->open(resource_path)) {
            return false;
        }
        if (!(ff_format_context_ = avformat_alloc_context())) {
            return false;
        }

        if (!custom_io_ptr_->init(ff_format_context_)) {
            return false;
        }
        if ( avformat_open_input(&ff_format_context_, nullptr, nullptr, nullptr) != 0) {
            return false;
        }
    }
    else if ( avformat_open_input(&ff_format_context_, resource_path_.c_str(), nullptr, nullptr) != 0) {
        return false; // Couldn't open resource/file
    }

    if ( avformat_find_stream_info(ff_format_context_, nullptr) <0 ) {
        return false; // Couldn't find stream information
    }
    read_codec_contexts();
    set_default_selected_streams();
    return true;
}

void av_format_context::close()
{
    const std::scoped_lock<std::mutex> lock(packet_queues_mutex_);
    for (media_type_t mt = media_type_t::video; mt != media_type_t::SIZE; ++mt) {
        auto media_index = to_size_t(mt);
        selected_stream_per_media_type_[media_index] = illegal_stream_index();
        stream_indices_per_media_type_[media_index].clear();
        packet_queue_per_media_type_[media_index].flush();
    }
    resource_path_.clear();

    primary_stream_index_ = no_stream_index;
    if (ff_format_context_) {
        avformat_close_input(&ff_format_context_);
    }
    ff_format_context_ = nullptr;
    stream_state() = stream_state_t::inactive;
    custom_io_ptr_.reset();
}

void av_format_context::selected_media_index_set(media_type_t mt, size_t stream_index)
{
    selected_stream_per_media_type_[to_size_t(mt)] = stream_index;
}

void av_format_context::primary_index_set(size_t stream_index)
{
    primary_stream_index_ = stream_index;
}

void av_format_context::set_default_selected_streams()
{
    for (media_type_t mt = media_type_t::video; mt != media_type_t::SIZE; ++mt) {
        selected_stream_per_media_type_[to_size_t(mt)] = first_stream_index(mt);
    }
    set_default_primary_stream();
}

void av_format_context::set_default_primary_stream()
{
    const size_t video_idx = selected_stream_per_media_type_[to_size_t(media_type_t::video)];
    const size_t audio_idx = selected_stream_per_media_type_[to_size_t(media_type_t::audio)];
    const size_t subtl_idx = selected_stream_per_media_type_[to_size_t(media_type_t::subtitle)];
    if      (video_idx != no_stream_index)  primary_stream_index_ = video_idx;
    else if (audio_idx != no_stream_index)  primary_stream_index_ = audio_idx;
    else if (subtl_idx != no_stream_index)  primary_stream_index_ = subtl_idx;
    else                                    primary_stream_index_ = 0;
}

// ----------------------
// --- Info functions ---
// ----------------------

size_t av_format_context::streams_count() const
{
    if (ff_format_context_) {
        return static_cast<size_t>(ff_format_context_->nb_streams);
    }
    return 0;
}

size_t av_format_context::streams_count(media_type_t mt) const
{
    if (ff_format_context_) {
        return stream_indices_per_media_type_[to_size_t(mt)].size();;
    }
    return 0;
}

size_t av_format_context::first_stream_index(media_type_t mt) const
{
    if (!media_type_valid(mt)) return no_stream_index;
    const auto& vec = stream_indices_per_media_type_[to_size_t(mt)];
    if (vec.empty()) return no_stream_index;
    return vec.front();
}

AVCodecID av_format_context::codec_id(size_t stream_index) const
{
    if (stream_index >= streams_count()) { return AV_CODEC_ID_NONE;   }
    return ff_format_context_->streams[stream_index]->codecpar->codec_id;
}

media_type_t av_format_context::stream_media_type(size_t stream_index) const
{
    if (stream_index >= streams_count()) {
        return media_type_t::SIZE;
    }

    const AVStream* ff_stream = ff_format_context_->streams[stream_index];
    const AVMediaType ff_media_type = ff_stream->codecpar->codec_type;
    const auto mt = static_cast<media_type_t>(ff_media_type);
    return mt;
}

std::set<media_type_t> av_format_context::set_of_each_media_type(const std::set<media_type_t>& types_to_skip) const
{
    std::set<media_type_t> all_media_types;
    for (media_type_t mt = media_type_t::video; mt != media_type_t::SIZE; ++mt) {
        if (types_to_skip.contains(mt)) continue;
        if (media_type_to_index(mt) != no_stream_index) {
            all_media_types.insert(mt);
        }
    }
    return all_media_types;
}

bool av_format_context::is_torrent() const
{
    if (custom_io_ptr_) {
        return custom_io_ptr_->protocol_name() == "magnet";
    }
    return false;
}


bool av_format_context::seek_time_pos(std::chrono::microseconds stream_pos)
{
    const size_t stream_index = primary_index();
    const av_base_duration av_seek_pos = std::chrono::duration_cast<av_base_duration>(stream_pos);

    const AVRational stream_time_base = ff_format_context_->streams[stream_index]->time_base;
    const int64_t seek_target = av_rescale_q(av_seek_pos.count(), (AV_TIME_BASE_Q), stream_time_base);

    const int32_t seek_flags = 0;

    const auto retval = av_seek_frame(ff_format_context_, stream_index, seek_target, seek_flags);
    if(retval < 0) {
        return false;
    }

    return true;
}


/**
 *
 *  https://libav-user.ffmpeg.narkive.com/IK2hsU44/once-again-about-seeking-in-video-file
 *
    seek_target_video = av_rescale_q(seek_target_video, AV_TIME_BASE_Q, pFormatCtx->streams[video_stream_index]->time_base);
    seek_target_audio = av_rescale_q(seek_target_audio, AV_TIME_BASE_Q, pFormatCtx->streams[audio_stream_index]->time_base);

 *  */
bool av_format_context::seek_time_pos(std::chrono::microseconds stream_pos, seek_dir dir)
{
    return seek_time_pos(primary_index(), stream_pos, dir);
}

bool av_format_context::seek_time_pos(size_t stream_index, std::chrono::microseconds stream_pos, seek_dir dir)
{
    const av_base_duration av_seek_pos = std::chrono::duration_cast<av_base_duration>(stream_pos);

    const AVRational stream_time_base = ff_format_context_->streams[stream_index]->time_base;
    const int64_t seek_target = av_rescale_q(av_seek_pos.count(), AV_TIME_BASE_Q, stream_time_base);

    int32_t seek_flags = 0;
    if (dir == seek_dir::backward) seek_flags |= AVSEEK_FLAG_BACKWARD;

    const auto retval = av_seek_frame(ff_format_context_, stream_index, seek_target, seek_flags);
    if(retval < 0) {
        return false;
    }

    return true;
}

bool av_format_context::seek_time_pos(media_type_t mt, std::chrono::microseconds stream_pos, seek_dir dir)
{
    return seek_time_pos(media_type_to_index(mt), stream_pos, dir);
}

// ---------------------
// --- XXX Functions ---
// ---------------------

av_codec_parameters av_format_context::codec_parameters(size_t stream_index) const
{
    if (stream_index >= streams_count()) { return av_codec_parameters(nullptr);   }
    return av_codec_parameters(ff_format_context_->streams[stream_index]->codecpar);
}

av_codec_context av_format_context::codec_context(size_t stream_index) const
{
    if (stream_index == no_stream_index) {
        return av_codec_context(nullptr, stream_index, AVRational());   // Invalid av_codec_context!
    }
    const AVRational strm_time_base = stream_time_base(stream_index);
    if (stream_index >= streams_count()) { return av_codec_context(nullptr, stream_index, strm_time_base);   }
    // retrieve audio codec
    const AVCodec* ff_codec = ff_find_decoder(stream_index);
    if (ff_codec == nullptr) {
        printf("Unsupported codec!\n");
        return av_codec_context(nullptr, stream_index, strm_time_base); // Invalid av_codec_context!
    }

    // retrieve codec context
    AVCodecParameters* ff_codec_params = ff_codec_parameters(stream_index);
    AVCodecContext* ff_codec_context = avcodec_alloc_context3(ff_codec);
    int ret = avcodec_parameters_to_context(ff_codec_context, ff_codec_params);
    if (ret != 0) {
        printf("Could not copy codec context.\n");
        return av_codec_context(nullptr, stream_index, strm_time_base);
    }

    // initialize the AVCodecContext to use the given AVCodec
    ret = avcodec_open2(ff_codec_context, ff_codec, nullptr);
    if (ret < 0) {
        printf("Could not open codec.\n");
        return av_codec_context(nullptr, stream_index, strm_time_base); // Invalid av_codec_context!
    }

    av_codec_context codec_ctx = av_codec_context(ff_codec_context, stream_index, strm_time_base);

//    return av_codec_context(ff_codec_context, stream_index, strm_time_base);
    codec_ctx.media_type_set(stream_media_type(stream_index));
    return codec_ctx;
}

av_codec_context av_format_context::codec_context(media_type_t selected_media) const
{
    const size_t stream_index = media_type_to_index(selected_media);
    return codec_context(stream_index);
}


// ------------------------
// --- Packet Functions ---
// ------------------------

/** Read a single packet from the stream
*/
av_packet av_format_context::read_packet() const
{
    av_packet packet = av_packet::alloc();
    int res = av_read_frame(ff_format_context_, packet.ff_packet_);
    if (res != 0) {
        packet.destroy(); // Ensures packet.valid() becomes false!
        return packet;
    }
    const auto stream_index = packet.stream_index();
    packet.media_type_set(stream_media_type(stream_index));

    /// Set a presentation time for the packet
    /// auto pts = time_from_stream_time(stream_index, packet.pts_stream_base());
    /// if (!packet.pts_valid()) {
    ///     pts = time_from_stream_time(stream_index, packet.dts_stream_base());    // Fall back to decode timestamp if pts is invalid!
    /// }
    /// packet.presentation_time_set(pts);

    set_packet_presentation_time(stream_index, packet);

    // if (packet.media_type_get() == media_type_t::video) {
    //     if (packet.ff_packet_) {
    //         std::cerr   << "FIXMENM packet media type:" << to_string(packet.media_type_get())
    //                     << " ps: " << cpaf::time::format_h_m_s_ms(packet.presentation_time())
    //                     << " pts_fixmenm: " << cpaf::time::format_h_m_s_ms(pts_fixmenm)
    //                     << " pts: " << packet.ff_packet_->pts
    //                     << " dts: " << packet.ff_packet_->dts
    //                     << "\n";
    //     }
    // }

    return packet;
}

/**
Read packet matching one of the selected streams.
Keep reading until a packet matching one of the selected streams are found or
end of stream is reached.
@return The packet read, which can be invalid if no more packets maching selected streams are left in
the stream.
*/
av_packet av_format_context::read_packet_selected() const
{
    av_packet packet = read_packet();

    while (packet.valid() && !packet_in_selected_stream(packet)) {
        packet = read_packet();
    }
    return packet;
}

/**
Test if packet belongs to one of the selected streams.
*/
bool av_format_context::packet_in_selected_stream(const av_packet& packet) const
{
    const media_type_t mt = packet.media_type_get();
    const size_t packet_index = packet.stream_index();
    const size_t media_type_int = to_size_t(mt);
    const size_t media_type_index = selected_stream_per_media_type_[media_type_int];
    const bool in_selected_stream = media_type_index == packet_index;
    return in_selected_stream;
}

/**
Read selected packet to respective queue.
Keep reading until a packet matching one of the selected streams are found or
end of stream is reached. If a packet mactching one of the selected streams is found
this packet is put into the respective lockless FIFO queue.
@return The media_type of the packet added to a queue or media_type::SIZE if
none is found.
*/
media_type_t av_format_context::read_packet_to_queue()
{
    av_packet packet = read_packet_selected();
    if (!packet.is_valid()) {
        return media_type_t::SIZE;
    }
    const media_type_t mt = packet.media_type_get();

    packet_queue_t& queue = packet_queue_get(mt);
    if (queue.capacity() == 0) {
        queue.resize_queue(packet_queue_capacity_);
    }
    if (queue.full()) {
        queue.resize_queue(queue.capacity()*2);
    }
    queue.push(std::move(packet));
    return mt;
}

bool av_format_context::read_each_type_to_queues(const std::set<media_type_t>& types_to_read)
{
    constexpr int max_packets_to_read = 100;
    auto rest_to_read = types_to_read;
    for (int i = 0; i < max_packets_to_read; ++i) {
        const auto mt = read_packet_to_queue();
        rest_to_read.erase(mt);
        if (rest_to_read.empty()) {
            break;
        }
    }
    return rest_to_read.empty();
}

media_type_t av_format_context::max_packet_queue_type(const std::set<media_type_t>& media_types_to_test)
{
    auto compare = [this](media_type_t mt_lhs, media_type_t mt_rhs) -> bool {
        return this->packet_queue(mt_lhs).size() < this->packet_queue(mt_rhs).size();
    };

    const auto it_max = std::max_element(media_types_to_test.begin(), media_types_to_test.end(), compare);

    return it_max != media_types_to_test.end() ? *it_max : media_type_t::SIZE;
}


/** Read packets in to all active queues until the designated media_type queue has
 reached a size as specified. So we put in one queue for each media type: audio, video,
subtitltes regardless of which primary media type we read. So even if we read video packets
we still put audio packet (for the selected audio stream) into it's own queue as well. Same
for subtitles.
@todo Add support for a possible threads_started_ test
*/
bool av_format_context::read_packets_to_queues(media_type_t mt, uint32_t fill_to_size)
{
    const packet_queue_t& queue = packet_queue(mt);

    if (queue.size() >= fill_to_size) {
        return true;
    }

    media_type_t mt_read = read_packet_to_queue();

    auto queue_size = queue.size();
    while ((mt_read != media_type_t::SIZE) && (queue_size < fill_to_size)) {
        mt_read = read_packet_to_queue();
        queue_size = queue.size();
    }
    return queue_size >= fill_to_size;
}

void av_format_context::packet_queue_pop(media_type_t mt)
{
    packet_queue_per_media_type_[to_size_t(mt)].pop();
}

av_packet av_format_context::packet_queue_front(media_type_t mt)
{
    return std::move(packet_queue_per_media_type_[to_size_t(mt)].front());
}

av_packet av_format_context::packet_queue_pop_front(media_type_t mt)
{
    const std::scoped_lock<std::mutex> lock(packet_queues_mutex_);
    packet_queue_t& queue = packet_queue_get(mt);

//    if (mt == media_type::subtitle) {
//        if (!queue.empty()) {
//            std::cerr << "FIXMENM packet_queue_pop_front subtitle\n";

////            return av_packet(); // FIXMENM
//        }
//    }

    if (queue.empty()) {
        return av_packet();
    }

    if (queue.empty()) {
        return av_packet();
    }
    auto packet = std::move(queue.front());
    queue.pop();
    return packet;
}

av_format_context::get_packet_fun av_format_context::get_packet_function(media_type_t mt)
{
    get_packet_fun fn = [this, mt] () {
        return this->packet_queue_pop_front(mt);
    };
    return fn;
}

std::chrono::microseconds av_format_context::packet_queue_pts(media_type_t mt) const
{
    return presentation_time(packet_queue(mt).front());
}

void av_format_context::flush_packet_queues()
{
    const std::scoped_lock<std::mutex> lock(packet_queues_mutex_);
    for (auto& queue : packet_queue_per_media_type_) {
        queue.flush();
    }
}


// --- Time functions ---

std::chrono::microseconds av_format_context::time_from_stream_time(
        size_t stream_index, int64_t stream_time_duration) const
{
    const AVRational stream_time_base = ff_format_context_->streams[stream_index]->time_base;
    const int64_t av_time_point = av_rescale_q(stream_time_duration, stream_time_base, AV_TIME_BASE_Q);

    av_base_duration av_duration(av_time_point);

    return std::chrono::duration_cast<std::chrono::microseconds>(av_duration);
}

std::chrono::microseconds av_format_context::time_from_stream_time(size_t stream_index, const av_packet& packet) const
{
    auto pts = time_from_stream_time(stream_index, packet.pts_stream_base());
    if (!packet.pts_valid()) {
        pts = time_from_stream_time(stream_index, packet.dts_stream_base());    // Fall back to decode timestamp if pts is invalid!
    }
    return pts;
}

void av_format_context::set_packet_presentation_time(size_t stream_index, av_packet& packet) const
{
    packet.presentation_time_set(time_from_stream_time(stream_index, packet));
}

std::chrono::microseconds av_format_context::presentation_time(const av_packet& packet) const
{
    return time_from_stream_time(packet.stream_index(), packet.pts_stream_base());
}

std::chrono::milliseconds av_format_context::presentation_time_ms(const av_packet& packet) const
{
    return duration_cast<milliseconds>(presentation_time(packet));
}

std::chrono::microseconds av_format_context::decode_time(const av_packet& packet) const
{
    return time_from_stream_time(packet.stream_index(), packet.dts_stream_base());
}


std::chrono::milliseconds av_format_context::decode_time_ms(const av_packet& packet) const
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(decode_time(packet));
}

std::chrono::microseconds av_format_context::presentation_time(const av_frame& frame) const
{
    return time_from_stream_time(frame.stream_index(), frame.pts_stream_base());
}

std::chrono::milliseconds av_format_context::presentation_time_ms(const av_frame& frame) const
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(presentation_time(frame));
}

std::chrono::microseconds av_format_context::best_effort_pts(const av_frame& frame) const
{
    return time_from_stream_time(frame.stream_index(), frame.ff_frame()->best_effort_timestamp);
}

std::chrono::milliseconds av_format_context::best_effort_pts_ms(const av_frame& frame) const
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(best_effort_pts(frame));
}

std::chrono::microseconds av_format_context::total_time() const
{
    if (!ff_format_context_) { return std::chrono::microseconds(0); }
    static_assert(AV_TIME_BASE == 1'000'000, "av_format_context::total_time() AV_TIME_BASE Is expected to be 1,000,000! Please add 'if' to compensate!");
    return std::chrono::microseconds(ff_format_context_->duration);
}

std::chrono::microseconds av_format_context::current_io_operation_duration() const
{
    if (!custom_io_ptr_) {
        return std::chrono::microseconds(0);
    }
    return custom_io_ptr_->current_io_operation_duration();
}

// -----------------------
// --- Debug functions ---
// -----------------------

void av_format_context::dump() const
{
    av_dump_format(ff_format_context_, 0, resource_path().c_str(), 0);
}

string av_format_context::queues_info() const
{
    stringstream ss;
    for ( size_t media_type_int = 0; media_type_int < selected_stream_per_media_type_.size(); ++media_type_int) {
        size_t stream_index = selected_stream_per_media_type_[media_type_int];
        if (stream_index == no_stream_index) continue;
        const media_type_t mt = static_cast<media_type_t>(media_type_int);
        const packet_queue_t& queue = packet_queue(mt);
        ss << "media: "  << to_string(mt)
           << ", stream: " << stream_index
           << ", pkt queue size: " << queue.size()
           << "\n";
    }

    return ss.str();
}


// ------------------------
// --- Helper functions ---
// ------------------------

void av_format_context::read_codec_contexts()
{
    for( size_t i=0; i < streams_count(); i++) {
        const AVStream* ff_stream = ff_format_context_->streams[i];
        const AVMediaType ff_media_type = ff_stream->codecpar->codec_type;
        if ( ff_media_type != AVMEDIA_TYPE_UNKNOWN) {
            const auto index_media_type = static_cast<size_t>(ff_media_type);
            stream_indices_per_media_type_[index_media_type].push_back(i);
        }
        stream_info_t si;
        si.stream_index = i;
        si.media_type = static_cast<media_type_t>(ff_media_type);
        si.meta_data = read_av_dictionary(ff_stream->metadata);

        if (si.meta_data.contains("language")) {
            si.language_code = cpaf::locale::language_codes::iso639_3_to_2(si.meta_data["language"]);
        }
        streams_info_.push_back(std::move(si));

    }
    primary_stream_index_ = default_primary_stream_index();
}

size_t av_format_context::default_primary_stream_index() const
{
    if (video_streams_count() > 0) return first_video_index();
    else if (audio_streams_count() > 0) return first_audio_index();
    if (subtitle_streams_count() > 0) return first_subtitle_index();
    return 0;
}


AVStream* av_format_context::ff_stream(size_t stream_index) const
{
    return ff_format_context_->streams[stream_index];
}

AVCodecParameters* av_format_context::ff_codec_parameters(size_t stream_index) const
{
    return ff_format_context_->streams[stream_index]->codecpar;
}


const AVCodec* av_format_context::ff_find_decoder(size_t stream_index) const
{
    return avcodec_find_decoder(codec_id(stream_index));
}


} //END namespace cpaf::video
