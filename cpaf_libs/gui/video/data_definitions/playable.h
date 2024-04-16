#pragma once
#include <chrono>
#include <string_view>
#include <cpaf_libs/utils/cpaf_json_utils.h>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/gui/color.h>

namespace cpaf::locale {
class translator;
}


namespace cpaf::gui::video {

class playable
{
public:
    struct subtitles_select_entry_t {
        std::string     path;
        std::string     language_name;
    };

    using subtitles_select_vector = std::vector<subtitles_select_entry_t>;

    static constexpr std::string_view               subtitle_user_selected_lc   = "subtitle_user_selected_lc";

private:
    static nlohmann::json   create_json             ();
public:


    playable();
    explicit playable(std::string path);
    explicit playable(nlohmann::json playable);
    playable(std::string path, std::string subtitle_path);
    playable(std::string path, std::string subtitle_path, const std::string& language_code);


    const nlohmann::json&           json                    () const { return jo_;  }

    void                            update_calculated       (const cpaf::locale::translator& tr,
                                                             const cpaf::video::stream_info_vec* streams_info_ptr,
                                                             bool force) const;
    std::string                     str                     (const std::string& id) const                       { return cpaf::json_value_str(jo_, id, "");     }
    std::int32_t                    int32                   (const std::string& id) const                       { return cpaf::json_value_int32(jo_, id, 0);    }
    bool                            bool_val                (const std::string& id) const                       { return cpaf::json_value_bool(jo_, id, "");    }
    float                           float_val               (const std::string& id) const                       { return cpaf::json_value_float(jo_, id, 0);    }
    std::chrono::seconds            time_s                  (const std::string& id) const                       { return std::chrono::seconds(cpaf::json_value_int32(jo_, id, 0)); }

    void                            str_set                 (const std::string& id, const std::string& val)     { jo_[id] = val; }
    void                            int32_set               (const std::string& id, int32_t val)                { jo_[id] = val; }
    void                            float_set               (const std::string& id, float val)                  { jo_[id] = val; }
    void                            time_s_set              (const std::string& id, std::chrono::seconds val)   { jo_[id] = val.count(); }


    void                            set_path                (std::string path);
    std::string                     path                    () const;
    bool                            has_path                () const    { return !path().empty(); }
    std::chrono::microseconds       start_time              () const;
    void                            set_start_time          (std::chrono::microseconds start_time);
    std::string                     start_time_str          () const;

    std::string                     find_best_subtitle_path  (std::string& language_code) const;
    void                            add_subtitle            (std::string subtitle_path, std::string_view language_code);
    void                            set_subtitle_user       (std::string subtitle_path)                         { str_set("subtitle_user", subtitle_path);  }
    std::string                     subtitle_user           () const                                            { return str("subtitle_user");              }
    bool                            has_subtitle            (std::string_view language_code = subtitle_user_selected_lc) const;
    nlohmann::json                  get_subtitle            (std::string_view language_code = subtitle_user_selected_lc) const;
    void                            remove_subtitle         (std::string_view language_code);
    std::vector<std::string>        subtitle_language_codes () const;
    const nlohmann::json&           subtitles               () const;

    const cpaf::video::subtitle_source_entries_t&
    selectable_subtitles                                    () const { return selectable_subtitles_; }

    cpaf::video::subtitle_source_entry_t
                                    selectable_subtitle_entry(std::string_view language_code) const;
    int32_t                         selectable_subtitle_index_of (std::string_view language_code) const;
    bool                            has_selectable_subtitle (std::string_view language_code) const { return selectable_subtitle_index_of(language_code) != -1; }

    bool                            is_valid                () const;

    std::string                     dbg_str                 () const;
    void                            dbg_print               () const;

private:
    nlohmann::json::iterator        find_subtitle                   (std::string_view language_code) const;
    void                            ensure_subtitles_array_exists   () const;
    void                            auto_set_location_type  ();

    mutable nlohmann::json                          jo_;
    nlohmann::json::object_t                        empty_object_;
    nlohmann::json::array_t                         empty_array_;
    mutable cpaf::video::subtitle_source_entries_t  selectable_subtitles_;
//    mutable subtitles_select_vector subtitles_select_entries_;
    mutable std::string                             translator_id_;
};

} // namespace cpaf::gui::video
