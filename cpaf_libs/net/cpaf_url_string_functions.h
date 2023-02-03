#ifndef CPAF_URL_STRING_FUNCTIONS_H
#define CPAF_URL_STRING_FUNCTIONS_H

#include <string>
#include <set>
#include <vector>

#include <boost/assign/list_of.hpp> 

namespace cpaf { namespace net {

/** unit Url_string_functions.h
Free functions for various string based URL manipulation. Many of these functions
are used internally by the Url class.
\author Martin Lutken */

// ----------------------
// --- Helper structs ---
// ----------------------
struct parameter_pair {
	std::string		sName;
	std::string		sValue;
	parameter_pair( const std::string& sNme, const std::string& sVal) : sName(sNme), sValue(sVal) {}
	std::string		string() const { return sName + "=" + sValue; }	
	bool			operator ==(const parameter_pair& rhs ) const { return	( sName	== rhs.sName ) && ( sValue	== rhs.sValue);	}

};

// ----------------
// --- Typedefs ---
// ----------------

typedef std::set<std::string>		string_set_t;				///< Typedef for use with sets of strings like G_validProtocols
typedef std::vector<std::string>	string_vec_t;				///< Typedef for use with vector of strings like when splitting a domain string like 'www.tv2.dk' into subparts
typedef std::vector<parameter_pair>	parameter_vec_t;			///< Typedef for use with vector of strings like when splitting a parameter string like '?login=ml&password=mypasswd' into subparts

// -----------------
// --- Constants ---
// -----------------

/** Set of strings with valid Url protocols, like http://, https://, etc.
\note Boost::assign is quite cool when you want to initialize containers with values 
	  easily, see http://www.boost.org/libs/assign/doc/index.html#list_of */
static const string_set_t			g_valid_protocols = boost::assign::list_of("http")("https")("ftp")("mailto");


/** Set of special two.component top domains like e.g. "co.uk" */
static const string_set_t			g_two_component_top_domains = boost::assign::list_of("co.uk");

// ----------------------
// --- Info Functions ---
// ----------------------
bool is_valid_protocol		( const std::string& sProtocol );


// -----------------------
// --- Parse Functions ---
// -----------------------


std::string parse_protocol	( const std::string& sUrl, std::string::size_type& iCurPos );
std::string parse_domain	( const std::string& sUrl, std::string::size_type& iCurPos );
std::string parse_path		( const std::string& sUrl, std::string::size_type& iCurPos );
std::string parse_parameters( const std::string& sUrl, std::string::size_type& iCurPos );

void split_domain			( const std::string& sDomain, string_vec_t& domainVec );
void split_parameters		( const std::string& sParameters, parameter_vec_t& paramVec );

bool parse_advance			( const std::string& sUrl, const std::string&	sLatestParsedPart, std::string::size_type& iCurPos );

}} //END namespace cpaf::net


#endif // CPAF_URL_STRING_FUNCTIONS_H

