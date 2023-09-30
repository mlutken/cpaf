
set (library_name cpaf_graphics)
add_library(${library_name} STATIC "")

set (CPAF_GRAPHICS_LINK_LIBS "")
set (CPAF_GRAPHICS_INCLUDE_DIRS "")

if ( (CPAF_GRAPHICS_SYSTEM_NAME MATCHES "sdl2") )
    set (CPAF_GRAPHICS_LINK_LIBS "cpaf_sdl2")
endif()




target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_graphics_types.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_graphics_system.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_graphics_system__${CPAF_GRAPHICS_SYSTEM_NAME}.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_graphics_system_base.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_graphics_surface.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_graphics_surface__${CPAF_GRAPHICS_SYSTEM_NAME}.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_graphics_surface_base.h

    ${CMAKE_CURRENT_LIST_DIR}/input/keyboard.h
    ${CMAKE_CURRENT_LIST_DIR}/input/keyboard__${CPAF_GRAPHICS_SYSTEM_NAME}.h
    ${CMAKE_CURRENT_LIST_DIR}/input/keyboard_base.h
    ${CMAKE_CURRENT_LIST_DIR}/input/mouse.h
    ${CMAKE_CURRENT_LIST_DIR}/input/mouse__${CPAF_GRAPHICS_SYSTEM_NAME}.h
    ${CMAKE_CURRENT_LIST_DIR}/input/mouse_base.h
    ${CMAKE_CURRENT_LIST_DIR}/input/touch.h
    ${CMAKE_CURRENT_LIST_DIR}/input/touch__${CPAF_GRAPHICS_SYSTEM_NAME}.h
    ${CMAKE_CURRENT_LIST_DIR}/input/touch_base.h

    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_graphics_system.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_graphics_system__${CPAF_GRAPHICS_SYSTEM_NAME}.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_graphics_system_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_graphics_surface.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_graphics_surface__${CPAF_GRAPHICS_SYSTEM_NAME}.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_graphics_surface_base.cpp

    ${CMAKE_CURRENT_LIST_DIR}/input/keyboard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/input/keyboard__${CPAF_GRAPHICS_SYSTEM_NAME}.cpp
    ${CMAKE_CURRENT_LIST_DIR}/input/keyboard_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/input/mouse.cpp
    ${CMAKE_CURRENT_LIST_DIR}/input/mouse__${CPAF_GRAPHICS_SYSTEM_NAME}.cpp
    ${CMAKE_CURRENT_LIST_DIR}/input/mouse_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/input/touch.cpp
    ${CMAKE_CURRENT_LIST_DIR}/input/touch__${CPAF_GRAPHICS_SYSTEM_NAME}.cpp
    ${CMAKE_CURRENT_LIST_DIR}/input/touch_base.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${library_name} cpaf_config )
target_link_libraries(${library_name} ${CPAF_GRAPHICS_LINK_LIBS} )
target_include_directories(${library_name} INTERFACE  ${PARENT2_DIR} ${PARENT_DIR})
target_include_directories(${library_name} PRIVATE ${CPAF_GRAPHICS_INCLUDE_DIRS} ${PARENT_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

