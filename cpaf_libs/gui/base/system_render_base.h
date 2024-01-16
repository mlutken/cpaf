#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <cpaf_libs/gui/gui_types.h>
#include <cpaf_libs/gui/color.h>


namespace cpaf::gui {


class system_render_base {
public:
    virtual ~system_render_base() = default;
    system_render_base(const system_render_base& other) = delete;
    system_render_base& operator=(const system_render_base& other) = delete;

    std::string                 dbg_characteristics         () const;
    void                        set_color                   (color col) { do_set_color(col); }
    void                        fill_rect                   (rect rect) const { do_fill_rect(rect); }

protected:
    void*                       get_native_renderer         ()       { return do_get_native_renderer(); }

    system_render_base() = default;
    // --- PROTECTED: Helper functions for derived classes ---

private:
    virtual void*               do_get_native_renderer      () = 0;
    virtual void                do_set_color                (color col) = 0;
    virtual void                do_fill_rect                (rect rect) const = 0;
};

} //END namespace cpaf::gui

