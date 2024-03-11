#include "playable.h"
#include <filesystem>
#include <cpaf_libs/filesystem/cpaf_file_directory_functions.h>

namespace fs = std::filesystem;
using namespace cpaf::filesystem;

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


void playable::set_location_type(nlohmann::json& jo) {
    const auto path = jo["path"];
    if (fs::exists(path)){
        jo["source_location_type"] = "local";
    }
    else if (detect_media_location(path) == media_location::remote) {
        jo["source_location_type"] = "remote";
    }
}

nlohmann::json playable::create_json(std::string path)
{
    auto jo = create_json();
    jo["path"] = std::move(path);
    set_location_type(jo);
    return jo;
}

nlohmann::json playable::create_json(std::string path, std::string subtitle_path)
{
    auto jo = create_json();
    jo["path"] = std::move(path);
    set_location_type(jo);
    jo["subtitles"]["default"] = std::move(subtitle_path);
    return jo;
}

nlohmann::json playable::create_json(std::string path,
                                           std::string subtitle_path,
                                           std::string language_code)
{
    auto jo = create_json();
    jo["path"] = std::move(path);
    set_location_type(jo);
    jo["subtitles"]["default"] = subtitle_path;
    jo["subtitles"][language_code] = std::move(subtitle_path);;
    return jo;
}


} // namespace cpaf::gui::video
