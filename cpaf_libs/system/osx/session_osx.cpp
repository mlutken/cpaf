// session_osx.cpp:	OSX implementation of the session functions.
//					


#include "../session.h"


namespace cpaf { namespace system {

// --------------------------------------------------
// --- Private platform specific helper functions ---
// --------------------------------------------------


// -------------------------------------------------------------
// --- Functions that needs platform specific implementation ---
// -------------------------------------------------------------
std::string username()
{
    return "OSX::username() TODO: IMPLEMENT ME!";
}

bool shutdown ( bool force )
{
	return true;
}


std::string
get_env( std::string const& sEnvName )
{
	return std::string("get_env NOT IMPLEMENTED YET");
}

void 
set_env( std::string const& sEnvName, std::string const& sEnvValue )
{
}


}} //END namespace cpaf::system

