#pragma once
#include <string>
#include <span>
#include <vector>
#include <map>
#include <cstdint>
#include <imgui.h>


namespace cpaf::gui {

/**

## Convert ttf files to c array use xxd to get started
 $ xxd -i myfont.ttf > myfont.h

*/
struct imgui_fonts {
    static  std::span<const unsigned char> manrope              ();
    static  std::span<const unsigned char> abeezee_regular      ();
    static  std::span<const unsigned char> abeezee_italic       ();
    static  std::span<const unsigned char> icons_fa_regular_400 ();
    static  std::span<const unsigned char> icons_fa_solid_900   ();

    static const std::span<const unsigned char>     font_data   (const std::string&  font_name);

    static imgui_fonts& instance();

    ImFont*     add                 (const std::string& font_name, int32_t size_pixels);
    ImFont*     merge_with_previous (const std::string& font_name, int32_t size_pixels, float merge_size_adjust_factor, uint32_t range_min, uint32_t range_max);
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
