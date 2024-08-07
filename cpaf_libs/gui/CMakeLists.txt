
set (library_name cpaf_gui)
add_library(${library_name} STATIC "")

set (CPAF_GUI_USE_IMGUI False)
if ( (CPAF_GRAPHICS_SYSTEM_NAME MATCHES "sdl2") )
    set (CPAF_GUI_USE_IMGUI True)
endif()

if ( CPAF_GUI_USE_IMGUI )
    find_package(imgui REQUIRED)
    set (IMGUI_BACKEND_DIR "${CMAKE_CURRENT_LIST_DIR}/imgui_backends/backends")
endif()

find_package(fmt REQUIRED)
find_package(PNG REQUIRED)

set (CPAF_GUI_LINK_LIBS "")
set (CPAF_GUI_INCLUDE_DIRS "")
set (CPAF_GUI_IMGUI_BACKEND_CPP_FILES "")


if ( (CPAF_GRAPHICS_SYSTEM_NAME MATCHES "sdl2") )
    set (CPAF_GUI_LINK_LIBS "cpaf_sdl2")
    set (CPAF_GUI_IMGUI_BACKEND_CPP_FILES "${IMGUI_BACKEND_DIR}/imgui_impl_sdl.cpp;${IMGUI_BACKEND_DIR}/imgui_impl_sdlrenderer.cpp")

    target_sources(${library_name}
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/platform_utils/sdl_convert.h
        ${CMAKE_CURRENT_LIST_DIR}/assets/fonts/imgui_fonts.h

        ${CMAKE_CURRENT_LIST_DIR}/video/demo_player_app.h

        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/platform_utils/sdl_convert.cpp
        ${CMAKE_CURRENT_LIST_DIR}/assets/fonts/imgui_fonts.cpp
        ${CMAKE_CURRENT_LIST_DIR}/assets/fonts/abeezee_regular.cpp
        ${CMAKE_CURRENT_LIST_DIR}/assets/fonts/abeezee_italic.cpp
        ${CMAKE_CURRENT_LIST_DIR}/assets/fonts/manrope_ttf.cpp
        ${CMAKE_CURRENT_LIST_DIR}/assets/icons/fa_regular_400.cpp
        ${CMAKE_CURRENT_LIST_DIR}/assets/icons/fa_solid_900.cpp

        ${CMAKE_CURRENT_LIST_DIR}/video/demo_player_app.cpp
    )
endif()


target_sources(${library_name}
    PUBLIC
    gui-overview.qmodel

    ${CMAKE_CURRENT_LIST_DIR}/color.h
    ${CMAKE_CURRENT_LIST_DIR}/gui_types.h

    ${CMAKE_CURRENT_LIST_DIR}/app.h
    ${CMAKE_CURRENT_LIST_DIR}/base/app_base.h
    ${CMAKE_CURRENT_LIST_DIR}/base/app__${CPAF_GUI_SYSTEM_NAME}.h

    ${CMAKE_CURRENT_LIST_DIR}/keyboard.h
    ${CMAKE_CURRENT_LIST_DIR}/base/keyboard_base.h
    ${CMAKE_CURRENT_LIST_DIR}/base/keyboard__${CPAF_GUI_SYSTEM_NAME}.h

    ${CMAKE_CURRENT_LIST_DIR}/mouse.h
    ${CMAKE_CURRENT_LIST_DIR}/base/mouse_base.h
    ${CMAKE_CURRENT_LIST_DIR}/base/mouse__${CPAF_GUI_SYSTEM_NAME}.h

    ${CMAKE_CURRENT_LIST_DIR}/system.h
    ${CMAKE_CURRENT_LIST_DIR}/base/system_base.h
    ${CMAKE_CURRENT_LIST_DIR}/base/system__${CPAF_GUI_SYSTEM_NAME}.h

    ${CMAKE_CURRENT_LIST_DIR}/system_window.h
    ${CMAKE_CURRENT_LIST_DIR}/base/system_window_base.h
    ${CMAKE_CURRENT_LIST_DIR}/base/system_window__${CPAF_GUI_SYSTEM_NAME}.h

    ${CMAKE_CURRENT_LIST_DIR}/system_render.h
    ${CMAKE_CURRENT_LIST_DIR}/base/system_render_base.h
    ${CMAKE_CURRENT_LIST_DIR}/base/system_render__${CPAF_GUI_SYSTEM_NAME}.h

    ${CMAKE_CURRENT_LIST_DIR}/texture.h
    ${CMAKE_CURRENT_LIST_DIR}/base/texture_base.h
    ${CMAKE_CURRENT_LIST_DIR}/base/texture__${CPAF_GUI_SYSTEM_NAME}.h

    ${CMAKE_CURRENT_LIST_DIR}/video/ui/controls_default.h
    ${CMAKE_CURRENT_LIST_DIR}/video/ui/controls.h
    ${CMAKE_CURRENT_LIST_DIR}/video/ui/controls_default__${CPAF_GUI_SYSTEM_NAME}.h

    ${CMAKE_CURRENT_LIST_DIR}/video/render/render.h
    ${CMAKE_CURRENT_LIST_DIR}/video/render/render_base.h
    ${CMAKE_CURRENT_LIST_DIR}/video/render/render__${CPAF_GUI_SYSTEM_NAME}.h

    ${CMAKE_CURRENT_LIST_DIR}/events/base_events.h
    ${CMAKE_CURRENT_LIST_DIR}/events/display_events.h
    ${CMAKE_CURRENT_LIST_DIR}/events/joystick_events.h
    ${CMAKE_CURRENT_LIST_DIR}/events/keyboard_events.h
    ${CMAKE_CURRENT_LIST_DIR}/events/mouse_events.h
    ${CMAKE_CURRENT_LIST_DIR}/events/sensor_events.h
    ${CMAKE_CURRENT_LIST_DIR}/events/touch_events.h
    ${CMAKE_CURRENT_LIST_DIR}/events/events__${CPAF_GUI_SYSTEM_NAME}.h

    ${CMAKE_CURRENT_LIST_DIR}/fonts/font_size.h

    ${CMAKE_CURRENT_LIST_DIR}/images/png_image.h

    ${CMAKE_CURRENT_LIST_DIR}/video/config.h
    ${CMAKE_CURRENT_LIST_DIR}/video/data_definitions/playable.h
    ${CMAKE_CURRENT_LIST_DIR}/video/data_definitions/playlist.h
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/pipeline_threads_types.h
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/pipeline_threads.h
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/audio_render_thread.h
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/audio_resampler_thread.h
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/packet_reader_thread.h
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/play_handler_thread.h
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/subtitle_downloader_thread.h
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/subtitle_reader_thread.h
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/video_render_thread.h

    ${CMAKE_CURRENT_LIST_DIR}/video/player.h
    ${CMAKE_CURRENT_LIST_DIR}/video/playlist_control.h

    ${CMAKE_CURRENT_LIST_DIR}/video/ui/assets/video_player_data.h

    ${CMAKE_CURRENT_LIST_DIR}/utils.h

    ${CMAKE_CURRENT_LIST_DIR}/assets/icons/icons_data.h

    PRIVATE
    ${CPAF_GUI_IMGUI_BACKEND_CPP_FILES}

    ${CMAKE_CURRENT_LIST_DIR}/color.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui_types.cpp

    ${CMAKE_CURRENT_LIST_DIR}/app.cpp
    ${CMAKE_CURRENT_LIST_DIR}/base/app_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/base/app__${CPAF_GUI_SYSTEM_NAME}.cpp

    ${CMAKE_CURRENT_LIST_DIR}/keyboard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/base/keyboard_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/base/keyboard__${CPAF_GUI_SYSTEM_NAME}.cpp

    ${CMAKE_CURRENT_LIST_DIR}/mouse.cpp
    ${CMAKE_CURRENT_LIST_DIR}/base/mouse_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/base/mouse__${CPAF_GUI_SYSTEM_NAME}.cpp

    ${CMAKE_CURRENT_LIST_DIR}/system.cpp
    ${CMAKE_CURRENT_LIST_DIR}/base/system_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/base/system__${CPAF_GUI_SYSTEM_NAME}.cpp

    ${CMAKE_CURRENT_LIST_DIR}/system_window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/base/system_window_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/base/system_window__${CPAF_GUI_SYSTEM_NAME}.cpp

    ${CMAKE_CURRENT_LIST_DIR}/system_render.cpp
    ${CMAKE_CURRENT_LIST_DIR}/base/system_render_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/base/system_render__${CPAF_GUI_SYSTEM_NAME}.cpp

    ${CMAKE_CURRENT_LIST_DIR}/texture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/base/texture_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/base/texture__${CPAF_GUI_SYSTEM_NAME}.cpp

    ${CMAKE_CURRENT_LIST_DIR}/video/ui/controls_default.cpp
    ${CMAKE_CURRENT_LIST_DIR}/video/ui/controls.cpp
    ${CMAKE_CURRENT_LIST_DIR}/video/ui/controls_default__${CPAF_GUI_SYSTEM_NAME}.cpp

    ${CMAKE_CURRENT_LIST_DIR}/video/render/render.cpp
    ${CMAKE_CURRENT_LIST_DIR}/video/render/render_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/video/render/render__${CPAF_GUI_SYSTEM_NAME}.cpp

    ${CMAKE_CURRENT_LIST_DIR}/events/event.cpp
    ${CMAKE_CURRENT_LIST_DIR}/events/base_events.cpp
    ${CMAKE_CURRENT_LIST_DIR}/events/display_events.cpp
    ${CMAKE_CURRENT_LIST_DIR}/events/joystick_events.cpp
    ${CMAKE_CURRENT_LIST_DIR}/events/keyboard_events.cpp
    ${CMAKE_CURRENT_LIST_DIR}/events/mouse_events.cpp
    ${CMAKE_CURRENT_LIST_DIR}/events/sensor_events.cpp
    ${CMAKE_CURRENT_LIST_DIR}/events/touch_events.cpp
    ${CMAKE_CURRENT_LIST_DIR}/events/events__${CPAF_GUI_SYSTEM_NAME}.cpp

    ${CMAKE_CURRENT_LIST_DIR}/fonts/font_size.cpp
    ${CMAKE_CURRENT_LIST_DIR}/fonts/font_size__${CPAF_PLATFORM_TYPE_NAME}.cpp

    ${CMAKE_CURRENT_LIST_DIR}/images/png_image.cpp

    ${CMAKE_CURRENT_LIST_DIR}/video/config.cpp
    ${CMAKE_CURRENT_LIST_DIR}/video/data_definitions/playable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/video/data_definitions/playlist.cpp
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/pipeline_threads.cpp
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/audio_render_thread.cpp
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/audio_resampler_thread.cpp
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/packet_reader_thread.cpp
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/play_handler_thread.cpp
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/subtitle_downloader_thread.cpp
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/subtitle_reader_thread.cpp
    ${CMAKE_CURRENT_LIST_DIR}/video/pipeline_threads/video_render_thread.cpp

    ${CMAKE_CURRENT_LIST_DIR}/video/player.cpp
    ${CMAKE_CURRENT_LIST_DIR}/video/playlist_control.cpp

    ${CMAKE_CURRENT_LIST_DIR}/video/ui/assets/video_player_control_icons.cpp

    ${CMAKE_CURRENT_LIST_DIR}/utils.cpp

    ${CMAKE_CURRENT_LIST_DIR}/assets/icons/icons_data.cpp

    ${CMAKE_CURRENT_LIST_DIR}/assets/icons/256/circle_pause_256.cpp
    ${CMAKE_CURRENT_LIST_DIR}/assets/icons/256/circle_play_256.cpp
)


get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)

if ( CPAF_GUI_USE_IMGUI )
    target_link_libraries(${library_name} ImGuiFileDialog )
    target_link_libraries(${library_name} imgui::imgui )
    target_link_libraries(${library_name} imguipp )
endif()

target_link_libraries(${library_name} cpaf_config )
target_link_libraries(${library_name} cpaf_locale )
target_link_libraries(${library_name} cpaf_math )
target_link_libraries(${library_name} cpaf_system )
target_link_libraries(${library_name} cpaf_filesystem )
target_link_libraries(${library_name} cpaf_video )
target_link_libraries(${library_name} ${CPAF_GUI_LINK_LIBS} )
target_link_libraries(${library_name} PNG::PNG )
target_link_libraries(${library_name} IconFontCppHeaders )
target_link_libraries(${library_name} fmt::fmt)
target_include_directories(${library_name} INTERFACE  ${PARENT2_DIR} ${PARENT_DIR})
target_include_directories(${library_name} PRIVATE ${CPAF_GUI_INCLUDE_DIRS})
target_include_directories(${library_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/imgui_backends )
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

