#include <iostream>
#include <array>
//#include <thread>
//#include <SDL2/SDL.h>

#include <cpaf_libs/gui/application.h>

//#include <cpaf_libs/graphics/cpaf_graphics_surface.h>
//#include <cpaf_libs/graphics/cpaf_graphics_system.h>

using namespace std;
//using namespace std::chrono_literals;
using namespace cpaf;
using namespace cpaf::gui;


class myapp : public cpaf::gui::application
{
public:
    using application::application;
};


int main(int /*argc*/, char* /*argv*/ [])
{


    std::cerr << "*** CPAF_AUDIO_SYS_EX1 ***\n";
//    myapp app{"App"};
    myapp app;
    app.run();

	return 0;
}
