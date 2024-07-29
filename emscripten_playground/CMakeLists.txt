set (library_name emscripten_playground)
add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/emscripten_playground.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/emscripten_playground.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_link_libraries(${library_name} "pthread")
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE ${PARENT2_DIR} ${PARENT_DIR} )
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

if ( (CPAF_GRAPHICS_SYSTEM_NAME MATCHES "sdl2") )
    target_link_libraries(${library_name} "cpaf_sdl2")
endif()


add_subdirectories_standard()
