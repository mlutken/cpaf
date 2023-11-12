#include "system__sdl2.h"

#include <SDL2/SDL.h>


namespace cpaf::gui {



// --------------------------
// --- Platform overrides ---
// --------------------------

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

