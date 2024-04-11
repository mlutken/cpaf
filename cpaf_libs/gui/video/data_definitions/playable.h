#pragma once
#include <chrono>
#include <string_view>
#include <nlohmann/json.hpp>

namespace cpaf::locale {
class translator;
}


namespace cpaf::gui::video {

class playable
{
public:
    static constexpr std::string_view               user_selected_subtitle_lc = "user_selected_subtitle";

    static nlohmann::json   create_json             ();
    static nlohmann::json   create_json             (std::string path);
    static nlohmann::json   create_json             (std::string path, std::string subtitle_path);
    static nlohmann::json   create_json             (std::string path, std::string subtitle_path, const std::string& language_code);
    static void             auto_set_location_type  (nlohmann::json& jo);
    static void             add_subtitle            (nlohmann::json& jo, std::string subtitle_path, std::string_view language_code= user_selected_subtitle_lc);


    playable();
    explicit playable(std::string path);
    explicit playable(nlohmann::json playable);
    playable(std::string path, std::string subtitle_path);
    playable(std::string path, std::string subtitle_path, const std::string& language_code);


    const nlohmann::json&           json                    () const { return jo_;  }

    void                            auto_set_location_type  ();

    void                            update_calculated       (const cpaf::locale::translator& tr);
    std::string                     path                    () const;
    std::chrono::microseconds       start_time              () const;
    void                            set_start_time          (std::chrono::microseconds start_time);
    std::string                     start_time_str          () const;
    std::string                     get_best_subtitle_path  (std::string_view language_code = user_selected_subtitle_lc) const;
    bool                            has_subtitle            (std::string_view language_code = user_selected_subtitle_lc) const;
    std::vector<std::string>        subtitle_language_codes () const;
    const nlohmann::json&           subtitles               () const;

    bool                            is_valid                () const;

    std::string                     dbg_str                 () const;
    void                            dbg_print               () const;

private:
    void                            update_subtitles        (const cpaf::locale::translator& tr);

    mutable nlohmann::json          jo_;
    nlohmann::json::object_t        empty_object_;
    nlohmann::json::array_t         empty_array_;
};

} // namespace cpaf::gui::video
