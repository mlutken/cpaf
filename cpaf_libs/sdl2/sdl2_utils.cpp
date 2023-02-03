#include "sdl2_utils.h"

namespace cpaf::sdl2 {


SDL_Texture* source;

// loading etc ...


surface_dimensions_t texture_dimensions(SDL_Texture* texture)
{
    surface_dimensions_t dimensions;
    auto retval = SDL_QueryTexture(texture, NULL, NULL, &dimensions.x(), &dimensions.y());
    if (retval != 0) {
        dimensions.set(-1,-1);
    }
    return dimensions;
}

SDL_Rect texture_destination_rect(SDL_Texture* texture, int32_t x_pos, int32_t y_pos)
{
    SDL_Rect rect;
    rect.x = x_pos;
    rect.y = y_pos;
    auto retval = SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    if (retval != 0) {
        rect.w = -1;
        rect.h = -1;
    }
    return rect;
}


} //END namespace cpaf::sdl2


