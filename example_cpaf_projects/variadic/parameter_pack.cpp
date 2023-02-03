#include "parameter_pack.h"


using namespace std;


int parameter_pack_test()
{
    cout << "\n--- parameter_pack_test() ---" << endl;
    cout << "parameter_pack_size<>()                  : " << parameter_pack_size<>() << '\n'
         << "parameter_pack_size<int>()               : " << parameter_pack_size<int>() << '\n'
         << "parameter_pack_size<char, int, double>() : " << parameter_pack_size<char, int, double>() << '\n';


//    //typedef type =
//    auto tup = parameter_pack_to_tuple(1, std::string("d"), 2.5f);
//    cout << "\n--- tuple pack ---" << endl;
//    cout << "tup.size: " << std::get<0>(tup) << endl;
//    cout << "tup<1>: " << std::get<1>(tup) << endl;
    return 0;
}
