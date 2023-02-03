#ifndef ALIGN_MACROS_H
#define ALIGN_MACROS_H

#if defined(_MSC_VER)
#define _ALIGNED(x) __declspec(align(x))
#else
#if defined(__GNUC__)
#define _ALIGNED(x) __attribute__ ((aligned(x)))
#endif
#endif

#define _ALIGNED_TYPE(t,x) typedef t _ALIGNED(x)


// SOME USAGE SAMPLES

/*
_ALIGNED_TYPE(double, 16) aligned_double_t;

_ALIGNED_TYPE(struct, CACHE_LINE) tagALIGNEDSTRUCT
{
    //STRUCT MEMBERS GO HERE
}aligned_struct_t;

_ALIGNED_TYPE(union, CACHE_LINE) tagALIGNEDUNION
{
    // UNION MEMBERS GO HERE

}aligned_union_t;

*/

#endif // ALIGN_MACROS_H
