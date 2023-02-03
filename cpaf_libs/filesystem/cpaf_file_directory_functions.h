#ifndef _CPAF_FILE_DIRECTORY_FUNCTIONS_H_
#define _CPAF_FILE_DIRECTORY_FUNCTIONS_H_

 
//#include "../include/cpaf.h"

//#include <iostream>         // NOTE: ML For debug only
#include <vector>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>


namespace cpaf { namespace filesystem {

/** \unit file_directory_functions.h
! Functions for file and directory manipulation.
This collection of free functions can be used to manipulate files and directories. 
This includes functions for copying, renaming files and for creating and reading the
contents of directories.
\todo Move DoxyS-comments for platform independent functions to cpp-files when function 
      parameters (design) are stable.
\author Martin Lutken 
*/

/// Vector of boost::filesystem::path's for getting lists of files / directories.
typedef std::vector<boost::filesystem::path>    filepaths_t;



// -----------------------------
// --- Types and Definitions ---
// -----------------------------
/** Enum describing whether a file/directory path is local or remote .
    Currently remote is for paths beginning with 'http://', 'https://' or 'ftp://' */
enum class media_location { local, remote };

/** Enum describing whether a file/directory path is local or remote .
    Currently remote is for paths beginning with 'http://', 'https://' or 'ftp://' */
enum class copy_mode { single_dir, recursive };

// -------------------------------------------------------------
// --- Functions that needs platform specific implementation ---
// -------------------------------------------------------------

const boost::filesystem::path
normalize( boost::filesystem::path& p );

int move(const boost::filesystem::path& src, const boost::filesystem::path& dst);
int copy(const boost::filesystem::path& src, const boost::filesystem::path& dst, copy_mode cm);


//CBS_EXTERN retval CBS_EXPORT


/** 
Retrieves a list of files in a directory. 
\return Vector of boost filepaths containing file names (i.e. no directories). 
\example _examples/ex_filesystem_files_list.cpp
\sa subdirectories_list(), subdirectories_list_recursive(), files_list_recursive() */
filepaths_t		
files_list              ( 
    const boost::filesystem::path& path,    ///< Path to directory to retreive files list for.
    bool full_paths,                        ///< Select wheter to return full path names or just leaf names.
    const boost::regex& re_match            ///< Only return names that match this regular expression, Use empty (default constructed) regex for matching all names.
                        );

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
    const boost::regex& re_match            ///< Only return names that match this regular expression, Use empty (default constructed) regex for matching all names.
                        );



/// Get creation time for a file or directory.
/// \todo MLU: Is this the same as last_write_time ???
void		
creation_time           		( const boost::filesystem::path& path );

// -------------------------------------------------------------------------------------
// --- Convenience functions. Can freely use platform specific implemented functions ---
// -------------------------------------------------------------------------------------
void remove_safe (const boost::filesystem::path& path);


/** Get the stem file name without any extenstions.
Works also in case we have more than one dot '.' in the filename:
@code
stem_base("/foo/bar.txt.gz"); // => 'bar'
stem_base("/foo/bar.txt"); // => 'bar'
stem_base("/foo/bar.gz"); // => 'bar'
stem_base("/foo/bar"); // => 'bar'
@endcode
*/
boost::filesystem::path stem_base(const boost::filesystem::path& path);

/** Get the stem file name, but only remove certain extensions.
@code
stem_base("/foo/baz.bar.txt.gz", {".txt", ".gz"}); // => 'baz.bar'
stem_base("/foo/baz.bar.txt.gz", {".txt", ".gz", ".bar"}); // => 'baz'
stem_base("/foo/baz.bar.txt.gz", {}); // => 'baz.bar.txt.gz'
@endcode
*/
boost::filesystem::path stem_remove_extensions(const boost::filesystem::path& path,
                                               const std::vector<std::string>& extensions_remove);


media_location  detect_media_location (const std::string& resource_path);


filepaths_t		
files_list 						( const boost::filesystem::path& path, bool full_paths, const std::string& wild );

filepaths_t		
subdirectories_list 			( const boost::filesystem::path& path, bool full_paths, const std::string& wild );




boost::filesystem::path 
intersection_from_start         ( const boost::filesystem::path& path1, const boost::filesystem::path& path2 );

boost::filesystem::path 
remove_from_start               ( const boost::filesystem::path& inPath, const boost::filesystem::path& remPath );

boost::filesystem::path 
relative_path                   ( const boost::filesystem::path& pathFrom, const boost::filesystem::path& fileTo );                                         

boost::filesystem::path 
remove_redundant_separators		( const boost::filesystem::path& filePath );

std::string 
without_file_extension          ( const std::string& sFileName );

boost::filesystem::path
without_file_extension          ( const boost::filesystem::path& path,
                                  const boost::filesystem::path& replacement );

boost::filesystem::path
replace_extension_copy          ( const boost::filesystem::path& path,
                                  const boost::filesystem::path& replacement );

bool
force_copy_file                 ( const boost::filesystem::path& srcFile,  const boost::filesystem::path& dstFile );

filepaths_t		
files_list_recursive            ( const boost::filesystem::path& path, bool full_paths, const boost::regex& re_match, int recursion_depth );
filepaths_t		
files_list_recursive            ( const boost::filesystem::path& path, bool full_paths, const std::string& wild, int recursion_depth );

filepaths_t		
subdirectories_list_recursive   ( const boost::filesystem::path& path, bool full_paths, const boost::regex& re_match, int recursion_depth );
filepaths_t		
subdirectories_list_recursive   ( const boost::filesystem::path& path, bool full_paths, const std::string& wild, int recursion_depth );


// --- read file functions ---
std::string     read_file   ( const boost::filesystem::path& path );
std::string     read_file	( const std::string& path );
bool            read_file	( std::string& contents, const boost::filesystem::path& path );
bool            read_file	( std::string& contents, const std::string& path );

void            write_file   ( const boost::filesystem::path& filePath, const char* szString );
void            write_file   (const boost::filesystem::path& filePath, const std::string& str );

void            append_to   (const boost::filesystem::path& filePath, const char* szString );
void            append_to   (const boost::filesystem::path& filePath, const std::string& str );
void            writelines  ( const boost::filesystem::path& filePath, const std::vector<std::string>& vecLines, bool bAppendNewLine = true );
void            appendlines ( const boost::filesystem::path& filePath, const std::vector<std::string>& vecLines, bool bAppendNewLine = true );
void            readlines   (std::vector<std::string>& vecLines, const boost::filesystem::path& filePath, const size_t maxLineLen = 16384 );

bool			directory_writeable	( const boost::filesystem::path& directoryPath );

}} // END namespace cpaf::filesystem


#endif //_CPAF_FILE_DIRECTORY_FUNCTIONS_H_


