set (library_name cpaf_net)
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)

add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_http_convenience.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_url.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_url_string_functions.h
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_webcc_client.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_http_convenience.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_url.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_url_string_functions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpaf_webcc_client.cpp
)

message("FIXMENM webcc_INCLUDE_DIRS: ${webcc_INCLUDE_DIRS}")
message("FIXMENM webcc_INCLUDE_DIRS: ${webcc_INCLUDE_DIRS}")
message("FIXMENM webcc_INCLUDE_DIRS: ${webcc_INCLUDE_DIRS}")
message("FIXMENM webcc_INCLUDE_DIRS: ${webcc_INCLUDE_DIRS}")
message("FIXMENM webcc_INCLUDE_DIRS: ${webcc_INCLUDE_DIRS}")
message("FIXMENM webcc_INCLUDE_DIRS: ${webcc_INCLUDE_DIRS}")
message("FIXMENM webcc_INCLUDE_DIRS: ${webcc_INCLUDE_DIRS}")

#add_library(webcc STATIC IMPORTED)
#set_target_properties(webcc PROPERTIES IMPORTED_LOCATION "${CPAF_INSTALL_DIR}/lib/libwebcc.a")

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_link_libraries(${library_name} CURL::libcurl)
target_link_libraries(${library_name} "webcc" )
target_link_libraries(${library_name} OpenSSL::SSL )
target_link_libraries(${library_name} OpenSSL::Crypto )
target_link_libraries(${library_name} ZLIB::ZLIB )
target_link_libraries(${library_name} "pthread" )
#target_link_libraries(${library_name} Boost::filesystem Boost::system )
target_link_libraries(${library_name} "boost_filesystem;boost_system" )
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} PUBLIC ${CMAKE_BINARY_DIR}/install/include )
target_include_directories(${library_name} PUBLIC ${CURL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${PARENT_DIR} ${CPAF_INSTALL_DIR}/include )
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

