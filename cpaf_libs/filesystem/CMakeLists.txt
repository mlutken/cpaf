set (library_name cpaf_filesystem)
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_file_directory_functions.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_special_dirs.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_file_directory_functions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/${CPAF_SYSTEM_NAME}/file_directory_functions_${CPAF_SYSTEM_NAME}.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_special_dirs.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_link_libraries(${library_name} "cpaf_config;cpaf_utils")
target_link_libraries(${library_name} Boost::filesystem Boost::system Boost::regex)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE ${Boost_INCLUDE_DIRS} ${PARENT_DIR} )
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

