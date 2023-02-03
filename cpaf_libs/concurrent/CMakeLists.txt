IF ( NOT CBS_BUILD )
    set (library_name cpaf_concurrent)
    add_library(${library_name} INTERFACE)

    target_sources(${library_name}
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/align_macros.h
        ${CMAKE_CURRENT_LIST_DIR}/SingleWriterSingleReaderLocklessFifo.h
        ${CMAKE_CURRENT_LIST_DIR}/ModuloFifo.h
    )

    get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
    get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
    target_include_directories(${library_name} INTERFACE ${Boost_INCLUDE_DIRS} ${PARENT2_DIR} ${PARENT_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
#     target_compile_options(${library_name} INTERFACE ${STANDARD_COMPILE_FLAGS})

    add_subdirectories_standard()
ELSE ()
    project ( cpaf_concurrent )
    include ( $ENV{CBS_ROOT_DIR}/cbs_common_rules.cbs )
    BUILD_LIBRARY ( cpaf_concurrent )
ENDIF ()

