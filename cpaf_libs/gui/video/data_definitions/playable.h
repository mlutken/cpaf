#pragma once
#include <chrono>
#include <string_view>
#include <cpaf_libs/utils/cpaf_json_utils.h>

namespace cpaf::locale {
class translator;
}


namespace cpaf::gui::video {

class playable
{
public:
    static constexpr std::string_view               user_subtitle_language_code = "user_selected_subtitle";

    static nlohmann::json   create_json             ();
    static nlohmann::json   create_json             (std::string path);
    static nlohmann::json   create_json             (std::string path, std::string subtitle_path);
    static nlohmann::json   create_json             (std::string path, std::string subtitle_path, const std::string& language_code);
    static void             auto_set_location_type  (nlohmann::json& jo);
    static void             add_subtitle            (nlohmann::json& jo, std::string subtitle_path, std::string_view language_code= user_subtitle_language_code);


    playable();
    explicit playable(std::string path);
    explicit playable(nlohmann::json playable);
    playable(std::string path, std::string subtitle_path);
    playable(std::string path, std::string subtitle_path, const std::string& language_code);


    const nlohmann::json&       json                    () const { return jo_;  }

    void                        auto_set_location_type  ();

    void                        update_calculated       (const cpaf::locale::translator& tr);
    std::string                 path                    () const;
    std::chrono::microseconds   start_time              () const;
    void                        set_start_time          (std::chrono::microseconds start_time);
    std::string                 start_time_str          () const;
///    std::string                 default_subtitle_path   (const std::string& language_code = "user_selected_subtitle") const;
    std::string                 get_best_subtitle_path  (std::string_view language_code = user_subtitle_language_code) const;
    bool                        has_subtitle            (std::string_view language_code = user_subtitle_language_code) const;
    std::vector<std::string>    subtitle_language_codes () const;



    bool                        is_valid                () const;

    std::string                 dbg_str                 () const;
    void                        dbg_print               () const;

private:


    mutable nlohmann::json jo_;
};

} // namespace cpaf::gui::video
