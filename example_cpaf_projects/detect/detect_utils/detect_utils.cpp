#include "detect_utils.h"
 
#include <iostream>
#include <cpaf_special_dirs.h>

namespace detect {

void detect_utils_fun1()
{
    std::cout<<"detect_utils_fun1()"<<std::endl;
}

void detect_utils_fun2()
{
    std::cout<<"detect_utils_fun2(): Documents dir:"<<cpaf::filesystem::special_dirs::documents().string()<<std::endl;
}

} // END namespace detect

