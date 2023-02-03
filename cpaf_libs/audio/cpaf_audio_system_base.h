#ifndef CPAF_AUDIO_SYSTEM_BASE_H
#define CPAF_AUDIO_SYSTEM_BASE_H

#include <cstdint>
#include <string>
#include <vector>


namespace cpaf::audio {

/** \unit cpaf_audio.h
 *  Free audio related functions like initialization etc.
\author Martin Lutken
*/

class system_base {
public:
	using device_names_t = std::vector<std::string>;
	system_base() = default;
	virtual ~system_base() = default;

	int32_t					init						() { return do_init(); }
	int32_t					play_devices_count			() const { return do_play_devices_count();		}
	int32_t					capture_devices_count		() const { return do_capture_devices_count();	}

	std::string				play_device_name			(int32_t device_index) const { return do_play_device_name(device_index); }
	std::string				capture_device_name			(int32_t device_index) const { return do_capture_device_name(device_index); }

	std::string				default_play_device_name	() const { return play_device_name(0); }
	std::string				default_capture_device_name	() const { return capture_device_name(0); }

	const device_names_t&	play_device_names			() const { return play_device_names_;		}
	const device_names_t&	capture_device_names		() const { return capture_device_names_;	}

	// --- Debug functions ---
	std::string				dbg_play_devices_str		(const std::string& caption = "Play devices") const;
	std::string				dbg_capture_devices_str		(const std::string& caption = "Capture devices") const;

protected:
	device_names_t			play_device_names_;
	device_names_t			capture_device_names_;

private:
	virtual int32_t			do_init						() = 0;
	virtual int32_t			do_play_devices_count		() const = 0;
	virtual int32_t			do_capture_devices_count	() const = 0;

	virtual std::string		do_play_device_name			(int32_t device_index) const = 0;
	virtual std::string		do_capture_device_name		(int32_t device_index) const = 0;



};


} //END namespace cpaf::audio


#endif // CPAF_AUDIO_SYSTEM_BASE_H


