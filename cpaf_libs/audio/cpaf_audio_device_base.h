#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <mutex>
#include <cpaf_libs/audio/cpaf_audio_types.h>


namespace cpaf::audio {


class device_base {
public:
    enum class type_t { play, capture };
    using play_callback_t = std::function<void (uint8_t* buffer, uint32_t buffer_max_len)>;

    device_base() = default;
    explicit device_base(type_t type) : type_(type) {}
    explicit device_base(const std::string&	device_name) : device_name_(device_name) {}
    device_base(const std::string& device_name, type_t type) : device_name_(device_name), type_(type) {}

    virtual ~device_base() = default;
    type_t				type					() const { return type_; }
    sample_format_t		sample_format			() const { return do_sample_format(); }
    void				sample_format_set		(sample_format_t format) { do_sample_format_set(format); }
    int32_t             sample_frequency		() const { return do_sample_frequency(); }
    void				sample_frequency_set	(int32_t sample_freq) { do_sample_frequency_set(sample_freq); }
    channel_layout_t    channel_layout          () const { return do_channel_layout(); }
    void				channels_layout_set		(channel_layout_t channel_layout) { do_channel_layout_set(channel_layout); }
    int32_t             channels_count			() const { return do_channels_count(); }
    void				channels_count_set		(int32_t channels_count) { do_channels_count_set(channels_count); }
    int32_t             samples_count			() const { return do_samples_count(); }
    void				samples_count_set		(int32_t samples_count) { do_samples_count_set(samples_count); }
    int32_t             buffer_size				() const { return do_buffer_size(); }

    void				characteristics_set		(int32_t sample_freq, sample_format_t format,
                                                 int32_t channels_count, int32_t samples_count) {
                                                  do_characteristics_set(sample_freq, format, channels_count, samples_count); }

    void				audio_format_set		(const audio_format_t audio_format);
    audio_format_t		audio_format            () const;


    void				play_callback_set		(play_callback_t&& cb);
    bool				open					() { changed_characteristics_reset(); return do_open(); }
    void				close					() { do_close(); }
    void				play					() { return do_play(); }
    void				pause					() { return do_pause(); }
    void				lock					() { return do_lock(); }
    void				unlock					() { return do_unlock(); }

    const std::string&	device_name				() const { return device_name_;		}

    bool				format_changed			() const { return req_format_changed_; }
    bool				sample_freq_changed		() const { return req_sample_frequency_changed_; }
    bool				channels_count_changed	() const { return req_channels_count_changed_; }
    bool				samples_count_changed	() const { return req_samples_count_changed_; }

    std::string         dbg_characteristics     () const;

protected:
    // --- PROTECTED: Helper functions for derived classes ---
    void				execute_play_callback		(uint8_t* buffer, int32_t buffer_max_len);
    void				device_name_set				(const std::string& device_name) { device_name_ = device_name; }
    void				format_changed_set			(bool changed) { req_format_changed_ = changed; }
    void				sample_freq_changed_set		(bool changed) { req_sample_frequency_changed_ = changed; }
    void				channels_count_changed_set	(bool changed) { req_channels_count_changed_ = changed; }
    void				samples_count_changed_set	(bool changed) { req_samples_count_changed_ = changed; }
    void				changed_characteristics_reset();

    std::mutex          access_mutex_;

private:
    virtual sample_format_t     do_sample_format            () const = 0;
    virtual void                do_sample_format_set		(sample_format_t format) = 0;
    virtual int32_t             do_sample_frequency			() const = 0;
    virtual void                do_sample_frequency_set		(int32_t sample_freq) = 0;
    virtual channel_layout_t    do_channel_layout			() const = 0;
    virtual void                do_channel_layout_set		(channel_layout_t channel_layout) = 0;
    virtual int32_t             do_channels_count			() const = 0;
    virtual void                do_channels_count_set		(int32_t channels_count) = 0;
    virtual int32_t             do_samples_count			() const = 0;
    virtual void                do_samples_count_set		(int32_t samples_count) = 0;
    virtual int32_t             do_buffer_size				() const = 0;
    virtual void                do_characteristics_set      (int32_t sample_freq, sample_format_t format,
                                                     int32_t channels_count, int32_t samples_count) = 0;
    virtual bool                do_open						() = 0;
    virtual void                do_close					() = 0;
    virtual void                do_play						() = 0;
    virtual void                do_pause					() = 0;
    virtual void                do_lock                     () = 0;
    virtual void                do_unlock					() = 0;

    play_callback_t     play_callback_                  = default_empty_play_callback;
    std::string			device_name_;
    type_t				type_							= type_t::play;
    bool				req_format_changed_				= false;
    bool				req_sample_frequency_changed_	= false;
    bool				req_channels_count_changed_		= false;
    bool				req_samples_count_changed_		= false;

    static void			default_empty_play_callback		(uint8_t*, int32_t ) {}

};

} //END namespace cpaf::audio

