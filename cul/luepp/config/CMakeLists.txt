set (lib_name luepp_config)

add_library(${lib_name} INTERFACE)

target_sources(${lib_name}
    INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/luepp_default_config.h
)

target_include_directories(${lib_name} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

