#pragma once

#include <SDL.h>
#include <imgui.h>

#include <cpaf_libs/gui/window.h>

namespace cpaf::gui {

struct WindowSize {
  int width;
  int height;
};

class DPIHandler {
 public:
  [[nodiscard]] static float get_scale();

     [[nodiscard]] static WindowSize get_dpi_aware_window_size(const window::settings& settings);

  static void set_render_scale(SDL_Renderer* renderer);
  static void set_global_font_scaling(ImGuiIO* io);
};

} // namespace capf::gui
