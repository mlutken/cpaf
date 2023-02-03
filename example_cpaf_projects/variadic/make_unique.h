#ifndef MAKE_UNIQUE_H
#define MAKE_UNIQUE_H

#include <string>
#include <memory>

template<class T, class... ArgTypes>
std::unique_ptr<T> make_unique(ArgTypes&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<ArgTypes>(args)...));
}

template<class T, class... ArgTypes>
std::unique_ptr<T> make_unique1(ArgTypes... args)
{
    return std::unique_ptr<T>(new T(args...));
}




int make_unique_test();

#endif // MAKE_UNIQUE_H
