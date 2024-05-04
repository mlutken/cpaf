#include "player.h"

#include <algorithm>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <concurrent/concurrent_utils.hpp>
#include <cpaf_libs/system/cpaf_session.h>
#include <cpaf_libs/audio/cpaf_audio_device.h>
#include <cpaf_libs/time/cpaf_time.h>
#include <cpaf_libs/torrent/torrents.h>
#include <cpaf_libs/video/io/subtitle_container.h>
#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/audio/cpaf_audio_device.h>
#include <cpaf_libs/gui/video/render/render.h>
#include <cpaf_libs/gui/video/ui/controls_default.h>


using namespace std;
using namespace cpaf::video;
using namespace cpaf::time;
using namespace std::chrono;
using namespace std::chrono_literals;

namespace cpaf::gui::video {

player::player(cpaf::audio::device& audio_device, locale::translator& tr)
    : audio_device_(audio_device)
    , tr_(tr)
    , subtitle_downloader_thread_(*this)
    , play_handler_thread_(*this)
{
    next_video_frame_ = av_frame::create_alloc();
    configuration.connect_for_changes([this]() {on_configuration_changed();});
    media_pipeline_threads_ = std::make_unique<pipeline_threads>(*this);
    media_pipeline_threads_->stop();
    media_pipeline_threads_->run();
    audio_device_.play_callback_set(audio_callback_get());
    audio_out_formats_set(to_ff_audio_format(audio_device_.audio_format()));
}

player::~player()
{
    terminate();
}

void player::set_main_window(system_window& main_window)
{
    main_window_ptr_ = &main_window;
    subtitle_downloader_thread_.run();
    play_handler_thread_.run();

    if (!video_controls_) {
        set_controls(std::make_unique<video::controls_default>(*this, configuration));
    }
    update_screen_size_factor();
}


/// @todo This function as opposed to close() ? Figure out!
void player::terminate()
{
    close_command();
    if (media_pipeline_threads_) {
        media_pipeline_threads().terminate();
        media_pipeline_threads().wait_for_all_terminated(1s);
    }
}


void player::open_media(playable playab)
{
    close_media(5ms);  // Abort any current media that might be opening or waiting for data while playing!
    play_handler_thread_.open_async(playab);
}

void player::open_media(const std::string& resource_path, std::chrono::microseconds start_time_pos)
{
    auto playab = cpaf::gui::video::playable(resource_path);
    playab.set_start_time(start_time_pos);
    open_media(playab);
}

void player::close_current_media()
{
    close_media_requested_ = true;
}

/// @todo Most likely we need to test this and make an async version!
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
    if (sti == stream_type_t::video && primary_source_stream_->has_media_type(media_type_t::video)) {
        return primary_source_stream_.get();
    }
    else if (sti == stream_type_t::audio && primary_source_stream_->has_media_type(media_type_t::audio)) {
        return primary_source_stream_.get();
    }
    else if (sti == stream_type_t::subtitle && primary_source_stream_->has_media_type(media_type_t::subtitle)) {
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
    if (sti == stream_type_t::video && primary_source_stream_->has_media_type(media_type_t::video)) {
        return primary_source_stream_.get();
    }
    else if (sti == stream_type_t::audio && primary_source_stream_->has_media_type(media_type_t::audio)) {
        return primary_source_stream_.get();
    }
    else if (sti == stream_type_t::subtitle && primary_source_stream_->has_media_type(media_type_t::subtitle)) {
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
    return  primary_stream_state_ == stream_state_t::playing;
}

const playable& player::cur_playable() const {
    // std::scoped_lock<std::mutex> lock(current_playable_mutex_);
    return current_playable_;
}

std::chrono::microseconds player::current_io_operation_duration() const
{
    if (!primary_source_stream_) {
        return std::chrono::microseconds(0);
    }
    return primary_source_stream_->current_io_operation_duration();
}

// ----------------
// --- Contexts ---
// ----------------
av_codec_context& player::video_codec_context() const
{
    std::scoped_lock<std::mutex> lock(video_codec_mutex_);
    if (!primary_source_stream_) {
        std::cerr << "LOG_CRITICAL: create_video_codec_ctx(), primary_source_stream_ is nullptr\n";
        return video_codec_ctx_;
    }

    if (!video_codec_ctx_.is_valid()) {
        auto* video_stream = source_stream(stream_type_t::video);
        if (video_stream){
            video_codec_ctx_ = video_stream->codec_context(video_stream_index());
            const bool mutex_already_locked = true;
            update_scaling_context(mutex_already_locked);
            av_format_context& video_fmt_ctx = primary_source_stream_->format_context(); // TODO: If we use multiple streams we need to get the right format ctx per stream here!
            video_codec_ctx_.get_packet_function_set(video_fmt_ctx.get_packet_function(media_type_t::video));
        }
    }
    return video_codec_ctx_;
}

av_codec_context& player::audio_codec_context() const
{
    if (!primary_source_stream_) {
        std::cerr << "LOG_CRITICAL: audio_codec_context(), primary_source_stream_ is nullptr\n";
        return audio_codec_ctx_;
    }

    if (!audio_codec_ctx_.is_valid()) {
        auto* audio_stream = source_stream(stream_type_t::audio);
        if (audio_stream){
            audio_codec_ctx_ = audio_stream->codec_context(audio_stream_index());
            //TODO: Should we call update_resampler_context() or similar named function?;
            auto&  audio_fmt_ctx = primary_source_stream_->format_context(); // TODO: If we use multiple streams we need to get the right format ctx per stream here!
            audio_codec_ctx_.get_packet_function_set(audio_fmt_ctx.get_packet_function(media_type_t::audio));
        }
    }
    return audio_codec_ctx_;
}

av_codec_context& player::subtitle_codec_context() const
{
    if (!primary_source_stream_) {
        std::cerr << "LOG_CRITICAL: subtitle_codec_context(), primary_source_stream_ is nullptr\n";
        return subtitle_codec_ctx_;
    }

    std::scoped_lock<std::mutex> lock(subtitle_codec_mutex_);
    if (!subtitle_codec_ctx_.is_valid()) {
        auto* subtitle_stream = source_stream(stream_type_t::subtitle);
        if (subtitle_stream){
            subtitle_codec_ctx_ = subtitle_stream->codec_context(subtitle_stream_index());
            auto&  subtitle_fmt_ctx = primary_source_stream_->format_context(); // TODO: If we use multiple streams we need to get the right format ctx per stream here!
            subtitle_codec_ctx_.get_packet_function_set(subtitle_fmt_ctx.get_packet_function(media_type_t::subtitle));
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

void player::video_dimensions_set(surface_dimensions_t dimensions)
{
    if (video_render_) {
        // if (keep_aspect_ratio_) {
        //     const float wfac = static_cast<float>(video_src_dimensions().width()) / static_cast<float>(dimensions.width());
        //     const float hfac = static_cast<float>(video_src_dimensions().height()) / static_cast<float>(dimensions.height());
        //     dimensions = video_src_dimensions().uniform_scale_x(dimensions.x());
        //     // if (wfac > hfac) {
        //     //     dimensions = video_src_dimensions().uniform_scale_x(dimensions.x());
        //     // }
        //     // else {
        //     //     dimensions = video_src_dimensions().uniform_scale_y(dimensions.y());
        //     // }
        // }
///        math::v2i32 delta {150, 100};
///        math::v2i32 size = dimensions - 2*delta;
///        rect r(delta, size);
///        video_render_->render_geometry_set(r); // FIXMENM

        video_render_->render_geometry_set(rect(dimensions));
    }
    update_screen_size_factor();

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
    if (video_codec_context().is_valid()) {
        return video_codec_context().dst_dimensions();
    }
    if (video_dst_dimensions_requested_.x() <= 0 && video_dst_dimensions_requested_.y() <= 0 ) {
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

// -------------------------------
// --- Subtitles setup/control ---
// -------------------------------
/// @todo If called outside of the selectable_subtitles() vector some thing will look strange at the very least in the GUI
/// @todo Consider making this private
void player::enqueue_subtitle_file(const std::string& subtitle_path,
                                   const std::string& language_code,
                                   std::chrono::microseconds subtitle_adjust_offset)
{
    subtitle_downloader_thread_.enqueue_subtitle(subtitle_path, language_code, subtitle_adjust_offset);
}


void player::subtitle_select(const std::string& language_code, std::chrono::microseconds subtitle_adjust_offset)
{
    const int32_t sel_index = cur_playable().selectable_subtitle_index_of(language_code);
    subtitle_select(sel_index, subtitle_adjust_offset);
}

void player::subtitle_select(int32_t selectable_subtitle_index, std::chrono::microseconds subtitle_adjust_offset)
{
    subtitle_source_ = subtitle_source_t::none;
    if (set_subtitle_helper(selectable_subtitle_index)) {
        const auto index = static_cast<uint32_t>(selectable_subtitle_index);
        auto entry = selectable_subtitles().at(index);
        if (entry.source == subtitle_source_t::text_file) {
            if (!entry.path.empty()) {
                subtitle_source_ = subtitle_source_t::text_file;
                enqueue_subtitle_file(entry.path, entry.language_code, subtitle_adjust_offset);
                return;
            }
        }
        else if (entry.source == subtitle_source_t::stream) {
            if (primary_source_stream_) {
                subtitle_source_ = subtitle_source_t::stream;
                subtitle_stream_index_set(entry.stream_index, subtitle_adjust_offset);
                return;
            }
        }
        // If we get here we have no working subtitle so disable
        set_subtitle_helper(-1);
    }
}

int32_t player::subtitle_selected_index() const
{
    return subtitle_selected_index_;
}


size_t player::subtitle_stream_index() const
{
    return subtitle_stream_index_ != no_stream_index ? subtitle_stream_index_ : source_stream(stream_type_t::subtitle)->first_subtitle_index();
}

void player::subtitle_stream_index_set(size_t stream_index, std::chrono::microseconds subtitle_adjust_offset)
{
    std::scoped_lock<std::mutex> lock(subtitle_codec_mutex_);
    if (primary_source_stream_) {
        cur_media_time().subtitles_offset_set(subtitle_adjust_offset);
        primary_source_stream_->subtitle_index_set(stream_index);
        subtitle_stream_index_ = stream_index;
        subtitle_codec_ctx_ = av_codec_context(); // Reset the subtitle code context
    }
    else {
        subtitle_stream_index_ = no_stream_index;
    }
}


/// @todo Make this private
void player::subtitle_container_set(std::unique_ptr<subtitle_container> container)
{
    if (!all_initialized()) {
        return;
    }
    subtitle_source_ = subtitle_source_t::text_file;
    cur_media_time_.subtitles_offset_set(container->subtitle_adjust_offset);
    media_pipeline_threads().subtitle_container_set(std::move(container));
}

// ---------------------------------------------
// --- Interfacing to surrounding app/system ---
// ---------------------------------------------

player::audio_play_callback_t player::audio_callback_get()
{
    return media_pipeline_threads().audio_callback_get();
}

/// @todo: Perhaps we do not need to call playable_update_calculated() every frame
void player::frame_update()
{
    handle_internal_events();
    if (is_playing()) {
        if (has_video_stream()) {
            video_frame_update(next_video_frame_);
            check_activate_subtitle();
        }
        cur_media_time().release_after_reset();
    }

    if (video_controls_) {
        video_controls_->show(show_controls_);
        video_controls_->render();
        if (should_show_stream_state()) {
            video_controls_->render_stream_state();
        }
    }

}

void player::toggle_full_screen()
{
    if (main_window_ptr_) {
        main_window_ptr_->toggle_full_screen_mode();
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

std::shared_ptr<torrent::torrents> player::torrents_get()
{
    if (!torrents_) {
        torrents_set(std::make_shared<torrent::torrents>());
        torrents_->start();
    }
    return torrents_;
}

void player::torrents_set(std::shared_ptr<torrent::torrents> tors)
{
    torrents_ = std::move(tors);
    torrents_->register_finished_event([this](auto tor_file){torrent_finished_event(tor_file);} );
}

// --------------------
// --- Play control ---
// --------------------

void player::seek_position(const std::chrono::microseconds& stream_pos, seek_dir dir)
{
    if (!ui_events_enabled_) { return; }
    if (resume_from_pause_on_seek_) { resume_playback(); }
    if (media_pipeline_threads_) {
        media_pipeline_threads().seek_position(stream_pos, dir);
    }
}

/// @todo If we use multiple streams we need to get the right format ctx per stream here!
void player::seek_position(const std::chrono::microseconds& stream_pos)
{
    if (!ui_events_enabled_) { return; }
    if (resume_from_pause_on_seek_) { resume_playback(); }
    if (media_pipeline_threads_) {
        media_pipeline_threads().seek_position(stream_pos);
    }
}

void player::seek_relative(const std::chrono::microseconds& delta_time)
{
    if (!ui_events_enabled_) { return; }
    if (resume_from_pause_on_seek_) { resume_playback(); }
    if (media_pipeline_threads_) {
        media_pipeline_threads().seek_relative(delta_time);
    }
}

void player::pause_playback()
{
    if (!ui_events_enabled_) { return; }
    internal_paused_set(true);
}

void player::resume_playback()
{
    if (!ui_events_enabled_) { return; }
    internal_paused_set(false);
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
    if (!ui_events_enabled_) { return; }
    internal_paused_set(is_paused);
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

void player::ui_window_active_set(bool ui_window_active)
{
    if (ui_window_active == ui_window_active_) {
        return;
    }
    if (ui_window_active)   { push_paused(); }
    else                    { pop_paused(); }

    ui_window_active_ = ui_window_active;
    ui_events_enabled_set(!ui_window_active);
}

// -----------------------
// --- Debug functions ---
// -----------------------
std::string player::queues_info() const
{
    return primary_stream().format_context().queues_info();
}

std::chrono::microseconds player::dbg_audio_front_time() const
{
    if (media_pipeline_threads_) {
        return media_pipeline_threads_->dbg_audio_front_time();
    }
    return std::chrono::microseconds(0);
}


// ---------------------------------
// --- PRIVATE: Helper functions ---
// ---------------------------------

/**
 *  * Called from play_handler_thread
*/
bool player::open_command(playable playab)
{
    std::cerr << fmt::format("\n---FIXMENM [{}] BEGIN Open playable ---\n", to_string(primary_stream_state()));
//    std::cerr << fmt::format("FIXMENM path: {}\n--------------------------------\n\n", playab.path());
    close_command();
    close_media_requested_ = false;
    primary_stream_state_ = stream_state_t::opening;

    const auto start_close_tp = steady_clock::now();
    cur_playable_set(playab);
    media_pipeline_threads().stop();
    pause_playback();

    stream_completely_downloaded_ = false;
    subtitle_selected_index_ = -1;
    subtitle_source_ = subtitle_source_t::none;

    reset_primary_stream(std::make_unique<cpaf::video::play_stream>([this]() {return torrents_get();}, &primary_stream_state_));
    media_pipeline_threads().flush_queues();

    float cur_time_ms = (duration_cast<microseconds>( steady_clock::now() - start_close_tp)).count() / 1000.0f; // FIXMENM
    std::cerr << fmt::format("---FIXMENM [{}]  START Open primary stream time: {} ms\n", to_string(primary_stream_state()), cur_time_ms);

    bool ok = open_primary_stream(cur_playable().path());

    cur_time_ms = (duration_cast<microseconds>( steady_clock::now() - start_close_tp)).count() / 1000.0f; // FIXMENM
    std::cerr << fmt::format("---FIXMENM [{}]  COMPLETED Open primary stream time: {} ms\n", to_string(primary_stream_state()), cur_time_ms);

    if (!ok) {
        cur_time_ms = (duration_cast<microseconds>( steady_clock::now() - start_close_tp)).count() / 1000.0f; // FIXMENM
        std::cerr << fmt::format("---FIXMENM [{}]  ABORTED During Open primary stream time: {} ms\n", to_string(primary_stream_state()), cur_time_ms);
        primary_stream_state() = stream_state_t::inactive;

        // TODO: Some cleanup needed here perhaps ?
        return false;
    }

    const bool force = true;
    cur_playable_upd_calc(force);

    // --- Prepare audio ---
    audio_device_.play_callback_set(audio_callback_get());
    audio_out_formats_set(to_ff_audio_format(audio_device_.audio_format()));
    audio_resampler_.in_formats_set(audio_codec_context());
    audio_resampler_.init();
    media_pipeline_threads().audio_resampler_set(audio_resampler_);

    // --- Seek postion ---
    seek_position(cur_playable().start_time());
    /// format_context().read_packets_to_queues(format_context().primary_media_type(), 10);

    primary_stream_state() = stream_state_t::open;
    cur_time_ms = (duration_cast<microseconds>( steady_clock::now() - start_close_tp)).count() / 1000.0f; // FIXMENM
    std::cerr << fmt::format("---FIXMENM DONE [{}]  Open playable time: {} ms\n", to_string(primary_stream_state()), cur_time_ms);
    return ok;
}

/// @todo Make player::start_playing() private
void player::start_playing()
{
    std::cerr << fmt::format("\n--- FIXMENM [{}] START Start playing ---\n", to_string(primary_stream_state()));

    audio_device_.play();
    if (has_video_stream()) {
        init_video(*main_window_ptr_);

        auto language_code = configuration.str("subtitles", "language_code");
        const auto entry = cur_playable().find_best_subtitle(language_code);
        subtitle_select(entry.language_code, entry.subtitle_adjust_offset);
    }
    media_pipeline_threads().start();
    resume_playback();
    primary_stream_state() = stream_state_t::playing;

    std::cerr << fmt::format("\n--- FIXMENM [{}] DONE Start playing ---\n", to_string(primary_stream_state()));
}

/**
* Called from play_handler_thread
*/
void player::close_command()
{
    std::cerr << fmt::format("\n\n*** FIXMENM close BEGIN [{}]\n", to_string(primary_stream_state()));
    //    std::cerr << fmt::format("FIXMENM path: {}\n----------------------\n\n", cur_playable().path());

    const auto start_close_tp = steady_clock::now();
    close_media(2s);
    reset_primary_stream();
    float cur_time_ms = (duration_cast<microseconds>( steady_clock::now() - start_close_tp)).count() / 1000.0f;
    std::cerr << fmt::format("*** FIXMENM close DONE [{}] time: {} ms\n", to_string(primary_stream_state()), cur_time_ms);
}


void player::close_media(std::chrono::milliseconds wait_for_threads_to_stop_time)
{
    std::cerr << "!!!! FIXMENM player::close_media() !!!!\n";
    audio_device_.pause(); // Pause audio.

    if (media_pipeline_threads_) {
        media_pipeline_threads_->stop();
    }
    if (primary_source_stream_) {
        primary_source_stream_->cancel_current_io();
    }

    if (media_pipeline_threads_ ) {
        media_pipeline_threads_->wait_for_all_stopped(wait_for_threads_to_stop_time);
    }
    current_playable_.clear();

    primary_stream_state() = stream_state_t::inactive;
}

void player::reset_primary_stream(std::unique_ptr<play_stream> new_primary_stream)
{
    if (primary_source_stream_) {
        primary_source_stream_->close();
    }

    {
        std::scoped_lock<std::mutex> lock(video_codec_mutex_);
        video_codec_ctx_ = av_codec_context{};
    }
    {
        std::scoped_lock<std::mutex> lock(audio_codec_mutex_);
        audio_codec_ctx_ = av_codec_context{};
    }
    {
        std::scoped_lock<std::mutex> lock(subtitle_codec_mutex_);
        subtitle_codec_ctx_ = av_codec_context{};
    }
    primary_source_stream_ = std::move(new_primary_stream);
}

bool player::all_initialized() const
{
    return media_pipeline_threads_ != nullptr;
}

void player::init_video(const system_window& main_window)
{
    video_render_ = render::create_video_render(*this, configuration, main_window, video_dst_dimensions(), video_src_dimensions());
    video_render_->video_codec_ctx_set(video_codec_context());
    video_render_->render_geometry_set(rect(main_window.size()));
}

bool player::open_stream(const std::string& resource_path, stream_type_t sti)
{
    const auto index = to_size_t(sti);
    source_streams_[index] = std::make_unique<play_stream>([this]() {return torrents_get();}, nullptr);
    const auto open_ok = source_streams_[index]->open(resource_path);

    return open_ok;
}

bool player::open_primary_stream(const std::string& resource_path)
{
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

void player::update_scaling_context(bool mutex_already_locked) const
{
    if (mutex_already_locked) {
        if (video_codec_ctx_.is_valid()) {
            video_codec_ctx_.init_scaling_context(ff_dst_pixel_format_, video_dst_dimensions_requested_, video_scaler_flags_, video_scaler_align_);
        }
        return;
    }

    std::scoped_lock<std::mutex> lock(video_codec_mutex_);
    if (video_codec_ctx_.is_valid()) {
        video_codec_ctx_.init_scaling_context(ff_dst_pixel_format_, video_dst_dimensions_requested_, video_scaler_flags_, video_scaler_align_);
    }
}

void player::handle_internal_events()
{
    if (close_media_requested_) {
        close_media_requested_ = false;
        handle_close_media();
    }
    else {
        handle_stream_state();
    }
}

void player::handle_stream_state()
{
    if (!primary_source_stream_) {
        return;
    }
    auto stream_state_expected = stream_state_t::open;
    if (primary_stream_state().compare_exchange_strong(stream_state_expected, stream_state_t::start_playing)) {
        start_playing();
        if (cb_start_playing_) { cb_start_playing_(); }
    }
}

void player::handle_close_media()
{
    close_media();
}

void player::torrent_finished_event(std::shared_ptr<cpaf::torrent::torrent> tor_file)
{
    if (tor_file) {
        auto expected = false;
        stream_completely_downloaded_.compare_exchange_strong(expected, true);
    }
}

void player::on_configuration_changed()
{
    const bool force = true;
    cur_playable_upd_calc(force);
}

void player::cur_playable_upd_calc(bool force)
{
    // std::scoped_lock<std::mutex> lock(current_playable_mutex_);
    if (primary_source_stream_) {
        current_playable_.update_calculated(tr(), &primary_source_stream_->streams_info(), force);
    }
    else {
        current_playable_.update_calculated(tr(), nullptr, force);
    }
}

void player::update_screen_size_factor()
{
    if (main_window_ptr_) {
        const auto play_dims = render_geometry().size();
        const auto screen_dims = cpaf::math::v2f(main_window_ptr_->display_size());
        screen_size_factor_raw_ = play_dims / screen_dims;
        screen_size_factor_use_ = screen_size_factor_raw_;
        screen_size_factor_use_.x() = std::clamp(screen_size_factor_use_.x(), 0.5f, 1.0f);
        screen_size_factor_use_.y() = std::clamp(screen_size_factor_use_.y(), 0.5f, 1.0f);
    }
    if (video_controls_) {
        video_controls_->on_player_size_changed();
    }
}

bool player::should_show_stream_state() const
{
            return true; // FIXMENM
    const auto& ss = primary_stream_state();
    return ss == stream_state_t::opening || is_waiting_for_io();
    // return ss == stream_state_t::opening || ss == stream_state_t::closing || is_waiting_for_io();
}

bool player::set_subtitle_helper(int32_t selectable_subtitle_index)
{
    subtitle_selected_index_ = std::clamp(selectable_subtitle_index, -1, (static_cast<int32_t>(selectable_subtitles().size() -1)));
    bool show_subtitles = subtitle_selected_index_ > -1;
    configuration.bool_set("subtitles", "show", show_subtitles);
    return show_subtitles;
}

void player::internal_paused_set(bool is_paused)
{
    if (is_paused) {
        cur_media_time_.pause_time();
        if (media_pipeline_threads_) {
            media_pipeline_threads_->pause_playback();
        }
    }
    else {
        cur_media_time_.resume_time();
        if (media_pipeline_threads_) {
            media_pipeline_threads_->resume_playback();
        }
    }
}

void player::push_paused()
{
    save_playback_is_paused_state_ = playback_is_paused();
    internal_paused_set(true);
}

void player::pop_paused()
{
    internal_paused_set(save_playback_is_paused_state_);
}

void player::cur_playable_set(playable playab)
{
    // std::scoped_lock<std::mutex> lock(current_playable_mutex_);
    current_playable_ = std::move(playab);
}

} // END namespace cpaf::gui::video
