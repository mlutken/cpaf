#pragma once
// ****************************************************************************
// CREATED_ON	: 20-03-03 19:00
// CREATED_BY	: Martin Lutken
// ----------------------------------------------------------------------------
// COPYRIGHT	:
// ****************************************************************************


#include <limits>

#include <math/base/gmath_config.h>

#define _GMATH					cpaf::math
#define _GMATH_BEGIN_NAMESPACE	namespace _GMATH {
#define _GMATH_END_NAMESPACE	}

#define _P2D					p2d
#define _P2D_BEGIN_NAMESPACE	namespace _GMATH { namespace _P2D {
#define _P2D_END_NAMESPACE	}}

#define _P3D					p3d
#define _P3D_BEGIN_NAMESPACE	namespace _GMATH { namespace _P3D {
#define _P3D_END_NAMESPACE	}}

// For the internal template utils functions
#define _UTILS									utils
#define _GMATH_INTERNAL_UTILS_BEGIN_NAMESPACE	namespace _GMATH { namespace _UTILS {
#define _GMATH_INTERNAL_UTILS_END_NAMESPACE		}}



_GMATH_BEGIN_NAMESPACE

// -------------------------------
// --- Namespace wide typedefs ---
// -------------------------------

// *** Map the basic built in types defined in gmath_config.h to this namespace ***
typedef gmath_char_t			char_t;
typedef gmath_bool_t			bool_t;
typedef gmath_int_t				int_t;
typedef gmath_unsigned_int_t	unsigned_int_t;
typedef gmath_float_t			float_t;
typedef gmath_double_t			double_t;

/** Enum for vector/matrix element acces. Enum defining X, Y, Z, W for indexing
vectors and matrices up to size 4/4x4 via the [] operator */
enum {X=0, Y=1, Z=2, W=3};






_GMATH_END_NAMESPACE

