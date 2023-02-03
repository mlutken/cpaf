#include <iostream>
#include <vector>
//https://www.jamesfmackenzie.com/2019/12/01/webassembly-graphics-with-sdl/

#include <SDL2/SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <stdlib.h>

using namespace std;
// using namespace boost::posix_time;

SDL_Window *window;
SDL_Renderer *sdl_renderer;
SDL_Surface *surface;

void drawRandomPixels() {
    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

    uint8_t* pixels = static_cast<uint8_t*>(surface->pixels);

    for (int i=0; i < 1048576; i++) {
        uint8_t randomByte = static_cast<uint8_t>(rand() % 255);
        pixels[i] = randomByte;
    }

    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

    SDL_Texture *screenTexture = SDL_CreateTextureFromSurface(sdl_renderer, surface);

    SDL_RenderClear(sdl_renderer);
    SDL_RenderCopy(sdl_renderer, screenTexture, NULL, NULL);
    SDL_RenderPresent(sdl_renderer);

    SDL_DestroyTexture(screenTexture);
}

int main(int /*argc*/, char* /*argv*/ []) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(512, 512, 0, &window, &sdl_renderer);
    surface = SDL_CreateRGBSurface(0, 512, 512, 32, 0, 0, 0, 0);

    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(drawRandomPixels, 0, 1);
    #else
    while(1) {
        drawRandomPixels();
        SDL_Delay(16);
    }
    #endif
}

