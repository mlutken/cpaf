#set (library_name cpaf_config INTERFACE)

#target_sources(${library_name}
#    INTERFACE
#    ${CMAKE_CURRENT_LIST_DIR}/config/cpaf_platform_definitions.h
#)

#target_include_directories(${library_name} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#add_subdirectories_standard()



set (library_name cpaf_config)

add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/config/cpaf_platform_definitions.h
)

set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE ${CMAKE_CURRENT_LIST_DIR})
