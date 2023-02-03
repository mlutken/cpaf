#include "cpaf_special_dirs.h"  
#include <config/cpaf_platform_definitions.h>


// ---------------------------------------------------------------
// --- Choose correct implementation file for current platform ---
// ---------------------------------------------------------------
#if (CPAF_SYSTEM_ID == CPAF_SYSTEM_ID_LINUX)
#   include  "linux/special_dirs_linux.cpp"
#endif

#if (CPAF_SYSTEM_ID == CPAF_SYSTEM_ID_WINDOWS)
#   include "windows/special_dirs_windows.cpp"
#endif

#if (CPAF_SYSTEM_ID == CPAF_SYSTEM_ID_OSX)
#   include  "osx/special_dirs_osx.cpp"
#endif
          
      
namespace cpaf { namespace filesystem {

//// Initalize special application start up directory
//const boost::filesystem::path special_dirs::startup = boost::filesystem::initial_path();


boost::filesystem::path special_dirs::startup_path()
{
    static const boost::filesystem::path pathStartUp = boost::filesystem::initial_path();
    return pathStartUp;
}


// --------------------------------------------------------------------------
// --- Convenience functions. Can freely use platform dependant functions ---
// --------------------------------------------------------------------------

    

}} // END namespace cpaf::filesystem

