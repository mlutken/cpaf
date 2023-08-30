#pragma once

#include <unordered_map>
#include <concurrent/srsw_fifo_s.hpp>
#include <libtorrent/alert_types.hpp>
#include <cpaf_libs/torrent/torrent_utils.h>

namespace cpaf::torrent {



class streaming_cache
{
public:

    streaming_cache();


    bool            insert_piece_data      (const lt::read_piece_alert* rpa);


private:
    std::unordered_map<int32_t, cache_piece_data>  cache_;
    estl::srsw_fifo_s<cache_piece_data, 64>        fifo_;
};

} // namespace cpaf::torrent

