#ifndef PARAMETER_PACK_H
#define PARAMETER_PACK_H
#include <iostream>
#include <tuple>


template<class... ArgTypes>
std::size_t parameter_pack_size()
{
    return sizeof...(ArgTypes);
}

template<class... ArgTypes>
std::tuple<size_t, ArgTypes...> parameter_pack_to_tuple(ArgTypes&&... args)
{
//    typedef std::tuple<size_t, ArgTypes...> ReturnTuple;

    return std::make_tuple(sizeof...(ArgTypes), args...);
}


int parameter_pack_test();

#endif // PARAMETER_PACK_H
