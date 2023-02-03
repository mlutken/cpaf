#ifndef _CBS_INTEGER_TYPES_H_
#define _CBS_INTEGER_TYPES_H_

/** \file cbs_integer_types.h
Typedefs for fixed sized integer types. CBS types for 8, 16, 32, 64 bits 
signed and unsigned integer types. The 16, 32 and 64 bit types are based on 
the defines CBS_INTXX_TYPE from 'cbs_config.h' which in turn comes from 
the cmake configure step via the 'cmake/set_cbs_integer_types.cmake' .
The 8 bit types are just set to be of type 'char'.
*/

#error cbs_integer_types_IS_NOT_TO_BE_USED_ANYMORE

#include "cbs.h"


CBS_BEGIN_NAMESPACE


// ---------------------------
// --- 8 bit integer types ---
// ---------------------------
typedef char                        int8_t;
typedef unsigned char		        uint8_t;


// ----------------------------
// --- 16 bit integer types ---
// ----------------------------
#ifndef	CBS_INT_16_DEFINED
    typedef CBS_INT16_TYPE         int16_t;
#   define CBS_INT_16_DEFINED
#endif
typedef unsigned CBS_INT16_TYPE    uint16_t;


// ----------------------------
// --- 32 bit integer types ---
// ----------------------------
#ifndef CBS_INT_32_DEFINED
    typedef CBS_INT32_TYPE         int32_t;
#   define CBS_INT_32_DEFINED
#endif
typedef unsigned CBS_INT32_TYPE    uint32_t;


// ----------------------------
// --- 64 bit integer types ---
// ----------------------------
#ifndef CBS_INT_64_DEFINED
    typedef CBS_INT64_TYPE         int64_t;
#   define CBS_INT_64_DEFINED
#endif
typedef unsigned CBS_INT64_TYPE    uint64_t;


// --------------------------------------------------------
// --- DoxyS / Doxygen documentation for the types here ---
// --------------------------------------------------------

/**
\typedef int8_t 
Signed 8 bit integer. With guaranteed size across all platforms.
*/

/**
\typedef uint8_t 
Unsigned 8 bit integer. With guaranteed size across all platforms.
*/

/**
\typedef int16_t 
Signed 16 bit integer. With guaranteed size across all platforms.
*/

/**
\typedef uint16_t 
Unsigned 16 bit integer. With guaranteed size across all platforms.
*/

/**
\typedef int32_t 
Signed 32 bit integer. With guaranteed size across all platforms.
*/

/**
\typedef uint32_t 
Unsigned 32 bit integer. With guaranteed size across all platforms.
*/

/**
\typedef int64_t 
Signed 64 bit integer. With guaranteed size across all platforms.
*/

/**
\typedef uint64_t 
Unsigned 64 bit integer. With guaranteed size across all platforms.
*/


CBS_END_NAMESPACE

#endif //_CBS_INTEGER_TYPES_H_
