#include "playable.h"
#include <filesystem>
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
    const auto path = jo["path"];
    if (fs::exists(path)){
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

std::string playable::start_time_str() const
{
    return cpaf::json_value_str(jo_["start_time"], "");
}

bool playable::is_valid() const
{
    return !cpaf::json_value_str(jo_["path"], "").empty();
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
