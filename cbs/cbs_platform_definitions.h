#ifndef _CBS_PLATFORM_DEFINITIONS_H_
#define _CBS_PLATFORM_DEFINITIONS_H_

// ------------------------------------------
// --- Platform system family definitions ---
// ------------------------------------------

#define CBS_SYSTEM_FAMILY_WINDOWS		2		///< Specifies that system is Windows.
#define CBS_SYSTEM_FAMILY_OSX			3		///< Specifies that system is Apple OSX.
#define CBS_SYSTEM_FAMILY_LINUX	        4		///< Specifies that system is Linux.
#define CBS_SYSTEM_FAMILY_WIN_CE		5		///< Specifies that system is Windows CE.
#define CBS_SYSTEM_FAMILY_WIN_MOBILE	6		///< Specifies that system is Windows Mobile.
#define CBS_SYSTEM_FAMILY_SOLARIS		7		///< Specifies that system is Sun Solaris.


// ---------------------------------------
// --- Platform sub system definitions ---
// ---------------------------------------

// Windows sub-systems
#define CBS_SUBSYS_WINDOWS_311			2		///< Specifies that subsystem is Windows 3.11.
#define CBS_SUBSYS_WINDOWS_95			3		///< Specifies that subsystem is Windows 95.
#define CBS_SUBSYS_WINDOWS_98			4		///< Specifies that subsystem is Windows 98.
#define CBS_SUBSYS_WINDOWS_ME			5		///< Specifies that subsystem is Windows Me.
#define CBS_SUBSYS_WINDOWS_XP			6		///< Specifies that subsystem is Windows XP.
#define CBS_SUBSYS_WINDOWS_NT_35		7		///< Specifies that subsystem is Windows NT 3.5.
#define CBS_SUBSYS_WINDOWS_NT_4		    8		///< Specifies that subsystem is Windows NT 4.
#define CBS_SUBSYS_WINDOWS_2000		    9		///< Specifies that subsystem is Windows 2000.
#define CBS_SUBSYS_WINDOWS_VISTA		10	    ///< Specifies that subsystem is Windows Vista.

// Linux sub-systems
#define CBS_SUBSYS_LINUX_LSB			20	    ///< Specifies that subsystem is Linux Standard Base. Most recent distros try to adhere to this standard.

//Apple OSX sub-systems
#define CBS_SUBSYS_DARWIN				40	    ///< Specifies that subsystem is open darwin
#define CBS_SUBSYS_CHEETAH				42	    ///< Specifies that subsystem is Apple OSX v. 10.0
#define CBS_SUBSYS_PUMA				    44	    ///< Specifies that subsystem is Apple OSX v. 10.1
#define CBS_SUBSYS_JAGUAR				46	    ///< Specifies that subsystem is Apple OSX v. 10.2
#define CBS_SUBSYS_PANTHER				48	    ///< Specifies that subsystem is Apple OSX v. 10.3
#define CBS_SUBSYS_TIGER				50	    ///< Specifies that subsystem is Apple OSX v. 10.4
#define CBS_SUBSYS_LEOPARD				52	    ///< Specifies that subsystem is Apple OSX v. 10.5

// ---------------------------------------------
// --- Platform processor family definitions ---
// ---------------------------------------------

#define CBS_PROCESSOR_FAMILY_I86		2		///< Specifies that processor is an Intel x86 or compatible like eg AMD.
#define CBS_PROCESSOR_FAMILY_ARM		3		///< Specifies that processor is an ARM.
#define CBS_PROCESSOR_FAMILY_MIPS		4		///< Specifies that processor is a MIPS.
#define CBS_PROCESSOR_FAMILY_PPC		5		///< Specifies that processor is a Power PC (IBM / Motorola).
#define CBS_PROCESSOR_FAMILY_XSCALE	    6		///< Specifies that processor is an Intel XScale.

// ----------------------------------------------
// --- Platform processor subtype definitions ---
// ----------------------------------------------

// x86 processor sub types
#define CBS_PROCESSOR_SUBTYPE_I8086    2       ///< 
#define CBS_PROCESSOR_SUBTYPE_I80186   3       ///< 
#define CBS_PROCESSOR_SUBTYPE_I80286   4       ///< 
#define CBS_PROCESSOR_SUBTYPE_I80386   5       ///< 
#define CBS_PROCESSOR_SUBTYPE_I80486   6       ///< 
#define CBS_PROCESSOR_SUBTYPE_PENTIUM  7       ///< 
#define CBS_PROCESSOR_SUBTYPE_CELERON  8       ///< 
#define CBS_PROCESSOR_SUBTYPE_XEON     9       ///< 
#define CBS_PROCESSOR_SUBTYPE_COREDUO  10      ///< 

#define CBS_PROCESSOR_SUBTYPE_ATHLON   20       ///< 
// TODO: Add more processor subtypes

// ---------------------------------------
// --- Platform endianness definitions ---
// ---------------------------------------

#define CBS_ENDIANNESS_LITTLE		    2		///< Specifies that processor is little endian. Examples are ix86 and compatible.
#define CBS_ENDIANNESS_BIG				3		///< Specifies that processor is big endian. Examples are Motorola 68000 and Power PC.


#endif //_CBS_PLATFORM_DEFINITIONS_H_

