#include <iostream>
#include <array>

#include <cpaf_libs/gui/application.h>
#include <cpaf_libs/gui/app.h>


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
    std::cerr << "*** CPAF GUI PLAYGROUND 2 ***\n";
//    myapp app{"App"};
    myapp app;
    app.run();

	return 0;
}
