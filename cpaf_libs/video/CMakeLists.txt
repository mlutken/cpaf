set (library_name cpaf_video)
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
add_library(${library_name} STATIC "")

find_package(ffmpeg REQUIRED)

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
    ${CMAKE_CURRENT_LIST_DIR}/ff_audio_format_t.h
    ${CMAKE_CURRENT_LIST_DIR}/media_stream_time.h
    ${CMAKE_CURRENT_LIST_DIR}/play_stream.h
    ${CMAKE_CURRENT_LIST_DIR}/subtitle_frame.h
    ${CMAKE_CURRENT_LIST_DIR}/io/custom_io_base.h
    ${CMAKE_CURRENT_LIST_DIR}/io/torrent_io.h
    ${CMAKE_CURRENT_LIST_DIR}/io/my_file_io.h
    ${CMAKE_CURRENT_LIST_DIR}/io/subtitle_container.h

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
    ${CMAKE_CURRENT_LIST_DIR}/ff_audio_format_t.cpp
    ${CMAKE_CURRENT_LIST_DIR}/media_stream_time.cpp
    ${CMAKE_CURRENT_LIST_DIR}/play_stream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/subtitle_frame.cpp

    ${CMAKE_CURRENT_LIST_DIR}/io/custom_io_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/io/torrent_io.cpp
    ${CMAKE_CURRENT_LIST_DIR}/io/my_file_io.cpp
    ${CMAKE_CURRENT_LIST_DIR}/io/subtitle_container.cpp


)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_link_libraries(${library_name} "cpaf_crypto")
target_link_libraries(${library_name} "cpaf_net")
target_link_libraries(${library_name} "cpaf_streams")
target_link_libraries(${library_name} "cpaf_audio")
target_link_libraries(${library_name} "cpaf_unicode;")
target_link_libraries(${library_name} "cpaf_utils")
target_link_libraries(${library_name} "cpaf_math")
target_link_libraries(${library_name} "cpaf_time")
target_link_libraries(${library_name} "luepp_concurrent")
target_link_libraries(${library_name} "luepp_containers")
target_link_libraries(${library_name} "cpaf_torrent")
target_link_libraries(${library_name} "cpaf_compression")
target_link_libraries(${library_name} ffmpeg::ffmpeg )

set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE ${Boost_INCLUDE_DIRS} ${PARENT2_DIR} ${PARENT_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

