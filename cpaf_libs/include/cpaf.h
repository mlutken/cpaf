#ifndef _CPAF_H_
#define _CPAF_H_

/** 
\file cpaf.h
Namespace and platform definitions for use throughout CPAF.
All of CPAF reside in namespace "cpaf", and within here we have 
namespaces gui, XX ... Applications build on CPAF reside in 
their own namespace.
*/

//OUT: #include <cbs.h>

#error We currently don't use cpaf.h header file
// -----------------------------
// --- Namespace definitions ---
// -----------------------------

#define _CPAF								cpaf
#define CPAF_BEGIN_NAMESPACE				namespace _CPAF {
#define CPAF_END_NAMESPACE					}

#define _DYNAMICLIBS				        dynamiclibs
#define CPAF_DYNAMICLIBS_BEGIN_NAMESPACE	namespace _CPAF { namespace _DYNAMICLIBS {
#define CPAF_DYNAMICLIBS_END_NAMESPACE	    }}

#define _GUI								gui
#define CPAF_GUI_BEGIN_NAMESPACE			namespace _CPAF { namespace _GUI {
#define CPAF_GUI_END_NAMESPACE				}}


#define _SYSTEM								system
#define CPAF_SYSTEM_BEGIN_NAMESPACE			namespace _CPAF { namespace _SYSTEM {
#define CPAF_SYSTEM_END_NAMESPACE			}}


CPAF_BEGIN_NAMESPACE


CPAF_END_NAMESPACE

// ---------------------------
// --- DoxyS Documentation ---
// ---------------------------

/**
\var CPAF_BEGIN_NAMESPACE
For including code into cpaf namespace.
\include namespace_macros.inc
*/

/**
\var CPAF_END_NAMESPACE
For including code into cpaf namespace.
\include namespace_macros.inc
*/


/**
\var CPAF_GUI_BEGIN_NAMESPACE
For including code into cpaf::gui namespace.
\include namespace_macros.inc
*/

/**
\var CPAF_GUI_END_NAMESPACE
For including code into cpaf::gui namespace.
\include namespace_macros.inc
*/


#endif //_CPAF_H_



