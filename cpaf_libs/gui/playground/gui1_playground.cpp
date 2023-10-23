#include <iostream>
#include <array>
#include <fmt/format.h>

#include <cpaf_libs/gui/application.h>
#include <cpaf_libs/gui/app.h>


using namespace std;
//using namespace std::chrono_literals;
using namespace cpaf;
using namespace cpaf::gui;


#if 1
class myapp : public cpaf::gui::app
{
public:
    using app::app;
};

#else
class myapp : public cpaf::gui::application
{
public:
    using application::application;
};
#endif

int main(int /*argc*/, char* /*argv*/ [])
{
    fmt::println("*** CPAF GUI PLAYGROUND 1 ***");
    fmt::println("sizeof(events::event) : {}", sizeof(events::event));
;
//    myapp app{"App"};
    myapp app;
    app.run();

	return 0;
}
