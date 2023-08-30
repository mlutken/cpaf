#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <optional>
#include <libtorrent/torrent_handle.hpp>

#include <cpaf_libs/torrent/file.h>


namespace cpaf::torrent {

class torrents;

class torrent
{
    friend class torrents;
private:
    /// Use torrents::open() to create instances of torrent!
    explicit                            torrent                         (const std::string& uri_or_name, lt::torrent_handle handle, torrents* parent_torrents_ptr);
public:
    torrent    ()                  = default;
    torrent    (const torrent& other) = default;

    torrent&                            operator=                       (const torrent& other) = default;

    file                                open                            (lt::file_index_t file_index);

    std::size_t                         read                            (void* buffer, std::size_t size_in_bytes);
    int                                 seek                            (int64_t offset, int whence);
    const std::string&                  uri                             () const                                    { return uri_; }
    const std::string&                  name                            () const                                    { return name_; }


    std::string                         largest_file_name               () const;
//    std::optional<lt::file_index_t>     largest_file_index              ();
    lt::file_index_t                    largest_file_index              () const;
    std::filesystem::path               largest_file_local_file_path    () const;
    std::vector<std::string>            all_file_names                  () const;
    std::vector<std::string>            all_file_paths                  () const;
    int                                 number_of_files                 () const;

    lt::file_index_t                    file_index_at_offset            (std::int64_t offset) const;
    lt::file_index_t                    file_index_at_piece             (lt::piece_index_t piece) const;

    std::vector<lt::file_slice>         map_block                       (lt::piece_index_t piece, std::int64_t offset, std::int64_t size) const;


    int                                 piece_length                    () const;
    int                                 num_pieces                      () const;

    bool                                is_valid                        () const                                    { return parent_torrents_ptr_ != nullptr; }
    bool                                has_meta_data                   () const;
    bool                                is_fully_downloaded             () const;
    void                                read_piece                      (lt::piece_index_t piece) const;

    void                                set_piece_downloaded            (lt::piece_index_t piece) ;
    bool                                is_piece_downloaded             (lt::piece_index_t piece) const;
    bool                                read_all_downloaded_pieces      () const;

private:
    using pieces_downloaded_set_t = std::unordered_set<lt::piece_index_t>;
    const lt::file_storage&             files_storage                   () const;

    std::string                         uri_;
    std::string                         name_;
    lt::torrent_handle                  handle_;
    pieces_downloaded_set_t             pieces_downloaded_;
    torrents*                           parent_torrents_ptr_ = nullptr;
};

} // namespace cpaf::torrent

