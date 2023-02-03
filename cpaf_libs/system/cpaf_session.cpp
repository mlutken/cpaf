#include "cpaf_session.h"
#include <chrono>
#include <thread>
#include <config/cpaf_platform_definitions.h>

// ---------------------------------------------------------------
// --- Choose correct implementation file for current platform ---
// ---------------------------------------------------------------
#if (CPAF_SYSTEM_ID == CPAF_SYSTEM_ID_LINUX)
#   include  "linux/session_linux.cpp"
#endif

#if (CPAF_SYSTEM_ID == CPAF_SYSTEM_ID_WINDOWS)
#   include "windows/session_windows.cpp"
#endif

#if (CPAF_SYSTEM_ID == CPAF_SYSTEM_ID_OSX)
#   include  "generic/session_osx.cpp"
#endif


namespace cpaf { namespace system {

void sleep_ms (uint32_t milli_seconds)
{
    const std::chrono::milliseconds ms_to_sleep(milli_seconds);
    std::this_thread::sleep_for(ms_to_sleep);
}

// --------------------------------------------------------------------------
// --- Convenience functions. Can freely use platform dependant functions ---
// --------------------------------------------------------------------------


}} //END namespace cpaf::system


/** 
\fn void cpaf::system::sleep   
Sleep current process for a specified number of seconds. */


/** 
\fn std::string cpaf::system::get_env   
Get environment variable.
\param  sEnvName Environment var name to get.
*/

/** 
\fn std::string cpaf::system::get_env   
Set environment variable.
\param  sEnvName  Environment var name to get.
\param  sEnvValue Environment value to set.
*/

