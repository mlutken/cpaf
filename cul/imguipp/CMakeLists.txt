set (library_name imguipp)

find_package(imgui REQUIRED)

add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/imguipp/imgui_rai.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/imguipp/imgui_rai.cpp
)

set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${library_name} imgui::imgui )
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

