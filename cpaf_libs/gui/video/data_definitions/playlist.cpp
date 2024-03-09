#include "playlist.h"
#include "playable.h"

namespace cpaf::gui::video {


nlohmann::json playlist::create_json()
{
    return nlohmann::json::parse(
        R"(
    {
        "data_type": "playlist",
        "items": [],
        "media_type": "",
        "name": "auto"
    }
    )");

}

nlohmann::json playlist::create_json(const std::vector<std::string>& paths, const std::string& media_type)
{
    auto pl = create_json();
    add_playables(pl, paths, media_type);
    return pl;
}

void playlist::add_playables(nlohmann::json& pl,
                             const std::vector<std::string>& paths,
                             const std::string& media_type)
{
    if (pl["media_type"].empty()) {
        pl["media_type"] = media_type;
    }
    for (const auto& p: paths) {
        pl["items"].push_back(playable::create_json(p));
    }
}

} // namespace cpaf::gui::video
