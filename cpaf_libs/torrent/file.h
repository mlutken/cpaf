#pragma once

#include <string>
#include <filesystem>
#include <libtorrent/torrent_handle.hpp>


namespace cpaf::torrent {

class files;

class file
{
    friend class files;
private:
    /// Use files::open() to create instances of torrent files!
    explicit                file                (const std::string& uri_or_name, lt::torrent_handle handle, files* parent_files_ptr);
public:
    file    ()                  = default;
    file    (const file& other) = default;

    file&                   operator=           (const file& other) = default;

    std::size_t             read                (void* buffer, std::size_t size_in_bytes);
    int                     seek                (int64_t offset, int whence);
    const std::string&      uri                 () const                                    { return uri_; }
    const std::string&      name                () const                                    { return name_; }
    bool                    is_valid            () const                                    { return parent_files_ptr_ != nullptr; }
    bool                    has_meta_data       () const;
    bool                    is_fully_downloaded () const;

private:
    std::string             uri_;
    std::string             name_;
    lt::torrent_handle      handle_;
    files*                  parent_files_ptr_ = nullptr;
};

} // namespace cpaf::torrent

