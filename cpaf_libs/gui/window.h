#pragma once

#include <SDL2/SDL.h>

#include <string>

namespace cpaf::gui {

class window {
 public:
    struct settings {
    std::string title;
    const int width{1280};
    const int height{720};
  };

    explicit window(const settings& settings);
  ~window();

  window(const window&) = delete;
  window(window&&) = delete;
  window& operator=(window other) = delete;
  window& operator=(window&& other) = delete;

  [[nodiscard]] SDL_Window* get_native_window() const;
  [[nodiscard]] SDL_Renderer* get_native_renderer() const;

 private:
  SDL_Window* m_window{nullptr};
  SDL_Renderer* m_renderer{nullptr};
};

} // namespace capf::gui
