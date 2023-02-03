#include "cpaf_graphics_system.h"



namespace cpaf::graphics{

system& system::instance()
{
	static system sys;
	return sys;
}

} //END namespace cpaf::graphics

