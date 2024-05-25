set (library_name cpaf_streams)
find_package(Boost COMPONENTS iostreams REQUIRED)
find_package(ZLIB REQUIRED)
add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_streams.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_stream_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_json_streams.h

    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_streams.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_stream_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_json_streams.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_link_libraries(${library_name} "cpaf_compression;cpaf_containers;cpaf_unicode;cpaf_utils")
target_link_libraries(${library_name} Boost::iostreams)
target_link_libraries(${library_name} ZLIB::ZLIB)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE ${Boost_INCLUDE_DIRS} ${PARENT2_DIR} ${PARENT_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

