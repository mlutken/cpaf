#pragma once

#include <SDL2/SDL_audio.h>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/gui/base/system_window_base.h>

#if (CPAF_GUI_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/gui/base/system_window__sdl2.h when CPAF_GUI_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

namespace cpaf::gui {


class system_window_platform : public system_window_base {
public:

private:
    // --- Platform overrides ---

    // --- PRIVATE: Helpers  ---
};


} //END namespace cpaf::gui
