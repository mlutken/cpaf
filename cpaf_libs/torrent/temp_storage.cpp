#include "temp_storage.h"

#include <iostream>

namespace cpaf {
namespace torrent {


// --------------------------------------
// --- Local ("private") helper class ---
// --------------------------------------

struct temp_storage
{
  explicit temp_storage(lt::file_storage const& fs) : m_files(fs) {}

  lt::span<char const> readv(lt::peer_request const r, lt::storage_error& ec) const
  {
    auto const i = m_file_data.find(r.piece);
    if (i == m_file_data.end())
    {
      ec.operation = lt::operation_t::file_read;
      ec.ec = boost::asio::error::eof;
      return {};
    }
    if (int(i->second.size()) <= r.start)
    {
      ec.operation = lt::operation_t::file_read;
      ec.ec = boost::asio::error::eof;
      return {};
    }
    return { i->second.data() + r.start, std::min(r.length, int(i->second.size()) - r.start) };
  }

  void writev(lt::span<char const> const b, lt::piece_index_t const piece, int const offset)
  {
    auto& data = m_file_data[piece];
    if (data.empty())
    {
      // allocate the whole piece, otherwise we'll invalidate the pointers
      // we have returned back to libtorrent
      int const size = piece_size(piece);
      data.resize(std::size_t(size));
    }
    TORRENT_ASSERT(offset + b.size() <= int(data.size()));
    std::memcpy(data.data() + offset, b.data(), std::size_t(b.size()));
  }

  lt::sha1_hash hash(lt::piece_index_t const piece
    , lt::span<lt::sha256_hash> const block_hashes, lt::storage_error& ec) const
  {
    auto const i = m_file_data.find(piece);
    if (i == m_file_data.end())
    {
      ec.operation = lt::operation_t::file_read;
      ec.ec = boost::asio::error::eof;
      return {};
    }
    if (!block_hashes.empty())
    {
      int const piece_size2 = m_files.piece_size2(piece);
      int const blocks_in_piece2 = m_files.blocks_in_piece2(piece);
      char const* buf = i->second.data();
      std::int64_t offset = 0;
      for (int k = 0; k < blocks_in_piece2; ++k)
      {
        lt::hasher256 h2;
        std::ptrdiff_t const len2 = std::min(lt::default_block_size, int(piece_size2 - offset));
        h2.update({ buf, len2 });
        buf += len2;
        offset += len2;
        block_hashes[k] = h2.final();
      }
    }
    return lt::hasher(i->second).final();
  }

  lt::sha256_hash hash2(lt::piece_index_t const piece, int const offset, lt::storage_error& ec)
  {
    auto const i = m_file_data.find(piece);
    if (i == m_file_data.end())
    {
      ec.operation = lt::operation_t::file_read;
      ec.ec = boost::asio::error::eof;
      return {};
    }

    int const piece_size = m_files.piece_size2(piece);

    std::ptrdiff_t const len = std::min(lt::default_block_size, piece_size - offset);

    lt::span<char const> b = {i->second.data() + offset, len};
    return lt::hasher256(b).final();
  }

private:
  int piece_size(lt::piece_index_t piece) const
  {
    int const num_pieces = static_cast<int>((m_files.total_size() + m_files.piece_length() - 1) / m_files.piece_length());
    return static_cast<int>(piece) < num_pieces - 1
      ? m_files.piece_length() : static_cast<int>(m_files.total_size() - std::int64_t(num_pieces - 1) * m_files.piece_length());
  }

  lt::file_storage const& m_files;
  std::map<lt::piece_index_t, std::vector<char>> m_file_data;
};

lt::storage_index_t pop(std::vector<lt::storage_index_t>& q)
{
  TORRENT_ASSERT(!q.empty());
  lt::storage_index_t const ret = q.back();
  q.pop_back();
  return ret;
}

// ----------------------------------------
// --- Main temp_disk_io implementation ---
// ----------------------------------------


libtorrent::storage_holder temp_disk_io::new_torrent(const libtorrent::storage_params& params, const std::shared_ptr<void>&)
{
    lt::storage_index_t const idx = m_free_slots.empty()
            ? m_torrents.end_index()
            : pop(m_free_slots);
    auto storage = std::make_unique<temp_storage>(params.files);
    if (idx == m_torrents.end_index()) m_torrents.emplace_back(std::move(storage));
    else m_torrents[idx] = std::move(storage);
    return lt::storage_holder(idx, *this);
}

void temp_disk_io::remove_torrent(const libtorrent::storage_index_t idx)
{
    m_torrents[idx].reset();
    m_free_slots.push_back(idx);
}

void temp_disk_io::async_read(libtorrent::storage_index_t storage, const libtorrent::peer_request& r, std::function<void (lt::disk_buffer_holder, const lt::storage_error&)> handler, libtorrent::disk_job_flags_t)
{
    // this buffer is owned by the storage. It will remain valid for as
    // long as the torrent remains in the session. We don't need any lifetime
    // management of it.
    lt::storage_error error;
    lt::span<char const> b = m_torrents[storage]->readv(r, error);

    post(m_ioc, [handler, error, b, this]
    { handler(lt::disk_buffer_holder(*this, const_cast<char*>(b.data()), int(b.size())), error); });
}

bool temp_disk_io::async_write(libtorrent::storage_index_t storage,
                               const libtorrent::peer_request& r,
                               const char* buf, std::shared_ptr<libtorrent::disk_observer>,
                               std::function<void (const lt::storage_error&)> handler, libtorrent::disk_job_flags_t)
{
//    std::cerr << "FIXMENM temp_disk_io::async_write() storage index: '" << storage << "' piece index: '" << r.piece << "'\n";
    lt::span<char const> const b = { buf, r.length };

    m_torrents[storage]->writev(b, r.piece, r.start);

    post(m_ioc, [=]{ handler(lt::storage_error()); });
    return false;
}

void temp_disk_io::async_hash(libtorrent::storage_index_t storage, const libtorrent::piece_index_t piece, lt::span<libtorrent::sha256_hash> block_hashes, libtorrent::disk_job_flags_t, std::function<void (lt::piece_index_t, const lt::sha1_hash&, const lt::storage_error&)> handler)
{
    lt::storage_error error;
    lt::sha1_hash const hash = m_torrents[storage]->hash(piece, block_hashes, error);
    post(m_ioc, [=]{ handler(piece, hash, error); });
}

void temp_disk_io::async_hash2(libtorrent::storage_index_t storage, const libtorrent::piece_index_t piece, const int offset, libtorrent::disk_job_flags_t, std::function<void (lt::piece_index_t, const lt::sha256_hash&, const lt::storage_error&)> handler)
{
    lt::storage_error error;
    lt::sha256_hash const hash = m_torrents[storage]->hash2(piece, offset, error);
    post(m_ioc, [=]{ handler(piece, hash, error); });
}

void temp_disk_io::async_move_storage(libtorrent::storage_index_t, std::string p, libtorrent::move_flags_t, std::function<void (lt::status_t, const std::string&, const lt::storage_error&)> handler)
{
    post(m_ioc, [=]{
        handler(lt::status_t::fatal_disk_error, p
                , lt::storage_error(lt::error_code(boost::system::errc::operation_not_supported, lt::system_category())));
    });
}

void temp_disk_io::async_delete_files(libtorrent::storage_index_t, libtorrent::remove_flags_t, std::function<void (const lt::storage_error&)> handler)
{
    post(m_ioc, [=]{ handler(lt::storage_error()); });
}

void temp_disk_io::async_check_files(libtorrent::storage_index_t, const libtorrent::add_torrent_params*, lt::aux::vector<std::string, libtorrent::file_index_t>, std::function<void (lt::status_t, const lt::storage_error&)> handler)
{
    post(m_ioc, [=]{ handler(lt::status_t::no_error, lt::storage_error()); });
}

void temp_disk_io::async_rename_file(libtorrent::storage_index_t, const libtorrent::file_index_t idx, const std::string name, std::function<void (const std::string&, lt::file_index_t, const lt::storage_error&)> handler)
{
    post(m_ioc, [=]{ handler(name, idx, lt::storage_error()); });
}

void temp_disk_io::async_stop_torrent(libtorrent::storage_index_t, std::function<void ()> handler)
{
    post(m_ioc, handler);
}

void temp_disk_io::async_set_file_priority(libtorrent::storage_index_t, lt::aux::vector<libtorrent::download_priority_t, libtorrent::file_index_t> prio, std::function<void (const lt::storage_error&, lt::aux::vector<lt::download_priority_t, lt::file_index_t>)> handler)
{
    post(m_ioc, [=]{
        handler(lt::storage_error(lt::error_code(
                                      boost::system::errc::operation_not_supported, lt::system_category())), std::move(prio));
    });
}

void temp_disk_io::free_disk_buffer(char*)
{
    // never free any buffer. We only return buffers owned by the storage
    // object
}


// --------------------------------------
// --- temp_disk constructor function ---
// --------------------------------------

std::unique_ptr<lt::disk_interface> temp_disk_constructor(
        lt::io_context& ioc, lt::settings_interface const&, lt::counters&)
{
    return std::make_unique<temp_disk_io>(ioc);
}

} // namespace torrent
} // namespace cpaf
