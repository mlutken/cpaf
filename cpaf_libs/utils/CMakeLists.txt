find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
find_package(nlohmann_json )

set (library_name cpaf_utils)
add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_debug_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_parse_numbers_generic.hpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_stl_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_string_utils.hpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_json_utils.h

    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_debug_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_string_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_json_utils.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} PUBLIC ${Boost_INCLUDE_DIRS} ${PARENT2_DIR} ${PARENT_DIR})

# TODO: luepp_develop is a staging area until ready to move the code to luepp!!
target_include_directories(${library_name} PUBLIC ${PARENT_DIR}/luepp_develop)

target_link_libraries(${library_name} nlohmann_json::nlohmann_json)
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()
