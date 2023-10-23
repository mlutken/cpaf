#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/gui/base/app_base.h>
#include <cpaf_libs/gui/events/events__sdl2.h>

#if (CPAF_GUI_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/gui/base/app__sdl2.h when CPAF_GUI_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

namespace cpaf::gui {


class app_platform : public app_base {
public:
    app_platform();
    virtual ~app_platform();

private:
    // --- Platform overrides ---
    void                    do_start_run				() override;
    events::event           do_get_event                () const override;
    void                    do_process_events           () override;
    void                    do_pre_frame_update         () override;
    void                    do_frame_update             () override;
    void                    do_post_frame_update		() override;
    size_2d                 do_main_window_size         () const override;

    std::unique_ptr<system_window_base>  do_create_system_window   (size_2d size, std::string_view title) const override;

    // --- PRIVATE: Helpers  ---
    void        initialize();

    // --- PRIVATE: Members ---
    events::events_sdl      events_converter_;
    SDL_Window*             main_window_    {nullptr};
    SDL_Renderer*           main_renderer_  {nullptr};

};


} //END namespace cpaf::gui
