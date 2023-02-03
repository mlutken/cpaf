#include "emplace_back.h"
#include <iostream>
#include "TestMe.h"

using namespace std;


int emplace_back_test()
{
    cout << "\n--- emplace_back_test() ---" << endl;
    MyVector<TestMe> v(5);
    TestMe m0(0, "zero");
    v.push_back(m0);
    v.push_back(TestMe(1, "one"));
    v.emplace_back(2, "two");
    //v[2].m_name = "two_two";
    v.display_vector();

    return 0;
}


