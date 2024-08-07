#pragma once

#include <string>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/gui/base/texture_base.h>

#if (CPAF_GUI_SYSTEM_ID != CPAF_SYSTEM_ID_GODOT)
#error Only include cpaf_libs/gui/base/texture__godot.h when CPAF_GUI_SYSTEM_ID is CPAF_SYSTEM_ID_GODOT
#endif

namespace cpaf::gui {



class texture_platform : public texture_base {
public:
    using texture_base::texture_base;	// "Import" constructors to scope
    ~texture_platform();

protected:
    friend class app_platform;
    explicit texture_platform(const size_2d& size);
private:
    void                            destroy                     ();

    // --- Platform overrides ---
    void                            do_update_blendmode         () override;
    void*                           do_get_native_texture       () const override;
    size_2d                         do_get_size					() const override;
    bool                            do_load_from_file           (const std::filesystem::path& local_path) override;
    bool                            do_png_from_memory          (const unsigned char* const data_ptr, size_t data_size) override;
    bool                            do_start_surface_pixel_access(size_2d size) override;
    bool                            do_end_surface_pixel_access () override;
    virtual uint32_t*               do_pixel_data_raw_ptr       () override;
    void                            do_set_pixel                (int32_t x, int32_t y, color pixel_color) override;
    void                            do_set_pixel                (int32_t x, int32_t y, uint32_t pixel_color) override;
    void                            do_set_pixel                (int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
    void                            do_draw_rect                (const rect& dst_rect, const color& rect_color) override;

    // --- PRIVATE: Helpers  ---

    // --- PRIVATE: Members  ---

};


} //END namespace cpaf::gui
