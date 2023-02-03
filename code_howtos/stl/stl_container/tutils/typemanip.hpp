#ifndef _TYPEMANIP_HPP_
#define _TYPEMANIP_HPP_
// ****************************************************************************
// CREATED_ON	: 09-07-03
// CREATED_BY	: Martin Lutken
// ****************************************************************************

#include "tutils.h"



_TUTILS_BEGIN_NAMESPACE

/** Class template select. (From loki library, but uses stl naming style here)
	Selects one of two types based upon a boolean constant
	Invocation: select<flag, T, U>::result
	where:
	flag is a compile-time boolean constant
	T and U are types
	Result evaluates to T if flag is true, and to U otherwise.*/
    template <bool flag, typename T, typename U>
    struct select
    {
        typedef T result;
    };
    template <typename T, typename U>
    struct select<false, T, U>
    {
        typedef U result;
    };


_TUTILS_END_NAMESPACE


#endif


