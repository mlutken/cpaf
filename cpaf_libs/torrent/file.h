#pragma once

#include <vector>
#include <libtorrent/torrent_handle.hpp>
#include <cpaf_libs/torrent/torrent_utils.h>

namespace cpaf::torrent {

class torrent;


class file
{
    friend class torrent;
private:
    /// Use torrent::open() to create instances of (torrent) file(s)!
    explicit                            file                (lt::file_index_t file_index, lt::torrent_handle handle, torrent* parent_torrent_ptr);
public:
    file        ()                  = default;
    file        (const file& other) = default;

    file&                               operator=           (const file& other) = default;

    std::size_t                         read                (void* buffer, std::size_t bytes_to_read) const;
    int                                 seek                (int64_t offset, int whence);


    int64_t                             size                () const;
    int64_t                             offset              () const;

    std::string                         name                () const;
    std::string                         path                () const;

    lt::piece_index_t                   piece_index_start   () const;
    lt::peer_request                    map_file            (std::int64_t offset, int size) const;

private:
    const lt::file_storage&             files_storage       () const;
    cache_piece_data                    get_file_data       (std::int64_t offset) const;
    std::vector<cache_piece_data>       get_file_data       (std::int64_t offset, int size) const;

    lt::file_index_t                    file_index_;
    lt::torrent_handle                  handle_;
    torrent*                            parent_torrent_ptr_ = nullptr;
    int64_t                             offsett_ =0;
//    std::string                         name_;

};

} // namespace capf::torrent

