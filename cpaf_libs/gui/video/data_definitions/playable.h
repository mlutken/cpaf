#pragma once
#include <cpaf_libs/utils/cpaf_json_utils.h>

namespace cpaf::gui::video {

class playable
{
public:
    static nlohmann::json create_json();
    static nlohmann::json create_json(std::string path);
    static nlohmann::json create_json(std::string path, std::string subtitle_path);
    static nlohmann::json create_json(std::string path, std::string subtitle_path, std::string language_code);
    static void set_location_type(nlohmann::json& jo);

private:
};

} // namespace cpaf::gui::video
