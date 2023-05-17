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

std::filesystem::path special_dirs::exe_path()
{
    char szExePath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szExePath, sizeof(szExePath));
    std::filesystem::path exePath = std::filesystem::canonical( szExePath );
    return exePath;
}


filepaths_t          
special_dirs::system_libs()
{
    namespace fs = std::filesystem;
    filepaths_t paths;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        paths.push_back( fs::path(szPath) ); 
    }
    return paths;
}


std::filesystem::path          
special_dirs::home()
{
    namespace fs = std::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}

std::filesystem::path          
special_dirs::app_data()
{
    namespace fs = std::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}

std::filesystem::path          
special_dirs::app_data_local()
{
    namespace fs = std::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}



std::filesystem::path          
special_dirs::documents()
{
    namespace fs = std::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}


std::filesystem::path          
special_dirs::pictures()
{
    namespace fs = std::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYPICTURES, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}

std::filesystem::path          
special_dirs::music()
{
    namespace fs = std::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYMUSIC, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}


std::filesystem::path          
special_dirs::video()
{
    namespace fs = std::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYVIDEO, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}

std::filesystem::path          
special_dirs::desktop()
{
    namespace fs = std::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_DESKTOP, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}


std::filesystem::path          
special_dirs::windows()
{
    namespace fs = std::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_WINDOWS, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}


std::filesystem::path          
special_dirs::system()
{
    namespace fs = std::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}

std::filesystem::path          
special_dirs::program_files()
{
    namespace fs = std::filesystem;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, SHGFP_TYPE_CURRENT, szPath))) {
        return fs::path(szPath); 
    }
    else    return fs::path(""); 
}



}} // END namespace cpaf::filesystem


