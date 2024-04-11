#include "playable.h"
#include <filesystem>
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
    add_subtitle(jo, subtitle_path);
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
        language_code = playable::user_selected_subtitle_lc;
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
        language_code = playable::user_selected_subtitle_lc;
    }

    for (auto& el : jo_["subtitles"].items()) {
        auto sub = el.value();
        if (sub["language_code"] == language_code) {
            return sub["path"];
        }
    }
    return "";
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
