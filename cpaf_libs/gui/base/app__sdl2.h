#pragma once

#include <memory>
#include <SDL2/SDL_audio.h>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/gui/base/app_base.h>

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
    void                do_run						() override;
    std::unique_ptr<system_window_base>  do_create_system_window   (size_2d size, std::string_view title) const override;

    // --- PRIVATE: Helpers  ---

};


} //END namespace cpaf::gui
