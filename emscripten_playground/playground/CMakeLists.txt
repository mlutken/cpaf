add_playground_cpp_exe(emscripten1 "emscripten_playground")
add_playground_cpp_exe(emscripten2 "emscripten_playground")
add_playground_cpp_exe(emscripten3 "emscripten_playground")
add_playground_cpp_exe(emscripten4 "emscripten_playground")


if ( CMAKE_SYSTEM_NAME MATCHES "Emscripten" )
    configure_file(${CMAKE_CURRENT_LIST_DIR}/emscripten1_playground.html ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/emscripten1_playground.html COPYONLY)
    configure_file(${CMAKE_CURRENT_LIST_DIR}/emscripten2_playground.html ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/emscripten2_playground.html COPYONLY)
    configure_file(${CMAKE_CURRENT_LIST_DIR}/emscripten3_playground.html ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/emscripten3_playground.html COPYONLY)
    configure_file(${CMAKE_CURRENT_LIST_DIR}/emscripten4_playground.html ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/emscripten4_playground.html COPYONLY)
endif()

