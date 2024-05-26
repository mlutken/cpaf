set (library_name cpaf_video)
add_library(${library_name} STATIC "")

#  ffmpeg: https://github.com/Microsoft/vcpkg/issues/1379

# Not working: See cpaf/local-vcpkg-ports/ffmpeg/FindFFMPEG.cmake.in
#
# set(ENABLE_AVDEVICE OFF) # FFMPEG_FIND(libavdevice   avdevice   avdevice.h)
# set(ENABLE_AVFILTER OFF) # FFMPEG_FIND(libavfilter   avfilter   avfilter.h)
# set(ENABLE_AVFORMAT ON) # FFMPEG_FIND(libavformat   avformat   avformat.h)
# set(ENABLE_AVCODEC ON) # FFMPEG_FIND(libavcodec    avcodec    avcodec.h)
# set(ENABLE_POSTPROC OFF) # FFMPEG_FIND(libpostproc   postproc   postprocess.h)
# set(ENABLE_SWRESAMPLE ON) # FFMPEG_FIND(libswresample swresample swresample.h)
# set(ENABLE_SWSCALE ON) # FFMPEG_FIND(libswscale    swscale    swscale.h)

find_package(FFMPEG REQUIRED)

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/audio_resampler.h
    ${CMAKE_CURRENT_LIST_DIR}/audio_types_convert.h
    ${CMAKE_CURRENT_LIST_DIR}/av_codec_context.h
    ${CMAKE_CURRENT_LIST_DIR}/av_frame.h
    ${CMAKE_CURRENT_LIST_DIR}/av_format_context.h
    ${CMAKE_CURRENT_LIST_DIR}/av_options.h
    ${CMAKE_CURRENT_LIST_DIR}/av_packet.h
    ${CMAKE_CURRENT_LIST_DIR}/av_samples_buffer.h
    ${CMAKE_CURRENT_LIST_DIR}/av_samples_queue.h
    ${CMAKE_CURRENT_LIST_DIR}/av_util.h
    ${CMAKE_CURRENT_LIST_DIR}/ff_audio_format_t.h
    ${CMAKE_CURRENT_LIST_DIR}/media_stream_time.h
    ${CMAKE_CURRENT_LIST_DIR}/play_stream.h
    ${CMAKE_CURRENT_LIST_DIR}/subtitle_frame.h
    ${CMAKE_CURRENT_LIST_DIR}/io/custom_io_base.h
    ${CMAKE_CURRENT_LIST_DIR}/io/torrent_io.h
    ${CMAKE_CURRENT_LIST_DIR}/io/my_file_io.h
    ${CMAKE_CURRENT_LIST_DIR}/io/subtitle_container.h
    ${CMAKE_CURRENT_LIST_DIR}/io/subtitle_container.h

    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/audio_resampler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/audio_types_convert.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_codec_context.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_frame.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_format_context.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_options.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_packet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_samples_buffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_samples_queue.cpp
    ${CMAKE_CURRENT_LIST_DIR}/av_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ff_audio_format_t.cpp
    ${CMAKE_CURRENT_LIST_DIR}/media_stream_time.cpp
    ${CMAKE_CURRENT_LIST_DIR}/play_stream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/subtitle_frame.cpp

    ${CMAKE_CURRENT_LIST_DIR}/io/custom_io_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/io/torrent_io.cpp
    ${CMAKE_CURRENT_LIST_DIR}/io/my_file_io.cpp
    ${CMAKE_CURRENT_LIST_DIR}/io/subtitle_container.cpp
    ${CMAKE_CURRENT_LIST_DIR}/io/subtitle_text_file_data.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_link_libraries(${library_name} PUBLIC "cpaf_locale")
target_link_libraries(${library_name} PUBLIC "cpaf_crypto")
target_link_libraries(${library_name} PUBLIC "cpaf_net")
target_link_libraries(${library_name} PUBLIC "cpaf_streams")
target_link_libraries(${library_name} PUBLIC "cpaf_audio")
target_link_libraries(${library_name} PUBLIC "cpaf_unicode;")
target_link_libraries(${library_name} PUBLIC "cpaf_utils")
target_link_libraries(${library_name} PUBLIC "cpaf_math")
target_link_libraries(${library_name} PUBLIC "cpaf_time")
target_link_libraries(${library_name} PUBLIC "luepp_concurrent")
target_link_libraries(${library_name} PUBLIC "luepp_containers")
target_link_libraries(${library_name} PUBLIC "cpaf_torrent")
target_link_libraries(${library_name} PUBLIC "cpaf_compression")

# Remove strange non libs added from the vcpkg ffmpeg recipe
list(REMOVE_ITEM FFMPEG_LIBRARIES optimized )
list(REMOVE_ITEM FFMPEG_LIBRARIES debug )

message ("FFMPEG_INCLUDE_DIRS: ${FFMPEG_INCLUDE_DIRS}")
message ("FFMPEG_LIBRARY_DIRS: ${FFMPEG_LIBRARY_DIRS}")
message ("FFMPEG_LIBRARY_DIRS: ${FFMPEG_LIBRARY_DIRS}")
message ("FFMPEG_LIBRARY_DIRS: ${FFMPEG_LIBRARY_DIRS}")
message ("FFMPEG_LIBRARIES: ${FFMPEG_LIBRARIES}")

target_include_directories( ${library_name} PUBLIC "${FFMPEG_INCLUDE_DIRS}")
target_link_directories( ${library_name} PUBLIC "${FFMPEG_LIBRARY_DIRS}")
target_link_libraries( ${library_name} PUBLIC "${FFMPEG_LIBRARIES}")

### target_link_libraries(${library_name} ffmpeg::ffmpeg )

set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

