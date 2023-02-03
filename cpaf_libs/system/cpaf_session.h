#ifndef CPAF_SESSION_H
#define CPAF_SESSION_H

 
#include <string>
#include <boost/cstdint.hpp>
//#include <iostream>         // NOTE: ML For debug only
//#include <cbs.h>


namespace cpaf { namespace system {

/** \unit session.h
! Functions for session management and related to current session.
\author Martin Lutken 
*/


// -----------------------------
// --- Types and Definitions ---
// -----------------------------


// -------------------------------------------------------------
// --- Functions that needs platform specific implementation ---
// -------------------------------------------------------------

/**
Get the current user name.
\return True if shutdown is posssible */
std::string username ();


/** 
Shuts down the system. 
\return True if shutdown is posssible */
bool shutdown ( bool force					///< If true, shutdown is forced. Open files will not be saved.
				);

/** 
Reboots the system. 
\return True if reboot is posssible */
bool reboot ( bool force					///< If true, shutdown is forced. Open files will not be saved.
			);


/** 
Get time since last user input. Typically the time since user last moved/clicked 
the mouse or types at the keyboard.
\return Time in miliseconds since last user activity */
boost::uint32_t last_input_time ();


/** 
Retrieves the number of milliseconds that have elapsed since the system was started. 
Counter wraps a appr. 49.7 days, so use get_tick_count64 if this is a problem.
\return Number of milliseconds that have elapsed since the system was started. */
boost::uint32_t get_tick_count32 ();

/** 
Retrieves the number of milliseconds that have elapsed since the system was started. 
\sa get_tick_count32
\return Number of milliseconds that have elapsed since the system was started. */
boost::uint64_t get_tick_count64 ();

void sleep_ms (uint32_t milli_seconds);


std::string
get_env( std::string const& sEnvName );

void 
set_env( std::string const& sEnvName, std::string const& sEnvValue );


/** Returns the ID of the current process. I.e. the system process ID of the
current (this) executable. */
int getProcessID();

/** Returns the ID of the named process. */
int getProcessID( const std::string& sProcessName );


}} //END namespace cpaf::system


#endif //CPAF_SESSION_H


