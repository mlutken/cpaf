// file_directory_functions_windows.cpp:	Windows implementation of the file and directory functions.
//					

#include <stdio.h> // MLU_DEBUG: Only temporary this one!!!


#include "../cpaf_file_directory_functions.h"

#include <io.h>
#include <windows.h>


namespace cpaf { namespace filesystem {

// --------------------------------------------------
// --- Private platform specific helper functions ---
// --------------------------------------------------


// -------------------------------------------------------------
// --- Functions that needs platform specific implementation ---
// -------------------------------------------------------------

const boost::filesystem::path
normalize( boost::filesystem::path& p )
{
    return p.normalize();
}

filepaths_t		
files_list ( const boost::filesystem::path& path, bool full_paths, const boost::regex& re_match )
{
    namespace fs = boost::filesystem;
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
subdirectories_list ( const boost::filesystem::path& path, bool full_paths, const boost::regex& re_match )
{
    namespace fs = boost::filesystem;
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


// filepaths_t		
// files_list ( const boost::filesystem::path& path, bool full_paths, const boost::regex& re_match )
// {
    // namespace fs = boost::filesystem;
    // struct _finddata_t fileinfo;
    // long hFile;
    // filepaths_t files;
// //    std::cout<<"Windows: files_list: "<<path.string()<<std::endl;

	// std::wstring sPath = path.native() ;
	// sPath +=  "\\*";
    // hFile=_wfindfirst( sPath.c_str() , &fileinfo);
    // if ( hFile != -1 ) {
	    // do 
	    // {
		    // if ( !(fileinfo.attrib & _A_SUBDIR) ) {
                // fs::path p(fileinfo.name);
                
                // // If not regular expression is empty, then skip names that does not match
                // // I.e. An empty regular expression we define to match all names
                // if ( !re_match.empty() && !boost::regex_match( p.string(), re_match ) ) continue;
                
                // if ( full_paths )   files.push_back( path / p );
                // else                files.push_back( p );
		    // }
	    // } 
	    // while (_findnext(hFile,&fileinfo)==0);
	    // _findclose(hFile);
    // }
    // return files;
// }

// filepaths_t		
// subdirectories_list ( const boost::filesystem::path& path, bool full_paths, const boost::regex& re_match )
// {
    // namespace fs = boost::filesystem;
    // struct _finddata_t fileinfo;
    // long hFile;
    // filepaths_t dirs;
// //    std::cout<<"Windows: subdirectories_list: "<<path.string()<<std::endl;

    // hFile=_findfirst((path.native() + "\\*").c_str(),&fileinfo);
    // if ( hFile != -1 ) {
	    // do 
	    // {
		    // if ( (fileinfo.attrib & _A_SUBDIR) ) {
                // boost::filesystem::path p(fileinfo.name);
                // if ( p.string() == ".." || p.string() == "." ) continue;

                // // If not regular expression is empty, then skip names that does not match
                // // I.e. An empty regular expression we define to match all names
                // if ( !re_match.empty() && !boost::regex_match( p.string(), re_match ) ) continue;

                // if ( full_paths )   dirs.push_back( path / p );
                // else                dirs.push_back( p );
		    // }
	    // } 
	    // while (_findnext(hFile,&fileinfo)==0);
	    // _findclose(hFile);
    // }
    // return dirs;
// }


}} // END namespace cpaf::filesystem


