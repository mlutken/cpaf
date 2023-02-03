
/* Include this before anything else if you have problems like multiple defined winsock functions.  */
#if (defined _WINDOWS) || (defined WINDOWS) || (defined _WIN32) 
#	include <winsock2.h>
#	include <basetsd.h>
#	define HAVE_SSIZE_T
#endif
#include <wx/wx.h>

