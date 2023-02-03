#ifndef _TMATH_P2D_BOX_
#define _TMATH_P2D_BOX_

#include "../../_tmath_internal_utils.hpp"


_P2D_BEGIN_NAMESPACE

/// DOXYS_OFF

// Storage policies here !!!

/// DOXYS_ON

/** 
Rectangle (2D) defined by width, height and a center. 
*/
template
<
    typename T,
    template <class> class StoragePolicy = PBoxStoreByValue
>
class box
    : public StoragePolicy<T>
{
	// ----------------
	// --- Typedefs ---
	// ----------------
	
public:	

};

_P2D_END_NAMESPACE



#endif	// _TMATH_P2D_BOX_
