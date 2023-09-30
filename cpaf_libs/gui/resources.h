#pragma once

#include <filesystem>
#include <string_view>

namespace cpaf::gui {


class resources {
 public:
  [[nodiscard]] static std::filesystem::path resource_path(const std::filesystem::path& file_path);
  [[nodiscard]] static std::filesystem::path font_path(const std::string_view& font_file);
};

} // namespace capf::gui
