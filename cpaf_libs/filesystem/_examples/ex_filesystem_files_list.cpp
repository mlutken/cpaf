#include <iostream>
#include <cpaf_file_directory_functions.h>
#include <cpaf_special_dirs.h>

static void ex_filesystem_files_list()
{
    using namespace std;
    using namespace cpaf::filesystem;
    namespace   fs = boost::filesystem;

    filepaths_t files;
    fs::path    ph          = special_dirs::home();
    bool        bFullPaths  = false;

    // First: NON recursive, match ALL files
    files   = files_list(ph, bFullPaths, boost::regex() );
    cout<<"Files in directory: "<<ph.string()<<endl<<endl;
    for ( filepaths_t::iterator it = files.begin(); it != files.end(); ++it ) {
        cout<<it->string()<<endl;
    }
    cout<<"Files count: "<<files.size()<<endl;
    cout<<endl<<endl;

    // Then:  RECURSIVE, match ONLY .txt files
    // Note: The '\' is escaped once because of C++ compiler and once for the regex.
    const boost::regex  filesMatch(".*\\.txt"); 
    int iRecurDepth     = -1;   // -1 Means recurse as far as possible.

    cout<<"Getting all *.txt files in home, might take half a minute or so..."<<endl;
    files   = files_list_recursive(ph, bFullPaths, filesMatch, iRecurDepth );
    cout<<"Recursive '*.txt' files in directory: "<<ph.string()<<endl<<endl;
    for ( filepaths_t::iterator it = files.begin(); it != files.end(); ++it ) {
        cout<<it->string()<<endl;
    }
    cout<<"Files count: "<<files.size()<<endl;
    cout<<endl<<endl;
}
