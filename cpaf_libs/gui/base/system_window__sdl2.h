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
    ~system_window_platform();

protected:
    friend class app_platform;
    system_window_platform(const size_2d& size, std::string_view title);
private:


    // --- Platform overrides ---
    size_2d                         do_get_size					() const override;
    std::string                     do_get_title    			() const override;
    void*                           do_get_native_window    	() override;
    void*                           do_get_native_renderer    	() override;
    system_render&                  do_renderer                 () override;
    std::shared_ptr<system_render>  do_renderer_shared          () const override;

    // --- PRIVATE: Helpers  ---

    // --- PRIVATE: Members  ---
    SDL_Window*                     sdl_window_             {nullptr};
    std::shared_ptr<system_render>  render_                 {nullptr};
    SDL_Renderer*                   sdl_window_renderer_    {nullptr};

};


} //END namespace cpaf::gui
