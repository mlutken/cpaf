#pragma once

#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/audio/cpaf_audio_device_base.h>

#if (CPAF_AUDIO_SYSTEM_ID != CPAF_SYSTEM_ID_GODOT)
#error Only include cpaf_libs/audio/cpaf_audio_device__godot.h when CPAF_AUDIO_SYSTEM_ID is CPAF_SYSTEM_ID_GODOT
#endif

namespace cpaf::audio {


class device_platform : public device_base {
public:
    friend void	sdl_native_audio_callback		(void* userdata, uint8_t* buffer, int buffer_max_len);
    device_platform();
    explicit device_platform(type_t type);
    explicit device_platform(const std::string&	device_name);
    device_platform(const std::string& device_name, type_t type);
     ~device_platform() override;


private:
    // --- Platform overrides ---
    sample_format_t     do_sample_format			() const override;
    void                do_sample_format_set		(sample_format_t format) override;
    int32_t             do_sample_frequency			() const  override;
    void                do_sample_frequency_set		(int32_t sample_freq)  override;
    channel_layout_t    do_channel_layout           () const override;
    void                do_channel_layout_set		(channel_layout_t channel_layout) override;
    int32_t             do_channels_count           () const override;
    void                do_channels_count_set		(int32_t channels_count) override;
    int32_t             do_samples_count			() const override;
    int32_t             do_buffer_size				() const override;
    void                do_samples_count_set		(int32_t samples_count) override;
    void                do_characteristics_set		(int32_t sample_freq, sample_format_t format,
                                                 int32_t channels_count, int32_t samples_count) override;

    bool                do_open						() override;
    void                do_close					() override final;
    void                do_play						() override;
    void                do_pause					() override;
    void                do_lock         			() override;
    void                do_unlock         			() override;

    // --- PRIVATE: Helpers  ---
    // static sample_format_t	to_sample_format_t			(SDL_AudioFormat sdl_format);
    // static SDL_AudioFormat	to_sdl_sample_format		(sample_format_t format);

    void					set_default_audio_format	();

};


} //END namespace cpaf::audio
