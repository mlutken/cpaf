// session_linux.cpp:	Linux implementation of the session functions.
//					


#include "../cpaf_session.h"

#include <unistd.h>
#include <sys/param.h>
#include <sys/user.h>
#include <pwd.h>

namespace cpaf { namespace system {

// --------------------------------------------------
// --- Private platform specific helper functions ---
// --------------------------------------------------


// -------------------------------------------------------------
// --- Functions that needs platform specific implementation ---
// -------------------------------------------------------------
std::string username()
{
    struct passwd *pw;
    uid_t uid;

    uid = geteuid ();
    pw = getpwuid (uid);
    if (pw) {
        return pw->pw_name;
    }
    return "";
}

bool shutdown ( bool /*force*/ )
{
    return true;
}


bool reboot ( bool /*force*/ )
{
    return false;
}

boost::uint32_t last_input_time () 
{
    return 0;
}

boost::uint32_t get_tick_count32 ()
{
    return 0;
}

// http://msdn2.microsoft.com/en-us/library/ms724411.aspx
boost::uint64_t get_tick_count64 ()
{
    return 0;
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
set_env( std::string const& /*sEnvName*/, std::string const& /*sEnvValue*/ )
{
}


int getProcessID()
{
    return getpid();
}

int getProcessID( const std::string& /*sProcessName*/ )
{

    // BSD version it seems ...
//    const char* csProcessName = sProcessName.c_str();
//    struct kinfo_proc *sProcesses = NULL, *sNewProcesses;
//    pid_t  iCurrentPid;
//    int    aiNames[4];
//    size_t iNamesLength;
//    int    i, iRetCode, iNumProcs;
//    size_t iSize;

//    iSize = 0;
//    aiNames[0] = CTL_KERN;
//    aiNames[1] = KERN_PROC;
//    aiNames[2] = KERN_PROC_ALL;
//    aiNames[3] = 0;
//    iNamesLength = 3;

//    iRetCode = sysctl(aiNames, iNamesLength, NULL, &iSize, NULL, 0);

//    /*
//     * Allocate memory and populate info in the  processes structure
//     */

//    do {
//        iSize += iSize / 10;
//        sNewProcesses = realloc(sProcesses, iSize);

//        if (sNewProcesses == 0) {
//            if (sProcesses)
//                free(sProcesses);
//            errx(1, "could not reallocate memory");
//        }
//        sProcesses = sNewProcesses;
//        iRetCode = sysctl(aiNames, iNamesLength, sProcesses, &iSize, NULL, 0);
//    } while (iRetCode == -1 && errno == ENOMEM);

//    iNumProcs = iSize / sizeof(struct kinfo_proc);
//    /*
//     * Search for the given process name and return its pid.
//     */

//    for (i = 0; i < iNumProcs; i++) {
//        iCurrentPid = sProcesses[i].kp_proc.p_pid;
//        if( strncmp(csProcessName, sProcesses[i].kp_proc.p_comm, MAXCOMLEN) == 0 ) {
//            free(sProcesses);
//            return iCurrentPid;
//        }
//    }

//    /*
//     * Clean up and return -1 because the given proc name was not found
//     */

//    free(sProcesses);
    return (-1);

}


}} //END namespace cpaf::system

