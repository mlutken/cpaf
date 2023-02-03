// session_windows.cpp:	Windows implementation of the session functions.
//					

/*
#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen
#include <afxdisp.h>        // MFC Automatisierungsklassen
#include <afxdtctl.h>		// MFC-Unterst�tzung f�r allgemeine Steuerelemente von Internet Explorer 4
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterst�tzung f�r g�ngige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT
*/


#include <iostream> // MLU_DEBUG: Only temporary this one!!!


#include "../cpaf_session.h"


#define _WIN32_WINNT 0x501
#include <windows.h>



namespace cpaf { namespace system {

// --------------------------------------------------
// --- Private platform specific helper functions ---
// --------------------------------------------------

std::string username()
{
    return "WINDOWS::username() TODO: IMPLEMENT ME!";
}


bool shutdown_reboot_helper( UINT iAction, bool bForce )
{

	// see http://msdn2.microsoft.com/en-us/library/aa376868.aspx
	// and http://www.devhood.com/tutorials/tutorial_details.aspx?tutorial_id=503
	DWORD	dwReason			= 0;
	if ( bForce ) iAction		= iAction | EWX_FORCE;
	
	bool bShutdownOk = true;
	if ( !ExitWindowsEx(iAction, dwReason) ) {
		HANDLE hToken; 
		TOKEN_PRIVILEGES tkp; 

		// Get a token for this process. 

		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
			std::cout<<"Shutdown/Rebbot error (1) Privilige change!"<<std::endl;
		}

		// Get the LUID for the shutdown privilege. 

		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 

		tkp.PrivilegeCount = 1;  // one privilege to set    
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

		// Get the shutdown privilege for this process. 

		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 

		// Cannot test the return value of AdjustTokenPrivileges. 

		if (GetLastError() != ERROR_SUCCESS) {
			std::cout<<"Shutdown/Rebbot error (2) Privilige change!"<<std::endl;
		}

		// Shut down the system and force all applications to close. 

		if (!ExitWindowsEx(iAction, dwReason)) {
			bShutdownOk = false;
			std::cout<<"Shutdown/Rebbot error (3). "<<std::endl; 
		}
	}
	return 	bShutdownOk;

}

// -------------------------------------------------------------
// --- Functions that needs platform specific implementation ---
// -------------------------------------------------------------

bool shutdown ( bool force )
{
	return shutdown_reboot_helper( EWX_SHUTDOWN, force );
}


bool reboot ( bool force )
{
	return shutdown_reboot_helper( EWX_REBOOT, force );
}

boost::uint32_t last_input_time () 
{
	LASTINPUTINFO lastInput;    
	lastInput.cbSize = sizeof(LASTINPUTINFO);
	
	BOOL success = GetLastInputInfo(&lastInput);
	
	if(!success)
	{
		DWORD err=GetLastError();
		// report error, throw exception, etc
	}
	
	DWORD lastInputTime = lastInput.dwTime;
	return lastInputTime;
}

boost::uint32_t get_tick_count32 ()
{
	return GetTickCount();
}

// http://msdn2.microsoft.com/en-us/library/ms724411.aspx
boost::uint64_t get_tick_count64 ()
{

#if(_WIN32_WINNT >= 0x0600)
	return GetTickCount64(); // Only on Vista
#else
	return static_cast<boost::uint64_t>(-1);
#endif
}


void sleep(unsigned int seconds)
{
	Sleep( seconds*1000 );
}

std::string
get_env( std::string const& sEnvName )
{
	const char* szEnvValue;
	std::string sEnvValue;
    szEnvValue = getenv( sEnvName.c_str() );
    if ( szEnvValue )	sEnvValue = szEnvValue;
    return sEnvValue;
}

void 
set_env( std::string const& sEnvName, std::string const& sEnvValue )
{
 //   setenv( sEnvName.c_str(), sEnvValue.c_str() );
}


int getProcessID()
{
    //printf("ERROR: getProcessID() needs to be implemented on this platform\n");
    return 0;
}

int getProcessID( const std::string& /*sProcessName */)
{
    return 0;
}


}} //END namespace cpaf::system


