// special_dirs_linux.cpp:	Linux implementation of the special directories class.
//					



#include "../cpaf_special_dirs.h"

#include <stdio.h>
#include <iostream>
#include <boost/tokenizer.hpp>

namespace cpaf { namespace filesystem {

// --------------------------------------------------
// --- Private platform specific helper functions ---
// --------------------------------------------------

/**
Get value of system environment variable.
\return String with the value of the enviroment variable. If the 
		variable does not exist an empty string is returned. */
static std::string get_env	(
	const std::string& env_name 			///< String designating the environment variable
							)
{
	const char* szEnvValue;
	std::string sEnvValue;
    szEnvValue = getenv( env_name.c_str() );
    if ( szEnvValue )	sEnvValue = szEnvValue;
    return sEnvValue;
}


/**
Get a list of paths from a string of paths. 
\example
filepaths_t paths paths_list_from_string("/usr/lib:/usr/local/lib", ":");
// Now paths variable holds a list of /usr/lib and /usr/local/lib
\endexample*/
static filepaths_t paths_list_from_string(
	const std::string& paths_string,		///< String of separator separated paths  
	const std::string& separator 			///< Separator used for separating the paths
										)
{
	namespace fs = std::filesystem;
	filepaths_t paths;
  	
  	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep( separator.c_str() );
 	tokenizer tok(paths_string, sep );
 	for(tokenizer::iterator beg=tok.begin(); beg!=tok.end();++beg){
        paths.push_back( fs::path(*beg) ); 
 	}
    return paths;
}


// -------------------------------------------------------------
// --- Functions that needs platform specific implementation ---
// -------------------------------------------------------------
std::filesystem::path special_dirs::temp()
{
    return "/tmp";
}

std::filesystem::path special_dirs::exe_path()
{
    std::filesystem::path exePath;
    char szExePath[PATH_MAX] = {0};
    auto iIgnore = readlink("/proc/self/exe", szExePath, sizeof(szExePath));
    if (iIgnore != -1) {
        exePath = std::filesystem::canonical( szExePath );
    }
    return exePath;
}

std::filesystem::path special_dirs::exe_dir()
{
    return exe_path().parent_path();
}


filepaths_t          
special_dirs::system_libs()
{
    namespace fs = std::filesystem;
    using namespace std;
    filepaths_t paths;
    string sLD_LIBRARY_PATH = get_env("LD_LIBRARY_PATH");
    cout<<"LD_LIBRARY_PATH: "<<sLD_LIBRARY_PATH<<endl;
    paths = paths_list_from_string(sLD_LIBRARY_PATH, ":");
    paths.push_back( fs::path("/lib") ); 
    paths.push_back( fs::path("/usr/lib") ); 
    return paths;
}


std::filesystem::path          
special_dirs::home()
{
    namespace fs = std::filesystem;
    return fs::path( get_env("HOME") ); 
}

std::filesystem::path          
special_dirs::app_data()
{
    namespace fs = std::filesystem;
    return fs::path( get_env("HOME")); 
}

std::filesystem::path          
special_dirs::app_data_local()
{
    namespace fs = std::filesystem;
    return fs::path( get_env("HOME")); 
}



std::filesystem::path          
special_dirs::documents()
{
    namespace fs = std::filesystem;
    std::string sPath = get_env("DOCUMENTS_DIR");
    if ( sPath != "" )	return fs::path( sPath );
    else				return special_dirs::home() / fs::path("Documents"); 
}


std::filesystem::path          
special_dirs::pictures()
{
    namespace fs = std::filesystem;
    std::string sPath = get_env("PICTURES_DIR");
    if ( sPath != "" )	return fs::path( sPath );
    else				return special_dirs::home() / fs::path("Pictures"); 
}

std::filesystem::path          
special_dirs::music()
{
    namespace fs = std::filesystem;
    std::string sPath = get_env("MUSIC_DIR");
    if ( sPath != "" )	return fs::path( sPath );
    else				return special_dirs::home() / fs::path("Music"); 
}


std::filesystem::path          
special_dirs::video()
{
    namespace fs = std::filesystem;
    std::string sPath = get_env("VIDEO_DIR");
    if ( sPath != "" )	return fs::path( sPath );
    else				return special_dirs::home() / fs::path("Video"); 
}

std::filesystem::path          
special_dirs::desktop()
{
    namespace fs = std::filesystem;
    std::string sPath = get_env("DESKTOP_DIR");
    if ( sPath != "" )	return fs::path( sPath );
    else				return special_dirs::home() / fs::path("Desktop"); 
}


std::filesystem::path          
special_dirs::windows()
{
    namespace fs = std::filesystem;
    return fs::path(""); 
}


std::filesystem::path          
special_dirs::system()
{
    namespace fs = std::filesystem;
    return fs::path(""); 
}

std::filesystem::path          
special_dirs::program_files()
{
    namespace fs = std::filesystem;
    return fs::path(""); 
}



}} // END namespace cpaf::filesystem


