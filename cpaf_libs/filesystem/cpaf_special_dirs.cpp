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
//const std::filesystem::path special_dirs::startup = std::filesystem::initial_path();


/// @todo: special_dirs::startup_path() returnrn current_path() !!!
std::filesystem::path special_dirs::startup_path()
{
    static const std::filesystem::path pathStartUp = std::filesystem::current_path();
    return pathStartUp;
}


// --------------------------------------------------------------------------
// --- Convenience functions. Can freely use platform dependant functions ---
// --------------------------------------------------------------------------

    

}} // END namespace cpaf::filesystem

