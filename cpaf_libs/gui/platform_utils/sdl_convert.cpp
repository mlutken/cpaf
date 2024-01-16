#include "sdl_convert.h"

namespace cpaf::gui {

SDL_Rect to_sdl_rect(render_geometry_t geom)
{
    return SDL_Rect{static_cast<int>(geom.top_left.x()),
                    static_cast<int>(geom.top_left.y()),
                    static_cast<int>(geom.size.width()),
                    static_cast<int>(geom.size.height())};
}

render_geometry_t to_render_geometry(SDL_Rect rect)
{
    return render_geometry_t{static_cast<float>(rect.x),
                    static_cast<float>(rect.y),
                    static_cast<float>(rect.w),
                    static_cast<float>(rect.h)};

 }

} // namespace cpaf::gui
