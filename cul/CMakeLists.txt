
# ------------------------
# --- ImGui Use or Not ---
# ------------------------
set (CPAF_GUI_USE_IMGUI False)
if ( (CPAF_GRAPHICS_SYSTEM_NAME MATCHES "sdl2") )
    set (CPAF_GUI_USE_IMGUI True)
endif()



# -----------------------
# --- ImGuiFileDialog ---
# -----------------------
if ( CPAF_GUI_USE_IMGUI )
    find_package(imgui REQUIRED)
    add_subdirectory(ImGuiFileDialog)
    set (ImGuiFileDialog_library_name ImGuiFileDialog)
    set_target_properties(${ImGuiFileDialog_library_name} PROPERTIES LINKER_LANGUAGE CXX)
    target_include_directories(${ImGuiFileDialog_library_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
    target_link_libraries(${ImGuiFileDialog_library_name} imgui::imgui )
    target_compile_options(${ImGuiFileDialog_library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})
endif()

# --- KZip ---
set (BUILD_TESTS OFF)
set (BUILD_SAMPLES OFF)
set (CREATE_DOCS OFF)
add_subdirectory(KZip)

add_subdirectory(luepp)
add_subdirectory(IconFontCppHeaders)

if ( CPAF_GUI_USE_IMGUI )
    add_subdirectory(imguipp)
endif()

#add_subdirectory(date)
if ( NOT (CMAKE_SYSTEM_NAME MATCHES "Emscripten") )

    # --- WebCC ---
    set(WEBCC_ENABLE_SSL   1 CACHE STRING "Enable SSL/HTTPS (need OpenSSL)? (1:Yes, 0:No)")
    set(WEBCC_ENABLE_GZIP  1 CACHE STRING "Enable gzip compression (need Zlib)? (1:Yes, 0:No)")
    add_subdirectory(webcc)
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/config.h.in" "${CMAKE_BINARY_DIR}/install/include/webcc/config.h" )

    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/base64.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/body.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/client.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/client_pool.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/client_session.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/common.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/connection.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/connection_pool.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/globals.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/gzip.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/logger.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/message.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/parser.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/queue.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/request_builder.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/request.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/request_parser.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/response_builder.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/response.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/response_parser.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/router.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/server.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/socket.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/string.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/url.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/utility.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/version.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )
    configure_file( "${CMAKE_CURRENT_LIST_DIR}/webcc/webcc/view.h" "${CMAKE_BINARY_DIR}/install/include/webcc/" )

    add_subdirectory(sparsehash)
    add_subdirectory(loki)
    add_subdirectory(utf8)
endif()
