#ifndef TPRINTF_H
#define TPRINTF_H
#include <iostream>

inline void tprintf(const char* format) // base function
{
    std::cout << format;
}

template<typename T, typename... ArgTypes>
inline void tprintf(const char* format, T value, ArgTypes... args) // recursive variadic function
{
    for ( ; *format != '\0'; format++ ) {
        if ( *format == '%' ) {
           std::cout << value;
           tprintf(format+1, args...); // "recursive" call
           return;
        }
        std::cout << *format;
    }
}


int tprintf_test();


#endif // TPRINTF_H
