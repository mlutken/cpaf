
macro( set_default varName defaultValue )
    if ( NOT DEFINED ${varName} )
        set ( ${varName} ${defaultValue} )
    endif ()
endmacro()



# --------------------
# --- Set defaults ---
# --------------------
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set_default (CPAF_PLATFORM_TYPE_NAME    "desktop"                       )
    set_default (CPAF_SYSTEM_NAME           "linux"                         )
    set_default (CPAF_AUDIO_SYSTEM_NAME     "sdl2"                          )
    set_default (CPAF_GRAPHICS_SYSTEM_NAME  "sdl2"                          )
    set_default (CPAF_GUI_SYSTEM_NAME       "linux"                         )
    set_default (CPAF_FILE_SYSTEM_NAME      "linux"                         )
    set_default (CPAF_PLATFORM_TYPE_ID      ${CPAF_PLATFORM_TYPE_ID_DESKTOP})
    set_default (CPAF_SYSTEM_ID             ${CPAF_SYSTEM_ID_LINUX}         )
    set_default (CPAF_AUDIO_SYSTEM_ID       ${CPAF_SYSTEM_ID_SDL2}          )
    set_default (CPAF_GRAPHICS_SYSTEM_ID    ${CPAF_SYSTEM_ID_SDL2}          )
    set_default (CPAF_GUI_SYSTEM_ID         ${CPAF_SYSTEM_ID_LINUX}         )
    set_default (CPAF_FILE_SYSTEM_ID        ${CPAF_SYSTEM_ID_LINUX}         )
# TODO: Other platforms like windows and osx
endif ()
