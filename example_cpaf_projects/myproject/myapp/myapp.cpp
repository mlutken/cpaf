
#include <iostream>

#include <mylib.h>


int main(int argc, char* argv[])
{
  using namespace std;
  
  cout << " Hello from CPAF/CBS ! "<<endl;
 
#ifdef LINUX
  cout << "Linux system!"<<endl; 
#else
  cout << "Windows system perhaps ?"<<endl; 
#endif
 
  my_lib_fun1();
  my_lib_fun2();
  return 0;
}

