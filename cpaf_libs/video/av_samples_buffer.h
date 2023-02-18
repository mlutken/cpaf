#ifndef CPAF_VIDEO_AV_SAMPLES_BUFFER_H
#define CPAF_VIDEO_AV_SAMPLES_BUFFER_H


extern "C"
{
#include <libavcodec/avcodec.h>
}

#include <memory>
#include <utility>
#include <array>
#include <functional>
#include <concurrent/srsw_fifo.hpp>
#include <cpaf_libs/video/av_util.h>


namespace cpaf::video {
class av_format_context;
/**
 *
 * @see Inspiration from unique_ptr: https://codereview.stackexchange.com/questions/163854/my-implementation-for-stdunique-ptr
 *  */
class av_samples_buffer
{
public:

    static av_samples_buffer create_flush_sample();
    // --- Constructors etc. ---
    av_samples_buffer() = default;
    av_samples_buffer (const av_samples_buffer&) = delete;
    av_samples_buffer (av_samples_buffer&& moving) noexcept { moving.swap(*this); }

    av_samples_buffer& operator=(const av_samples_buffer&)  = delete;
    av_samples_buffer& operator=(av_samples_buffer&& moving) noexcept { moving.swap(*this); return *this; }
    ~av_samples_buffer();

    void            swap                                    (av_samples_buffer& src) noexcept;

    int             samples_alloc_array_and_samples
                                                            (int* linesize, int nb_channels, int nb_samples,
                                                             AVSampleFormat sample_format, int align);

    int             samples_alloc                           (int* linesize, int nb_channels, int nb_samples,
                                                             AVSampleFormat sample_format, int align);


    void                        is_flush_sample_set         ()              { is_flush_sample_ = true; }
    bool                        is_flush_sample             () const		{ return is_flush_sample_; }
    bool                        valid                       () const		{ return resampled_data_ != nullptr && raw_buffer_start() != nullptr; }
    bool                        empty                       () const		{ return !valid() || size() == 0; }
    uint8_t**                   raw_buffer                  ()				{ return resampled_data_; }
    const uint8_t*              raw_buffer_start            () const    	{ return *resampled_data_; }
    uint8_t*                    raw_buffer_start            ()              { return *resampled_data_; }
    uint8_t*                    out_buffer                  ()              { return raw_buffer_start() + buffer_pos_; }
    const uint8_t*              out_buffer                  () const		{ return raw_buffer_start() + buffer_pos_; }
    int32_t                     size                        () const		{ return buffer_size_ - buffer_pos_;}
    void                        initial_size_set            (int32_t size)	{ buffer_size_ = size;}
    int32_t                     current_buffer_pos          () const        { return buffer_pos_;}
    void                        reset_current_buffer_pos    (int32_t pos)   { buffer_pos_ = pos;}
    void                        reset_current_buffer_pos    ()              { buffer_pos_ = 0;}
    float                       bytes_per_microsecond       () const        { return bytes_per_microsecond_; }
    void                        bytes_per_microsecond_set   (float count)   { bytes_per_microsecond_ = count; }
    int32_t                     bytes_per_sample            () const        { return bytes_per_sample_; }
    void                        bytes_per_sample_set        (int32_t count) { bytes_per_sample_ = count; }
    pipeline_control_t          pipeline_control            () const        { return pipeline_control_; }
    void                        pipeline_control_set        (pipeline_control_t pc) { pipeline_control_ = pc; }
    const pipeline_index_t&     pipeline_index              () const { return pipeline_index_; }
    void                        pipeline_index_set          (pipeline_index_t index) { pipeline_index_ = index; }

    int32_t                     skip_samples                (int32_t bytes_to_skip);
    int32_t                     skip_samples                (const std::chrono::microseconds& us_to_skip);
    int32_t                     add_samples                 (int32_t bytes_to_add);
    int32_t                     add_samples                 (const std::chrono::microseconds& us_to_add);
    int32_t                     copy_out                    (uint8_t* dest_buf);
    int32_t                     copy_out                    (uint8_t* dest_buf, int32_t max_bytes_to_copy);
    int32_t                     copy_out                    (uint8_t* dest_buf, const std::chrono::microseconds& max_duration_to_copy);

    void						presentation_time_set       (const std::chrono::microseconds& ts) { presentation_time_ = ts; }
    std::chrono::microseconds   presentation_time           () const { return presentation_time_ + bytes_to_duration(bytes_consumed()); }
    std::chrono::milliseconds   presentation_time_ms        () const { return std::chrono::duration_cast<std::chrono::milliseconds>(presentation_time()); }
    std::chrono::microseconds   duration                    () const { return bytes_to_duration(size()); }
    std::chrono::milliseconds   duration_ms                 () const { return std::chrono::duration_cast<std::chrono::milliseconds>(duration()); }
    std::chrono::microseconds   bytes_to_duration           (int32_t bytes) const;

    // --- Info functions ---
    std::string                 dbg_string_short            () const;

private:
    int32_t                     bytes_consumed              () const { return buffer_size_ - size(); }
    std::chrono::microseconds	presentation_time_		= illegal_timestamp();
    uint8_t**					resampled_data_			= nullptr;
    int64_t						buffer_size_			= -1;
    int64_t						buffer_pos_				= 0;
    float                       bytes_per_microsecond_  = 0;
    int32_t                     bytes_per_sample_       = 0;
    pipeline_control_t          pipeline_control_       = pipeline_control_t::none;
    pipeline_index_t            pipeline_index_         = 0;
    bool                        is_flush_sample_        = false;
};



} //END namespace cpaf::video


#endif //CPAF_VIDEO_AV_SAMPLES_BUFFER_H


