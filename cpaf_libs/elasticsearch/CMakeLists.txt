find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
set (library_name cpaf_elasticsearch)
add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_elasticsearch.h
    ${CMAKE_CURRENT_LIST_DIR}/es_connection.h
    ${CMAKE_CURRENT_LIST_DIR}/es_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/records/base_record.h
    ${CMAKE_CURRENT_LIST_DIR}/records/read_strategy_base.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_elasticsearch.cpp
    ${CMAKE_CURRENT_LIST_DIR}/es_connection.cpp
    ${CMAKE_CURRENT_LIST_DIR}/es_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/records/base_record.cpp
    ${CMAKE_CURRENT_LIST_DIR}/records/read_strategy_base.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
## get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_link_libraries(${library_name} "utf8;cpaf_utils;cpaf_streams;cpaf_unicode;cpaf_net")
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
## target_include_directories(${library_name} INTERFACE ${PARENT2_DIR} ${PARENT_DIR})
target_include_directories(${library_name} PRIVATE ${PARENT_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()
