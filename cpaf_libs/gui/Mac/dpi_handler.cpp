#include "../dpi_handler.h"

#include <cmath>

#include "Core/Debug/Instrumentor.hpp"

namespace cpaf::gui {


float dpi_handler::get_scale() {
//  APP_PROFILE_FUNCTION();

  constexpr int display_index{0};
  // @todo: This should be 72.0F on Mac, but it seems like it is not. I'm not
  //  sure why, but this works ¯\_(ツ)_/¯
  const float default_dpi{96.0F};
  float dpi{default_dpi};

  SDL_GetDisplayDPI(display_index, nullptr, &dpi, nullptr);

  return std::floor(dpi / default_dpi);
}

WindowSize dpi_handler::get_dpi_aware_window_size(const window::settings& settings) {
//  APP_PROFILE_FUNCTION();

  return {settings.width, settings.height};
}

void dpi_handler::set_render_scale(SDL_Renderer* renderer) {
//  APP_PROFILE_FUNCTION();

  auto scale{get_scale()};
  SDL_RenderSetScale(renderer, scale, scale);
}

void dpi_handler::set_global_font_scaling(ImGuiIO* io) {
//  APP_PROFILE_FUNCTION();

  io->FontGlobalScale = 1.0F / get_scale();
}

} // namespace capf::gui
