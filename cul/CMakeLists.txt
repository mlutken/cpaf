add_subdirectory(luepp)
if ( NOT (CMAKE_SYSTEM_NAME MATCHES "Emscripten") )
    add_subdirectory(sparsehash)
    add_subdirectory(fifo_map)
    add_subdirectory(json)
    add_subdirectory(loki)
    add_subdirectory(utf8)
endif()

