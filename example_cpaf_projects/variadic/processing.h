#ifndef PROCESSING_H
#define PROCESSING_H
#include <iostream>

inline void process(int i)
{
    std::cout << "process(int): " << i << std::endl;
}

inline void process(float f)
{
    std::cout << "process(float): " << f << std::endl;
}

inline void process(const std::string& s)
{
    std::cout << "process(string): '" << s << "'" << std::endl;
}



template<typename T>
inline void process_all(T value) // base function
{
    std::cout << "base_process: ";
    process(value);
}

template<typename T, typename... ArgTypes>
inline void process_all(T value, ArgTypes... args) // recursive variadic function
{
    std::cout << "main_process: ";
    process(value);
    process_all(args...); // "recursive" call
}


int processing_test();






















///

//template< int i >
//class FACTOR{
//  public:
//      enum {RESULT = i * FACTOR<I-1>::RESULT};
//};

//template<>
//class FACTOR< 1 >{
//  public:
//      enum {RESULT = 1};
//};



#endif // PROCESSING_H
