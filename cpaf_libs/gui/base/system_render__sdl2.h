#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/gui/base/system_render_base.h>

#if (CPAF_GUI_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/gui/base/system_render__sdl2.h when CPAF_GUI_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

namespace cpaf::gui {



class system_render_platform : public system_render_base {
public:
    explicit system_render_platform(void* sdl_renderer) : sdl_renderer_(reinterpret_cast<SDL_Renderer*>(sdl_renderer)) {}
    ~system_render_platform();

protected:
    friend class system_window;
    friend class system_window_platform;

    explicit system_render_platform(SDL_Window* sdl_window);

private:


    // --- Platform overrides ---
    void*               do_get_native_renderer    	() override;
    void                do_set_color                (color col) override;
    void                do_fill_rect                (render_geometry_t rect) const override;

    // --- PRIVATE: Helpers  ---

    // --- PRIVATE: Members  ---
    SDL_Renderer*           sdl_renderer_       {nullptr};
};


} //END namespace cpaf::gui
