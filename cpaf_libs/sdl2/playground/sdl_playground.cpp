#include <iostream>
#include <vector>
//https://www.jamesfmackenzie.com/2019/12/01/webassembly-graphics-with-sdl/

#include <SDL2/SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <stdlib.h>

using namespace std;

#if 1
#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    // SDL_setenv("SDL_VIDEODRIVER", "x11", 1);  // or "wayland"
    int err = 0;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    const uint32_t window_flags{SDL_WINDOW_RESIZABLE |  SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN};
    // const uint32_t window_flags{SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN};
    // const uint32_t window_flags{ SDL_WINDOW_SHOWN};

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        window_flags
        );
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Uint32 format = SDL_GetWindowPixelFormat(window);
    std::cout << "Window pixel format: " << SDL_GetPixelFormatName(format) << std::endl;
    // std::cout << "SDL version: " << SDL_GetVersion() << std::endl;

    SDL_RendererInfo render_info;

    err = SDL_GetRendererInfo(renderer, &render_info);
    if (err == 0) {
        std::cout << "render_info: " << render_info.name << std::endl;
    }

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 123, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

#else

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

void print_sdl_err()
{
    cerr << "SDL_GetError: " << SDL_GetError() << "\n";
}

int main(int /*argc*/, char* /*argv*/ []) {
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    int err = 0;
    err = SDL_Init(SDL_INIT_VIDEO);
    if (err != 0) {
        cerr << "SDL_Init: " << err << "\n";
        print_sdl_err();
    }
    err = SDL_CreateWindowAndRenderer(512, 512, 0, &window, &sdl_renderer);
    if (err != 0) {
        cerr << "SDL_CreateWindowAndRenderer: " << err << "\n";
        print_sdl_err();
    }
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

#endif
