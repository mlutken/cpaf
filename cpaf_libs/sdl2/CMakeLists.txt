set (library_name cpaf_sdl2)

if ( NOT (CMAKE_SYSTEM_NAME MATCHES "Emscripten") )
find_package(SDL2 REQUIRED)
endif()

add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_sdl2.h
    ${CMAKE_CURRENT_LIST_DIR}/sdl2_utils.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_sdl2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/sdl2_utils.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_link_libraries(${library_name} PUBLIC "cpaf_math" )
target_link_libraries(${library_name} PUBLIC "${SDL2_LIBRARIES}" )
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE ${Centurion_INCLUDE_DIRS} ${PARENT2_DIR} ${PARENT_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})


add_subdirectories_standard()

