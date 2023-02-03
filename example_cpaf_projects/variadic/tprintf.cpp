#include "tprintf.h"
#include <iostream>

using namespace std;



int tprintf_test()
{
    cout << "\n--- tprintf_test() ---" << endl;
    tprintf("% world% %\n","Hello",'!',123);
    return 0;
}

