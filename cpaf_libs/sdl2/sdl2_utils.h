#ifndef CPAF_SDL2_UTILS_H
#define CPAF_SDL2_UTILS_H


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <SDL2/SDL.h>
#include <math/base/v2.hpp>

namespace cpaf::sdl2 {
using surface_dimensions_t = cpaf::math::v2<int32_t>;

surface_dimensions_t    texture_dimensions      (SDL_Texture* texture);
SDL_Rect                texture_destination_rect(SDL_Texture* texture, int32_t x_pos, int32_t y_pos);

} //END namespace cpaf::sdl2


#endif //CPAF_SDL2_UTILS_H


