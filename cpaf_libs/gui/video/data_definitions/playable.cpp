#include "playable.h"
#include <filesystem>
#include <algorithm>
#include <string>
#include <cpaf_libs/utils/cpaf_json_utils.h>
#include <cpaf_libs/locale/translator.h>
#include <cpaf_libs/locale/language_codes.h>
#include <cpaf_libs/filesystem/cpaf_file_directory_functions.h>
#include <cpaf_libs/time/cpaf_time.h>

namespace fs = std::filesystem;
using namespace std;
using namespace cpaf;
using namespace cpaf::filesystem;
using namespace std::chrono;
using namespace nlohmann;

namespace cpaf::gui::video {

// ------------------------
// --- STATIC functions ---
// ------------------------

nlohmann::json playable::create_json()
{
    return nlohmann::json::parse(
    u8R"json(
    {
        "data_type": "playable",
        "description": "",
        "genre": "",
        "media_type": "",
        "path": "",
        "primary_image": "",
        "rating": -1,
        "imdb_rating": -1,
        "source_location_type": "",
        "start_time": "0",
        "subtitles": [
        ],
        "title": "",
        "trailers": [],
        "year": 0
    }
    )json"
    );

}


// ------------------------
// --- PUBLIC functions ---
// ------------------------

playable::playable()
    : jo_(playable::create_json())
{
    dbg_print(); // FIXMENM
}

playable::playable(std::string path)
    : playable()
{
    set_path(path);
}

playable::playable(nlohmann::json playable)
    : jo_(std::move(playable))
{
    auto_set_location_type();
}

playable::playable(std::string path, std::string subtitle_path)
    : playable()
{
    set_path(path);
    add_subtitle(subtitle_path, subtitle_user_selected_lc);
}

playable::playable(std::string path, std::string subtitle_path, const std::string& language_code)
    : playable()
{
    set_path(path);
    add_subtitle(subtitle_path, language_code);
}

void playable::update_calculated(const locale::translator& tr) const
{
    if (translator_id_ == tr.id()) {
        return;
    }
    const auto sub_source = cpaf::video::subtitle_source_t::text_file;
    translator_id_ = tr.id();
    selectable_subtitles_.clear();
    selectable_subtitles_.push_back({subtitle_user(), string(subtitle_user_selected_lc), tr.tr("User selected"), sub_source});
    for (auto& el : jo_["subtitles"].items()) {
        auto& sub_jo = el.value();
        const auto language_code = cpaf::json_value_str(sub_jo, "language_code", "");
        if (language_code.empty()) {
            continue;
        }
        const string language_name = tr.tr(cpaf::locale::language_codes::language_name(language_code));
        selectable_subtitles_.push_back({sub_jo["path"], language_code, language_name, sub_source});
    }
    auto comp = [](const auto& entry_lhs, const auto entry_rhs) { return entry_lhs.language_name < entry_rhs.language_name; };

    std::sort(selectable_subtitles_.begin() +2, selectable_subtitles_.end(), comp);
}

void playable::set_path(std::string path)
{
    jo_["path"] = std::move(path);
    auto_set_location_type();
}

std::string playable::path() const
{
    return cpaf::json_value_str(jo_["path"], "");
}

microseconds playable::start_time() const
{
    return duration_cast<microseconds>(cpaf::time::parse_duration(start_time_str(), nanoseconds(0)));
}

void playable::set_start_time(std::chrono::microseconds start_time)
{
    auto start_seconds = duration_cast<std::chrono::seconds>(start_time);
    jo_["start_time"] = std::to_string(start_seconds.count()) + "s";
}

std::string playable::start_time_str() const
{
    return str("start_time");
}

/** Try finding a subtitle file with given language code
First look for exact match on desired language_code.
If not found see if we have a user (most likely from command line) specified path that we can use
In this case we modify the inout parameter language_code to be
playable::subtitle_user_selected_lc ("subtitle_user_selected_lc")
*/
std::string playable::find_best_subtitle_path(std::string& language_code) const
{
    for (auto& el : jo_["subtitles"].items()) {
        auto sub = el.value();
        if (sub["language_code"] == language_code) {
            return sub["path"];
        }
    }
    language_code = subtitle_user_selected_lc;
    return subtitle_user();
}

void playable::add_subtitle(std::string subtitle_path, std::string_view language_code)
{
    ensure_subtitles_array_exists();
    auto it = find_subtitle(language_code);
    const auto end = jo_["subtitles"].end();
    if (it != end) {
        nlohmann::json& subtitle_jo = *it;
        subtitle_jo["path"] = std::move(subtitle_path);
    }
    else {
        jo_["subtitles"].push_back({{"path", subtitle_path}, {"language_code", language_code}});
    }
}


bool playable::has_subtitle(std::string_view language_code) const
{
    if (language_code == subtitle_user_selected_lc) {
        return !subtitle_user().empty();
    }
    for (auto& el : jo_["subtitles"].items()) {
        auto sub = el.value();
        if (sub["language_code"] == language_code) {
            return true;
        }
    }
    return false;
}

nlohmann::json playable::get_subtitle(std::string_view language_code) const
{
    nlohmann::json sub_jo;
    sub_jo["language_code"] = language_code;
    if (language_code == subtitle_user_selected_lc) {
        sub_jo["path"] = subtitle_user();
    }
    else {
        ensure_subtitles_array_exists();
        auto it = find_subtitle(language_code);
        const auto end = jo_["subtitles"].end();
        if (it != end) {
            sub_jo = *it;
        }
    }

    return sub_jo;
}

void playable::remove_subtitle(std::string_view language_code)
{
    ensure_subtitles_array_exists();
    const auto end = jo_["subtitles"].end();

    json::iterator it = find_subtitle(language_code);
    if (it != end) {
        jo_["subtitles"].erase(it);
    }
}

/// @todo Implement subtitle_language_codes()
std::vector<std::string> playable::subtitle_language_codes() const
{
    std::vector<std::string> language_codes;
    for (auto& el : jo_["subtitles"].items()) {
        auto sub = el.value();
        const auto language_code = cpaf::json_value_str(sub, "language_code", "");
        if (!language_code.empty()) {
            language_codes.push_back(language_code);
        }
    }
    return language_codes;
}

const nlohmann::json& playable::subtitles() const
{
    return cpaf::json_value_array_cref(jo_, "subtitles", empty_array_);
}

int32_t playable::selectable_subtitle_index_of(std::string_view language_code) const
{
    if (language_code.empty()) {
        return -1;
    }

    int32_t index = 0;
    for (const auto& entry: selectable_subtitles_) {
        if (entry.language_code == language_code) {
            return index;
        }
        ++index;
    }
    return -1;
}

bool playable::is_valid() const
{
    return !cpaf::json_value_str(jo_["path"], "").empty();
}

std::string playable::dbg_str() const
{
    std::stringstream ss;
    ss << jo_.dump(4);
    return ss.str();
}

void playable::dbg_print() const
{
    std::cerr << dbg_str() << "\n";
}

// -------------------------
// --- PRIVATE functions ---
// -------------------------

json::iterator playable::find_subtitle(std::string_view language_code) const
{
    json::iterator it = jo_["subtitles"].begin();
    const auto end = jo_["subtitles"].end();
    for(; it != end; ++it) {
        nlohmann::json& subtitle_jo = *it;
        const std::string& lc = subtitle_jo["language_code"];

        if (lc == language_code) {
            break;
        }
    }
    return it;
}

void playable::ensure_subtitles_array_exists() const
{
    if (!jo_.contains("subtitles")) {
        jo_["subtitles"] = json::array();
    }
}

void playable::auto_set_location_type()
{
    fs::path p(path());
    std::error_code ec;
    if (fs::exists(p, ec)){
        jo_["source_location_type"] = "local";
    }
    else if (detect_media_location(path()) == media_location::remote) {
        jo_["source_location_type"] = "remote";
    }
}


} // namespace cpaf::gui::video
