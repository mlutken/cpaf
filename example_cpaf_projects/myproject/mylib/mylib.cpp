#include "mylib.h"
 
#include <iostream>
#include <cpaf_special_dirs.h>

void my_lib_fun1()
{
    std::cout<<"my_lib_fun1()"<<std::endl;
}

void my_lib_fun2()
{
    std::cout<<"my_lib_fun2(): Documents dir:"<<cpaf::filesystem::special_dirs::documents().string()<<std::endl;
}


