#pragma once
#include <vector>
#include <string>
#include <memory>
#include <filesystem>

#include <cpaf_libs/net/cpaf_http_convenience.h>
#include <cpaf_libs/video/av_util.h>

namespace KZip {
class ZipArchive;
}

namespace cpaf::video {


/**

@see https://github.com/troldal/KZip


*/
class subtitle_text_file_data
{
public:
    static std::string          get_archive_path        (const std::string& resource_path);
    static std::string          get_archive_srt_path    (const std::string& resource_path);

    subtitle_text_file_data();
    ~subtitle_text_file_data();
    explicit                    subtitle_text_file_data (const std::string& resource_path,
                                                         std::chrono::milliseconds timeout = std::chrono::minutes(1));
    bool                        open                    (const std::string& resource_path,
                                                         std::chrono::milliseconds timeout = std::chrono::minutes(1));
    void                        cancel_open             ();

    // ----------------------
    // --- Info functions ---
    // ----------------------
    const std::string&          resource_path			() const { return resource_path_; }
    std::string                 archive_path			() const;
    std::string                 archive_srt_file_path   () const;
    std::string                 archive_info_file_path  () const;
    void                        language_code_set		(std::string_view lc) { language_code_ = lc; }
    const std::string&          language_code			() const { return language_code_; }
    bool                        is_zip_archive			() const;
    size_t                      zip_num_entries         () const;
    std::vector<std::string>    zip_entry_names         () const;
    std::string                 find_first_srt_in_zip   () const;
    std::string                 find_first_info_in_zip  () const;
    bool                        has_file_in_zip         (const std::string& file_path_in_archive) const;
    bool                        resource_is_remote      () const { return !local_download_path_.empty(); }
    cpaf::net::curl::state_t    download_state          () const;
    const cpaf::net::curl&      curl                    () const { return curl_; }


    // ----------------------
    // --- Data functions ---
    // ----------------------
    const std::string&          srt_file_data			() const;
    const std::string&          info_file_data			() const;

    // --- ---
    void                        language_code_set       (const std::string language_code) { language_code_ = language_code; }

private:
    // ------------------------
    // --- PRIVATE: Helpers ---
    // ------------------------
    void                        close                   ();
    void                        wait_for_curl_ready     ();
    bool                        is_network_url          (const std::string& path) const;

    bool                        download_and_open_file  (const std::string& resource_path,
                                                         std::chrono::milliseconds timeout = std::chrono::minutes(1));
    std::string                 read_srt_data           () const;
    std::string                 read_info_data          () const;
    std::string                 read_text_file_from_zip (const std::string& file_path_in_archive) const;
    std::string                 read_srt_data_from_file (const std::string& subtitle_file_path) const;

    std::string                                 resource_path_;
    std::string                                 archive_path_;
    std::filesystem::path                       local_download_path_;
    std::string                                 language_code_;
    mutable std::string                         archive_srt_file_path_;
    mutable std::string                         archive_info_file_path_;
    mutable std::string                         srt_file_data_;
    mutable std::string                         info_file_data_;
    std::unique_ptr<KZip::ZipArchive>           zip_archive_;
    cpaf::net::curl                             curl_;
};

} // namespace cpaf::video
