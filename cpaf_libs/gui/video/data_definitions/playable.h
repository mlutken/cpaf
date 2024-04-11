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
    static constexpr std::string_view               subtitle_none_selected_lc   = "subtitle_none_selected_lc";
    static constexpr std::string_view               subtitle_user_selected_lc   = "subtitle_user_selected_lc";

    static nlohmann::json   create_json             ();
    static nlohmann::json   create_json             (std::string path);
    static nlohmann::json   create_json             (std::string path, std::string subtitle_path);
    static nlohmann::json   create_json             (std::string path, std::string subtitle_path, const std::string& language_code);
    static void             auto_set_location_type  (nlohmann::json& jo);
    static void             add_subtitle            (nlohmann::json& jo, std::string subtitle_path, std::string_view language_code= subtitle_user_selected_lc);


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
    std::string                     get_best_subtitle_path  (std::string_view language_code = subtitle_user_selected_lc) const;
    void                            add_subtitle            (std::string subtitle_path, std::string_view language_code);
    bool                            has_subtitle            (std::string_view language_code = subtitle_user_selected_lc) const;
    void                            remove_subtitle         (std::string_view language_code);
    std::vector<std::string>        subtitle_language_codes () const;
    const nlohmann::json&           subtitles               () const;

    bool                            is_valid                () const;

    std::string                     dbg_str                 () const;
    void                            dbg_print               () const;

private:
    nlohmann::json::iterator        add_subtitle_helper             (std::string subtitle_path, std::string_view language_code);
    nlohmann::json::iterator        find_subtitle                   (std::string_view language_code);
    void                            ensure_subtitles                ();
    void                            ensure_subtitle_none_selected   ();
    void                            ensure_subtitle_user_selected   (std::string user_selected_subtitle_path = "");
    void                            ensure_default_subtitles        (std::string user_selected_subtitle_path = "");

    void                            update_subtitles                (const cpaf::locale::translator& tr);

    mutable nlohmann::json          jo_;
    nlohmann::json::object_t        empty_object_;
    nlohmann::json::array_t         empty_array_;
};

} // namespace cpaf::gui::video
