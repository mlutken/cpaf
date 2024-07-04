#pragma once

#include <string>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/gui/base/system_window_base.h>

#if (CPAF_GUI_SYSTEM_ID != CPAF_SYSTEM_ID_GODOT)
#error Only include cpaf_libs/gui/base/system_window__godot.h when CPAF_GUI_SYSTEM_ID is CPAF_SYSTEM_ID_GODOT
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
    size_2d                         do_size                     () const override;
    size_2d                         do_display_size             () const override;
    std::string                     do_get_title    			() const override;
    int32_t                         do_display_index   			() const override;
    bool                            do_window_mode_set          (window_mode_t wm) override;
    void*                           do_get_native_window    	() override;
    void*                           do_get_native_renderer    	() override;
    system_render&                  do_renderer                 () override;
    std::shared_ptr<system_render>  do_renderer_shared          () const override;

    // --- PRIVATE: Helpers  ---

    // --- PRIVATE: Members  ---
    // GODOT_Window*                     godot_window_             {nullptr}; Pseudo code !!!
    void*                           godot_window_             {nullptr};
    std::shared_ptr<system_render>  render_                 {nullptr};
    // SDL_Renderer*                   sdl_window_renderer_    {nullptr};

};


} //END namespace cpaf::gui
