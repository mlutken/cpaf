#include "../resources.h"

#include <SDL.h>

namespace cpaf::gui {

static const std::string BASE_PATH{SDL_GetBasePath()};

std::filesystem::path resources::resource_path(const std::filesystem::path& file_path) {
  std::filesystem::path font_path{BASE_PATH};
  font_path /= file_path;
  return font_path;
}

std::filesystem::path resources::font_path(const std::string_view& font_file) {
  return resource_path(font_file);
}

} // namespace capf::gui
