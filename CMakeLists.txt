cmake_minimum_required(VERSION 3.5)
project(cpaf)

set ( CPAF_VERSION_MAJOR     1   )
set ( CPAF_VERSION_MINOR     0   )
set ( CPAF_VERSION_PATCH     0   )
set ( CPAF_VERSION_STRING    "${CPAF_VERSION_MAJOR}.${CPAF_VERSION_MINOR}.${CPAF_VERSION_PATCH}"    )

set ( CPAF_INSTALL_DIR "$ENV{HOME}/cpaf/${CPAF_VERSION_MAJOR}.${CPAF_VERSION_MINOR}/install")

set (CPAF_IS_TOP_LEVEL_PROJECT FALSE)
if ( "${CMAKE_PROJECT_NAME}" MATCHES "cpaf")
    set (CPAF_IS_TOP_LEVEL_PROJECT TRUE)
endif ()


if (CPAF_IS_TOP_LEVEL_PROJECT)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    enable_testing()

    SET(QT_CREATOR_SKIP_PACKAGE_MANAGER_SETUP TRUE)
    SET(QT_CREATOR_SKIP_CONAN_SETUP TRUE)

    include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/platform_definitions.cmake)
    include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/default_platform_definitions.cmake)
    include (${CMAKE_CURRENT_LIST_DIR}/tools/cmake/set_platform_compiler_defines.cmake)

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/tools/cmake)

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
      file(COPY_FILE "${CMAKE_CURRENT_LIST_DIR}/cul/luepp/3rdparty/conan/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
    #  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake" TLS_VERIFY ON)
    endif()


    include(${CMAKE_BINARY_DIR}/conan.cmake)

    conan_cmake_autodetect(settings)

    conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_CURRENT_LIST_DIR}
                        BUILD missing
                        REMOTE conancenter
                        SETTINGS ${settings})

    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()


if ( NOT (CMAKE_SYSTEM_NAME MATCHES "Emscripten") )
endif()

# Set some default platform/system names
if ( (CPAF_AUDIO_SYSTEM_NAME MATCHES "") )
    set (CPAF_AUDIO_SYSTEM_NAME "sdl2")
endif()


add_subdirectory(include)
add_subdirectory(cul)
add_subdirectory(cpaf_libs)
add_subdirectory(emscripten_playground)
