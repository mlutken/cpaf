#include "cpaf_audio_system.h"



namespace cpaf::audio{

system& system::instance()
{
	static system sys;
	return sys;
}

} //END namespace cpaf::audio

