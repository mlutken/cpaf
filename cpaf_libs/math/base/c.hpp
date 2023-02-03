#ifndef _C_HPP_
#define _C_HPP_
// ****************************************************************************
// CREATED_ON	: 31-08-03 09:13
// CREATED_BY	: Martin Lutken
// ----------------------------------------------------------------------------
// COPYRIGHT	:
// ****************************************************************************

// --- Include files ---
#include "gmath.h"

_GMATH_BEGIN_NAMESPACE

/**
Mathematical real number constants. Generic version uses float. Specialization
exist for double precision.
*/
template <typename T>
class C
{
public:
    //---------------------
    //--- Trigonometric ---
    //---------------------

    /** PI. Definition of PI.*/
    static const T			PI()
    { return static_cast<T>(3.141592653589793f);}

    /** Convert radians to degrees. Multiplication constant used to convert
    from radians to degrees.
    Example: angle_in_radians = angle_in_degrees * C<float>::TODEG().*/
    static const T			TODEG()
    { return static_cast<T>(180.f) / PI();}

    /** Convert degrees to radians. Multiplication constant used to convert
    from degrees to radians.
    Example: angle_in_radians = angle_in_degrees * C<float>::TODEG().*/
    static const T			TORAD()
    { return static_cast<T>( PI() / static_cast<T>(180.f) );}


    //------------
    //--- Misc ---
    //------------

    /** Epsilon value. More usable epsilon for practical numerical calculations
        than the one found in numeric_limits.*/
    static const T			EPSILON()
    { return static_cast<T>(1.f / 8192.f);}

};

//----------------------------------------
//--- Double precision specialization. ---
//----------------------------------------
template <>
class C<double_t>
{
public:
    //---------------------
    //--- Trigonometric ---
    //---------------------
    static constexpr double_t	PI()
    { return static_cast<double_t>(3.14159265358979323846264338327950288);}

    static constexpr double_t	TODEG()
    { return static_cast<double_t>(180.0) / PI();}

    static constexpr double_t	TORAD()
    { return static_cast<double_t>( PI() / static_cast<double_t>(180.0));}

    //------------
    //--- Misc ---
    //------------
    static constexpr double_t	EPSILON()
    { return static_cast<double_t>(1.f / 8192.f);} // NOT_IMPL: Better (smaller) value needed XX

};

using Cf = C<float>;
using Cd = C<double>;



_GMATH_END_NAMESPACE



#endif	// _C_HPP_
