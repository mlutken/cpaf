#include "playable.h"
#include <filesystem>
#include <string>
#include <cpaf_libs/filesystem/cpaf_file_directory_functions.h>
#include <cpaf_libs/time/cpaf_time.h>

namespace fs = std::filesystem;
using namespace cpaf::filesystem;
using namespace std::chrono;

namespace cpaf::gui::video {

nlohmann::json playable::create_json()
{

    return nlohmann::json::parse(
        R"(
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
    )");

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

std::string playable::default_subtitle_path(const std::string& language_code) const
{
    auto path = cpaf::json_value_str(jo_, language_code, "");
    if (path.empty()) {
        path = cpaf::json_value_str(jo_["subtitles"]["default"], "");
    }
    return path;
}

std::string playable::get_best_subtitle_path(std::string& language_code) const
{
    std::string path;
    if (!language_code.empty()) {
        path = cpaf::json_value_str(jo_, language_code, "");
        if (!path.empty()) {
            return path;
        }
    }
    language_code = "default";
    path = cpaf::json_value_str(jo_["subtitles"]["default"], "");
    return path;

}

bool playable::has_subtitle(const std::string& language_code) const
{
    return jo_["subtitles"].contains(language_code);
}

/// @todo Implement subtitle_language_codes()
std::vector<std::string> playable::subtitle_language_codes() const
{
    std::vector<std::string> language_codes;

    return language_codes;
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

nlohmann::json playable::create_json(std::string path)
{
    auto jo = create_json();
    jo["path"] = std::move(path);
    auto_set_location_type(jo);
    return jo;
}

nlohmann::json playable::create_json(std::string path, std::string subtitle_path)
{
    auto jo = create_json();
    jo["path"] = std::move(path);
    auto_set_location_type(jo);
    jo["subtitles"]["default"] = std::move(subtitle_path);
    return jo;
}

nlohmann::json playable::create_json(std::string path,
                                     std::string subtitle_path,
                                     const std::string& language_code)
{
    auto jo = create_json();
    jo["path"] = std::move(path);
    auto_set_location_type(jo);
    jo["subtitles"]["default"] = subtitle_path;
    jo["subtitles"][language_code] = std::move(subtitle_path);;
    return jo;
}


} // namespace cpaf::gui::video
