#include "cpaf_system_info.h"

// ---------------------------------------------------------------
// --- Choose correct implementation file for current platform ---
// ---------------------------------------------------------------
#if (CPAF_SYSTEM_ID == CPAF_SYSTEM_ID_LINUX)
//#   include  "linux/system_info_linux.cpp"
#endif

#if (CPAF_SYSTEM_ID == CPAF_SYSTEM_ID_WINDOWS)
//#   include "windows/system_info_windows.cpp"
#endif

#if (CPAF_SYSTEM_ID == CPAF_SYSTEM_ID_OSX)
//#   include  "generic/system_info_osx.cpp"
#endif


namespace cpaf { namespace system {





}} //END namespace cpaf::system

