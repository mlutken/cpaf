#pragma once
#include <vector>
#include <cpaf_libs/utils/cpaf_json_utils.h>

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
