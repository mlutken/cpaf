set (library_name luepp_memory)

add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/allocator.hpp
    ${CMAKE_CURRENT_LIST_DIR}/shared_ptr.hpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_link_libraries(${library_name} "luepp_atomic;luepp_config")
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE ${PARENT_DIR})

add_subdirectories_standard()
