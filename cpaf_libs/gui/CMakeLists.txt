
set (library_name cpaf_gui)
add_library(${library_name} STATIC ""
    utils.h utils.cpp)

find_package(imgui REQUIRED)

set (IMGUI_BACKEND_DIR "${CMAKE_CURRENT_LIST_DIR}/imgui_backends/backends")

set (CPAF_GUI_LINK_LIBS "")
set (CPAF_GUI_INCLUDE_DIRS "")
set (CPAF_GUI_IMGUI_BACKEND_CPP_FILES "")


if ( (CPAF_GRAPHICS_SYSTEM_NAME MATCHES "sdl2") )
    set (CPAF_GUI_LINK_LIBS "cpaf_sdl2")
    set (CPAF_GUI_IMGUI_BACKEND_CPP_FILES "${IMGUI_BACKEND_DIR}/imgui_impl_sdl.cpp;${IMGUI_BACKEND_DIR}/imgui_impl_sdlrenderer.cpp")
endif()




target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/base/app_base.h
    ${CMAKE_CURRENT_LIST_DIR}/base/app__${CPAF_AUDIO_SYSTEM_NAME}.h
    ${CMAKE_CURRENT_LIST_DIR}/app.h
    ${CMAKE_CURRENT_LIST_DIR}/application.h
    ${CMAKE_CURRENT_LIST_DIR}/dpi_handler.h
    ${CMAKE_CURRENT_LIST_DIR}/resources.h
    ${CMAKE_CURRENT_LIST_DIR}/utils.h
    ${CMAKE_CURRENT_LIST_DIR}/window.h

    ${CMAKE_CURRENT_LIST_DIR}/assets/fonts/built_in_fonts.h

    PRIVATE
    ${CPAF_GUI_IMGUI_BACKEND_CPP_FILES}
    ${CMAKE_CURRENT_LIST_DIR}/base/app_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/base/app__${CPAF_AUDIO_SYSTEM_NAME}.cpp
    ${CMAKE_CURRENT_LIST_DIR}/app.cpp
    ${CMAKE_CURRENT_LIST_DIR}/application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/${CMAKE_SYSTEM_NAME}/dpi_handler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/${CMAKE_SYSTEM_NAME}/resources.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/window.cpp

    ${CMAKE_CURRENT_LIST_DIR}/assets/fonts/built_in_fonts.cpp
    ${CMAKE_CURRENT_LIST_DIR}/assets/fonts/manrope_ttf.cpp
)


get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${library_name} cpaf_config )
target_link_libraries(${library_name} cpaf_math )
target_link_libraries(${library_name} ${CPAF_GUI_LINK_LIBS} )
target_link_libraries(${library_name} imgui::imgui )
target_include_directories(${library_name} INTERFACE  ${PARENT2_DIR} ${PARENT_DIR})
target_include_directories(${library_name} PRIVATE ${CPAF_GUI_INCLUDE_DIRS})
target_include_directories(${library_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/imgui_backends )
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

