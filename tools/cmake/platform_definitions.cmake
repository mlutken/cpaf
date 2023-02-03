set (BOOST_MIN_VERSION 1.55)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

macro( set_default varName defaultValue )
    if ( NOT DEFINED ${varName} )
        set ( ${varName} ${defaultValue} )
    endif ()
endmacro()


# This file defines platform names, #defines and IDs etc. as well as setting
# default values for those values.
# IMPORTANT: Must match the definitions set in [cpaf/]include/config/cpaf_platform_definitions.h
#            The reason for the redundancy is to make it easier for users that do not want
#            to compile this using cmake!

# ----------------------------------
# --- Platform type id #defines  ---
# ----------------------------------

set (CPAF_PLATFORM_TYPE_ID_DESKTOP      1   )
set (CPAF_PLATFORM_TYPE_ID_EMBEDDED     2   )
set (CPAF_PLATFORM_TYPE_ID_WEB          3   )
set (CPAF_PLATFORM_TYPE_ID_MOBILE       4   )

# ---------------------------------------------
# --- System/abstraction layer id #defines  ---
# ---------------------------------------------
set (CPAF_SYSTEM_ID_LINUX               1   )
set (CPAF_SYSTEM_ID_WINDOWS             2   )
set (CPAF_SYSTEM_ID_OSX                 3   )
set (CPAF_SYSTEM_ID_FREERTOS            4   )
set (CPAF_SYSTEM_ID_BROWSER             5   )
set (CPAF_SYSTEM_ID_ANDROID             6   )
set (CPAF_SYSTEM_ID_IOS                 7   )
set (CPAF_SYSTEM_ID_SDL2                30  )
set (CPAF_SYSTEM_ID_GODOT               31  )
set (CPAF_SYSTEM_ID_QT                  32  )

# ---------------------------------
# --- Audio system id #defines  ---
# ---------------------------------
set (CPAF_AUDIO_SYSTEM_ID_LINUX         ${CPAF_SYSTEM_ID_LINUX}     )   # 1

# ------------------------------------
# --- Graphics system id #defines  ---
# ------------------------------------
set (CPAF_GRAPHICS_SYSTEM_ID_LINUX      ${CPAF_SYSTEM_ID_LINUX}     )   # 1

# -------------------------------
# --- GUI system id #defines  ---
# -------------------------------
set (CPAF_GUI_SYSTEM_ID         ${CPAF_SYSTEM_ID_LINUX}     )   # 1

# --------------------------------
# --- File system id #defines  ---
# --------------------------------
set (CPAF_FILE_SYSTEM_ID         ${CPAF_SYSTEM_ID_LINUX}     )   # 1

