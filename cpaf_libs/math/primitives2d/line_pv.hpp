#ifndef _TMATH_P2D_LINE_PV_
#define _TMATH_P2D_LINE_PV_

#include "../../_tmath_internal_utils.hpp"


_P2D_BEGIN_NAMESPACE

/// DOXYS_OFF

// Storage policies here !!!

/// DOXYS_ON

/** 
Line (2D) defined by a point and a (direction) vector. 
*/
template
<
    typename T,
    template <class> class StoragePolicy = PLinePVStoreByValue
>
class line_pv
    : public StoragePolicy<T>
{
	// ----------------
	// --- Typedefs ---
	// ----------------
	
public:	

};

_P2D_END_NAMESPACE



#endif	// _TMATH_P2D_LINE_PV_
