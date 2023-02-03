// special_dirs_osx.cpp:	OSX implementation of the special directories class.
//					


#include "../file_directory_functions.h"
#include "../special_dirs.h"


namespace cpaf { namespace filesystem {

// --------------------------------------------------
// --- Private platform specific helper functions ---
// --------------------------------------------------


// -------------------------------------------------------------
// --- Functions that needs platform specific implementation ---
// -------------------------------------------------------------

filepaths_t          
special_dirs::system_libs()
{
    namespace fs = boost::filesystem;
    filepaths_t paths;
//        paths.push_back( fs::path(szPath, fs::native) ); 
    return paths;
}


boost::filesystem::path          
special_dirs::home()
{
    namespace fs = boost::filesystem;
    return fs::path(""); 
}

boost::filesystem::path          
special_dirs::app_data()
{
    namespace fs = boost::filesystem;
    return fs::path(""); 
}

boost::filesystem::path          
special_dirs::app_data_local()
{
    namespace fs = boost::filesystem;
    return fs::path(""); 
}



boost::filesystem::path          
special_dirs::documents()
{
    namespace fs = boost::filesystem;
    return fs::path(""); 
}


boost::filesystem::path          
special_dirs::pictures()
{
    namespace fs = boost::filesystem;
    return fs::path(""); 
}

boost::filesystem::path          
special_dirs::music()
{
    namespace fs = boost::filesystem;
    return fs::path(""); 
}


boost::filesystem::path          
special_dirs::video()
{
    namespace fs = boost::filesystem;
    return fs::path(""); 
}

boost::filesystem::path          
special_dirs::desktop()
{
    namespace fs = boost::filesystem;
    return fs::path(""); 
}


boost::filesystem::path          
special_dirs::windows()
{
    namespace fs = boost::filesystem;
    return fs::path(""); 
}


boost::filesystem::path          
special_dirs::system()
{
    namespace fs = boost::filesystem;
    return fs::path(""); 
}

boost::filesystem::path          
special_dirs::program_files()
{
    namespace fs = boost::filesystem;
    return fs::path(""); 
}



}} // END namespace cpaf::filesystem


