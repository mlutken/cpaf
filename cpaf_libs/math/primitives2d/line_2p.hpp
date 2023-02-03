#ifndef _TMATH_P2D_LINE_2P_
#define _TMATH_P2D_LINE_2P_


#include "../../_tmath_internal_utils.hpp"


_P2D_BEGIN_NAMESPACE

/// DOXYS_OFF

// Storage policies here !!!

/// DOXYS_ON

/** 
Line (2D) defined by 2 points. 
*/
template
<
    typename T,
    template <class> class StoragePolicy = PLine2PStoreByValue
>
class line_2p
    : public StoragePolicy<T>
{
	// ----------------
	// --- Typedefs ---
	// ----------------
	
public:	

};

_P2D_END_NAMESPACE



#endif	// _TMATH_P2D_LINE_2P_
