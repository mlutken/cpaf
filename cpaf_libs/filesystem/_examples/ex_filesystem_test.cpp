#include <iostream>
#include <string>
#include <bzlib.h>
#include <zlib.h>

 
int main(int argc, char* argv[])
{
    using namespace std;
    cout<<"ex_filesystem_test"<<endl;
    string sBz2Ver = BZ2_bzlibVersion();
    string sZVer = zlibVersion();
    cout<<"BZ2_bzlibVersion: "<<sBz2Ver<<endl;
    cout<<"zlibVersion: "<<sZVer<<endl;
    return 0;
}

