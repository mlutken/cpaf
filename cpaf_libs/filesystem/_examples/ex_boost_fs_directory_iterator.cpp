#include <iostream>
#include <string>
#include <cpaf_special_dirs.h>

static void ex_boost_fs_directory_iterator()
{
    namespace fs = boost::filesystem;
    namespace cfs = cpaf::filesystem;
    fs::path ph = cfs::special_dirs::startup;

    fs::directory_iterator dirEnd   = fs::directory_iterator();
    fs::directory_iterator itDir    = fs::directory_iterator(ph);
    for ( ; itDir != dirEnd; ++itDir ) {
        std::cout<<itDir->string()<<std::endl;
    }
}
