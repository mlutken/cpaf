set (library_name cpaf_time)
find_package(Boost COMPONENTS date_time REQUIRED)
add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_time.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_time.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${library_name} PUBLIC Boost::date_time)
target_link_libraries(${library_name} PUBLIC cpaf_unicode)
target_include_directories(${library_name} INTERFACE ${PARENT2_DIR} ${PARENT_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

