set (library_name cpaf_torrent)
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(LibtorrentRasterbar REQUIRED)
# Compiling openSSL for webasm/emscripten: https://github.com/BOINC/boinc/issues/3086
add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/server.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/server.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_link_libraries(${library_name} "cpaf_utils")
target_link_libraries(${library_name} LibtorrentRasterbar::torrent-rasterbar)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE ${Boost_INCLUDE_DIRS} ${PARENT2_DIR} ${PARENT_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

