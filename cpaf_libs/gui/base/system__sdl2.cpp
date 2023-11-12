#include "system__sdl2.h"

#include <fmt/format.h>
#include <SDL2/SDL.h>


namespace cpaf::gui {



// --------------------------
// --- Platform overrides ---
// --------------------------

system_platform::~system_platform()
{
    fmt::println("system_platform::DESTRUCTOR");
    SDL_Quit();
}

int32_t system_platform::do_init()
{
    const uint32_t init_flags{SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS};
    const int32_t retval = SDL_Init(init_flags);
    return retval;
}


// ------------------------
// --- PRIVATE: Helpers ---
// ------------------------


} // END namespace cpaf::gui

