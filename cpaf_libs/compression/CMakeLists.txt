set (library_name cpaf_compression)
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)

add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_compression_functions.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_compression_functions.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_link_libraries(${library_name} "cpaf_containers;cpaf_filesystem")
target_link_libraries(${library_name} Boost::regex)
target_link_libraries(${library_name} ZLIB::ZLIB)
target_link_libraries(${library_name} BZip2::BZip2)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE ${Boost_INCLUDE_DIRS} ${PARENT2_DIR} ${PARENT_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

