#include <iostream>
#include <png.h>
#include <test_lib.h>
      
      
int main ( int argc, char* argv[] )
{
	using namespace std;
	cout<<"link_test example executable"<<endl;
	  
	cout << "png_access_version_number: " << png_access_version_number() << endl; 
	
	utf8_test();
	
	return 0;
}


