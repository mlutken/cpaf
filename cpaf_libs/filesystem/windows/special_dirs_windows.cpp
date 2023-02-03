// special_dirs_windows.cpp:	Windows implementation of the special directories class.
//					


#include "../cpaf_special_dirs.h"

#include <io.h>
#include <windows.h>
#include <shlobj.h>


namespace cpaf { namespace filesystem {

// --------------------------------------------------
// --- Private platform specific helper functions ---
// --------------------------------------------------


// -------------------------------------------------------------
// --- Functions that needs platform specific implementation ---
// -------------------------------------------------------------

boost::filesystem::path special_dirs::exe_path()
{
    char szExePath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szExePath, sizeof(szExePath));
    boost::filesystem::path exePath = boost::filesystem::canonical( szExePath );
    return exePath;
}


filepaths_t          
special_dirs::system_libs()
{
    namespace fs = boost::filesystem;
    filepaths_t paths;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        paths.push_back( fs::path(szPath) ); 
    }
    return paths;
}


boost::filesystem::path          
special_dirs::home()
{
    namespace fs = boost::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}

boost::filesystem::path          
special_dirs::app_data()
{
    namespace fs = boost::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}

boost::filesystem::path          
special_dirs::app_data_local()
{
    namespace fs = boost::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}



boost::filesystem::path          
special_dirs::documents()
{
    namespace fs = boost::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}


boost::filesystem::path          
special_dirs::pictures()
{
    namespace fs = boost::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYPICTURES, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}

boost::filesystem::path          
special_dirs::music()
{
    namespace fs = boost::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYMUSIC, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}


boost::filesystem::path          
special_dirs::video()
{
    namespace fs = boost::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYVIDEO, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}

boost::filesystem::path          
special_dirs::desktop()
{
    namespace fs = boost::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_DESKTOP, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}


boost::filesystem::path          
special_dirs::windows()
{
    namespace fs = boost::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_WINDOWS, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}


boost::filesystem::path          
special_dirs::system()
{
    namespace fs = boost::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}

boost::filesystem::path          
special_dirs::program_files()
{
    namespace fs = boost::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}



}} // END namespace cpaf::filesystem


