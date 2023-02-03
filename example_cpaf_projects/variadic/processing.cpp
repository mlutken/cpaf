#include "processing.h"

#include <iostream>

using namespace std;



int processing_test()
{
    cout << "\n--- processing_test() ---" << endl;
    process_all(1, std::string("d"), 2.5f);
    return 0;
}


