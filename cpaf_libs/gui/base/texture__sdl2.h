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
    ~texture_platform();

protected:
    friend class app_platform;
    explicit texture_platform(const size_2d& size);
private:
    void                            destroy                     ();

    // --- Platform overrides ---
    size_2d                         do_get_size					() const override;
    bool                            do_load_from_file           (const std::filesystem::path& local_path);
//    std::string                     do_get_title    			() const override;
//    int32_t                         do_display_index   			() const override;
//    void*                           do_get_native_window    	() override;
//    void*                           do_get_native_renderer    	() override;
//    system_render&                  do_renderer                 () override;
//    std::shared_ptr<system_render>  do_renderer_shared          () const override;

    // --- PRIVATE: Helpers  ---

    // --- PRIVATE: Members  ---
//    SDL_Window*                     sdl_window_             {nullptr};
    SDL_Texture*                       sdl_texture_         {nullptr};

};


} //END namespace cpaf::gui
