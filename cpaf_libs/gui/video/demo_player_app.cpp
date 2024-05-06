#include "demo_player_app.h"

#include <fmt/format.h>
#include <imguipp/imgui_rai.h>
#include <cpaf_libs/gui/system.h>
#include <cpaf_libs/gui/system_render.h>
#include <cpaf_libs/time/cpaf_time.h>
#include <cpaf_libs/gui/images/png_image.h>
#include <cpaf_libs/gui/assets/icons/icons_data.h>
#include <cpaf_libs/gui/video/data_definitions/playlist.h>
#include <cpaf_libs/gui/video/data_definitions/playable.h>
#include <cpaf_libs/system/cpaf_system_info.h>
#include <cpaf_libs/utils/cpaf_json_utils.h>
// #include <mooweelib/moowee_debug_utils.h>

#include <cpaf_libs/gui/video/ui/assets/video_player_data.h> // FIXMENM


using namespace std::chrono_literals;
using namespace std::chrono;
using namespace std;
using namespace cpaf;
using namespace cpaf::gui;
using namespace cpaf::gui::events;
using namespace cpaf::time;
using namespace cpaf::video;

namespace fs = std::filesystem;


namespace cpaf::gui::video {
// --------------------------
// --- demo_event_handler ---
// --------------------------

demo_event_handler::demo_event_handler(cpaf::gui::video::player& player)
    : player_(player)
{
}

cpaf::gui::events::is_handled demo_event_handler::handle_event(const cpaf::gui::events::event& evt)
{
    events::is_handled handled = events::is_handled::no;

    if (auto evt_ptr = evt.as<events::window>(); evt_ptr != nullptr) {
        handle_window_event(*evt_ptr);
    }
    else if (auto evt_ptr = evt.as<events::keyboard>(); evt_ptr != nullptr) {
        handle_keyboard_event(*evt_ptr);
    }

    return handled;
}

events::is_handled demo_event_handler::handle_window_event(const cpaf::gui::events::window& evt)
{
    events::is_handled handled = events::is_handled::no;
    if (evt.tp == events::window::type::resized) {
        player_.player_geometry_set(evt.width, evt.height);
    }
    std::cout << std::flush;

    return handled;
}

events::is_handled demo_event_handler::handle_keyboard_event(const cpaf::gui::events::keyboard& evt)
{
    events::is_handled handled = events::is_handled::no;
    if (evt.tp == events::keyboard::type::pressed) {
        return handle_key_pressed(evt);
        // player_.video_dimensions_set(evt.width, evt.height);
    }
    //    std::cout << std::flush;

    return handled;
}

events::is_handled demo_event_handler::handle_key_pressed(const cpaf::gui::events::keyboard& evt)
{
    is_handled handled = is_handled::yes;
    switch (evt.k) {
    case keyboard::key::space:  player_.toggle_pause_playback(); break;
    case keyboard::key::p:      player_.pause_playback(); break;
    case keyboard::key::c:      player_.resume_playback(); break;
    case keyboard::key::up:     player_.seek_relative(skip_time_large_); break;
    case keyboard::key::down:   player_.seek_relative(-skip_time_large_); break;
    case keyboard::key::left:   player_.seek_relative(-skip_time_small_); break;
    case keyboard::key::right:  player_.seek_relative(skip_time_small_); break;
    default:
        break;
        handled = is_handled::no;
    }
    return handled;
}


// -----------------------
// --- demo_player_app ---
// -----------------------

demo_player_app::demo_player_app()
    : cpaf::gui::app()
    , audio_device_{}
    , player_{audio_device_, tr()}
    , player_event_handler_{player_}
{
    player_.cur_media_time().video_offset_set(-300ms);
    initial_window_size_set({1200,800});
    if (!audio_device_.open()){
        std::cerr << "ERROR: [demo_player_app] Could not open audio device\n";
    }

    player_.configuration.connect_for_changes([this]() {on_configuration_changed();});

    on_configuration_changed(); // Read in default configuration

///    audio_device_.play_callback_set(player_.audio_callback_get());
///    player_.audio_out_formats_set(to_ff_audio_format(audio_device_.audio_format()));
    player_.cb_start_playing([this]() {
        // dbg_print();
    });
}

void demo_player_app::set_initial_playlist(const nlohmann::json& playlist)
{
    playlist_ = playlist;

    // std::cerr << playlist_.dump(4) << "\n"; // FIXMENM
}

void demo_player_app::set_initial_start_time(std::chrono::microseconds media_start_time_pos)
{
    media_start_time_pos_ = media_start_time_pos;
}

void demo_player_app::open_path(const string& resource_path)
{
    player_.open_media(resource_path, media_start_time_pos_);
///    auto playab = cpaf::gui::video::playable(resource_path);
///    return open_playable(playab.json());
}

void demo_player_app::open_playable_or_list(const nlohmann::json& playable_or_list)
{
    const auto data_type= playable_or_list["data_type"].get<std::string>();
    nlohmann::json playable;
    if (data_type == "playable") {
        open_playable(playable_or_list);
    }
    else if (data_type == "playlist" && !playable_or_list["items"].empty()) {
        open_playlist(playable_or_list);
    }

    std::cerr << "LOG_ERR: open unknown data_type: '" << data_type << "'\n";
}

void demo_player_app::open_playable(const cpaf::gui::video::playable& playab)
{
    player_.open_media(playab);
}

void demo_player_app::open_playable(const nlohmann::json& playable_jo)
{
    open_playable(cpaf::gui::video::playable(playable_jo));
}

bool demo_player_app::open_playlist(const nlohmann::json& playlist)
{
    const auto data_type= playlist["data_type"].get<std::string>();
    if (data_type != "playable") {
        return false;
    }

    playlist_ = playlist;
    playlist_play_item(0);
    return true;
}

bool demo_player_app::playlist_play_item(uint32_t pl_index)
{
    if (!is_valid_playlist_index(pl_index)) {
        return false;
    }

    playlist_current_index_ = pl_index;
    // TODO FIXMENM
    auto playab = cpaf::gui::video::playable(playlist_["items"][playlist_current_index_]);
    if (!playab.is_valid()) {
        return false;
    }

    // player_.open_async(playab.path(), playab.start_time());
    player_.open_media(playab);
    return true;
}

bool demo_player_app::playlist_play_next()
{
    const auto index = playlist_get_next_play_index();
    if (index == playlist_current_index_) {
        return false;
    }
    return playlist_play_item(index);
}

bool demo_player_app::playlist_play_prev()
{
    const auto index = playlist_get_prev_play_index();
    if (index == playlist_current_index_) {
        return false;
    }
    return playlist_play_item(index);
}

void demo_player_app::dbg_print() const
{
    cerr << "AV_CH_LAYOUT_STEREO    : " << AV_CH_LAYOUT_STEREO << "\n";
    cerr << "Primary resource path  : " << player_.primary_resource_path() << "\n";
    cerr << "Video resource path    : " << player_.video_resource_path() << "\n";
    cerr << "Audio resource path    : " << player_.audio_resource_path() << "\n";
    cerr << "streams          count : " << player_.primary_stream().streams_count() << "\n";
    cerr << "video   streams  count : " << player_.primary_stream().video_streams_count() << "\n";
    cerr << "audio   streams  count : " << player_.primary_stream().audio_streams_count() << "\n";
    cerr << "subtitle streams count : " << player_.primary_stream().subtitle_streams_count() << "\n";
    cerr << "First video stream     : " << player_.primary_stream().first_video_index() << "\n";
    cerr << "First audio stream     : " << player_.primary_stream().first_audio_index() << "\n";
    cerr << "First subtitle stream  : " << player_.primary_stream().first_subtitle_index() << "\n";
    cerr << "Primary stream index   : " << player_.primary_stream().primary_index() << "\n";
    cerr << "Primary stream media   : " << to_string(player_.primary_stream().primary_media_type()) << "\n";
    cerr << "Total time             : " << cpaf::time::format_h_m_s(player_.total_time()) << "\n";
    cerr << "sizeof(AVFrame)        : " << sizeof(AVFrame) << "\n";
    cerr << "sizeof(AVPacket)       : " << sizeof(AVPacket) << "\n";
    cerr << "AV_TIME_BASE           : " << AV_TIME_BASE << "\n";
    cerr << "Endiannes              : " << cpaf::system::endianess_str() << "\n";
    //	av_ctx.dump();


    if (player_.has_video_stream()) {
        auto& video_codec_context = player_.video_codec_context();
        cerr << "INFO: Source Video W x H            : " << video_codec_context.width() << " x " << video_codec_context.height() << "\n";
        cerr << "INFO: Source Video W x H            : " << player_.video_src_dimensions().x() << " x " << player_.video_src_dimensions().y() << "\n";
        cerr << "INFO: Dest   Video W x H            : " << player_.video_dst_dimensions().x() << " x " << player_.video_dst_dimensions().y() << "\n";
        cerr << "INFO: audio_stream_index()          : " << player_.audio_stream_index() << "\n";
    }

    auto all_selected_types_set = player_.primary_stream().format_context().set_of_each_media_type();
    cerr << "All selected mediatypes: ";
    for(const auto mt: all_selected_types_set) {
        cerr << to_string(mt) << " ";
    }
    cerr << "\n";
}

void demo_player_app::start_run()
{
    using namespace cpaf::gui;
    ImGuiStyle style;
    ImGui::StyleColorsDark(&style);
    ImGui::GetStyle() = style;
    // FIXMENM render
    // std::cerr << main_window().renderer().dbg_characteristics() << "\n";
    // dbg_text_texture_ = std::make_unique<cpaf::gui::texture>(renderer_shared());

    static const fs::path FIXMENM_bitmap_path = "/home/ml/code/crawler/moowees/sandbox/playground/img/hello.bmp";
//    static const fs::path FIXMENM_png_path = "/home/ml/code/crawler/moowees/sandbox/playground/img/circle-play.png";
    static const fs::path FIXMENM_png_path = "/home/ml/code/crawler/moowees/sandbox/playground/img/circle-pause.png";

    // cpaf::gui::png_image png;
//    png.open_local(FIXMENM_png_path);
//    png.open_from_memory(cpaf::gui::icons_data::png_data("circle_play_256"));
    // png.open_from_memory(cpaf::gui::icons_data::png_data("circle_pause_256"));
    // cerr << "FIXMENM PNG dimensions: " << png.size() << "\n";
    // cerr << "FIXMENM PNG bit depth : " << (int)png.bit_depth() << "\n";
    // cerr << "FIXMENM PNG color type: " << png.png_color_type_str() << "\n";
    ///// png.read_to_memory();




//    dbg_text_texture_->load_from_file(FIXMENM_bitmap_path);
//    dbg_text_texture_->load_from_file(FIXMENM_png_path);
//    dbg_text_texture_->load_png_from_memory(cpaf::gui::icons_data::png_data("circle_play_256"));
    // dbg_text_texture_->load_png_from_memory(cpaf::gui::video_player_data::video_player_control_icons());
    // cerr << "FIXMENM texture dimensions: " << dbg_text_texture_->size() << "\n";

//    dbg_text_texture_->set_blendmode(blendmode_t::additive);
//    dbg_text_texture_->start_surface_pixel_access(png.size());
//    dbg_text_texture_->draw_rect({{0,0}, png.size()}, color(1, 0, 0, 0.5));

//    png.copy_pixels_out(dbg_text_texture_->pixel_data_raw_ptr());

//    dbg_text_texture_->end_surface_pixel_access();



    player_.set_main_window(this->main_window());
    const std::vector<uint32_t> font_sizes{6, 9, 11, 13, 16, 20, 25, 31, 38, 45, 53, 62, 72};
    add_fonts("abeezee_regular", font_sizes);
    add_fonts(default_font(), font_sizes);
    set_default_font(default_font(),16);

    //    const auto icon_fonts_size_adjust_factor = 2.5f / 3.0f;
    //    add_fonts("abeezee_regular", "icons_fa_solid_900", icon_fonts_size_adjust_factor, ICON_MIN_FA, ICON_MAX_16_FA, font_sizes);

    if (!playlist_["items"].empty()) {
        playlist_play_item(playlist_start_play_index());
    }
}


void demo_player_app::frame_update()
{
    using namespace cpaf::time;

    // return; // FIXMENM
//    fmt::println("FIXMENM time: {} / {}", format_h_m_s(player_.current_time()), format_h_m_s(player_.remaining_time()) );
//    std::cout << std::format("FIXMENM time: {:%H:%M:%S}\n", player_.cur_media_time().current_time_pos());
//    fmt::println("FIXMENM default dpi: {}", system::instance().default_dpi());
//    std::cout << std::endl;

    player_.frame_update();
    player_.show_controls_set(ui_visible());

    render_main_menu();
    render_debug_panel();
    if (m_show_imgui_dbg_window) {
        ImGui::ShowDemoWindow();
    }

//    if (dbg_text_texture_) {
//        ImGui::Rai imrai;
//        imrai.StyleColor(ImGuiCol_Text, {0, 1, 0, 1})
//            .StyleColor(ImGuiCol_Button, {0, 0, 0, 0})
//            .StyleColor(ImGuiCol_ButtonActive, {0, 0, 0, 0})
//            .StyleColor(ImGuiCol_ButtonHovered, {0, 0, 0, 0})
//            ;

//        auto img_size = dbg_text_texture_->size();
//        auto display_size = main_window().display_size();
//        auto screen_size_fac = player_.player_to_screen_size_factor();
//        ImGui::Begin("SDL2/SDL_Renderer Texture Test");
//        ImGui::Text("pointer = %p", dbg_text_texture_.get());
//        ImGui::Text("screen_size_fac = %f x %f", screen_size_fac.width(), screen_size_fac.height());
//        ImGui::Text("display_size = %d x %d", display_size.width(), display_size.height());
//        ImGui::Text("video_dst_dimensions() = %d x %d", player_.video_dst_dimensions().width(), player_.video_dst_dimensions().height());
//        auto texture_ptr = dbg_text_texture_->native_texture<void>();
//        auto imvec = ImVec2(img_size.width()/4, img_size.height()/4);
//        ImGui::ImageButton("testbtn", texture_ptr, imvec, {0.5,0}, {0.75,0.25}, {0,0,0,0}, {1,1,1,1});
//        ImGui::End();
//    }

//    ImGui::ShowDemoWindow(); // FIXMENM

//    render_video();
    // return;
    // ImGui::DockSpaceOverViewport();


//    return; // FIXMENM demo_player_app::frame_update() NO MENU!!!!

    // Whatever GUI to implement here ...

    // ImGui::SetCursorPos({0,0});
//    if (m_show_some_panel) {
//        ImGui::SetNextWindowPos({400, 300}, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
//        ImGui::SetNextWindowSize({400, 20}, ImGuiCond_::ImGuiCond_Always);

//        // ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize
//        ImGui::Begin("Some panel", &m_show_some_panel, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
//        // ImGui::Text("Hello World dsf");
//        ImGui::TextColored(ImVec4{0,255,0,1}, "Hello World dsf");
//        ImGui::End();
//    }


    std::this_thread::sleep_for(1ms);
}

cpaf::gui::events::is_handled demo_player_app::event_handler(const cpaf::gui::events::event& evt)
{
    events::is_handled handled = player_event_handler_.handle_event(evt);
    if (handled == events::is_handled::no) {
        handled = app::event_handler(evt);
    }
    return handled;
}

void demo_player_app::render_main_menu()
{
    if (!ui_visible()) {
        return;
    }

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Mattesen Shh", "Cmd+A")) {
                open_path("/home/ml/pCloudDrive/My Videos/torplay/Anders-Matthesen-Shh.mp4");
            }
            if (ImGui::MenuItem("Sarai (audio)", "Cmd+S")) {
                open_path("/home/ml/pCloudDrive/My Videos/torplay/sarai.mp3");
            }
            if (ImGui::MenuItem("Dybbøl 1864", "Cmd+D")) {
                open_path("/home/ml/pCloudDrive/My Videos/torplay/1864.S01E01.MultiSubs.1080p.HDTV-pir8.mkv");
            }
            if (ImGui::MenuItem("Rise of The Beasts (tor)", "Cmd+R")) {
                open_path("magnet:?xt=urn:btih:C76F8A64A3CE02149872531373EA3352D2B878C4&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2F9.rarbg.to%3A2920%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337&tr=udp%3A%2F%2Ftracker.internetwarriors.net%3A1337%2Fannounce&tr=udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.pirateparty.gr%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.cyberia.is%3A6969%2Fannounce");
            }
//            if (ImGui::MenuItem("Rambo (tor)", "Cmd+R")) {
//                open_path("magnet:?xt=urn:btih:8567e2d162aba1bfd3b81cf88b257ce462eee761&dn=Rambo.First.Blood.1982.REMASTERED.1080p.BluRay.x265-RARBG&tr=http%3A%2F%2Ftracker.trackerfix.com%3A80%2Fannounce&tr=udp%3A%2F%2F9.rarbg.me%3A2890&tr=udp%3A%2F%2F9.rarbg.to%3A2890");
//            }
            if (ImGui::MenuItem("FLash (tor)", "Cmd+F")) {
                open_path("magnet:?xt=urn:btih:9F74D9D6DD01EB1B89B6EF0D1C75DCF3CDDA1DD2&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2F9.rarbg.to%3A2920%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337&tr=udp%3A%2F%2Ftracker.internetwarriors.net%3A1337%2Fannounce&tr=udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.pirateparty.gr%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.cyberia.is%3A6969%2Fannounce");
            }
            if (ImGui::MenuItem("Batman (tor)", "Cmd+B")) {
                open_path("magnet:?xt=urn:btih:A54926C2E07B0E5F0243954330B599B31C804F0B&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2F9.rarbg.to%3A2920%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337&tr=udp%3A%2F%2Ftracker.internetwarriors.net%3A1337%2Fannounce&tr=udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.pirateparty.gr%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.cyberia.is%3A6969%2Fannounce");
            }
            if (ImGui::MenuItem("Poltergeist (tor)", "Cmd+P")) {
                open_path("magnet:?xt=urn:btih:988C8A2D20E34EDBE38990D6F66573B6B7C7D6FE&dn=Poltergeist%20(2015)%201080p%20BrRip%20x264%20-%20YIFY&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3A6969%2Fannounce&tr=udp%3A%2F%2Fopen.stealth.si%3A80%2Fannounce&tr=udp%3A%2F%2Ftracker.torrent.eu.org%3A451%2Fannounce&tr=udp%3A%2F%2Ftracker.bittor.pw%3A1337%2Fannounce&tr=udp%3A%2F%2Fpublic.popcorn-tracker.org%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.dler.org%3A6969%2Fannounce&tr=udp%3A%2F%2Fexodus.desync.com%3A6969&tr=udp%3A%2F%2Fopentracker.i2p.rocks%3A6969%2Fannounce");
            }
            if (ImGui::MenuItem("Aquaman - Lost Kingdom (tor)", "Cmd+A")) {
                open_path("magnet:?xt=urn:btih:8F48713F050FF5F767C7E3B3D69F8545899A4A8E&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2F9.rarbg.to%3A2920%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337&tr=udp%3A%2F%2Ftracker.internetwarriors.net%3A1337%2Fannounce&tr=udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.pirateparty.gr%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.cyberia.is%3A6969%2Fannounce");
            }
            if (ImGui::MenuItem("Jubel - Klingande (stream video)", "Cmd+J")) {
                open_path("https://filedn.com/lcSuQ7Auh0JBqfSqAxnHdrX/thormio/Klingande%20-%20Jubel%20%28Official%20Music%20Video%29.mp4");
            }
            if (ImGui::MenuItem("Songbird - Kenny G(stream audio)", "Cmd+K")) {
                open_path("https://filedn.com/lcSuQ7Auh0JBqfSqAxnHdrX/thormio/songbird.mp3");
            }
            if (ImGui::MenuItem("Close current media", "Cmd+C")) {
                player_.close_current_media();
            }
            if (ImGui::MenuItem("Test command", "Cmd+T")) {
                player_.close_current_media();
            }

            if (ImGui::MenuItem("Exit", "Cmd+Q")) {
                stop();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Some Panel", nullptr, &m_show_some_panel);
            ImGui::MenuItem("Debug Panel", nullptr, &m_show_debug_panel);
            ImGui::MenuItem("DebugWindow", nullptr, &m_show_imgui_dbg_window);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

}

void demo_player_app::render_debug_panel()
{
    if (m_show_debug_panel) {
        ImGui::Begin("Debug panel", &m_show_debug_panel);
        // ImGui::Text("User config path: %s", config_path().string().c_str());
        // const auto adjus_secs = std::chrono::duration_cast<seconds>(player_.cur_media_time().subtitles_adjust_offset());
        // const auto subtitle_time = format_h_m_s(player_.cur_media_time().subtitles_time_pos());
        // ImGui::TextColored(ImVec4{0,255,0,1}, "Subtitle adjust time: %d -> %s", adjus_secs.count(), subtitle_time.c_str());
        // ImGui::TextColored(ImVec4{240,55,0,1}, "Front video frame time: %s  -> %f s",
        //                    format_h_m_s_ms(player_.next_video_frame().presentation_time()).c_str(),
        //                    player_.next_video_frame().presentation_time_ms().count() / 1'000.0f
        //                    );
        // ImGui::TextColored(ImVec4{140,55,77,1}, "Front audio frame time: %s  -> %f s",
        //                    format_h_m_s_ms(player_.dbg_audio_front_time()).c_str(),
        //                    player_.dbg_audio_front_time().count() / 1'000'000.0f
        //                    );
        // ImGui::Separator();
        // ImGui::Text("Time since last mouse move: %s", format_h_m_s(time_since_last_mouse_or_touch()).c_str() );

        auto video_src_dims = player_.video_src_dimensions().cast_to<float>();
        auto render_dims = player_.render_geometry().size().cast_to<float>();
        auto screen_dims = main_window_size().cast_to<float>();

        cpaf::math::v2f video_scale = render_dims / player_.video_src_dimensions_float();

        // ImGui::Text("Video src dims: %s, aspect: %f", video_src_dims.to_string().c_str(), video_src_dims.aspect_ratio<float>());
        // ImGui::Text("Render geometry: %s, aspect: %f", render_dims.to_string().c_str(), render_dims.aspect_ratio<float>());
        // ImGui::Text("Screen dims: %s, aspect: %f", screen_dims.to_string().c_str(), screen_dims.aspect_ratio<float>());
        // ImGui::Text("Video scale: %s", video_scale.to_string().c_str());
        ImGui::Text("current_io_operation_duration: %s", format_h_m_s_ms(player_.current_io_operation_duration()).c_str() );
        ImGui::Text("Is playing: %d", player_.is_playing() );
        // ImGui::Text("Global font scaling %f", io.FontGlobalScale);
        // ImGui::Text("UI scaling factor: %f", font_scaling_factor);
        ImGui::End();
    }
}

uint32_t demo_player_app::playlist_get_next_play_index() const
{
    uint32_t index = playlist_current_index_ +1;
    if (!is_valid_playlist_index(index)) {
        index = 0;
    }
    return index;
}

uint32_t demo_player_app::playlist_get_prev_play_index() const
{
    uint32_t index = playlist_current_index_;
    if (index == 0) {
        return playlist_size() - 1;
    }

    return index -1;
}

void demo_player_app::on_configuration_changed()
{
    show_ui_timeout_ = player_.configuration.time_s("controls", "show_ui_time_out");
}


} // namespace cpaf::gui::video
