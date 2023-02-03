#ifndef CPAF_VIDEO_AV_OPTONS_H
#define CPAF_VIDEO_AV_OPTONS_H


extern "C"
{
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
}

#include <string>
//#include <string_view>

namespace cpaf::video {

template <class OBJECT>
class av_options
{
public:
    av_options() = default;
    ~av_options() = default;
    OBJECT* ff  () { return object_; }
    const OBJECT* ff  () const { return object_; }
    operator bool() const { return object_ != nullptr; }

    int set                 (const std::string& name, const std::string& val, int search_flags = 0)         { return av_opt_set (object_, name.c_str(), val.c_str(), search_flags);}
    int set                 (const std::string& name, int64_t val, int search_flags = 0)                    { return av_opt_set_int (object_, name.c_str(), val, search_flags);}
    int set                 (const std::string& name, uint64_t val, int search_flags = 0)                   { return av_opt_set_int (object_, name.c_str(), static_cast<int64_t>(val), search_flags);}
    int set                 (const std::string& name, uint32_t val, int search_flags = 0)                   { return av_opt_set_int (object_, name.c_str(), static_cast<int64_t>(val), search_flags);}
    int set                 (const std::string& name, int32_t val, int search_flags = 0)                    { return av_opt_set_int (object_, name.c_str(), static_cast<int64_t>(val), search_flags);}
    int set                 (const std::string& name, double val, int search_flags = 0)                     { return av_opt_set_double (object_, name.c_str(), val, search_flags);}
    int set                 (const std::string& name, float val, int search_flags = 0)                      { return av_opt_set_double (object_, name.c_str(), static_cast<double>(val), search_flags);}
    int set                 (const std::string& name, const AVRational& val, int search_flags = 0)          { return av_opt_set_q (object_, name.c_str(), val, search_flags);}
    int set_bin             (const std::string& name, const uint8_t *val, int size, int search_flags = 0)   { return av_opt_set_bin (object_, name.c_str(), val, size, search_flags);}
    int set_image_size      (const std::string& name, int w, int h, int search_flags = 0)                   { return av_opt_set_image_size(object_, name.c_str(), w, h, search_flags);}
    int set_pixel_fmt       (const std::string& name, enum AVPixelFormat fmt, int search_flags = 0)         { return av_opt_set_pixel_fmt(object_, name.c_str(),fmt, search_flags);}
    int set_sample_fmt      (const std::string& name, enum AVSampleFormat fmt, int search_flags = 0)        { return av_opt_set_sample_fmt(object_, name.c_str(),fmt, search_flags);}
    int set_video_rate      (const std::string& name, const AVRational& val, int search_flags = 0)          { return av_opt_set_video_rate(object_, name.c_str(),val, search_flags);}
    int set_channel_layout  (const std::string& name, int64_t ch_layout, int search_flags = 0)              { return av_opt_set_channel_layout(object_, name.c_str(),ch_layout, search_flags);}
    int set_channel_layout  (const std::string& name, uint64_t ch_layout, int search_flags = 0)             { return av_opt_set_channel_layout(object_, name.c_str(),static_cast<int64_t>(ch_layout), search_flags);}

protected:
    OBJECT* object_ = nullptr;
};


class av_swr_context : public av_options<SwrContext>
{
public:
    av_swr_context              ()                      { object_ = swr_alloc();    }
    ~av_swr_context             ()                      { swr_free(&object_);       }
    int         init            ()                      { return swr_init(object_); }
    int64_t     delay           (int64_t base) const    { return swr_get_delay(object_, base);}
    int         convert         (uint8_t **out, int out_count, const uint8_t **in , int in_count){
                                    return swr_convert(object_, out, out_count, in , in_count);
                                }

    void    free() {
        swr_free(&object_);
        object_ = nullptr;
    }

    void    alloc() {
        object_ = swr_alloc();
    }

    void    realloc() {
        swr_free(&object_);
        object_ = swr_alloc();
    }
};



} //END namespace cpaf::video


#endif // CPAF_VIDEO_AV_OPTONS_H


