#include "player.h"

#include <algorithm>
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
{
    next_video_frame_ = av_frame::create_alloc();
    configuration.connect_for_changes([this]() {on_configuration_changed();});

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
    update_screen_size_factor();
}

/// @todo Make player::start_playing() private
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

    media_pipeline_threads().audio_resampler_set(audio_resampler_);
    video_fmt_ctx.read_packets_to_queues(video_fmt_ctx.primary_media_type(), 10);

    playable_.dbg_print(); // FIXMENM

    media_pipeline_threads().start();
    set_stream_state(stream_state_t::playing);
    check_activate_subtitle();
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
    auto playab = cpaf::gui::video::playable(resource_path);
    return open(playab);
}

bool player::open(playable playab)
{
    playable_ = std::move(playab);

    close();
    subtitle_selected_index_ = -1;
    subtitle_source_ = subtitle_source_t::none;
    primary_resource_path_ = playable_.path();
    pause_playback();


    std::cerr << "\n---FIXMENM Open playable ---\n" << playable_.json().dump(4) << "\n FIXMENM\n";
    //    configuration.dbg_print(); // FIXMENM

    start_time_pos_ = playable_.start_time();
    primary_source_stream_ = std::make_unique<cpaf::video::play_stream>([this]() {return torrents_get();});
    const bool ok = open_primary_stream(playable_.path());

    const bool force = true;
    playable_update_calculated(force);

    auto language_code = configuration.str("subtitles", "language_code");
    const auto entry = playable_.find_best_subtitle(language_code);

    if (entry.is_valid()) {
        subtitle_select(entry.language_code);
    }

    return ok;
}

void player::open_async(playable playab)
{
    open_thread_ = std::make_unique<std::thread>( [=,this]() { this->open(std::move(playab)); } );
    open_thread_->detach();
}

void player::open_async(const std::string& resource_path, std::chrono::microseconds start_time_pos)
{
    auto playab = cpaf::gui::video::playable(resource_path);
    playab.set_start_time(start_time_pos);
    open_async(playab);
}


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
        set_stream_state(stream_state_t::inactive);
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
        media_pipeline_threads().flush_queues();
        media_pipeline_threads_.reset(nullptr);
    }
    if (primary_source_stream_) {
        primary_source_stream_->close();
        primary_source_stream_.reset(nullptr);
    }
    audio_device_.pause(); // Pause audio
    primary_resource_path_.clear();
    video_codec_ctx_ = av_codec_context{};
    audio_codec_ctx_ = av_codec_context{};
}

void player::close_async()
{
    open_thread_ = std::make_unique<std::thread>( [=,this]() { this->close(); } );
    open_thread_->detach();
}

void player::cancel_async_open() {
    set_stream_state(stream_state_t::inactive);
    open_thread_.reset(nullptr);
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
    const auto cur_stream_state_int = to_int(stream_state());
    return cur_stream_state_int >= to_int(stream_state_t::playing);
}

void player::set_stream_state(stream_state_t stream_state)
{
    if (!primary_source_stream_) {
        primary_stream().stream_state() = stream_state;;
    }
}

stream_state_t player::stream_state() const
{
    if (!primary_source_stream_) {
        return stream_state_t::inactive;
    }
    return primary_stream().stream_state();
}

// ----------------
// --- Contexts ---
// ----------------
av_codec_context& player::video_codec_context() const
{
    if (!primary_source_stream_) {
        std::cerr << "LOG_CRITICAL: create_video_codec_ctx(), primary_source_stream_ is nullptr\n";
        return video_codec_ctx_;
    }

    std::scoped_lock<std::mutex> lock(video_codec_mutex_);
    if (!video_codec_ctx_.is_valid()) {
        auto* video_stream = source_stream(stream_type_t::video);
        if (video_stream){
            video_codec_ctx_ = video_stream->codec_context(video_stream_index());
            update_scaling_context();
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

    std::scoped_lock<std::mutex> lock(audio_codec_mutex_);
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

void player::video_dimensions_set(const surface_dimensions_t& dimensions)
{
    if (video_render_) {
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

// -------------------------------
// --- Subtitles setup/control ---
// -------------------------------
/// @todo If called outside of the selectable_subtitles() vector some thing will look strange at the very least in the GUI
void player::open_subtitle(const std::string& subtitle_path, const std::string& language_code)
{
    subtitle_downloader_thread_.enqueue_subtitle(subtitle_path, language_code);
}


/// @todo Do implement correctly
/// @todo set subtitle also when using subtitle_source_t::stream
void player::subtitle_select(const std::string& language_code)
{
    std::cerr << "FIXMENM  Subtitle select: '" << language_code << "'\n";
    const int32_t sel_index = playable_.selectable_subtitle_index_of(language_code);
    subtitle_select(sel_index);
}

void player::subtitle_select(int32_t selectable_subtitle_index)
{
    subtitle_source_ = subtitle_source_t::none;
    std::string language_code = "";
    if (set_subtitle_helper(selectable_subtitle_index)) {
        const auto index = static_cast<uint32_t>(selectable_subtitle_index);
        auto entry = selectable_subtitles().at(index);
        if (entry.source == subtitle_source_t::text_file) {
            if (!entry.path.empty()) {
                subtitle_source_ = subtitle_source_t::text_file;
                open_subtitle(entry.path, language_code);
                return;
            }
        }
        else if (entry.source == subtitle_source_t::stream) {
            if (primary_source_stream_) {
                subtitle_source_ = subtitle_source_t::stream;
                subtitle_stream_index_set(entry.stream_index);
                return;
            }
        }
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

void player::subtitle_stream_index_set(size_t stream_index)
{
    std::scoped_lock<std::mutex> lock(subtitle_codec_mutex_);
    if (primary_source_stream_) {
        primary_source_stream_->subtitle_index_set(stream_index);
        subtitle_stream_index_ = stream_index;
        subtitle_codec_ctx_ = av_codec_context(); // Reset the subtitle code context
    }
    else {
        subtitle_stream_index_ = no_stream_index;
    }
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

/// @todo: Perhaps we do not need to call playable_update_calculated() every frame
void player::frame_update()
{
    handle_internal_events();
    if (is_playing()) {

//        const bool force = false;
//        playable_update_calculated(force);

        if (has_video_stream()) {
            video_frame_update(next_video_frame_);
            check_activate_subtitle();
        }
        cur_media_time().release_after_reset();
    }
    if (video_render_ && show_stream_state()) {
        video_render_->render_stream_state();
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
    if (media_pipeline_threads_) {
        media_pipeline_threads_->pause_playback();
    }
}

void player::resume_playback()
{
    cur_media_time_.resume_time();
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

bool player::open_primary_stream(const std::string& resource_path)
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
    if (stream_state_reference().compare_exchange_strong(stream_state_expected, stream_state_t::playing)) {
        start_playing(start_time_pos_);
        if (cb_start_playing_) { cb_start_playing_(); }
    }
}

void player::torrent_finished_event(std::shared_ptr<cpaf::torrent::torrent> tor_file)
{
    if (tor_file) {
        auto stream_state_expected = stream_state_t::playing;
        stream_state_reference().compare_exchange_strong(stream_state_expected, stream_state_t::playing_local);
    }

}

void player::on_configuration_changed()
{
    std::cerr << "FIXMENM player::on_configuration_changed()\n";
    const bool force = true;
    playable_update_calculated(force);
}

void player::playable_update_calculated(bool force)
{
    if (primary_source_stream_) {
        playable_.update_calculated(tr(), &primary_source_stream_->streams_info(), force);
    }
    else {
        playable_.update_calculated(tr(), nullptr, force);
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

bool player::show_stream_state() const
{
    const auto ss = stream_state();
    return ss == stream_state_t::opening || ss == stream_state_t::waiting_for_data;
}

bool player::set_subtitle_helper(int32_t selectable_subtitle_index)
{
    subtitle_selected_index_ = std::clamp(selectable_subtitle_index, -1, (static_cast<int32_t>(selectable_subtitles().size() -1)));
    bool show_subtitles = subtitle_selected_index_ > -1;
    configuration.bool_set("subtitles", "show", show_subtitles);
    return show_subtitles;
}

} // END namespace cpaf::gui::video
