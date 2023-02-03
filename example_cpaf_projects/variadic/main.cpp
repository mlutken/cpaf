#include <iostream>
#include "parameter_pack.h"
#include "make_unique.h"
#include "emplace_back.h"
#include "tprintf.h"
#include "processing.h"
#include "mytuple.h"
#include "Delegate.h"

using namespace std;


int main()
{
//    parameter_pack_test();
//    make_unique_test();
//    emplace_back_test();
//    processing_test();
//    tprintf_test();
//    mytuple_test();
    Delegate_test();

    return 0;
}


template<typename... Arguments>
class VariadicTemplate;
