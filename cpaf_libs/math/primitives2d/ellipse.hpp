#ifndef _TMATH_P2D_ELLIPSE_
#define _TMATH_P2D_ELLIPSE_

#include "../../_tmath_internal_utils.hpp"


_P2D_BEGIN_NAMESPACE

/// DOXYS_OFF

// Storage policies here !!!

/// DOXYS_ON

/** 
Ellipse (2D) defined by 2 points and 2 radius'es .
\todo How to define an ellipse. See fx. http://mathworld.wolfram.com/Ellipse.html 
*/
template
<
    typename T,
    template <class> class StoragePolicy = PEllipseStoreByValue
>
class ellipse
    : public StoragePolicy<T>
{
	// ----------------
	// --- Typedefs ---
	// ----------------
	
public:	

};

_P2D_END_NAMESPACE



#endif	// _TMATH_P2D_ELLIPSE_
