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
        "subtitles": {
            "default": ""
        },
        "title": "",
        "trailers": [],
        "year": 0
    }
    )json"
    );

}


nlohmann::json playable::create_json(std::string path)
{
    auto jo = create_json();
    jo["path"] = std::move(path);
    jo["subtitles"] = json::array();
    auto_set_location_type(jo);
    return jo;
}

nlohmann::json playable::create_json(std::string path, std::string subtitle_path)
{
    auto jo = create_json();
    jo["path"] = std::move(path);
    auto_set_location_type(jo);
    add_subtitle(jo, subtitle_path, "");
    return jo;
}

nlohmann::json playable::create_json(std::string path,
                                     std::string subtitle_path,
                                     const std::string& language_code)
{
    auto jo = create_json();
    jo["path"] = std::move(path);
    auto_set_location_type(jo);
    add_subtitle(jo, subtitle_path, language_code);
    return jo;
}


void playable::auto_set_location_type(nlohmann::json& jo) {
    const auto path = jo["path"].get<std::string>();
    fs::path p(path);
    std::error_code ec;
    if (fs::exists(path, ec)){
        jo["source_location_type"] = "local";
    }
    else if (detect_media_location(path) == media_location::remote) {
        jo["source_location_type"] = "remote";
    }
}

void playable::add_subtitle(nlohmann::json& jo, std::string subtitle_path, std::string_view language_code)
{
    if (subtitle_path.empty()) {
        return;
    }
    if (!jo.contains("subtitles")) {
        jo["subtitles"] = json::array();
    }

    if (language_code.empty()) {
        language_code = playable::subtitle_user_selected_lc;
    }
    jo["subtitles"].push_back( {{"path", std::move(subtitle_path)}, {"language_code", language_code}});
}

// ------------------------
// --- PUBLIC functions ---
// ------------------------

playable::playable()
    : jo_(playable::create_json())
{
}

playable::playable(std::string path)
    : jo_(playable::create_json(std::move(path)))
{
}

playable::playable(nlohmann::json playable)
    : jo_(std::move(playable))
{
}

playable::playable(std::string path, std::string subtitle_path)
    : jo_(playable::create_json(std::move(path), std::move(subtitle_path)))
{
}

playable::playable(std::string path, std::string subtitle_path, const std::string& language_code)
    : jo_(playable::create_json(std::move(path), std::move(subtitle_path), language_code))
{
}

void playable::auto_set_location_type()
{
    playable::auto_set_location_type(jo_);
}

void playable::update_calculated(const locale::translator& tr)
{
    update_subtitles(tr);
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
    auto start_seconds = duration_cast<seconds>(start_time);
    jo_["start_time"] = std::to_string(start_seconds.count()) + "s";
}

std::string playable::start_time_str() const
{
    return cpaf::json_value_str(jo_["start_time"], "");
}

std::string playable::get_best_subtitle_path(std::string_view language_code) const
{
    if (language_code.empty()) {
        language_code = playable::subtitle_user_selected_lc;
    }

    for (auto& el : jo_["subtitles"].items()) {
        auto sub = el.value();
        if (sub["language_code"] == language_code) {
            return sub["path"];
        }
    }
    return "";
}

void playable::add_subtitle(std::string subtitle_path, std::string_view language_code)
{
    ensure_subtitles();
    if (language_code.empty()) {
        language_code = playable::subtitle_user_selected_lc;
    }

    if (language_code == subtitle_none_selected_lc) {
        ensure_subtitle_none_selected();
    }
    else if (language_code == subtitle_user_selected_lc) {
        ensure_subtitle_user_selected(subtitle_path);
    }
    else {
        add_subtitle_helper(subtitle_path, language_code);
    }
}

bool playable::has_subtitle(std::string_view language_code) const
{
    if (language_code.empty()) {
        return false;
    }
    for (auto& el : jo_["subtitles"].items()) {
        auto sub = el.value();
        if (sub["language_code"] == language_code) {
            return true;
        }
    }
    return false;
}

void playable::remove_subtitle(std::string_view language_code)
{
    ensure_subtitles();
    const auto end = jo_["subtitles"].end();

//    json::iterator it = jo_["subtitles"].begin();
//    for(; it != end; ++it) {
//        if ((*it)["language_code"] == language_code) {
////        if (it->at("language_code") == language_code) {
//            break;
//        }
//    }

    json::iterator it = find_subtitle(language_code);
    if (it != end) {
        jo_["subtitles"].erase(it);
    }

//    auto finder = [language_code](const auto& el) -> bool {
////        return language_code == el.value()["language_code"];
//        return language_code == el.value()["language_code"].template get<std::string>();
//    };
//    const auto& jitems = jo_["subtitles"].items();
//    const auto iter = std::find_if(jitems.begin(), jitems.end(), finder);
//    if (iter != jitems.end()) {
//        jo_["subtitles"].erase(iter);
//    }
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
json::iterator playable::add_subtitle_helper(std::string subtitle_path, std::string_view language_code)
{
    auto it = find_subtitle(language_code);
    const auto end = jo_["subtitles"].end();
    if (it != end) {
        nlohmann::json& subtitle_jo = *it;
        subtitle_jo["path"] = std::move(subtitle_path);
    }
    else {
        jo_["subtitles"].push_back({{"path", std::move(subtitle_path)}, {"language_code", language_code}});
    }
}


json::iterator playable::find_subtitle(std::string_view language_code)
{
    json::iterator it = jo_["subtitles"].begin();
    const auto end = jo_["subtitles"].end();
    for(; it != end; ++it) {
        if ((*it)["language_code"].get<std::string>() == language_code) {
            break;
        }
    }
    return it;
}

void playable::ensure_subtitles()
{
    if (!jo_.contains("subtitles")) {
        jo_["subtitles"] = json::array();
    }
}

void playable::ensure_subtitle_none_selected()
{
    remove_subtitle(subtitle_none_selected_lc);
    jo_["subtitles"].insert(jo_["subtitles"].begin(),  {{"path", ""}, {"language_code", subtitle_none_selected_lc}});
}

void playable::ensure_subtitle_user_selected(std::string user_selected_subtitle_path)
{
    auto it = find_subtitle(subtitle_user_selected_lc);
    nlohmann::json subtitle_jo;
    if (it != jo_["subtitles"].end()) {
        subtitle_jo = *it;
    }
    if (!user_selected_subtitle_path.empty()) {
        subtitle_jo["path"] = std::move(user_selected_subtitle_path);
    }
    subtitle_jo["language_code"] = subtitle_user_selected_lc;
    remove_subtitle(subtitle_user_selected_lc);

    jo_["subtitles"].insert(jo_["subtitles"].begin()+1,  subtitle_jo);
}

void playable::ensure_default_subtitles(std::string user_selected_subtitle_path)
{
    ensure_subtitles();
    ensure_subtitle_none_selected();
    ensure_subtitle_user_selected(user_selected_subtitle_path);
}


void playable::update_subtitles(const locale::translator& tr)
{
    for (auto& el : jo_["subtitles"].items()) {
        auto& sub = el.value();
        const auto language_code = cpaf::json_value_str(sub, "language_code", "");
        if (!language_code.empty()) {
            sub["language_name_en"] = cpaf::locale::language_codes::language_name(language_code);
            sub["language_name"] = cpaf::locale::language_codes::language_name(language_code, tr);
        }
    }
}


} // namespace cpaf::gui::video
