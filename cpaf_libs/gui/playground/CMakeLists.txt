find_package(Boost COMPONENTS program_options REQUIRED)

add_playground_cpp_exe(gui1 "cpaf_gui")
add_playground_cpp_exe(demo_player_app "cpaf_gui;Boost::program_options")
