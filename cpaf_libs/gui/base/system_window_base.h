#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/gui/gui_types.h>


namespace cpaf::gui {
class system_render;

class system_window_base {
public:
    virtual ~system_window_base() = default;
    system_window_base(const system_window_base& other) = delete;
    system_window_base& operator=(const system_window_base& other) = delete;

    std::string                     dbg_characteristics         () const;
    size_2d                         size                        () const            { return do_size(); }
    size_2d                         display_size                () const            { return do_display_size(); }
    std::string                     get_title                   () const            { return do_get_title(); }
    int32_t                         display_index               () const            { return do_display_index(); }
    window_mode_t                   window_mode                 () const            { return window_mode_; }
    void                            toggle_full_screen_mode     ();
    void                            window_mode_set             (window_mode_t wm);

    system_render&                  renderer                    ()       { return do_renderer(); }
    std::shared_ptr<system_render>  renderer_shared             () const { return do_renderer_shared(); }


    // --- Static Info functions ---
    static const window_mode_t      default_window_mode;
    static const window_mode_t      default_window_full_screen_mode;

protected:
    void*                           get_native_window           ()       { return do_get_native_window();   }
    void*                           get_native_renderer         ()       { return do_get_native_renderer(); }

    system_window_base() = default;
    // system_window_base          (const size_2d& size, std::string_view title);
    // --- PROTECTED: Helper functions for derived classes ---
    // --- PROTECTED: Members for derived classes ---
    window_mode_t                   window_mode_ = default_window_mode;

private:
    virtual size_2d                         do_size                     () const = 0;
    virtual size_2d                         do_display_size             () const = 0;
    virtual std::string                     do_get_title                () const = 0;
    virtual int32_t                         do_display_index            () const = 0;
    virtual bool                            do_window_mode_set          (window_mode_t wm)  = 0;
    virtual void*                           do_get_native_window        () = 0;
    virtual void*                           do_get_native_renderer      () = 0;
    virtual system_render&                  do_renderer                 () = 0;
    virtual std::shared_ptr<system_render>  do_renderer_shared          () const = 0;
};

} //END namespace cpaf::gui

