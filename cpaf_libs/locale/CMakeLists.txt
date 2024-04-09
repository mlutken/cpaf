set (library_name cpaf_locale)
add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/language_codes.h
    ${CMAKE_CURRENT_LIST_DIR}/translator.h
    ${CMAKE_CURRENT_LIST_DIR}/translator_if.h
    ${CMAKE_CURRENT_LIST_DIR}/ui_translator.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/language_codes.cpp
    ${CMAKE_CURRENT_LIST_DIR}/translator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/translator_if.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui_translator.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} PUBLIC ${PARENT2_DIR} ${PARENT_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

