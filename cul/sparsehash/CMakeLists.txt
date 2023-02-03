set (library_name sparsehash)

add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/sparsehash/dense_hash_map
    ${CMAKE_CURRENT_LIST_DIR}/sparsehash/dense_hash_set
    ${CMAKE_CURRENT_LIST_DIR}/sparsehash/sparse_hash_map
    ${CMAKE_CURRENT_LIST_DIR}/sparsehash/sparse_hash_set
    ${CMAKE_CURRENT_LIST_DIR}/sparsehash/sparsetable
    ${CMAKE_CURRENT_LIST_DIR}/sparsehash/traits
)

set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

