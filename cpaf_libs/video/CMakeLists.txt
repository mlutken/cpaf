set (library_name cpaf_video)
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
add_library(${library_name} STATIC "")

# --- FFmpeg ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavdevice
    libavfilter
    libavformat
    libavcodec
    libswresample
    libswscale
    libavutil
)

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/audio_resampler.h
    ${CMAKE_CURRENT_LIST_DIR}/audio_types_convert.h
    ${CMAKE_CURRENT_LIST_DIR}/av_codec.h
    ${CMAKE_CURRENT_LIST_DIR}/av_codec_context.h
    ${CMAKE_CURRENT_LIST_DIR}/av_codec_parameters.h
    ${CMAKE_CURRENT_LIST_DIR}/av_frame.h
    ${CMAKE_CURRENT_LIST_DIR}/av_format_context.h
    ${CMAKE_CURRENT_LIST_DIR}/av_options.h
    ${CMAKE_CURRENT_LIST_DIR}/av_packet.h
    ${CMAKE_CURRENT_LIST_DIR}/av_samples_buffer.h
    ${CMAKE_CURRENT_LIST_DIR}/av_samples_queue.h
    ${CMAKE_CURRENT_LIST_DIR}/av_util.h
    ${CMAKE_CURRENT_LIST_DIR}/media_stream_time.h
    ${CMAKE_CURRENT_LIST_DIR}/play_stream.h
    ${CMAKE_CURRENT_LIST_DIR}/player.h

    ${CMAKE_CURRENT_LIST_DIR}/pipeline_threads/pipeline_threads_types.h
    ${CMAKE_CURRENT_LIST_DIR}/pipeline_threads/pipeline_threads.h
    ${CMAKE_CURRENT_LIST_DIR}/pipeline_threads/audio_render_thread.h
    ${CMAKE_CURRENT_LIST_DIR}/pipeline_threads/audio_resampler_thread.h
    ${CMAKE_CURRENT_LIST_DIR}/pipeline_threads/packet_reader_thread.h
    ${CMAKE_CURRENT_LIST_DIR}/pipeline_threads/video_render_thread.h

    ${CMAKE_CURRENT_LIST_DIR}/render/render.h
    ${CMAKE_CURRENT_LIST_DIR}/render/render_base.h
    ${CMAKE_CURRENT_LIST_DIR}/render/render__sdl2.h

    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/audio_resampler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/audio_types_convert.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_codec.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_codec_context.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_codec_parameters.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_frame.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_format_context.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_options.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_packet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_samples_buffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_samples_queue.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/media_stream_time.cpp
    ${CMAKE_CURRENT_LIST_DIR}/play_stream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/player.cpp

    ${CMAKE_CURRENT_LIST_DIR}/pipeline_threads/pipeline_threads.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pipeline_threads/audio_render_thread.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pipeline_threads/audio_resampler_thread.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pipeline_threads/packet_reader_thread.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pipeline_threads/video_render_thread.cpp

    ${CMAKE_CURRENT_LIST_DIR}/render/render.cpp
    ${CMAKE_CURRENT_LIST_DIR}/render/render_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/render/render__sdl2.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_link_libraries(${library_name} "cpaf_audio;cpaf_utils;cpaf_math;cpaf_time;luepp_concurrent;luepp_containers")
target_link_libraries(${library_name} PkgConfig::FFMPEG )

set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE ${Boost_INCLUDE_DIRS} ${PARENT2_DIR} ${PARENT_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

