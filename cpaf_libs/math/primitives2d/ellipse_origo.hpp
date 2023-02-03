#ifndef _TMATH_P2D_ELLIPSE_ORIGO_
#define _TMATH_P2D_ELLIPSE_ORIGO_

#include "../../_tmath_internal_utils.hpp"


_P2D_BEGIN_NAMESPACE


// No need for storage policy classes here! We have only a scalars (radius'es).


/** 
Ellipse (2D) defined by 2 radius'es centered around origo.
\todo How to define an ellipse. See fx. http://mathworld.wolfram.com/Ellipse.html 
*/
template
<
    typename T
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



#endif	// _TMATH_P2D_ELLIPSE_ORIGO_
