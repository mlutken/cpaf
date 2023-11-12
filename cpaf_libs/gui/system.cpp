#include "system.h"



namespace cpaf::gui{

system& system::instance()
{
	static system sys;
	return sys;
}

} //END namespace cpaf::gui

