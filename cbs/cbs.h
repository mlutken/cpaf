#ifndef _CBS_H_
#define _CBS_H_

/** 
\file cbs.h
*/

#include <cbs_config.h>
#include "cbs_platform_definitions.h"

// -----------------------------
// --- Namespace definitions ---
// -----------------------------

#define _CBS                       cbs
#define CBS_BEGIN_NAMESPACE        namespace _CBS {
#define CBS_END_NAMESPACE          }


// Dynamic modules/libraries export  
#   if ( WIN32 )
#       define CBS_EXTERN	extern "C" __declspec(dllexport)
#       define CBS_EXPORT
#       define CBS_CALL	__cdecl
#   else
#       define CBS_EXTERN	extern "C"
#       define CBS_EXPORT 	__attribute__ ((visibility ("default")))
#       define CBS_CALL
#   endif



/**
\var CBS_EXTERN
For exporting symbols in dynamic modules(libraries).
Often used in conjunction with CBS_EXPORT in the header file like this:

\code CBS_EXTERN my_return_type CBS_EXPORT my_exported_fun(); \endcode

This ensures correctly exported symbols/function of all supprted CPAF platforms. 
For more information please refer to dynamiclibs directory in cpaf_libs. 
*/

/**
\var CBS_EXPORT
For exporting symbols in dynamic modules(libraries).
Often used in conjunction with CBS_EXTERN in the header file like this:

\code CBS_EXTERN my_return_type CBS_EXPORT my_exported_fun(); \endcode

This ensures correctly exported symbols/function of all supprted CPAF platforms.
For more information please refer to dynamiclibs directory in cpaf_libs. 
*/

#endif //_CBS_H_



