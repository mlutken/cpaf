#include "sdl_convert.h"

namespace cpaf::gui {

SDL_Rect to_sdl_rect(rect geom)
{
    return SDL_Rect{static_cast<int>(geom.top_left().x()),
                    static_cast<int>(geom.top_left().y()),
                    static_cast<int>(geom.size().width()),
                    static_cast<int>(geom.size().height())};
}

rect to_render_geometry(SDL_Rect rct)
{
    return rect{static_cast<float>(rct.x),
                static_cast<float>(rct.y),
                static_cast<float>(rct.w),
                static_cast<float>(rct.h)};

 }

} // namespace cpaf::gui
