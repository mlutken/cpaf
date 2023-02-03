#include "make_unique.h"
#include <iostream>

#include "TestMe.h"

using namespace std;



int make_unique_test()
{
    cout << "\n--- make_unique_test() ---" << endl;

    TestMe me3 (3, "three");
    auto test_us_3 = make_unique<TestUs>(3.5f, std::move(me3));

//      auto test_us_3 = make_unique<TestUs>(3.5f, TestMe(3, "three"));
    //   TestMe m4(std::move(me3));
   //    TestMe m5(TestMe(5, "five"));
   //   auto test_us_3 = make_unique<TestUs>(3.5f, me3);


    return 0;
}




