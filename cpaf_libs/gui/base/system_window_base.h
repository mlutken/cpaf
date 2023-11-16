#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <cpaf_libs/gui/gui_types.h>


namespace cpaf::gui {
class system_render;

class system_window_base {
public:
    virtual ~system_window_base() = default;
    system_window_base(const system_window_base& other) = delete;
    system_window_base& operator=(const system_window_base& other) = delete;

    std::string                     dbg_characteristics         () const;
    size_2d                         get_size                    () const { return do_get_size(); }
    std::string                     get_title                   () const { return do_get_title(); }

    system_render&                  renderer                    ()       { return do_renderer(); }
    std::shared_ptr<system_render>  renderer_shared             () const { return do_renderer_shared(); }

protected:
    void*                           get_native_window           ()       { return do_get_native_window();   }
    void*                           get_native_renderer         ()       { return do_get_native_renderer(); }

    system_window_base() = default;
    // system_window_base          (const size_2d& size, std::string_view title);
    // --- PROTECTED: Helper functions for derived classes ---

private:
    virtual size_2d                         do_get_size                 () const = 0;
    virtual std::string                     do_get_title                () const = 0;
    virtual void*                           do_get_native_window        () = 0;
    virtual void*                           do_get_native_renderer      () = 0;
    virtual system_render&                  do_renderer                 () = 0;
    virtual std::shared_ptr<system_render>  do_renderer_shared          () const = 0;
};

} //END namespace cpaf::gui

