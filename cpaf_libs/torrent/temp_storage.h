#pragma once

// https://www.libtorrent.org/reference-Custom_Storage.html#overview

#include <map>

#include <libtorrent/session.hpp>
#include <libtorrent/hasher.hpp>
#include <libtorrent/file_storage.hpp>


namespace cpaf {
namespace torrent {

struct temp_storage;



struct temp_disk_io final : lt::disk_interface, lt::buffer_allocator_interface
{
  explicit temp_disk_io(lt::io_context& ioc): m_ioc(ioc) {}

  void settings_updated() override {}

  lt::storage_holder new_torrent(lt::storage_params const& params
    , std::shared_ptr<void> const&) override;

  void remove_torrent(lt::storage_index_t const idx) override;

  void abort(bool) override {}

  void async_read(lt::storage_index_t storage, lt::peer_request const& r
    , std::function<void(lt::disk_buffer_holder block, lt::storage_error const& se)> handler
    , lt::disk_job_flags_t) override;

  bool async_write(lt::storage_index_t storage, lt::peer_request const& r
    , char const* buf, std::shared_ptr<lt::disk_observer>
    , std::function<void(lt::storage_error const&)> handler
    , lt::disk_job_flags_t) override;

  void async_hash(lt::storage_index_t storage, lt::piece_index_t const piece
    , lt::span<lt::sha256_hash> block_hashes, lt::disk_job_flags_t
    , std::function<void(lt::piece_index_t, lt::sha1_hash const&, lt::storage_error const&)> handler) override;

  void async_hash2(lt::storage_index_t storage, lt::piece_index_t const piece
    , int const offset, lt::disk_job_flags_t
    , std::function<void(lt::piece_index_t, lt::sha256_hash const&, lt::storage_error const&)> handler) override;

  void async_move_storage(lt::storage_index_t, std::string p, lt::move_flags_t
    , std::function<void(lt::status_t, std::string const&, lt::storage_error const&)> handler) override;

  void async_release_files(lt::storage_index_t, std::function<void()>) override {}

  void async_delete_files(lt::storage_index_t, lt::remove_flags_t
    , std::function<void(lt::storage_error const&)> handler) override;

  void async_check_files(lt::storage_index_t
    , lt::add_torrent_params const*
    , lt::aux::vector<std::string, lt::file_index_t>
    , std::function<void(lt::status_t, lt::storage_error const&)> handler) override;

  void async_rename_file(lt::storage_index_t
    , lt::file_index_t const idx
    , std::string const name
    , std::function<void(std::string const&, lt::file_index_t, lt::storage_error const&)> handler) override;

  void async_stop_torrent(lt::storage_index_t, std::function<void()> handler) override;

  void async_set_file_priority(lt::storage_index_t
    , lt::aux::vector<lt::download_priority_t, lt::file_index_t> prio
    , std::function<void(lt::storage_error const&
      , lt::aux::vector<lt::download_priority_t, lt::file_index_t>)> handler) override;

  void async_clear_piece(lt::storage_index_t, lt::piece_index_t index
    , std::function<void(lt::piece_index_t)> handler) override
  {
    post(m_ioc, [=]{ handler(index); });
  }

  // implements buffer_allocator_interface
  void free_disk_buffer(char*) override;

  void update_stats_counters(lt::counters&) const override {}

  std::vector<lt::open_file_state> get_status(lt::storage_index_t) const override
  { return {}; }

  void submit_jobs() override {}

private:

  lt::aux::vector<std::shared_ptr<temp_storage>, lt::storage_index_t> m_torrents;

  // slots that are unused in the m_torrents vector
  std::vector<lt::storage_index_t> m_free_slots;

  // callbacks are posted on this
  lt::io_context& m_ioc;
};



// --------------------------------------
// --- temp_disk constructor function ---
// --------------------------------------
std::unique_ptr<lt::disk_interface> temp_disk_constructor(lt::io_context& ioc, lt::settings_interface const&, lt::counters&);

} // namespace torrent
} // namespace cpaf

