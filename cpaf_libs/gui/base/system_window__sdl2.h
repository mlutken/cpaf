#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/gui/base/system_window_base.h>

#if (CPAF_GUI_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/gui/base/system_window__sdl2.h when CPAF_GUI_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

namespace cpaf::gui {


class system_window_platform : public system_window_base {
public:
    explicit system_window_platform(const size_2d& size);
    system_window_platform(const size_2d& size, std::string_view title);

private:
    // --- Platform overrides ---
    size_2d             do_get_size					() override;
    std::string         do_get_title    			() override;

    // --- PRIVATE: Helpers  ---
};


} //END namespace cpaf::gui
