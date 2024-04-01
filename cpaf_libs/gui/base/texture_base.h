#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <filesystem>
#include <cpaf_libs/gui/gui_types.h>


namespace cpaf::gui {
class system_render;

class texture_base {
public:
    texture_base();
    explicit texture_base(std::shared_ptr<system_render>  render);
    virtual ~texture_base() = default;
    texture_base(const texture_base& other) = delete;
    texture_base& operator=(const texture_base& other) = delete;

    void                            set_render                  (std::shared_ptr<system_render> render);

    size_2d                         size                        () const { return do_get_size(); }
    bool                            load_from_file              (const std::filesystem::path& local_path) { return do_load_from_file(local_path); }
//    int32_t                         display_index               () const { return do_display_index(); }

//    system_render&                  renderer                    ()       { return do_renderer(); }
//    std::shared_ptr<system_render>  renderer_shared             () const { return do_renderer_shared(); }

protected:
//    void*                           get_native_window           ()       { return do_get_native_window();   }
//    void*                           get_native_renderer         ()       { return do_get_native_renderer(); }

    // texture_base          (const size_2d& size, std::string_view title);
    // --- PROTECTED: Helper functions for derived classes ---
    std::shared_ptr<system_render>  render_                 {nullptr};

private:
    virtual size_2d                         do_get_size                 () const = 0;
    virtual bool                            do_load_from_file           (const std::filesystem::path& local_path) = 0;
//    virtual int32_t                         do_display_index            () const = 0;
//    virtual void*                           do_get_native_window        () = 0;
//    virtual void*                           do_get_native_renderer      () = 0;
//    virtual system_render&                  do_renderer                 () = 0;
//    virtual std::shared_ptr<system_render>  do_renderer_shared          () const = 0;
};

} //END namespace cpaf::gui

