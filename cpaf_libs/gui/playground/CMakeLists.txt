find_package(Boost COMPONENTS program_options REQUIRED)

if ( CPAF_GUI_USE_IMGUI )
    add_playground_cpp_exe(gui1 "cpaf_gui")
endif()

if ( (CPAF_GRAPHICS_SYSTEM_NAME MATCHES "sdl2") )
    add_playground_cpp_exe(demo_player_app "cpaf_gui;Boost::program_options")
endif()

