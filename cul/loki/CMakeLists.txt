
IF ( NOT CBS_BUILD )
    set (library_name loki)
    add_library(${library_name} STATIC "")

    target_sources(${library_name}
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include/loki/Singleton.h
        PRIVATE
#         ${CMAKE_CURRENT_LIST_DIR}/src/OrderedStatic.cpp
#         ${CMAKE_CURRENT_LIST_DIR}/src/SafeFormat.cpp
#         ${CMAKE_CURRENT_LIST_DIR}/src/SmallObj.cpp
#         ${CMAKE_CURRENT_LIST_DIR}/src/SmartPtr.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/Singleton.cpp
#         ${CMAKE_CURRENT_LIST_DIR}/src/StrongPtr.cpp
    )

    get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
    set_target_properties(${library_name} PROPERTIES LINKER_LANGUAGE CXX)
    set_target_properties(${library_name} PROPERTIES COMPILE_FLAGS "-fpermissive -Wno-deprecated" )
    target_include_directories(${library_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )
    target_compile_options(${library_name} PRIVATE ${STANDARD_COMPILE_FLAGS})

    add_subdirectories_standard()

ELSE ( NOT CBS_BUILD )
	project ( loki )
	include ( $ENV{CBS_ROOT_DIR}/cbs_common_rules.cbs )
	BUILD_LIBRARY ( loki ) 
ENDIF ( NOT CBS_BUILD )
