#pragma once
#include <vector>
#include <nlohmann/json.hpp>

namespace cpaf::gui::video {

class playlist
{
public:
    static nlohmann::json   create_json     ();
    static nlohmann::json   create_json     (const std::vector<std::string>& paths,
                                             const std::string& media_type);
    static void             add_playables   (nlohmann::json& pl,
                                             const std::vector<std::string>& paths,
                                             const std::string& media_type);

};

} // namespace cpaf::gui::video
