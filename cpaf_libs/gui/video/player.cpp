#include "player.h"

#include <fmt/format.h>
#include <cpaf_libs/system/cpaf_session.h>
#include <cpaf_libs/audio/cpaf_audio_device.h>
#include <cpaf_libs/time/cpaf_time.h>
#include <cpaf_libs/torrent/torrents.h>
#include <cpaf_libs/video/io/subtitle_container.h>
#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/audio/cpaf_audio_device.h>
#include <cpaf_libs/gui/video/render/render.h>
#include <cpaf_libs/gui/video/ui/controls_default.h>
#include <cpaf_libs/gui/video/data_definitions/playable.h>


using namespace cpaf::video;
using namespace cpaf::time;
using namespace std::chrono;
using namespace std::chrono_literals;

namespace cpaf::gui::video {

player::player(cpaf::audio::device& audio_device)
    : audio_device_(audio_device)
    , subtitle_downloader_thread_(*this)
{
    next_video_frame_ = av_frame::create_alloc();

//    pause_playback();
}

player::~player()
{
}

void player::set_main_window(const system_window& main_window)
{
    main_window_ptr_ = &main_window;
    subtitle_downloader_thread_.start();

    if (!video_controls_) {
        set_controls(std::make_unique<video::controls_default>(*this, configuration));
    }
}

void player::start_playing(const std::chrono::microseconds& start_time_pos)
{
    media_pipeline_threads_ = std::make_unique<pipeline_threads>(*this);

    audio_device_.play_callback_set(audio_callback_get());
    audio_out_formats_set(to_ff_audio_format(audio_device_.audio_format()));
    audio_device_.play();

    // source_stream(stream_type_t::video)
    if (has_video_stream()) {
        init_video(*main_window_ptr_);
    }

    auto&  video_fmt_ctx = primary_stream().format_context(); // TODO: If we use multiple streams we need to get the right format ctx per stream here!
    auto&  audio_fmt_ctx = primary_stream().format_context(); // TODO: If we use multiple streams we need to get the right format ctx per stream here!
    auto&  subtitle_fmt_ctx = primary_stream().format_context(); // TODO: If we use multiple streams we need to get the right format ctx per stream here!
    video_fmt_ctx.seek_time_pos(start_time_pos);
    audio_fmt_ctx.seek_time_pos(start_time_pos);
    subtitle_fmt_ctx.seek_time_pos(start_time_pos);
    audio_resampler_.in_formats_set(audio_codec_context());

    audio_resampler_.init();
    video_codec_context().get_packet_function_set(video_fmt_ctx.get_packet_function(media_type::video));
    audio_codec_context().get_packet_function_set(video_fmt_ctx.get_packet_function(media_type::audio));
    subtitle_codec_context().get_packet_function_set(video_fmt_ctx.get_packet_function(media_type::subtitle));

    media_pipeline_threads().audio_resampler_set(audio_resampler_);
    video_fmt_ctx.read_packets_to_queues(video_fmt_ctx.primary_media_type(), 10);

    media_pipeline_threads().start();
    stream_state() = stream_state_t::playing;
    // FIXMENM check_activate_subtitle();
    resume_playback();
}

void player::terminate()
{
    close();
    media_pipeline_threads().terminate();
    std::this_thread::sleep_for(1ms);
}

/// @todo need to support reading "hybrid" resources that (potentially) specify different streams
///       for video, audio, key_frames etc.. For now we simply open a single primary stream with this.
bool player::open(const std::string& resource_path)
{
    close();
    primary_resource_path_ = resource_path;
    // Create media pipeline threads
    pause_playback();
    primary_source_stream_ = std::make_unique<cpaf::video::play_stream>([this]() {return torrents_get();});
    return open_primary_stream(resource_path, "");
}

bool player::open(const playable& playab)
{
    close();
    primary_resource_path_ = playab.path();
    // Create media pipeline threads
    pause_playback();

    std::cerr << "\n---FIXMENM ---\n" << playab.json().dump(4) << "\n FIXMENM\n";
    auto language_code = configuration.str("user", "subtitle_language_code");
    fmt::println("FIXMENM open playable language_code: '{}'", language_code); std::cout << std::endl;
    const auto subtitle_path = playab.get_best_subtitle_path(language_code);
    fmt::println("FIXMENM open playable subtitle path: {},  language_code: '{}'", subtitle_path, language_code); std::cout << std::endl;

    start_time_pos_ = playab.start_time();
    primary_source_stream_ = std::make_unique<cpaf::video::play_stream>([this]() {return torrents_get();});
    const bool ok = open_primary_stream(playab.path(), subtitle_path);

    if (!subtitle_path.empty()) {
        open_subtitle(subtitle_path, language_code);
    }

    return ok;
}

void player::open_async(const playable& playab)
{
    open_thread_ = std::make_unique<std::thread>( [=,this]() { this->open(playab); } );
    open_thread_->detach();
}

void player::open_async(const std::string& resource_path, std::chrono::microseconds start_time_pos)
{
    primary_resource_path_ = resource_path;
    start_time_pos_ = start_time_pos;

    open_thread_ = std::make_unique<std::thread>( [=,this]() { this->open(resource_path); } );
    open_thread_->detach();

}

void player::open_subtitle(const std::string& subtitle_path, const std::string& language_code)
{
    subtitle_downloader_thread_.enqueue_subtitle(subtitle_path, language_code);
}

// bool player::open_subtitle_file(const std::string& subtitle_path, const std::string& language_code)
// {
//     // fmt::println("FIXMENM player::open_subtitle_file {} , {}", subtitle_path, language_code);
// }

// bool player::open_subtitle_file(const std::string& subtitle_path)
// {
//     if (!subtitle_path.empty()) {
//         fmt::println("FIXMENM Subtitle resource path: {}", subtitle_path); std::cout << std::endl;
//         auto sc = subtitle_container::create_from_path(subtitle_path, std::chrono::seconds(30));

//         if (!sc->is_valid()) {
//             return false;
//         }

// //        fmt::println("---------------------------- START");
// //        for (auto frame: sc->subtitles()) {
// //            fmt::println("{}", frame.to_string());
// //        }
// //        fmt::println("---------------------------- DONE");

//         auto it = sc->find_first_after(10min);
//         if (it != sc->end()) {
//             fmt::println("--------\n{}\n", it->to_string());
//         }
//         std::cerr << "FIXMENM Need to set subtitle container only after we start playying! Since the pipelie_threads are not created before !!!\n";
//         std::cerr << "FIXMENM Need to set subtitle container only after we start playying! Since the pipelie_threads are not created before !!!\n";
//         std::cerr << "FIXMENM Need to set subtitle container only after we start playying! Since the pipelie_threads are not created before !!!\n";
//         std::cerr << "FIXMENM Need to set subtitle container only after we start playying! Since the pipelie_threads are not created before !!!\n";
//         std::cerr << "FIXMENM Need to set subtitle container only after we start playying! Since the pipelie_threads are not created before !!!\n";
//         std::cerr << "FIXMENM Need to set subtitle container only after we start playying! Since the pipelie_threads are not created before !!!\n";
//         std::cerr << "FIXMENM Need to set subtitle container only after we start playying! Since the pipelie_threads are not created before !!!\n";
//         std::cerr << "FIXMENM Need to set subtitle container only after we start playying! Since the pipelie_threads are not created before !!!\n";
//         /// subtitle_container_set(std::move(sc));
//     }

//     return true;
// }





// void player::open_subtitle_file_async(const std::string& subtitle_path)
// {
//     std::cerr << "FIXMENM open_subtitle_file_async\n";
// //    open_subtitle_thread_ = std::make_unique<std::thread>( [=,this]()
// //                                                          {
// //                                                              while(1) {
// //                                                                  std::this_thread::sleep_for(1s);
// //                                                                  std::cerr << "Subtitle test thread ...\n";
// //                                                              }
// //                                                          } );

//     //    if (open_subtitle_thread_) {
// //        cpaf::system::kill_thread(*open_subtitle_thread_);
// //    }
//     open_subtitle_thread_ = std::make_unique<std::thread>( [=,this]() { this->open_subtitle_file(subtitle_path); } );


//     open_subtitle_thread_->detach();

// }

/**
 *  @todo If threads are taking too long to close, then force close/delete them!
 *
*/
void player::close()
{
    if (!primary_source_stream_) {
        return;
    }
    if (media_pipeline_threads_) {
        stream_state() = stream_state_t::inactive;
        pause_playback();
        int count = 0;
        const auto ts_end = std::chrono::steady_clock::now() + 5s;
        while (!media_pipeline_threads().all_threads_are_paused()
               &&(std::chrono::steady_clock::now() < ts_end))
        {
            std::cerr << "FIXMENM waiting for threads to ṕause: " << ++count << "\n";
            std::this_thread::sleep_for(100ms);
        }
        std::cerr << "FIXMENM DONE for threads to ṕause!!! : " << ++count << "\n";
    }
    audio_device_.pause(); // Pause audio
    primary_resource_path_.clear();
    primary_source_stream_->close();
    media_pipeline_threads().flush_queues();
    video_codec_ctx_ = av_codec_context{};
    audio_codec_ctx_ = av_codec_context{};
    primary_source_stream_.reset(nullptr);
    media_pipeline_threads_.reset(nullptr);
}

void player::close_async()
{
    open_thread_ = std::make_unique<std::thread>( [=,this]() { this->close(); } );
    open_thread_->detach();
}

void player::cancel_async_open() {
    stream_state() = stream_state_t::inactive;
    open_thread_.reset(nullptr);
}

bool player::open_secondary(const std::string& resource_path, stream_type_t sti)
{
    return open_stream(resource_path, sti);
}

bool player::has_source_stream(stream_type_t sti) const
{
    return source_stream(sti) != nullptr;
}

// TODO: A little convoluted this implementation
const play_stream* player::source_stream(stream_type_t sti) const
{
    const auto index = to_size_t(sti);
    if (source_streams_[index]) {
        return source_streams_[index].get();
    }
    if (sti == stream_type_t::video && primary_source_stream_->has_media_type(media_type::video)) {
        return primary_source_stream_.get();
    }
    else if (sti == stream_type_t::audio && primary_source_stream_->has_media_type(media_type::audio)) {
        return primary_source_stream_.get();
    }
    else if (sti == stream_type_t::subtitle && primary_source_stream_->has_media_type(media_type::subtitle)) {
        return primary_source_stream_.get();
    }
    return nullptr;
}

// TODO: A little convoluted this implementation
play_stream* player::source_stream(stream_type_t sti)
{
    const auto index = to_size_t(sti);
    if (source_streams_[index]) {
        return source_streams_[index].get();
    }
    if (sti == stream_type_t::video && primary_source_stream_->has_media_type(media_type::video)) {
        return primary_source_stream_.get();
    }
    else if (sti == stream_type_t::audio && primary_source_stream_->has_media_type(media_type::audio)) {
        return primary_source_stream_.get();
    }
    else if (sti == stream_type_t::subtitle && primary_source_stream_->has_media_type(media_type::subtitle)) {
        return primary_source_stream_.get();
    }
    return nullptr;
}

std::chrono::microseconds player::total_time() const {
    if (!primary_source_stream_) {
        return std::chrono::microseconds(0);
    }

    return primary_stream().total_time();
}

bool player::is_playing() const
{
    if (!primary_source_stream_) {
        return false;
    }
    const auto cur_stream_state_int = to_int(stream_state());
    return cur_stream_state_int >= to_int(stream_state_t::playing);
}

// ----------------
// --- Contexts ---
// ----------------
av_codec_context& player::video_codec_context() const
{
    if (!video_codec_ctx_.is_valid()) {
        auto* video_stream = source_stream(stream_type_t::video);
        if (video_stream){
            video_codec_ctx_ = video_stream->codec_context(video_stream_index());
            update_scaling_context();
        }
    }
    return video_codec_ctx_;
}

av_codec_context& player::audio_codec_context() const
{
    if (!audio_codec_ctx_.is_valid()) {
        auto* audio_stream = source_stream(stream_type_t::audio);
        if (audio_stream){
            audio_codec_ctx_ = audio_stream->codec_context(audio_stream_index());
            //TODO: Should we call update_resampler_context() or similar named function?;
        }
    }
    return audio_codec_ctx_;
}

av_codec_context& player::subtitle_codec_context() const
{
    if (!subtitle_codec_ctx_.is_valid()) {
        auto* subtitle_stream = source_stream(stream_type_t::subtitle);
        if (subtitle_stream){
            subtitle_codec_ctx_ = subtitle_stream->codec_context(subtitle_stream_index());
        }
    }
    return subtitle_codec_ctx_;
}

// ---------------------------
// --- Video setup/control ---
// ---------------------------
void player::video_dimensions_set(int32_t width, int32_t height)
{
    video_dimensions_set({width, height});
}

void player::video_dimensions_set(const surface_dimensions_t& dimensions)
{
    if (video_render_) {
        video_render_->render_geometry_set(rect(dimensions));
    }
    if (main_window_ptr_) {
        const auto play_dims = cpaf::math::v2f(dimensions);
        const auto screen_dims = cpaf::math::v2f(main_window_ptr_->display_size());
        player_to_screen_size_factor_ = play_dims / screen_dims;
    }

//    player_to_screen_size_factor_

//    video_dst_dimensions_requested_ = dimensions; // TODO: This is currently not working as intended!
//    update_scaling_context(); // TODO: This is currently not working as intended!
}

void player::video_scaler_flags_set(int32_t flags)
{
    video_scaler_flags_ = flags;
    update_scaling_context();
}

void player::video_scaler_align_set(int32_t align)
{
    video_scaler_align_ = align;
    update_scaling_context();
}

void player::ff_dst_pixel_format_set(AVPixelFormat pixel_format)
{
    ff_dst_pixel_format_ = pixel_format;
}


// ----------------------------
// --- Video info functions ---
// ----------------------------

surface_dimensions_t player::video_src_dimensions() const
{
    return video_codec_context().dimensions();
}

math::v2f player::video_src_dimensions_float() const
{
    return video_codec_context().dimensions_float();
}

surface_dimensions_t player::video_dst_dimensions() const
{
    if (video_codec_ctx_.is_valid()) {
        return video_codec_ctx_.dst_dimensions();
    }
    else if (video_dst_dimensions_requested_.x() <= 0 && video_dst_dimensions_requested_.y() <= 0 ) {
        return video_src_dimensions();
    }
    return video_dst_dimensions_requested_;
}


size_t player::video_stream_index() const
{
    return video_stream_index_ != no_stream_index ? video_stream_index_ : source_stream(stream_type_t::video)->first_video_index();
}

rect player::render_geometry() const
{
    if (video_render_) {
        return video_render_->render_geometry();
    }
    else if (main_window_ptr_) {
        const auto size = main_window_ptr_->size();
        return rect(0, 0, size.width(), size.height());
    }
    
    return rect();
}

// ---------------------------
// --- Audio setup/control ---
// ---------------------------

bool player::audio_out_formats_set(const audio::device& audio_device)
{
    return audio_resampler_.out_formats_set(audio_device);
}

bool player::audio_out_formats_set(const ff_audio_format_t& ff_audio_format)
{
    return audio_resampler_.out_formats_set(ff_audio_format);
}

// ----------------------------
// --- Audio info functions ---
// ----------------------------

size_t player::audio_stream_index() const
{
    return audio_stream_index_ != no_stream_index ? audio_stream_index_ : source_stream(stream_type_t::audio)->first_audio_index();
}


size_t player::subtitle_stream_index() const
{
    return subtitle_stream_index_ != no_stream_index ? subtitle_stream_index_ : source_stream(stream_type_t::subtitle)->first_subtitle_index();
}


void player::subtitle_container_set(std::unique_ptr<subtitle_container> container)
{
    if (!all_initialized()) {
        return;
    }
//    const auto is_paused = playback_paused();
//    pause_playback();
    subtitle_source_ = subtitle_source_t::text_file;
    media_pipeline_threads().subtitle_container_set(std::move(container));
//    playback_paused_set(is_paused);
}

// -------------------------------
// --- Subtitles setup/control ---
// -------------------------------

// ---------------------------------------------
// --- Interfacing to surrounding app/system ---
// ---------------------------------------------

player::audio_play_callback_t player::audio_callback_get()
{
    return media_pipeline_threads().audio_callback_get();
}

void player::frame_update()
{
///    static int FIXMENM_count = 0;
///   if (FIXMENM_count % 500 == 0) {
///        std::cerr << "FIXMENM " << steady_now_h_m_s_ms()
///                  << ", playing: " << is_playing()
///                  << "\n";
///    }
///    ++FIXMENM_count;

    handle_internal_events();
    if ( is_playing()) {
        if (has_video_stream()) {
            video_frame_update(next_video_frame_);
            check_activate_subtitle();
        }
        cur_media_time().release_after_reset();
    }
    if (show_controls_ && video_controls_) {
        video_controls_->render();
    }

}

void player::video_frame_update(av_frame& current_frame, cpaf::gui::video::render& video_render)
{
    if (media_pipeline_threads_) {
        media_pipeline_threads().video_frame_update(current_frame, video_render);
    }
}

void player::video_frame_update(av_frame& current_frame)
{
    if (media_pipeline_threads_) {
        video_frame_update(current_frame, *video_render_);
    }
}

std::shared_ptr<torrent::torrents> player::torrents_get() const
{
    if (!torrents_) {
        torrents_ = std::make_shared<torrent::torrents>();
        torrents_->start();
    }
    return torrents_;
}

// --------------------
// --- Play control ---
// --------------------

void player::seek_position(const std::chrono::microseconds& stream_pos, seek_dir dir)
{
    if (resume_from_pause_on_seek_) { resume_playback(); }
    if (media_pipeline_threads_) {
        media_pipeline_threads().seek_position(stream_pos, dir);
    }
}

void player::seek_position(const std::chrono::microseconds& stream_pos)
{
    if (resume_from_pause_on_seek_) { resume_playback(); }
    if (media_pipeline_threads_) {
        media_pipeline_threads().seek_position(stream_pos);
    }
}

void player::seek_relative(const std::chrono::microseconds& delta_time)
{
    if (resume_from_pause_on_seek_) { resume_playback(); }
    if (media_pipeline_threads_) {
        media_pipeline_threads().seek_relative(delta_time);
    }
}

void player::pause_playback()
{
    cur_media_time_.pause_time();
    ////threads_paused_ = true;
    if (media_pipeline_threads_) {
        media_pipeline_threads_->pause_playback();
    }
}

void player::resume_playback()
{
    cur_media_time_.resume_time();
    ////threads_paused_ = false;
    if (media_pipeline_threads_) {
        media_pipeline_threads_->resume_playback();
    }
}

void player::toggle_pause_playback()
{
    if (playback_is_paused()) {
        resume_playback();
    }
    else {
        pause_playback();
    }
}

void player::playback_paused_set(bool is_paused)
{
    if (is_paused) {
        resume_playback();
    }
    else {
        pause_playback();
    }
}

bool player::playback_is_paused() const {
    if (media_pipeline_threads_) {
        return media_pipeline_threads().playback_paused();
    }
    return true;
}

seek_state_t player::seek_state() const
{
    if (media_pipeline_threads_) {
        return media_pipeline_threads_->seek_state();
    }
    return seek_state_t::unknown;
}

std::chrono::microseconds player::seek_from_position() const
{
    if (media_pipeline_threads_) {
        return media_pipeline_threads_->seek_from_position();
    }
    return 0us;
}

std::chrono::microseconds player::seek_position_requested() const
{
    if (media_pipeline_threads_) {
        return media_pipeline_threads_->seek_position_requested();
    }
    return 0us;
}

void player::set_controls(std::unique_ptr<controls> controls)
{
    video_controls_ = std::move(controls);
}

// -----------------------
// --- Debug functions ---
// -----------------------
std::string player::queues_info() const
{
    return primary_stream().format_context().queues_info();
}

bool player::all_initialized() const
{
    return media_pipeline_threads_ != nullptr;
}

// ---------------------------------
// --- PRIVATE: Helper functions ---
// ---------------------------------
void player::init_video(const system_window& main_window)
{
    /// std::cerr << "FIXMENM player::init_video 1\n";
    video_render_ = render::create_video_render(*this, configuration, main_window, video_dst_dimensions());
    video_render_->video_codec_ctx_set(video_codec_context());
    video_render_->render_geometry_set(rect(main_window.size()));

    /// std::cerr << "FIXMENM player::init_video 4\n";
    ////    video_render_->render_geometry_set(render_geometry_t({100,100}, main_window.get_size())); // TEST ONLY!
}

bool player::open_stream(const std::string& resource_path, stream_type_t sti)
{
    const auto index = to_size_t(sti);
    source_streams_[index] = std::make_unique<play_stream>([this]() {return torrents_get();});
    const auto open_ok = source_streams_[index]->open(resource_path);

    return open_ok;
}

/// @todo Implement subtitle path handling here
bool player::open_primary_stream(const std::string& resource_path, const std::string& subtitle_path)
{
    primary_resource_path_ = resource_path;
    const auto open_ok = primary_stream().open(resource_path);
    return open_ok;
}

void player::check_activate_subtitle()
{
    if (subtitle_downloader_thread_.has_subtitle()) {
        auto container = subtitle_downloader_thread_.dequeue_subtitle();
        subtitle_container_set(std::move(container));
    }
}

void player::update_scaling_context() const
{
    if (video_codec_ctx_.is_valid()) {
        video_codec_ctx_.init_scaling_context(ff_dst_pixel_format_, video_dst_dimensions_requested_, video_scaler_flags_, video_scaler_align_);
    }
}

void player::handle_internal_events()
{
    handle_stream_state();
}

void player::handle_stream_state()
{
    if (!primary_source_stream_) {
        return;
    }
    auto stream_state_expected = stream_state_t::open;
    if (stream_state().compare_exchange_strong(stream_state_expected, stream_state_t::playing)) {
        start_playing(start_time_pos_);
        if (cb_start_playing_) { cb_start_playing_(); }
    }
}

} // END namespace cpaf::gui::video
