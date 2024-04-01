#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/gui/base/texture_base.h>

#if (CPAF_GUI_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/gui/base/texture__sdl2.h when CPAF_GUI_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
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
    void*                           do_get_native_texture       () const override;
    size_2d                         do_get_size					() const override;
    bool                            do_load_from_file           (const std::filesystem::path& local_path) override;
    bool                            do_start_surface_pixel_access  (size_2d size) override;
    bool                            do_end_surface_pixel_access() override;
    virtual uint32_t*               do_pixel_data_raw_ptr       () override;
    void                            do_set_pixel                (int32_t x, int32_t y, color pixel_color) override;
    void                            do_set_pixel                (int32_t x, int32_t y, uint32_t pixel_color) override;
    void                            do_set_pixel                (int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
    void                            do_draw_rect                (const rect& dst_rect, const color& rect_color) override;

    // --- PRIVATE: Helpers  ---
    void                            update_blend_mode           ();

    // --- PRIVATE: Members  ---
    SDL_Texture*                       sdl_texture_         {nullptr};
    SDL_Surface*                       sdl_surface_         {nullptr};

};


} //END namespace cpaf::gui
