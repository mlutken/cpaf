// session_generic.cpp:	Generic implementation of the session functions.
//					


#include "../session.h"


namespace cpaf { namespace system {

// --------------------------------------------------
// --- Private platform specific helper functions ---
// --------------------------------------------------


// -------------------------------------------------------------
// --- Functions that needs platform specific implementation ---
// -------------------------------------------------------------

bool shutdown ( bool force )
{
	return true;
}




}} //END namespace cpaf::system

