set (library_name cpaf_torrent)
find_package(OpenSSL REQUIRED)
find_package(fmt REQUIRED)
find_package(LibtorrentRasterbar REQUIRED)
# Compiling openSSL for webasm/emscripten: https://github.com/BOINC/boinc/issues/3086
add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/file.h
    ${CMAKE_CURRENT_LIST_DIR}/streaming_cache.h
    ${CMAKE_CURRENT_LIST_DIR}/torrent.h
    ${CMAKE_CURRENT_LIST_DIR}/torrents.h
    ${CMAKE_CURRENT_LIST_DIR}/torrent_utils.h

    ${CMAKE_CURRENT_LIST_DIR}/temp_storage.h

    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/file.cpp
    ${CMAKE_CURRENT_LIST_DIR}/streaming_cache.cpp
    ${CMAKE_CURRENT_LIST_DIR}/torrent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/torrents.cpp
    ${CMAKE_CURRENT_LIST_DIR}/torrent_utils.cpp

    ${CMAKE_CURRENT_LIST_DIR}/temp_storage.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_link_libraries(${library_name} "cpaf_utils")
target_link_libraries(${library_name} luepp_concurrent)
target_link_libraries(${library_name} LibtorrentRasterbar::torrent-rasterbar)
target_link_libraries(${library_name} fmt::fmt)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE ${PARENT2_DIR} ${PARENT_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

