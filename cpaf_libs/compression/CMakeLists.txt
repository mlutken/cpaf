set (library_name cpaf_compression)
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)
# find_package(Zippy REQUIRED)

add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_compression_functions.h
    ${CMAKE_CURRENT_LIST_DIR}/compression_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/file.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_compression_functions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/compression_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/file.cpp
)


##message("FIXMENM Boost::regex: '${Boost::regex}'")
#message("FIXMENM ZLIB::ZLIB  : '${ZLIB_LIBRARIES}'")
#message("FIXMENM ZLIB_INCLUDE_DIRS  : '${ZLIB_INCLUDE_DIRS}'")
#message("FIXMENM BZip2::BZip2  : '${BZip2_LIBRARIES}'")
#message("FIXMENM BZip2::INC  : '${BZip2_INCLUDE_DIRS}'")
#message("FIXMENM ZLIB_LIBRARIES  : '${ZLIB_LIBRARIES}'")
##message("FIXMENM BZip2::BZip2: '${BZip2::BZip2}'")
#message("FIXMENM Boost_INCLUDE_DIRS: '${Boost_INCLUDE_DIRS}'")


get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
##target_link_libraries(${library_name} PUBLIC "BZip2::BZip2;cpaf_containers")
target_link_libraries(${library_name} PUBLIC cpaf_containers)
target_link_libraries(${library_name} PUBLIC cpaf_filesystem)
target_link_libraries(${library_name} PUBLIC ZLIB::ZLIB)
target_link_libraries(${library_name} PUBLIC BZip2::BZip2)
# target_link_libraries(${library_name} PUBLIC Zippy::Zippy)
target_link_libraries(${library_name} PUBLIC KZip::KZip)
#target_link_libraries(${library_name}  PUBLIC ${ZLIB_LIBRARIES})
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE  ${PARENT2_DIR} ${PARENT_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

