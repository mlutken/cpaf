
set (library_name cpaf_audio)
add_library(${library_name} STATIC "")

set (CPAF_AUDIO_LINK_LIBS "")
set (CPAF_AUDIO_INCLUDE_DIRS "")

#set (public_headers
#    )

#set (private_sources
#    )

if ( (CPAF_AUDIO_SYSTEM_NAME MATCHES "sdl2") )
    set (CPAF_AUDIO_LINK_LIBS "cpaf_sdl2")
endif()




target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_audio_types.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_audio_system.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_audio_system__${CPAF_AUDIO_SYSTEM_NAME}.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_audio_system_base.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_audio_device.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_audio_device__${CPAF_AUDIO_SYSTEM_NAME}.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_audio_device_base.h

    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_audio_types.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_audio_system.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_audio_system__${CPAF_AUDIO_SYSTEM_NAME}.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_audio_system_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_audio_device.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_audio_device__${CPAF_AUDIO_SYSTEM_NAME}.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_audio_device_base.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${library_name} "cpaf_config;${CPAF_AUDIO_LINK_LIBS}" )
target_include_directories(${library_name} INTERFACE  ${PARENT2_DIR} ${PARENT_DIR})
target_include_directories(${library_name} PRIVATE ${CPAF_AUDIO_INCLUDE_DIRS} ${PARENT_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

