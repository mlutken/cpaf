#ifndef _CPAF_SPECIAL_DIRS_H_
#define _CPAF_SPECIAL_DIRS_H_


#include "cpaf_file_directory_functions.h"

namespace cpaf { namespace filesystem {



/** Class with solely static members for getting various special directories.
The special directories are e.g. application's initial startup directory, current 
users home directory, the Documents directory and so on. If the current platform 
does not support the concept of a given directory say "Documents" then and empty 
path is returned. 
\example _examples/ex_filesystem_special_dirs.cpp
\remark The windows functions for getting these directories are described here : 
        http://msdn.microsoft.com/library/default.asp?url=/library/en-us/shellcc/platform/shell/reference/functions/shgetfolderpath.asp */
class special_dirs {
public:
    // Constant designating the applications initial start up directory.
    // Very handy when referring to paths relative to startup directory.
    // \sa home()
//    static const std::filesystem::path    startup;
    /** Function to the applications initial start up directory.
    Very handy when referring to paths relative to startup directory.
    \sa home() */
    static std::filesystem::path    startup_path();

    /** Function to default temporary directory.
    \sa home() */
    static std::filesystem::path    temp();

    /**
      Returns the full path to the executable. Also works if exe file is run using PATH
      (i.e. not with actual path to executable ).
      \see http://stackoverflow.com/questions/933850/how-to-find-the-location-of-the-executable-in-c
      */
    static std::filesystem::path    exe_path();

    /**
      Returns the full path to the executables directory. Also works if exe file is run using PATH
      (i.e. not with actual path to executable ).
      \see http://stackoverflow.com/questions/933850/how-to-find-the-location-of-the-executable-in-c
      */
    static std::filesystem::path    exe_dir();

    /** 
    Get system (dynamic/static libs) libraries list. On Windows this is typically 
    just one path (c:/windows/system32) and on unices it will be someting like
    $/lib$,$/usr/lib$,$/usr/local/lib$. Use this list when seaching for dynamic 
    libraries.
    \todo Implement special_dirs::home() on Mac, CE */
    static filepaths_t                      system_libs();

    /** 
    Get home directory of current user. If your business is to store 
    application specific settings then perhaps it's better to use 
    special_dirs::app_data(), since it fx on Windows returns "application data" 
    directory, which is a better place. On unix systems it is the same as home()
    \sa special_dirs::app_data(), special_dirs::app_data_local()
    \todo Implement special_dirs::home() on Mac, CE */
    static std::filesystem::path          home();

    /// Get application data directory of current user. On windows it's a 
    /// special dir in home and on unices (incl. Mac) it's same as home()
    /// \sa special_dirs::home(), special_dirs::app_data_local()
    /// \todo Implement special_dirs::app_data() on Mac, CE
    static std::filesystem::path          app_data();

    /// Get application data directory of current user. On windows it's a 
    /// special dir in home and on unices (incl. Mac) it's same as home()
    /// \sa special_dirs::app_data(), special_dirs::home()
    /// \todo Implement special_dirs::app_data() on Mac, CE
    static std::filesystem::path          app_data_local();

    /// Get Documents directory of current user.
    /// \todo Implement special_dirs::documents() on Mac, CE
    static std::filesystem::path          documents();

    /// Get Pictures directory of current user.
    /// \todo Implement special_dirs::pictures() on Mac, CE
    static std::filesystem::path          pictures();

    /// Get Music directory of current user.
    /// \todo Implement special_dirs::music() on Mac, CE
    static std::filesystem::path          music();

    /// Get Video directory of current user.
    /// \todo Implement special_dirs::video() on Mac, CE
    static std::filesystem::path          video();

    /// Get Desktop directory of current user.
    /// \todo Implement special_dirs::video() on Mac, CE
    static std::filesystem::path          desktop();

    /// Get Windows directory.
    /// \note Only non-empty on Windows systems
    /// \todo Implement special_dirs::windows() on CE
    static std::filesystem::path          windows();

    /// Get Windows System directory.
    /// \note Only non-empty on Windows systems, but see special_dirs::system_libs()
    /// \sa special_dirs::system_libs()
    /// \todo Implement special_dirs::system() on CE
    static std::filesystem::path          system();

    /// Get Windows "Program Files" directory.
    /// \note Only non-empty on Windows systems
    /// \todo Implement special_dirs::program_files() on CE
    static std::filesystem::path          program_files();
};


}} // END namespace cpaf::filesystem


#endif //_CPAF_SPECIAL_DIRS_H_


