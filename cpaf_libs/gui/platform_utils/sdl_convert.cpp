#include "sdl_convert.h"

namespace cpaf::gui {

SDL_Rect to_sdl_rect(rect geom)
{
    return SDL_Rect{static_cast<int>(geom.top_left().x()),
                    static_cast<int>(geom.top_left().y()),
                    static_cast<int>(geom.size().width()),
                    static_cast<int>(geom.size().height())};
}

rect to_rect(SDL_Rect rct)
{
    return rect{static_cast<float>(rct.x),
                static_cast<float>(rct.y),
                static_cast<float>(rct.w),
                static_cast<float>(rct.h)};

}


SDL_BlendMode to_sdl_blendmode(blend_mode_t blendmode)
{
    switch (blendmode) {
    case blend_mode_t::none :                return SDL_BLENDMODE_NONE; break;
    case blend_mode_t::alpha :               return SDL_BLENDMODE_BLEND; break;
    case blend_mode_t::additive :            return SDL_BLENDMODE_ADD; break;
    case blend_mode_t::color_modulate :      return SDL_BLENDMODE_MOD; break;
    case blend_mode_t::color_multiply :      return SDL_BLENDMODE_MUL; break;
    default:
        break;
    }
    return SDL_BLENDMODE_NONE;
}

uint32_t to_sdl_window_mode(window_mode_t wm)
{
    switch (wm) {
    case window_mode_t::window:                 return 0; break;
    case window_mode_t::full_screen:            return SDL_WINDOW_FULLSCREEN; break;
    case window_mode_t::full_screen_desktop:    return SDL_WINDOW_FULLSCREEN_DESKTOP; break;
    default: break;
    }
    return 0; // Normal window
}

window_mode_t to_window_mode(uint32_t sdl_window_mode)
{
    switch (sdl_window_mode) {
    case 0:                             return window_mode_t::window; break;
    case SDL_WINDOW_FULLSCREEN:         return window_mode_t::full_screen; break;
    case SDL_WINDOW_FULLSCREEN_DESKTOP: return window_mode_t::full_screen_desktop; break;
    default: break;
    }
    return window_mode_t::window; // Normal window
}

} // namespace cpaf::gui
