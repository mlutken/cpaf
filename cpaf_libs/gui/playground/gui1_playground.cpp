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

static constexpr std::array<unsigned char, 2> a3 = {'s', 'f'};


int main(int /*argc*/, char* /*argv*/ [])
{


    std::cerr << "*** CPAF_AUDIO_SYS_EX1 ***\n";
    application app{"App"};
    app.run();

	return 0;
}
