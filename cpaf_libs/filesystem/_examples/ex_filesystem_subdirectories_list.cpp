#include <iostream>
#include <cpaf_file_directory_functions.h>
#include <cpaf_special_dirs.h>

static void ex_filesystem_subdirectories_list()
{
    using namespace std;
    using namespace cpaf::filesystem;
    namespace   fs = std::filesystem;

    filepaths_t files;
    fs::path    ph          = special_dirs::home();
    bool        bFullPaths  = true;

    // First: NON recursive, match ALL files
    files   = subdirectories_list(ph, bFullPaths, boost::regex() );
    cout<<"Dirs in directory: "<<ph.string()<<endl<<endl;
    for ( filepaths_t::iterator it = files.begin(); it != files.end(); ++it ) {
        cout<<it->string()<<endl;
    }
    cout<<"Dirs count: "<<files.size()<<endl;
    cout<<endl<<endl;

    // Then:  RECURSIVE, match ALL dirs
    int iRecurDepth     = 1;
    bFullPaths          = false;
    files   = subdirectories_list_recursive(ph, bFullPaths, boost::regex(), iRecurDepth );
    cout<<"Recursive dirs in directory: "<<ph.string()<<endl<<endl;
    for ( filepaths_t::iterator it = files.begin(); it != files.end(); ++it ) {
        cout<<it->string()<<endl;
    }
    cout<<"Dirs count: "<<files.size()<<endl;
    cout<<endl<<endl;
}
