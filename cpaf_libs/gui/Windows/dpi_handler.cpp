#include "../dpi_handler.h"

//#include "Core/Debug/Instrumentor.hpp"

namespace cpaf::gui {

float dpi_handler::get_scale() {
//  APP_PROFILE_FUNCTION();

  constexpr int display_index{0};
  const float default_dpi{96.0F};
  float dpi{default_dpi};

  SDL_GetDisplayDPI(display_index, nullptr, &dpi, nullptr);

  return dpi / default_dpi;
}

WindowSize dpi_handler::get_dpi_aware_window_size(const window::settings& settings) {
//  APP_PROFILE_FUNCTION();

  const float scale{dpi_handler::get_scale()};
  const int width{static_cast<int>(static_cast<float>(settings.width) * scale)};
  const int height{static_cast<int>(static_cast<float>(settings.height) * scale)};
  return {width, height};
}

void dpi_handler::set_render_scale([[maybe_unused]] SDL_Renderer* renderer) {
//  APP_PROFILE_FUNCTION();
  // do nothing
}

void dpi_handler::set_global_font_scaling([[maybe_unused]] ImGuiIO* io) {
//  APP_PROFILE_FUNCTION();
  // do nothing
}

} // namespace capf::gui
