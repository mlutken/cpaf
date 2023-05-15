#ifndef CPAF_VIDEO_AV_SAMPLES_QUEUE_H
#define CPAF_VIDEO_AV_SAMPLES_QUEUE_H



#include <utility>
#include <mutex>
#include <concurrent/srsw_fifo.hpp>
#include <cpaf_libs/video/av_samples_buffer.h>


namespace cpaf::video {
/**
 *  @todo Do we really need to ad functionality to this wrapper at all ?
 *        Not used currently!
 *
 * Lockless fifo of audio av_samples_buffer(s)
 *  */

// ---------------------
// --- Samples queue ---
// ---------------------
using samples_queue_callback_t = std::function<void(const av_samples_buffer&)>;

class av_samples_queue
{
public:
    explicit av_samples_queue(size_t queue_size);

    bool                        empty       () const { return fifo_.empty();    }
    bool                        full        () const { return fifo_.full();     }
    size_t                      size        () const { return fifo_.size();     }
    bool                        push        (av_samples_buffer buffer);
    av_samples_buffer&          front       ();
    const av_samples_buffer&    front       () const;
    void                        pop         ();
    av_samples_buffer           pop_front   ();
    void                        flush       ();

    int32_t                     copy_audio_samples  (uint8_t* dest_buf,  int32_t bytes_to_copy,
                                                     const samples_queue_callback_t& queue_pop_callback);
    int32_t                     copy_audio_samples  (uint8_t* dest_buf, int32_t bytes_to_copy,
                                                     const std::chrono::microseconds& sync_to_media_time,
                                                     const std::chrono::microseconds& sync_ok_interval,
                                                     const samples_queue_callback_t& queue_pop_callback);

private:
    bool more_than_a_buffer_behind(const std::chrono::microseconds& sync_to_media_time) const
    {
        return (sync_to_media_time - front().presentation_time()) > front().duration();
    }

    void skip_audio_samples_helper(const std::chrono::microseconds& sync_to_media_time)
    {
        while (!empty() && ( (sync_to_media_time - front().presentation_time()) > front().duration()) ) {
            pop();
        }
    }

    estl::srsw_fifo<av_samples_buffer>  fifo_;
    std::mutex                          fifo_mutex_;
};



} //END namespace cpaf::video


#endif //CPAF_VIDEO_AV_SAMPLES_QUEUE_H
