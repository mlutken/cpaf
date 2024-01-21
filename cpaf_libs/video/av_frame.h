#ifndef CPAF_VIDEO_AV_FRAME_H
#define CPAF_VIDEO_AV_FRAME_H


extern "C"
{
#include <libavutil/frame.h>
}

#include <memory>
#include <utility>
#include <chrono>
#include <filesystem>
#include <cpaf_libs/video/av_util.h>

namespace cpaf::video {
class av_codec_context;

/**
 *
 * @see Inspiration from unique_ptr: https://codereview.stackexchange.com/questions/163854/my-implementation-for-stdunique-ptr
 *  */
class av_frame
{
public:
    static uint32_t extract_num_counter;
    enum class create_mode {null, alloc};

    static av_frame create_alloc();
    // --- Constructors etc. ---
    av_frame();
    explicit av_frame(create_mode cm, size_t stream_index);


    void alloc_frame_struct() { ff_frame_ = av_frame_alloc(); }

    av_frame (const av_frame&) = delete;
    av_frame (av_frame&& moving) noexcept
    {
        create_number_ = extract_num_counter++;
        moving.swap(*this);
//        dbg_print("'MOVING CONSTRUCT frame!");
    }

    av_frame& operator=(const av_frame&)  = delete;
    av_frame& operator=(av_frame&& moving) noexcept
    {
        moving.swap(*this);
        return *this;
    }

    ~av_frame();

    explicit  operator bool() const { return is_valid(); }
    void swap(av_frame& src) noexcept;
//https://en.cppreference.com/w/cpp/container/priority_queue

    // --- Info functions ---
    uint32_t                create_number       () const { return create_number_;    }
    AVFrame*                ff_frame            () const { return ff_frame_; }
    bool                    is_valid            () const { return ff_frame_ != nullptr; }
    int64_t                 pts_stream_base     () const { return ff_frame_->pts; }
    int32_t                 video_width         () const { return ff_frame_->width; }
    int32_t                 video_height        () const { return ff_frame_->height; }
    /// DEPRCATED bool                    key_frame           () const { return ff_frame_->key_frame == 1; }
    size_t                  stream_index        () const { return stream_index_; }
    int                     nb_samples          () const { return ff_frame_->nb_samples; }

    const std::chrono::microseconds&    presentation_time       () const { return presentation_time_; }
    std::chrono::milliseconds           presentation_time_ms    () const { return std::chrono::duration_cast<std::chrono::milliseconds>(presentation_time()); }
    std::chrono::microseconds           distance_to             (std::chrono::microseconds media_time_point) const { return media_time_point - presentation_time_; }
    std::chrono::microseconds           distance_to_abs         (std::chrono::microseconds media_time_point) const;

    // --- Setters ---
    void            stream_index_set        (size_t stream_index) { stream_index_ = stream_index; }
    void            presentation_time_set   (const std::chrono::microseconds& ts) { presentation_time_ = ts; }

//	const std::string& resource_path	() const { return resource_path_; }

    // --- Scale image functions ---
    bool            init_scaling    (size_t image_size_bytes, AVPixelFormat pixel_format,
                                     int32_t dst_width, int32_t dst_height,
                                     int32_t dst_image_align = 32);
    bool            init_scaling    (const av_codec_context& codec_context);
    bool            save_as_ppm     (const std::filesystem::path& file_path) const;
    bool            save_as_ppm_auto(const std::filesystem::path& frames_dir) const;


    // --- Debug functions ---
    void			dump            () const;
    void            dbg_print       (const std::string_view msg) const;
    std::string     dbg_string_short() const;
    void            dbg_print_short () const;

private:
    AVFrame*                    ff_frame_           = nullptr;
    uint8_t*                    image_buffer_       = nullptr;
    size_t                      stream_index_       = illegal_stream_index();
    std::chrono::microseconds   presentation_time_  = illegal_timestamp();
    uint32_t                    create_number_      = 0;
};


inline bool operator< (const av_frame& lhs, const av_frame& rhs)
{
    return lhs.pts_stream_base() < rhs.pts_stream_base();
}

} //END namespace cpaf::video


#endif //CPAF_VIDEO_AV_FRAME_H


