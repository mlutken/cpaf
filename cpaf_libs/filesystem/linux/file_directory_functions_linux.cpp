// file_directory_functions_linux.cpp:	Linux implementation of the file and directory functions.
//					

 
#include "../cpaf_file_directory_functions.h"


#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <string>


namespace cpaf { namespace filesystem {

// --------------------------------------------------
// --- Private platform specific helper functions ---
// --------------------------------------------------
// this returns true if the path is a dir,
// 0 if it isn't, and -1 and sets errno
// if there was an error.
static int path_is_dir( const char *inPath ) 
{
	struct stat st_buf;
	
	if( stat( inPath, &st_buf ) )
		return -1;

	return S_ISDIR( st_buf.st_mode );
}

// this returns true if the path exists
// false if it doesn't, and -1 and sets errno if there
// was an error other than ENOENT
static int path_exists( const char *inPath )
{
	struct stat st_buf;
	int ret;

	ret = stat( inPath, &st_buf );
	if( ret == 0 )
		return 1;
	else if( errno == ENOENT )
	{
		return 0;
		errno = 0; // to avoid confusing things
	}
	else
		return -1;
}



// -------------------------------------------------------------
// --- Functions that needs platform specific implementation ---
// -------------------------------------------------------------

const boost::filesystem::path
normalize( boost::filesystem::path& p )
{
    return p.lexically_normal();
}

int move (const boost::filesystem::path& src, const boost::filesystem::path& dst)
{
    using namespace std;
    const std::string cmd = "mv -f "s + src.string() + " " + dst.string() + " 2>/dev/null";
    return system(cmd.c_str());
}

int copy (const boost::filesystem::path& src, const boost::filesystem::path& dst, copy_mode cm)
{
    using namespace std;
    std::string cmd = cm == copy_mode::recursive ? "cp -fr "s : "cp f"s;
    cmd += src.string() + " " + dst.string() + " 2>/dev/null";
    return system(cmd.c_str());
}


filepaths_t		
files_list ( const boost::filesystem::path& path, bool full_paths, const boost::regex& re_match  )
{
    namespace fs = boost::filesystem;
    filepaths_t files;
		
	int ret = path_exists( path.native().c_str() );
	if ( ret == 1 )
	{
		if( path_is_dir( path.native().c_str() ) )
		{
			// opendir
			DIR *dir = opendir( path.native().c_str() );
			if ( dir )
			{
				struct dirent *de;
                while( (de=readdir(dir)) != nullptr )
				{
					fs::path p( de->d_name );
					if( path_is_dir( (path / p).native().c_str() ) )	continue;
					if ( p.string() == ".." || p.string() == "." ) 		continue;

                    // If not regular expression is empty, then skip names that does not match
                    // I.e. An empty regular expression we define to match all names
                    if ( !re_match.empty() && !boost::regex_match( p.string(), re_match ) ) continue;

					if ( full_paths )   files.push_back( path / p );
					else                files.push_back( p );
				}
				closedir(dir);
			}
		}
	}
    
    return files;
}

filepaths_t		
subdirectories_list ( const boost::filesystem::path& path, bool full_paths, const boost::regex& re_match  )
{
    namespace fs = boost::filesystem;
    filepaths_t dirs;
	
	int ret = path_exists( path.native().c_str() );
	if ( ret == 1 )
	{
		if( path_is_dir( path.native().c_str() ) )
		{
			// opendir
			DIR *dir = opendir( path.native().c_str() );
			if ( dir )
			{
				struct dirent *de;
                while( (de=readdir(dir)) != nullptr )
				{
					fs::path p( de->d_name );
					if( !path_is_dir( (path / p).native().c_str() ) ) 	continue;
					if ( p.string() == ".." || p.string() == "." ) 					continue;   // Perhaps not needed here!

                    // If not regular expression is empty, then skip names that does not match
                    // I.e. An empty regular expression we define to match all names
                    if ( !re_match.empty() && !boost::regex_match( p.string(), re_match ) ) continue;

					if ( full_paths )   dirs.push_back( path / p );
					else                dirs.push_back( p );
				}
				closedir(dir);
			}
		}
	}
    return dirs;
}


}} // END namespace cpaf::filesystem


