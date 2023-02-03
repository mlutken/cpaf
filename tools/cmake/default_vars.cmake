# Set a default build type if none was specified
set(default_build_type "Release")
 
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# CMAKE_RUNTIME_OUTPUT_DIRECTORY
# 
# Where to put all the RUNTIME target files when built.
#
# This variable is used to initialize the RUNTIME_OUTPUT_DIRECTORY property on all the targets. 
# See that target property # for additional information.

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/pdb)


IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (BASE_LIB_DIR "/usr/lib/x86_64-linux-gnu")
    set (BASE_INCLUDE_DIR "/usr/include")
ENDIF ()
