#include "cpaf_audio_device__sdl2.h"
#include <unordered_map>

////#include <cpaf_libs/utils/cpaf_stl_utils.h>


namespace cpaf::audio {

// Friend function/static callback from SDL
void sdl_native_audio_callback	(void* userdata, uint8_t* buffer, int buffer_max_len)
{
    device_platform* device_sdl = static_cast<device_platform*>(userdata);
    device_sdl->execute_play_callback(buffer, buffer_max_len);
}


device_platform::device_platform()
    : device_base()
{
    set_default_audio_format();
}

device_platform::device_platform(type_t type)
    : device_base(type)
{
    set_default_audio_format();
}

device_platform::device_platform(const std::string& device_name)
 : device_base(device_name)
{
    set_default_audio_format();
}

device_platform::device_platform(const std::string& device_name, type_t type)
    : device_base(device_name, type)
{
    set_default_audio_format();
}

device_platform::~device_platform()
{
    do_close();
}

// --------------------------
// --- Platform overrides ---
// --------------------------

sample_format_t device_platform::do_format() const
{
    return to_format_t(sdl_audio_spec_.format);
}

void device_platform::do_format_set(sample_format_t format)
{
    sdl_audio_spec_.format = to_sdl_format(format);
}

int32_t device_platform::do_sample_frequency() const
{
    return sdl_audio_spec_.freq;
}

void device_platform::do_sample_frequency_set(int32_t sample_freq)
{
    sdl_audio_spec_.freq = sample_freq;
}

int32_t device_platform::do_channels_count() const
{
    return sdl_audio_spec_.channels;
}

void device_platform::do_channels_count_set(int32_t channels_count)
{
    sdl_audio_spec_.channels = channels_count;
}

int32_t device_platform::do_samples_count() const
{
    return sdl_audio_spec_.samples;
}

int32_t device_platform::do_buffer_size() const
{
    return static_cast<int32_t>(sdl_audio_spec_.size);
}

void device_platform::do_samples_count_set(int32_t samples_count)
{
    sdl_audio_spec_.samples = samples_count;
}

void device_platform::do_characteristics_set(int32_t sample_freq, sample_format_t format, int32_t channels_count, int32_t samples_count)
{
    sdl_audio_spec_.freq		= static_cast<int>(sample_freq);
    sdl_audio_spec_.format		= to_sdl_format(format);
    sdl_audio_spec_.channels	= channels_count;
    sdl_audio_spec_.samples		= samples_count;
    sdl_audio_spec_.silence		= 0;
    sdl_audio_spec_.callback	= sdl_native_audio_callback;
    sdl_audio_spec_.userdata	= this;
}

bool device_platform::do_open()
{
    const int iscapture = type() == type_t::capture ? 1 : 0;
    SDL_AudioSpec spec_obtained;
    constexpr int allowed_changes = 0;


    if (device_name().empty()) {
        native_device_id_ = SDL_OpenAudioDevice(nullptr, iscapture, &sdl_audio_spec_, &spec_obtained, allowed_changes);

        // NOTE: This zero in SDL_GetAudioDeviceName(0, iscapture) works to get the "default", but why using the
        //       native_device_id_ will not work, I do not know.
        const char* const device_name_cstr = SDL_GetAudioDeviceName(0, iscapture);
        if (device_name_cstr) {
            device_name_set(device_name_cstr);
        }
    }
    else {
        native_device_id_ = SDL_OpenAudioDevice(device_name().c_str(), iscapture, &sdl_audio_spec_, &spec_obtained, allowed_changes);
    }

    format_changed_set(sdl_audio_spec_.format != spec_obtained.format);
    sample_freq_changed_set(sdl_audio_spec_.freq != spec_obtained.freq);
    channels_count_changed_set(sdl_audio_spec_.channels != spec_obtained.channels);
    samples_count_changed_set(sdl_audio_spec_.samples != spec_obtained.samples);
    return native_device_id_ >= 2;
}

void device_platform::do_close()
{
    SDL_CloseAudioDevice(native_device_id_);
}

void device_platform::do_play()
{
    constexpr int pause_on = 0;
    SDL_PauseAudioDevice(native_device_id_, pause_on);
}

void device_platform::do_pause()
{
    constexpr int pause_on = 1;
    SDL_PauseAudioDevice(native_device_id_, pause_on);
}

// -------------------------
// --- PRIVATE: Helpers  ---
// -------------------------

sample_format_t device_platform::to_format_t(SDL_AudioFormat sdl_format)
{
    static const std::unordered_map<SDL_AudioFormat, sample_format_t> m = {
        {AUDIO_U8,sample_format_t::u8},
        {AUDIO_S8,sample_format_t::s8},
        {AUDIO_U16,sample_format_t::u16},
        {AUDIO_S16,sample_format_t::s16},
        {AUDIO_U16LSB,sample_format_t::u16lsb},
        {AUDIO_S16LSB,sample_format_t::u16msb},
        {AUDIO_U16MSB,sample_format_t::s16lsb},
        {AUDIO_S16MSB,sample_format_t::s16msb},
        {AUDIO_S32LSB,sample_format_t::s32lsb},
        {AUDIO_S32MSB,sample_format_t::s32msb},
        {AUDIO_F32LSB,sample_format_t::f32lsb},
        {AUDIO_F32MSB,sample_format_t::f32msb},
        {0,sample_format_t::SIZE},
        {1,sample_format_t::INVALID}
    };
    return m.at(sdl_format);
}

SDL_AudioFormat device_platform::to_sdl_format(sample_format_t format)
{
    static const std::unordered_map<sample_format_t, SDL_AudioFormat> m = {
        {sample_format_t::u8,AUDIO_U8},
        {sample_format_t::s8,AUDIO_S8},
        {sample_format_t::u16,AUDIO_U16},
        {sample_format_t::s16,AUDIO_S16},
        {sample_format_t::u16lsb,AUDIO_U16LSB},
        {sample_format_t::u16msb,AUDIO_S16LSB},
        {sample_format_t::s16lsb,AUDIO_U16MSB},
        {sample_format_t::s16msb,AUDIO_S16MSB},
        {sample_format_t::s32lsb,AUDIO_S32LSB},
        {sample_format_t::s32msb,AUDIO_S32MSB},
        {sample_format_t::f32lsb,AUDIO_F32LSB},
        {sample_format_t::f32msb,AUDIO_F32MSB},
        {sample_format_t::SIZE,0},
        {sample_format_t::INVALID,1}
    };
    return m.at(format);
}


void device_platform::set_default_audio_format()
{
    sdl_audio_spec_.freq		= 44100;
    sdl_audio_spec_.format		= AUDIO_S16LSB;		// Signed, little endian, 16 bit samples
    sdl_audio_spec_.channels	= 2;				// Stereo
    sdl_audio_spec_.samples		= 4096;				// Stereo
    sdl_audio_spec_.silence		= 0;
    sdl_audio_spec_.callback	= sdl_native_audio_callback;
    sdl_audio_spec_.userdata	= this;
}



} // END namespace cpaf::audio

