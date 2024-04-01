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


SDL_BlendMode to_sdl_blendmode(blendmode_t blendmode)
{
    switch (blendmode) {
    case blendmode_t::none :                return SDL_BLENDMODE_NONE; break;
    case blendmode_t::alpha :               return SDL_BLENDMODE_BLEND; break;
    case blendmode_t::additive :            return SDL_BLENDMODE_ADD; break;
    case blendmode_t::color_modulate :      return SDL_BLENDMODE_MOD; break;
    case blendmode_t::color_multiply :      return SDL_BLENDMODE_MUL; break;
    default:
        break;
    }
    return SDL_BLENDMODE_NONE;
}

} // namespace cpaf::gui
