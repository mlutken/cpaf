// file_directory_functions_generic.cpp:	Generic implementation of the file and directory functions.
//					


#include "../cpaf_file_directory_functions.h"


namespace cpaf { namespace filesystem {

// --------------------------------------------------
// --- Private platform specific helper functions ---
// --------------------------------------------------


// -------------------------------------------------------------
// --- Functions that needs platform specific implementation ---
// -------------------------------------------------------------



filepaths_t		
files_list ( const std::filesystem::path& path, bool full_paths, const boost::regex& re_match )
{
    namespace fs = std::filesystem;
    filepaths_t files;
    fs::directory_iterator dirEnd   = fs::directory_iterator();
    fs::directory_iterator itDir    = fs::directory_iterator(path);
  
    for ( ; itDir != dirEnd; ++itDir ) {
        if ( !is_directory( *itDir ) ) {
            fs::path p( itDir->path().filename() );

            // If not regular expression is empty, then skip names that does not match
            // I.e. An empty regular expression we define to match all names
            if ( !re_match.empty() && !boost::regex_match( p.string(), re_match ) ) continue;

            if ( full_paths )   files.push_back( path / p );
            else                files.push_back( p );
        }
    }
    return files;
}

filepaths_t		
subdirectories_list ( const std::filesystem::path& path, bool full_paths, const boost::regex& re_match )
{
    namespace fs = std::filesystem;
    filepaths_t dirs;
    fs::directory_iterator dirEnd   = fs::directory_iterator();
    fs::directory_iterator itDir    = fs::directory_iterator(path);
  
    for ( ; itDir != dirEnd; ++itDir ) {
        if ( is_directory( *itDir ) ) {
            fs::path p( itDir->path().filename() );
            if ( p.string() == ".." || p.string() == "." ) continue;

            // If not regular expression is empty, then skip names that does not match
            // I.e. An empty regular expression we define to match all names
            if ( !re_match.empty() && !boost::regex_match( p.string(), re_match ) ) continue;

            if ( full_paths )   dirs.push_back( path / p );
            else                dirs.push_back( p );
        }
    }
    return dirs;
}

}} // END namespace cpaf::filesystem
