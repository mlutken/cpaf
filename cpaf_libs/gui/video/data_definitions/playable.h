#pragma once
#include <chrono>
#include <cpaf_libs/utils/cpaf_json_utils.h>

namespace cpaf::gui::video {

class playable
{
public:
    static nlohmann::json create_json();
    static nlohmann::json create_json(std::string path);
    static nlohmann::json create_json(std::string path, std::string subtitle_path);
    static nlohmann::json create_json(std::string path, std::string subtitle_path, const std::string& language_code);
    static void auto_set_location_type(nlohmann::json& jo);

    playable();
    explicit playable(std::string path);
    explicit playable(nlohmann::json playable);
    playable(std::string path, std::string subtitle_path);
    playable(std::string path, std::string subtitle_path, const std::string& language_code);


    const nlohmann::json&       json                    () const { return jo_;  }

    void                        auto_set_location_type  ();

    std::string                 path                    () const;
    std::chrono::microseconds   start_time              () const;
    std::string                 start_time_str          () const;
    std::string                 default_subtitle_path   (const std::string& language_code = "default") const;
    bool                        has_subtitle            (const std::string& language_code = "default") const;
    std::vector<std::string>    subtitle_language_codes () const;



    bool                        is_valid                () const;


private:
    nlohmann::json jo_;
};

} // namespace cpaf::gui::video
