#include <iostream>
#include <cpaf_special_dirs.h>

static void ex_filesystem_special_dirs()
{
    using namespace std;
    using namespace cpaf::filesystem;
    cout<<"Startup:         "<<special_dirs::startup.string()<<endl;
    cout<<"Home:            "<<special_dirs::home().string()<<endl;
    cout<<"App Data:        "<<special_dirs::app_data().string()<<endl;
    cout<<"App Data Local:  "<<special_dirs::app_data_local().string()<<endl;
    cout<<"Documents:       "<<special_dirs::documents().string()<<endl;
    cout<<"Pictures:        "<<special_dirs::pictures().string()<<endl;
    cout<<"Music:           "<<special_dirs::music().string()<<endl;
    cout<<"Video:           "<<special_dirs::video().string()<<endl;
    cout<<"Desktop:         "<<special_dirs::desktop().string()<<endl;
    cout<<"Windows:         "<<special_dirs::windows().string()<<endl;
    cout<<"System:          "<<special_dirs::system().string()<<endl;
    cout<<"Program Files:   "<<special_dirs::program_files().string()<<endl;
   
    filepaths_t sysLibs    = special_dirs::system_libs();
    cout<<"System libs:"<<endl;
    for (  filepaths_t::iterator it = sysLibs.begin(); it != sysLibs.end(); ++it ) {
       cout<<it->string()<<endl;
    }

}
