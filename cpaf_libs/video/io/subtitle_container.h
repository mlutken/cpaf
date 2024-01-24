#pragma once

#include <cpaf_libs/video/av_util.h>

namespace cpaf::video {



class subtitle_container
{
public:
    subtitle_container() = default;
    void                        resource_path_set       (const std::string& resource_path);

    // ----------------------
    // --- Info functions ---
    // ----------------------
    const std::string&          resource_path			() const { return resource_path_; }
    std::string                 archive_path			() const;
    const std::string&          language_code			() const { return language_code_; }
    bool                        is_zip_archive			() const;

    // --- ---
    void                        language_code_set       (const std::string language_code) { language_code_ = language_code; }

private:

    std::string         resource_path_;
    std::string         language_code_;

};

} // namespace cpaf::video
