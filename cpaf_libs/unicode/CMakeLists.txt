set (library_name cpaf_unicode)
add_library(${library_name} STATIC "")

find_package(Boost COMPONENTS locale REQUIRED)
### find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
find_package(ICU REQUIRED COMPONENTS data i18n io tu uc) # https://github.com/microsoft/vcpkg/issues/9352

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_unicode_chars.hpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_u8string_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_u8string_split.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_u8string_parse_numbers.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_u16string_parse_numbers.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_u16string_parse_numbers.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_u16string_split.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_u16string_utils.h

    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_u8string_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_u8string_split.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_u8string_parse_numbers.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_u16string_parse_numbers.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_u16string_split.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_u16string_utils.cpp
)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_link_libraries(${library_name} PUBLIC "cpaf_utils")
target_link_libraries(${library_name} PUBLIC ICU::i18n)
target_link_libraries(${library_name} PUBLIC ICU::uc)
target_link_libraries(${library_name} PUBLIC ICU::data)
target_link_libraries(${library_name} PUBLIC ICU::io)
target_link_libraries(${library_name} PUBLIC ICU::tu)
target_link_libraries(${library_name} PUBLIC Boost::locale)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} PUBLIC  ${ICU_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${PARENT2_DIR} ${PARENT_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()
