#include "subtitle_text_file_data.h"
#include <filesystem>
#include <Zippy.hpp>
#include <fmt/format.h>

#include <cpaf_libs/unicode/cpaf_u8string_utils.h>
#include <cpaf_libs/crypto/crypto_hash.h>
#include <cpaf_libs/filesystem/cpaf_special_dirs.h>
#include <cpaf_libs/net/cpaf_http_convenience.h>
#include <cpaf_libs/streams/cpaf_streams.h>
#include <cpaf_libs/filesystem/cpaf_file_directory_functions.h>

using namespace std;
using namespace cpaf::crypto;
using namespace cpaf::unicode;
using namespace cpaf::filesystem;
using namespace std::chrono;

// Some CURL command line commands:
// Download to detected filename from server: curl -O -J -L https://www.opensubtitles.org/en/subtitleserve/sub/3421248
// Download to specified new filename: curl -o myfile.zip -L https://www.opensubtitles.org/en/subtitleserve/sub/3421248


namespace cpaf::video {

string subtitle_text_file_data::get_archive_path(const std::string& resource_path)
{
    const auto p = cpaf::unicode::substring_between(resource_path, "", "#");
    if (p.empty()) {
        return resource_path;
    }
    return p;
}

string subtitle_text_file_data::get_archive_srt_path(const std::string& resource_path)
{
    return cpaf::unicode::substring_between(resource_path, "#", "");
}


subtitle_text_file_data::subtitle_text_file_data(const std::string& resource_path)
{
    open(resource_path);
}

subtitle_text_file_data::~subtitle_text_file_data()
{
    close();
}

void subtitle_text_file_data::open(const std::string& resource_path)
{
    close();
    resource_path_ = resource_path;
    archive_srt_file_path_ = get_archive_srt_path(resource_path);
    archive_path_ = get_archive_path(resource_path);

    if (is_network_url(resource_path)) {
        download_and_open_file(resource_path);
    }
    else if ( std::filesystem::exists(archive_path_) ) {
        if (cpaf::compression::detect_is_zip_file(archive_path_)) {
            zip_archive_ = std::make_unique<Zippy::ZipArchive>();
            zip_archive_->Open(archive_path_);
        }
    }
}

string subtitle_text_file_data::archive_path() const
{
    if (is_zip_archive()) {
        return archive_path_;
    }
    return "";
}

string subtitle_text_file_data::archive_srt_file_path() const
{
    if (archive_srt_file_path_.empty()) {
        archive_srt_file_path_ = find_first_srt_in_zip();
    }
    return archive_srt_file_path_;
}

string subtitle_text_file_data::archive_info_file_path() const
{
    if (archive_info_file_path_.empty()) {
        archive_info_file_path_ = find_first_info_in_zip();
    }
    return archive_info_file_path_;
}



bool subtitle_text_file_data::is_zip_archive() const
{
    return zip_archive_ != nullptr;
}


size_t subtitle_text_file_data::zip_num_entries() const
{
    if (zip_archive_) {
        return static_cast<size_t>(zip_archive_->GetNumEntries(false, true));
    }
    return 0u;
}

std::vector<string> subtitle_text_file_data::zip_entry_names() const
{
    if (zip_archive_) {
        return zip_archive_->GetEntryNames(false, true);
    }
    return {};
}

string subtitle_text_file_data::find_first_srt_in_zip() const
{
    const auto entries = zip_entry_names();
    for (auto path_in_archive: entries) {
        if (path_in_archive.ends_with(".srt")) {
            return path_in_archive;
        }
    }
    if (!entries.empty()) {
        return entries[0];
    }
    return "";
}

string subtitle_text_file_data::find_first_info_in_zip() const
{
    for (auto path_in_archive: zip_entry_names()) {
        if (path_in_archive.ends_with(".nfo")) {
            return path_in_archive;
        }
    }
    return "";
}

bool subtitle_text_file_data::has_file_in_zip(const std::string& file_path_in_archive) const
{
    if (!zip_archive_) {
        return false;
    }
    return zip_archive_->HasEntry(file_path_in_archive);
}

const string& subtitle_text_file_data::srt_file_data() const
{
    if (srt_file_data_.empty()) {
        srt_file_data_ = to_utf8(read_srt_data());
    }
    return srt_file_data_;
}

const string& subtitle_text_file_data::info_file_data() const
{
    if (info_file_data_.empty()) {
        info_file_data_ = to_utf8(read_info_data());
    }
    return info_file_data_;
}

void subtitle_text_file_data::close()
{

    cpaf::filesystem::remove_safe(local_download_path_);

    resource_path_.clear();
    archive_path_.clear();
    local_download_path_.clear();
    language_code_.clear();
    archive_srt_file_path_.clear();
    archive_info_file_path_.clear();
    srt_file_data_.clear();
    info_file_data_.clear();
    zip_archive_.reset(nullptr);
}

bool subtitle_text_file_data::is_network_url(const string& path) const
{
    return path.starts_with("https://") || path.starts_with("http://") || path.starts_with("ftp://");
}

// ------------------------
// --- PRIVATE: Helpers ---
// ------------------------

void subtitle_text_file_data::download_and_open_file(const std::string& resource_path)
{
    local_download_path_ = special_dirs::temp() / sha1(resource_path);
    const auto download_url = get_archive_path(resource_path);

    const auto res = cpaf::net::curl_http_download_file(download_url, local_download_path_.string());
    if (res == CURLE_OK) {
        if (cpaf::compression::detect_is_zip_file(local_download_path_)) {
            zip_archive_ = std::make_unique<Zippy::ZipArchive>();
            zip_archive_->Open(local_download_path_);
        }
    }
    else {
        cpaf::filesystem::remove_safe(local_download_path_);
        local_download_path_.clear();
    }
}

string subtitle_text_file_data::read_srt_data() const
{
    if (is_zip_archive()) {
        return read_text_file_from_zip(archive_srt_file_path());
    }

    // Not an archive
    if (resource_is_remote()) {
        return read_srt_data_from_file(local_download_path_);
    }
    return read_srt_data_from_file(resource_path_);
}

std::string cpaf::video::subtitle_text_file_data::read_info_data() const
{
    if (is_zip_archive()) {
        return read_text_file_from_zip(archive_info_file_path());
    }
    return "";
}


string subtitle_text_file_data::read_text_file_from_zip(const std::string& file_path_in_archive) const
{
    if (!has_file_in_zip(file_path_in_archive)) {
        return "";
    }

    std::string str;
    try {
        str = zip_archive_->GetEntry(file_path_in_archive).GetDataAsString();
    } catch (...) {
        str.clear();
    }
    return str;
}

string subtitle_text_file_data::read_srt_data_from_file(const std::string& subtitle_file_path) const
{
    if (std::filesystem::exists(subtitle_file_path)) {
        return cpaf::streams::to_string(subtitle_file_path);
    }
    return "";
}




} // namespace cpaf::video
