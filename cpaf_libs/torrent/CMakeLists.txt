set (library_name cpaf_torrent)
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
find_package(OpenSSL REQUIRED)
# Compiling openSSL for webasm/emscripten: https://github.com/BOINC/boinc/issues/3086
add_library(${library_name} STATIC "")

target_sources(${library_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/server.h
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/server.cpp
)

set (LIBTORRENT_INCLUDES "${CPAF_INSTALL_DIR}/include")
add_library(libtorrent STATIC IMPORTED)

#IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set_target_properties(libtorrent PROPERTIES IMPORTED_LOCATION "${CPAF_INSTALL_DIR}/lib/libtorrent-rasterbar.a")
set_target_properties(libtorrent PROPERTIES IMPORTED_LINK_INTERFACE_LIBRARIES "${OPENSSL_LIBRARIES}" )
#ENDIF ()

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT2_DIR ${PARENT_DIR} DIRECTORY)
target_link_libraries(${library_name} "libtorrent;cpaf_utils")
target_link_libraries(${library_name} "cpaf_utils")
target_link_libraries(${library_name} boost_system)
#target_link_libraries(${library_name} Boost::system)
set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${library_name} INTERFACE ${LIBTORRENT_INCLUDES} ${Boost_INCLUDE_DIRS} ${PARENT2_DIR} ${PARENT_DIR})
target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

add_subdirectories_standard()

