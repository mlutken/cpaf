#pragma once
#include <string>
#include <span>
#include <vector>
#include <map>
#include <cstdint>

struct ImGuiIO;
struct ImFont;

namespace cpaf::gui {


struct imgui_fonts {
    static  std::span<const unsigned char> manrope();

    static const std::span<const unsigned char>     font_data   (const std::string&  font_name);

    static imgui_fonts& instance();

    ImFont*     add                 (const std::string& font_name, int32_t size_pixels);
    bool        add                 (const std::string& font_name, const std::vector<int32_t>& size_pixels);
    void        set_default         (const std::string& font_name, int32_t size_pixels);

    ImFont*     get                 (const std::string& font_name, int32_t size_pixels);
    ImFont*     get                 (const std::string& font_name, int32_t size_pixels, int32_t create_dist_pixels);

    ImFont*     find                (const std::string& font_name, int32_t size_pixels) const;

    void        add_pending_requested_fonts();

private:
    using font_size_map = std::map<int32_t, ImFont*, std::less<>>;
    using font_lookup_map = std::map<std::string, font_size_map, std::less<>>;

    struct to_add_t {
        std::string font;
        int32_t size_pixels;
    };

    using requested_fonts_vec = std::vector<to_add_t>;

    ImFont*    find_create_closest  (const std::string& font_name, int32_t size_pixels, int32_t create_dist_pixels);

    font_lookup_map         all_fonts_;
    requested_fonts_vec     requested_fonts_;
};

} // namespace capf::gui
