#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <filesystem>
#include <cpaf_libs/gui/gui_types.h>
#include <cpaf_libs/gui/color.h>


namespace cpaf::gui {
class system_render;

class texture_base {
public:
    texture_base();
    explicit texture_base(std::shared_ptr<system_render> render);
    virtual ~texture_base() = default;
    texture_base(const texture_base& other) = delete;
    texture_base& operator=(const texture_base& other) = delete;

    void                            set_render                  (std::shared_ptr<system_render> render);
    void                            set_blendmode               (blendmode_t blendmode)                         { blendmode_ = blendmode; }
    blendmode_t                     blendmode                   () const                                        { return blendmode_; }

    size_2d                         size                        () const                                        { return do_get_size();                         }
    bool                            load_from_file              (const std::filesystem::path& local_path)       { return do_load_from_file(local_path);         }
    bool                            start_surface_pixel_access  (size_2d size)                                  { return do_start_surface_pixel_access(size);   }
    bool                            end_surface_pixel_access    ()                                              { return do_end_surface_pixel_access();         }
    uint32_t*                       pixel_data_raw_ptr          ()                                              { return do_pixel_data_raw_ptr();               }
    uint32_t                        pixel_data_raw_size_bytes   () const;

    void                            set_pixel                   (int32_t x, int32_t y, color pixel_color)       { do_set_pixel(x, y, pixel_color); }
    void                            set_pixel                   (int32_t x, int32_t y, uint32_t pixel_color)    { do_set_pixel(x, y, pixel_color); }
    void                            set_pixel                   (int32_t x, int32_t y,
                                                                 uint8_t r, uint8_t g, uint8_t b, uint8_t a)    { do_set_pixel(x, y, r, g, b, a); }
    void                            draw_rect                   (const rect& dst_rect, const color& rect_color) { do_draw_rect(dst_rect, rect_color); }

protected:
    void*                           get_native_texture          () const { return do_get_native_texture(); }

    // --- PROTECTED: Helper functions for derived classes ---
    std::shared_ptr<system_render>  render_                 {nullptr};
    blendmode_t                     blendmode_              {blendmode_t::none};

private:
    virtual void*                           do_get_native_texture           () const = 0;
    virtual size_2d                         do_get_size                     () const = 0;
    virtual bool                            do_load_from_file               (const std::filesystem::path& local_path) = 0;
    virtual bool                            do_start_surface_pixel_access   (size_2d size) = 0;
    virtual bool                            do_end_surface_pixel_access     () = 0;
    virtual uint32_t*                       do_pixel_data_raw_ptr           () = 0;
    virtual void                            do_set_pixel                    (int32_t x, int32_t y, color pixel_color) = 0;
    virtual void                            do_set_pixel                    (int32_t x, int32_t y, uint32_t pixel_color) = 0;
    virtual void                            do_set_pixel                    (int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;
    virtual void                            do_draw_rect                    (const rect& dst_rect, const color& rect_color) = 0;
};


} //END namespace cpaf::gui

