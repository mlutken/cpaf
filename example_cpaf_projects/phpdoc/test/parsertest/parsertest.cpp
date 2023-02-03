
#include <iostream>
#include <phpstructure/phpdoc_utils.h>

using namespace std;
using namespace phpdoc;

int main(int argc, char* argv[])
{
  
  cout << "Parser Test!" << endl;
 
#ifdef LINUX
  cout << "Linux system!"<<endl; 
#else
  cout << "Windows system perhaps ?"<<endl; 
#endif
 
  return 0;
}
