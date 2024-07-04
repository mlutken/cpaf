#pragma once

#include <string>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/gui/base/system_render_base.h>

#if (CPAF_GUI_SYSTEM_ID != CPAF_SYSTEM_ID_GODOT)
#error Only include cpaf_libs/gui/base/system_render__godot.h when CPAF_GUI_SYSTEM_ID is CPAF_SYSTEM_ID_GODOT
#endif

namespace cpaf::gui {



class system_render_platform : public system_render_base {
public:
    // explicit system_render_platform(void* godot_renderer) : godot_renderer_(reinterpret_cast<GODOT_Renderer*>(godot_renderer)) {}
    explicit system_render_platform(void* godot_renderer) : godot_renderer_(godot_renderer) {}
    ~system_render_platform();

protected:
    friend class system_window;
    friend class system_window_platform;

    // explicit system_render_platform(SDL_Window* sdl_window);

private:


    // --- Platform overrides ---
    void*               do_get_native_renderer          () override;
    void                do_set_color                    (color col) override;
    void                do_fill_rect                    (rect rect) const override;
    bool                do_render_texture               (const cpaf::gui::texture& tex, rect src_rect, rect dst_rect) override;
    cpaf::math::v2i32   do_maximum_texture_dimensions   () const override;

    // --- PRIVATE: Helpers  ---

    // --- PRIVATE: Members  ---
    // GODOT_Renderer*           godot_renderer_       {nullptr}; PSeudo code !!
    void*           godot_renderer_       {nullptr};
};


} //END namespace cpaf::gui
