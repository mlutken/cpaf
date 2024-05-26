set (library_name cpaf_crypto)
find_package(Boost COMPONENTS uuid REQUIRED)
add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/crypto_hash.h
    ${CMAKE_CURRENT_LIST_DIR}/md5sum.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/crypto_hash.cpp
    ${CMAKE_CURRENT_LIST_DIR}/md5sum.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_include_directories(${library_name}  INTERFACE ${Boost_INCLUDE_DIRS})
target_link_libraries(${library_name}  ${Boost_LIBRARIES})
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE ${PARENT2_DIR} )
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

