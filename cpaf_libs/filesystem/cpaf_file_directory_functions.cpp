#include "cpaf_file_directory_functions.h"

#include <iostream>
#include <fstream>
#include <boost/filesystem/convenience.hpp>
#include <utils/cpaf_string_utils.hpp>
#include <config/cpaf_platform_definitions.h>


// ---------------------------------------------------------------
// --- Choose correct implementation file for current platform ---
// ---------------------------------------------------------------

#if (CPAF_SYSTEM_ID == CPAF_SYSTEM_ID_LINUX)
#   include  "linux/file_directory_functions_linux.cpp"
#endif

#if (CPAF_SYSTEM_ID == CPAF_SYSTEM_ID_WINDOWS)
#   include "windows/file_directory_functions_windows.cpp"
#endif

#if (CPAF_SYSTEM_ID == CPAF_SYSTEM_ID_OSX)
#   include  "generic/file_directory_functions_generic.cpp"
#endif
      
namespace cpaf { namespace filesystem {

// --------------------------------------------------------------------------
// --- Convenience functions. Can freely use platform dependant functions ---
// --------------------------------------------------------------------------

/** Safely removes the file or directory.
 * If t he path does not exists nothing is done.
 * If the path is a directory it is safely removed.
*/
void remove_safe(const boost::filesystem::path& path)
{
    namespace fs = boost::filesystem;
    boost::system::error_code ec;
    if (fs::exists(path, ec)) {
        if (fs::is_directory(path)) {
            fs::remove_all(path, ec);
        }
        else {
            fs::remove(path, ec);
        }
    }
}

boost::filesystem::path stem_base(const boost::filesystem::path& path)
{
    auto stem = path.stem();
    while (stem.has_extension()) {
        stem = stem.stem();
    }
    return stem;
}

boost::filesystem::path stem_remove_extensions(
        const boost::filesystem::path& path,
        const std::vector<std::string>& extensions_remove)
{
    auto stem = path.filename();
    const auto begin = extensions_remove.begin();
    const auto end = extensions_remove.end();

    while (std::find(begin, end, stem.extension().string()) != end ) {
        stem = stem.stem();
    }
    return stem;
}


/**
Detect the media type for a file 'remote' or 'local'.
*/
media_location detect_media_location(const std::string& resource_path)
{
    using namespace boost::algorithm;
    if ( starts_with(resource_path, "https://") ||
         starts_with(resource_path, "http://")  ||
         starts_with(resource_path, "ftp://")
       ) {
        return media_location::remote;
    }
    return media_location::local;
}


/** 
Retrieves a list of files in a directory. 
\return Vector of boost filepaths containing file names (i.e. no directories). 
\example _examples/ex_filesystem_files_list.cpp
\sa subdirectories_list(), subdirectories_list_recursive(), files_list_recursive() */
filepaths_t		
files_list              ( 
    const boost::filesystem::path& path,    ///< Path to directory to retreive files list for.
    bool full_paths,                        ///< Select wheter to return full path names or just leaf names.
    const std::string& wild            		///< Only return names that match this wildcard/globbing expression, Use empty string for matching all names.
)
{
    namespace fs = boost::filesystem;
    filepaths_t files;
    fs::directory_iterator dirEnd   = fs::directory_iterator();
    fs::directory_iterator itDir    = fs::directory_iterator(path);
  
    for ( ; itDir != dirEnd; ++itDir ) {
        if ( !is_directory( *itDir ) ) {
            fs::path p( itDir->path().filename() );

            // If not wildcard expression is empty, then skip names that does not match
            // I.e. An empty wildcard expression we define to match all names
            if ( !wild.empty() && !cpaf::wildcmp( p.string(), wild ) ) continue;

            if ( full_paths )   files.push_back( path / p );
            else                files.push_back( p );
        }
    }
    return files;
}




/** 
Retrieves a list of subdirectories in a directory. The list of directories 
will never contain current dir and parent dir. I.e. '.' and '..' are never 
included in the list.
\return Vector of boost filepaths containing subdirectories names (i.e. no files). 
\example _examples/ex_filesystem_subdirectories_list.cpp
\sa subdirectories_list_recursive(), files_list(), files_list_recursive() */
filepaths_t		
subdirectories_list     ( 
    const boost::filesystem::path& path,    ///< Path to directory to retreive subdirectories list for.
    bool full_paths,                        ///< Select wheter to return full path names or just leaf names.
    const std::string& wild            		///< Only return names that match this wildcard/globbing expression, Use empty string for matching all names.
)
{
    namespace fs = boost::filesystem;
    filepaths_t dirs;
    fs::directory_iterator dirEnd   = fs::directory_iterator();
    fs::directory_iterator itDir    = fs::directory_iterator(path);
  
    for ( ; itDir != dirEnd; ++itDir ) {
        if ( is_directory( *itDir ) ) {
            fs::path p( itDir->path().filename());
            if ( p.string() == ".." || p.string() == "." ) continue;

            // If not wildcard expression is empty, then skip names that does not match
            // I.e. An empty wildcard expression we define to match all names
            if ( !wild.empty() && !cpaf::wildcmp( p.string(), wild ) ) continue;

            if ( full_paths )   dirs.push_back( path / p );
            else                dirs.push_back( p );
        }
    }
    return dirs;
}



/**  
Get common part of the two from the start. Finds the intersection of two parts 
from the beginning of the paths. See the example below.
\return The intersection/common-part (from the root and forward) of the two paths.
\example
    namespace fs = boost::filesystem;
    fs::path path1("z:/hello/horse/goat/pig/cow/sheep");
    fs::path path2("z:/hello/horse/whale/dolphin");
    intersection_from_start(inPath, remPath) == fs::path("z:/hello/horse"); 
\endexample
*/
boost::filesystem::path 
intersection_from_start(
    const boost::filesystem::path& path1,  ///< [in] Path 1
    const boost::filesystem::path& path2   ///< [in] Path 2 
                        )                                         
{                                                                                 
    namespace fs = boost::filesystem;
    fs::path::iterator it1  = path1.begin();
    fs::path::iterator it2  = path2.begin();

    if ( *it1 != *it2 ) 
    {
        assert(*it1 == *it2 );
        return fs::path();    // If path do not have same root, return an empty path !!!
    }
    fs::path intersectionPath;
    while ( (it1 != path1.end()) && (it2 != path2.end()) && (*it1 == *it2))
    {
        intersectionPath /= fs::path(*it1);
        ++it1;
        ++it2;
    }
    return intersectionPath;
}



/** 
Remove part of path from beginning. See the example below. If paths does 
not have the same root then inPath is returned.
\return inPath with the intersection of inPath and remPath removed.
\example
    namespace fs = boost::filesystem;
    fs::path inPath("z:/hello/horse/goat/pig/cow/sheep");
    fs::path remPath("z:/hello/horse");
    remove_from_start(inPath, remPath) == fs::path("goat/pig/cow/sheep"); 
\endexample
*/

boost::filesystem::path 
remove_from_start( 
    const boost::filesystem::path& inPath,   ///< [in] Path to remove start of path from
    const boost::filesystem::path& remPath   ///< [in] Path to remove from inPath
                  )                                         
{                                                                                 
    namespace fs = boost::filesystem;
	
    fs::path::iterator itIn   = inPath.begin();
    fs::path::iterator itRem  = remPath.begin();

    if ( *itIn != *itRem ) 
    {
        // assert(*itIn == *itRem );
        return inPath;  // If path do not have same root, just return inPath !!!
    }
    while ((itIn != inPath.end()) && (itRem != remPath.end()) &&  (*itIn == *itRem) )
    {
        ++itIn;
        ++itRem;
    }
    fs::path outPath;
    for ( ;itIn != inPath.end(); ++itIn )
    {
        outPath /= fs::path( *itIn ); // fs::native so we can cope with spaces in file names
    }
    return outPath;
}

/** 
Make relative path from dir to file. Use this function to make a relative 
link to a file in one directory, from a given directory. Typically one has 
an absolute from path (no filename only path) and an absolute path to the 
file to link to (path and filename). See examples.
\return Relative path from pathFrom to fileTo. 
\example
    namespace fs = boost::filesystem;
    fs::path linkFrom("z:/hello/horse/goat/pig/cow/sheep" );
    fs::path linkTo("z:/hello/horse/whale/dolphin/seal.txt");
    relative_path(linkFrom, linkTo) == fs::path("../../../../whale/dolphin/seal.txt");
\endexample
*/
boost::filesystem::path 
relative_path (
    const boost::filesystem::path& pathFrom, ///< [in] Path to link from. No file name only path.
    const boost::filesystem::path& fileTo    ///< [in] Path/filename to link to. Path including the filename to link to.
              )                                         
{                                                                                 
    namespace fs = boost::filesystem;
    fs::path::iterator itFrom = pathFrom.begin();
    fs::path::iterator itTo   = fileTo.begin();

    if ( *itFrom != *itTo ) 
    {
        assert(*itFrom == *itTo );
        return fileTo;  // If path do not have same root, just return "to path"!!!
    }
    while ( (itFrom != pathFrom.end()) && (itTo != fileTo.end()) && (*itFrom == *itTo) )
    {
        ++itFrom;
        ++itTo;
    }
    fs::path relPath;
    for ( ;itFrom != pathFrom.end(); ++itFrom )
    {
        relPath /= "..";
    }
    for ( ;itTo != fileTo.end(); ++itTo)
    {
        relPath /= *itTo;
    }
    return relPath;
}

/** 
Get path with redundant separators '/'s removed .
\return Path with redundanr separators '/'s removed.  */
boost::filesystem::path 
remove_redundant_separators (
    const boost::filesystem::path&	filePath	///< [in] Path to normalize.
)
{
    namespace fs = boost::filesystem;
    fs::path  normPath;
    fs::path::const_iterator it = filePath.begin();
    for ( ; it != filePath.end(); ++it ) {
		normPath /= *it;
    }
    return normPath;
}



/** 
Get filename without extension. if more than one extension only the last 
one is removed.
\return String containing the filename without it's (last) extension.*/
std::string 
without_file_extension  ( 
    const std::string& sFileName    ///< String which is considered to be a filename
                        )
{
    using namespace std;
    string::size_type iPos = sFileName.rfind('.');
    if (iPos != string::npos && (iPos > 0) ) 
    {
        return string(sFileName, 0, iPos);
    }
    else return sFileName;
}


/** 
Get filename without extension. if more than one extension only the last 
one is removed.
\return String containing the filename without it's (last) extension. */
boost::filesystem::path 
without_file_extension(
    const boost::filesystem::path& filePath     ///< Path name
                      )
{
    namespace fs = boost::filesystem;
    fs::path  basePath = filePath.branch_path();
    std::string sLeaf = filePath.filename().string();
    sLeaf = without_file_extension(sLeaf);
    return basePath / fs::path(sLeaf);
}

/** Replace extension and return path as copy */
boost::filesystem::path replace_extension_copy(
        const boost::filesystem::path& path,
        const boost::filesystem::path& replacement )
{
    auto p = path;
    p.replace_extension(replacement);
    return p;
}


/** 
Force copy file. File is always copied. If file exists it's overwritten. 
If destination path does not exist it is created. */
bool
force_copy_file (
    const boost::filesystem::path& srcFile,  ///< [in] Full path to source file.
    const boost::filesystem::path& dstFile   ///< [in] Full path to destination file.
                 )
{
    namespace fs = boost::filesystem;
    if ( !fs::exists(srcFile) )       return false;
    if (  fs::is_directory(srcFile) ) return false;

    fs::path absDstDir = dstFile.branch_path();
    if (!absDstDir.empty()) {
        fs::create_directories( absDstDir );
    }
    boost::system::error_code ec;
    fs::copy_file(srcFile, dstFile, fs::copy_option::overwrite_if_exists, ec);
    return ec.value() == boost::system::errc::errc_t::success;
}


// Helper function to recurse down a directory. 
/// \exclude
static void filesDirsListRecursive ( 
    filepaths_t&            paths,              ///< Vector of paths that we are building (appending to)
    boost::filesystem::path curSearchPath,      ///< Current path to search for subdirs or files in
    boost::filesystem::path curPrependPath,     ///< Current path to prepend the actual local leaf name
    bool                    bGetFiles,          ///< Select get list of file or subdirs
    const boost::regex&     re_match,           ///< Only return names that match this regular expression, Use empty (default constructed) regex for matching all names.
    int                     iDepth )            ///< Max recursion depth. -1 means infinite. 
{
    filepaths_t::iterator it;
    filepaths_t retrievePaths;

    filepaths_t subDirs                     = subdirectories_list( curSearchPath, false, boost::regex() );
    
    if ( bGetFiles )        retrievePaths   = files_list(curSearchPath, false, re_match );
    else                    retrievePaths   = subdirectories_list( curSearchPath, false, re_match );

    for ( it = retrievePaths.begin(); it != retrievePaths.end(); ++it ) {
        paths.push_back(curPrependPath / *it);
    }

    if ( iDepth == 0   )  return;
    if ( iDepth >  0   )  iDepth--;

    // Recurse
    for ( it = subDirs.begin(); it != subDirs.end(); ++it ) {
       // std::cout<<"Recur: "<<(curSearchPath / *it).string()<<std::endl;
        if ( !symbolic_link_exists( curSearchPath / *it ) ) {
            filesDirsListRecursive(paths, curSearchPath / *it, curPrependPath / *it, bGetFiles, re_match, iDepth );
        }
    }
}


// Helper function to recurse down a directory. 
/// \exclude
static void filesDirsListRecursive ( 
    filepaths_t&            paths,              ///< Vector of paths that we are building (appending to)
    boost::filesystem::path curSearchPath,      ///< Current path to search for subdirs or files in
    boost::filesystem::path curPrependPath,     ///< Current path to prepend the actual local leaf name
    bool                    bGetFiles,          ///< Select get list of file or subdirs
    const std::string&      wild,           	///< Only return names that match this wildcard/globbing expression, Use empty string for matching all names.
    int                     iDepth )            ///< Max recursion depth. -1 means infinite. 
{
    filepaths_t::iterator it;
    filepaths_t retrievePaths;

    filepaths_t subDirs                     = subdirectories_list( curSearchPath, false, boost::regex() );
    
    if ( bGetFiles ) {
        retrievePaths   = files_list(curSearchPath, false, wild );
    }
    else {
        retrievePaths   = subdirectories_list( curSearchPath, false, wild );
    }

    for ( it = retrievePaths.begin(); it != retrievePaths.end(); ++it ) {
        paths.push_back(curPrependPath / *it);
    }

    if ( iDepth == 0   ) {
        return;
    }
    if ( iDepth >  0   ) {
        iDepth--;
    }

    // Recurse
    for ( it = subDirs.begin(); it != subDirs.end(); ++it ) {
        if ( !symbolic_link_exists( curSearchPath / *it ) ) {
            filesDirsListRecursive(paths, curSearchPath / *it, curPrependPath / *it, bGetFiles, wild, iDepth );
        }
    }
}


/** 
Recursively retrieves a list of files in a directory. Select max recursion depth 
where -1 means indefinite, and selecting 0 will yield same result as calling 
files_list. If \a full_paths is set to false the paths returned will be relative to 
the input search path.
\return Vector of boost filepaths containing file names (i.e. no directories).
\example _examples/ex_filesystem_files_list.cpp
\sa subdirectories_list(), subdirectories_list_recursive(), files_list() */
filepaths_t		
files_list_recursive    ( 
    const boost::filesystem::path& path,    ///< Path to directory to retreive files list for.
    bool full_paths,                        ///< Select wheter to return full path names or just leaf names.
    const boost::regex& re_match,           ///< Only return names that match this regular expression, Use empty (default constructed) regex for matching all names.
    int recursion_depth                     ///< Max recursion depth. -1 Means infinite. 
                        )
{
    filepaths_t files;
    boost::filesystem::path prependPath("");
    if ( full_paths )       prependPath = path;

    filesDirsListRecursive(files, path, prependPath, true, re_match, recursion_depth);
    return files;
}

/** 
Recursively retrieves a list of files in a directory. Select max recursion depth 
where -1 means indefinite, and selecting 0 will yield same result as calling 
files_list. If \a full_paths is set to false the paths returned will be relative to 
the input search path.
\return Vector of boost filepaths containing file names (i.e. no directories).
\sa subdirectories_list(), subdirectories_list_recursive(), files_list() */
filepaths_t		
files_list_recursive    ( 
    const boost::filesystem::path& path,    ///< Path to directory to retreive files list for.
    bool full_paths,                        ///< Select wheter to return full path names or just leaf names.
    const std::string& wild, 	          	///< Only return names that match this wildcard/globbing expression, Use empty string for matching all names.
    int recursion_depth                     ///< Max recursion depth. -1 Means infinite. 
                        )
{
    filepaths_t files;
    boost::filesystem::path prependPath("");
    if ( full_paths )       prependPath = path;

    filesDirsListRecursive(files, path, prependPath, true, wild, recursion_depth);
    return files;
}


/** 
Recursively retrieves a list of subdirectories in a directory. Select max recursion depth 
where -1 means indefinite, and selecting 0 will yield same result as calling 
subdirectories_list. If \a full_paths is set to false the paths returned will be relative to 
the %input search path. The list of directories will never contain current dir and parent dir. 
I.e. '.' and '..' are never included in the list.
\return Vector of boost filepaths containing subdirectory names (i.e. no files). 
\example _examples/ex_filesystem_subdirectories_list.cpp
\sa subdirectories_list(), files_list(), files_list_recursive() */
filepaths_t		
subdirectories_list_recursive       ( 
    const boost::filesystem::path& path,    ///< Path to directory to retrieve subdirectories list for.
    bool full_paths,                        ///< Select wheter to return full path names or just leaf names.
    const boost::regex& re_match,           ///< Only return names that match this regular expression, Use empty (default constructed) regex for matching all names.
    int recursion_depth                     ///< Max recursion depth. -1 Means infinite. 
                                    )
{
    filepaths_t dirs;
    boost::filesystem::path prependPath("");
    if ( full_paths )       prependPath = path;

    filesDirsListRecursive(dirs, path, prependPath, false, re_match, recursion_depth);
    return dirs;
}


/** 
Recursively retrieves a list of subdirectories in a directory. Select max recursion depth 
where -1 means indefinite, and selecting 0 will yield same result as calling 
subdirectories_list. If \a full_paths is set to false the paths returned will be relative to 
the %input search path. The list of directories will never contain current dir and parent dir. 
I.e. '.' and '..' are never included in the list.
\return Vector of boost filepaths containing subdirectory names (i.e. no files). 
\sa subdirectories_list(), files_list(), files_list_recursive() */
filepaths_t		
subdirectories_list_recursive       ( 
    const boost::filesystem::path& path,    ///< Path to directory to retrieve subdirectories list for.
    bool full_paths,                        ///< Select wheter to return full path names or just leaf names.
    const std::string& wild, 	          	///< Only return names that match this wildcard/globbing expression, Use empty string for matching all names.
    int recursion_depth                     ///< Max recursion depth. -1 Means infinite. 
                                    )
{
    filepaths_t dirs;
    boost::filesystem::path prependPath("");
    if ( full_paths )       prependPath = path;

    filesDirsListRecursive(dirs, path, prependPath, false, wild, recursion_depth);
    return dirs;
}


/** Read contents of a file into a string */
std::string read_file						(
	const boost::filesystem::path& path 	///< [in] Path to file
)
{
	return read_file(path.string());
}

/** Read contents of a file into a string */
std::string read_file						(
	const std::string& path 				///< [in] Path to file
)
{
	std::string sContents;	
	read_file(sContents, path);
	return sContents;
}



/** Read contents of a file into a string */
bool read_file						(
	std::string& contents, 					///< [out] Contents of file read
	const boost::filesystem::path& 	path	///< [in] Path to file
)
{
	return read_file(contents, path.string());
}


/** Read contents of a file into a string */
bool read_file						(
		std::string& contents, 			///< [out] Contents of file read
		const std::string& path 		///< [in] Path to file
)
{
	std::ifstream in(path.c_str(), std::ios_base::in);
	if (!in)
	{
		std::cerr << "Error: Could not open input file: " << path << std::endl;
		return false;
	}
	in.unsetf(std::ios::skipws); // No white space skipping!
	std::copy(
		std::istream_iterator<char>(in),
		std::istream_iterator<char>(),
		std::back_inserter(contents));
	return true;
}

void write_file( const boost::filesystem::path& filePath,
                  const char* szString )
{
    std::ofstream os(filePath.string());
    os << szString;
}

void write_file( const boost::filesystem::path& filePath,
                  const std::string& str )
{
    std::ofstream os(filePath.string());
    os << str;
}

void append_to ( const boost::filesystem::path& filePath,
                 const char* szString )
{
    std::ofstream os;
    os.open(filePath.string(), std::ios_base::app);
    os << szString;
}

void  append_to ( const boost::filesystem::path& filePath,
                  const std::string& str )
{
    std::ofstream os;
    os.open(filePath.string(), std::ios_base::app);
    os << str;
}


void writelines ( const boost::filesystem::path& filePath,
                  const std::vector<std::string>& vecLines,
                  bool bAppendNewLine )
{
    std::ofstream os(filePath.string());
    std::vector<std::string>::const_iterator        it    = vecLines.begin();
    const std::vector<std::string>::const_iterator  itEND = vecLines.end();
    if ( bAppendNewLine ) {
        for ( ; it != itEND; ++it ) os << *it << "\n";
    }
    else {
        for ( ; it != itEND; ++it ) os << *it;
    }
}

void appendlines ( const boost::filesystem::path& filePath,
                   const std::vector<std::string>& vecLines,
                   bool bAppendNewLine )
{
    std::ofstream os;
    os.open(filePath.string(), std::ios_base::app);
    std::vector<std::string>::const_iterator        it    = vecLines.begin();
    const std::vector<std::string>::const_iterator  itEND = vecLines.end();
    if ( bAppendNewLine ) {
        for ( ; it != itEND; ++it ) os << *it << "\n";
    }
    else {
        for ( ; it != itEND; ++it ) os << *it;
    }
}


void readlines (std::vector<std::string>& vecLines,
                const boost::filesystem::path& filePath,
                const size_t maxLineLen )
{
    std::ifstream is(filePath.string());
    std::string line;
    line.reserve(maxLineLen);
    while (std::getline(is, line))
    {
        vecLines.push_back( line );
    }
}

/** Test whether we can write to a given directory.
\return true if we can write, false otherwise. */
bool directory_writeable(const boost::filesystem::path& directoryPath )
{
	namespace bf = boost::filesystem;
	bf::path path = directoryPath;
	bf::path filePath = path / "___A_VERY_UNLIKELY_FILE_NAME_TO_EXISTS_ALREADY__";

	bool writeable = false;
	// remove a possibly existing test file
    boost::system::error_code ec;
    if (bf::exists(filePath, ec)) {
        bf::remove(filePath, ec);
    }

	// check that the path exists
	if(!bf::is_directory(path)) return false;

	// try to write in the location
	std::ofstream outfile (filePath.string().c_str());
	outfile << "I can write!" << std::endl;
	outfile.close();

	if(!outfile.fail() && !outfile.bad())
	{
		writeable = true;
	}
    if (bf::exists(filePath, ec))
        bf::remove(filePath, ec);
    return writeable;
}

}} // END namespace cpaf::filesystem


