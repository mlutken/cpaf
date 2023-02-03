#ifndef CPAF_VIDEO_AV_PACKET_H
#define CPAF_VIDEO_AV_PACKET_H


extern "C"
{
#include <libavcodec/avcodec.h>
}

#include <memory>
#include <utility>
#include <chrono>
#include <concurrent/srsw_fifo.hpp>
#include <cpaf_libs/video/av_util.h>


namespace cpaf::video {
class av_format_context;
/**
 *
 * @see Inspiration from unique_ptr: https://codereview.stackexchange.com/questions/163854/my-implementation-for-stdunique-ptr
 *  */
class av_packet
{
public:
    friend class av_format_context;

    av_packet (const av_packet&) = delete;
    av_packet& operator=(const av_packet&)  = delete;

    friend class av_format_context;
    static uint32_t extract_num_counter;
    enum class create_mode {null, alloc};

    static av_packet alloc();
    // --- Constructors etc. ---
    av_packet();
    explicit av_packet(create_mode cm);
    av_packet (av_packet&& moving) noexcept;
    av_packet& operator=(av_packet&& moving) noexcept;
    ~av_packet();

    void                    swap                (av_packet& src) noexcept;
    void                    alloc_packet_struct () { ff_packet_ = av_packet_alloc(); }
    uint32_t                create_number       () const    { return create_number_;    }
    bool                    valid               () const    { return ff_packet_ != nullptr;    }
    media_type              media_type_get      () const    { return media_type_;    }
    size_t                  stream_index        () const    { return static_cast<size_t>(ff_packet_->stream_index);    }
    pipeline_control_t      pipeline_control    () const    { return pipeline_control_; }
    void                    pipeline_control_set(pipeline_control_t pc) { pipeline_control_ = pc; }
    const pipeline_index_t& pipeline_index      () const { return pipeline_index_; }
    void                    pipeline_index_set  (pipeline_index_t index) { pipeline_index_ = index; }

    // --- Info functions ---
    AVPacket*           ff_packet           () const { return ff_packet_; }
    bool                is_valid            () const { return ff_packet_ != nullptr; }
    int64_t             pts_stream_base     () const { return ff_packet_ ? ff_packet_->pts : std::numeric_limits<int64_t>::min(); }
    int64_t             dts_stream_base     () const { return ff_packet_ ? ff_packet_->dts : std::numeric_limits<int64_t>::min(); }
    bool                dts_valid           () const { return ff_packet_ ? ff_packet_->dts != AV_NOPTS_VALUE: false; }
    bool                pts_valid           () const { return ff_packet_ ? ff_packet_->pts != AV_NOPTS_VALUE: false; }

    const std::chrono::microseconds&   presentation_time       () const { return presentation_time_; }
    std::chrono::milliseconds          presentation_time_ms    () const { return std::chrono::duration_cast<std::chrono::milliseconds>(presentation_time()); }

    // --- Debug functions ---
    void                dump                    () const;
    void                dbg_print               (const std::string_view msg) const;
    std::string         dbg_string_short        () const;
    void                dbg_print_short         () const;

private:
    void                destroy                 ();
    void                media_type_set          (media_type mt) { media_type_ = mt;    }
    void                presentation_time_set   (const std::chrono::microseconds& ts) { presentation_time_ = ts; }

    AVPacket*                   ff_packet_          = nullptr;
    uint32_t                    create_number_      = 0;
    media_type                  media_type_         = media_type::unknown;
    std::chrono::microseconds   presentation_time_  = illegal_timestamp();
    pipeline_control_t          pipeline_control_   = pipeline_control_t::none;
    pipeline_index_t            pipeline_index_     = 0;
};

using packet_queue_t = estl::srsw_fifo<av_packet>;

inline bool operator< (const av_packet& lhs, const av_packet& rhs)
{
    return lhs.pts_stream_base() > rhs.pts_stream_base();
}

} //END namespace cpaf::video


#endif //CPAF_VIDEO_AV_PACKET_H


