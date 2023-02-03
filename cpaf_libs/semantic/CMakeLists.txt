find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
set (library_name cpaf_semantic)
add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/misc/cpaf_attributes.h

    ${CMAKE_CURRENT_LIST_DIR}/config/classification_rules.h
    ${CMAKE_CURRENT_LIST_DIR}/config/semantic_data_resources.h
    ${CMAKE_CURRENT_LIST_DIR}/config/configuration.h
    ${CMAKE_CURRENT_LIST_DIR}/readers/classification_data_reader_base.h
    ${CMAKE_CURRENT_LIST_DIR}/readers/json_classification_data_reader.h
    ${CMAKE_CURRENT_LIST_DIR}/tokens/token_attributes.h
    ${CMAKE_CURRENT_LIST_DIR}/tokens/token.h
    ${CMAKE_CURRENT_LIST_DIR}/tokens/token_parser.h
    ${CMAKE_CURRENT_LIST_DIR}/primitives/char_match.h
    ${CMAKE_CURRENT_LIST_DIR}/primitives/pattern_matcher.h
    ${CMAKE_CURRENT_LIST_DIR}/primitives/pattern_matcher_node.h
    ${CMAKE_CURRENT_LIST_DIR}/primitives/word_classes_lookup.h
    ${CMAKE_CURRENT_LIST_DIR}/fragments/fragment_attribute_weights.h
    ${CMAKE_CURRENT_LIST_DIR}/fragments/fragment_matcher.h
    ${CMAKE_CURRENT_LIST_DIR}/fragments/fragment_matcher_branch_point.h
    ${CMAKE_CURRENT_LIST_DIR}/fragments/fragment_node.h
    ${CMAKE_CURRENT_LIST_DIR}/fragments/fragment_try_order.h
    ${CMAKE_CURRENT_LIST_DIR}/fragments/skipped_tokens.h
    ${CMAKE_CURRENT_LIST_DIR}/scores/scores_accumulator.h
    ${CMAKE_CURRENT_LIST_DIR}/scores/source_fields.h
    ${CMAKE_CURRENT_LIST_DIR}/classifier/collapse_string.h
    ${CMAKE_CURRENT_LIST_DIR}/classifier/fragments_classifier.h
    ${CMAKE_CURRENT_LIST_DIR}/classifier/semantic_classifier.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_semantic.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/misc/cpaf_attributes.cpp

    ${CMAKE_CURRENT_LIST_DIR}/config/classification_rules.cpp
    ${CMAKE_CURRENT_LIST_DIR}/config/semantic_data_resources.cpp
    ${CMAKE_CURRENT_LIST_DIR}/config/configuration.cpp
    ${CMAKE_CURRENT_LIST_DIR}/readers/classification_data_reader_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/readers/json_classification_data_reader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tokens/token_attributes.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tokens/token.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tokens/token_parser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/primitives/char_match.cpp
    ${CMAKE_CURRENT_LIST_DIR}/primitives/pattern_matcher.cpp
    ${CMAKE_CURRENT_LIST_DIR}/primitives/pattern_matcher_node.cpp
    ${CMAKE_CURRENT_LIST_DIR}/primitives/word_classes_lookup.cpp
    ${CMAKE_CURRENT_LIST_DIR}/fragments/fragment_attribute_weights.cpp
    ${CMAKE_CURRENT_LIST_DIR}/fragments/fragment_matcher.cpp
    ${CMAKE_CURRENT_LIST_DIR}/fragments/fragment_matcher_branch_point.cpp
    ${CMAKE_CURRENT_LIST_DIR}/fragments/fragment_node.cpp
    ${CMAKE_CURRENT_LIST_DIR}/fragments/fragment_try_order.cpp
    ${CMAKE_CURRENT_LIST_DIR}/fragments/skipped_tokens.cpp
    ${CMAKE_CURRENT_LIST_DIR}/scores/scores_accumulator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/scores/source_fields.cpp
    ${CMAKE_CURRENT_LIST_DIR}/classifier/collapse_string.cpp
    ${CMAKE_CURRENT_LIST_DIR}/classifier/fragments_classifier.cpp
    ${CMAKE_CURRENT_LIST_DIR}/classifier/semantic_classifier.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_semantic.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_link_libraries(${library_name} "cpaf_streams;cpaf_unicode;sparsehash")
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE ${PARENT2_DIR} ${PARENT_DIR})
target_include_directories(${library_name} PRIVATE ${PARENT_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()
