
#include <iostream> // Debug only 
#include <string>
#include <set>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/format.hpp>

//#include <cpaf_file_directory_functions.h>

#include "cpaf_url_string_functions.h"
#include "cpaf_url.h"


namespace cpaf { namespace net {
 
// ------------------------------------
// --- Static member initialization ---
// ------------------------------------

// ------------------------------------
// --- Constructor / Init Functions ---
// ------------------------------------

url::url(
	const std::string& sUrl			///< 
) 
{
	assign ( sUrl );
//	m_Path = cpaf::filesystem::remove_redundant_separators ( m_Path );
};

url::url(
	const std::string& sUrl,			///< 
	bool bIsFile						///<
) 
{
	assign ( sUrl );
	m_bIsFile =  bIsFile;
};



// ---------------------
// --- Set Functions ---
// ---------------------

/** Set protocol */
void url::protocol ( const std::string& sProtocol )
{
	if ( is_valid_protocol( sProtocol ) )	m_sProtocol = sProtocol;
	else	{
		if ( sProtocol != "" ) std::cout << "ERROR : url::protocol INVALID protocol:" << sProtocol << std::endl;
		m_sProtocol = "";
	}
}

/** Set domain/site */
void url::domain ( const std::string& sDomain)
{
	split_domain( sDomain, m_Domain);
}

/** Set parameters */
void url::parameters ( const std::string& sParameters )
{
	split_parameters( sParameters, m_Parameters );
}

// ---------------------
// --- Get Functions ---
// ---------------------

/** Get URL as string. */
std::string	url::string() const 
{ 
	std::string s;
	if ( has_protocol()	)	s.append( protocol() + "://"		);
	if ( has_domain()	)	s.append( domain_as_string()		);
	if ( has_path()		)	s.append( path().string()			);
	if ( has_parameters())	s.append( parameters_as_string()	);
	return s;
};


/// Get domain as a string
std::string	url::domain_as_string() const 
{ 
	std::string sDomain;
	for ( string_vec_t::const_reverse_iterator it = m_Domain.rbegin(); it != m_Domain.rend(); ++it ) {
		sDomain.append(*it);
		if ( it < m_Domain.rend() -1) sDomain.append(1, '.');
	}
	return sDomain; 
}

/// Get paramters as a string
std::string	url::parameters_as_string() const 
{ 
	std::string sParameters(1, '?');
	for ( parameter_vec_t::const_iterator it = m_Parameters.begin(); it != m_Parameters.end(); ++it ) {
		sParameters.append(it->string());
		if ( it < m_Parameters.end() -1) sParameters.append(1, '&');
	}
	return sParameters; 
}


std::string	url::top_domain() const 
{
    if ( !m_Domain.empty() )	return m_Domain.front();
	else						return ""; 
}

std::string	url::second_level_domain() const 
{
	if ( m_Domain.size() > 1 )	return *(m_Domain.begin() +1);
	else						return ""; 
}

std::string	url::sub_domains() const 
{
	using namespace std;
	if ( m_Domain.size() >= 3 )	{
		std::string sDomain;
		for ( string::size_type i = m_Domain.size() -1; i >= 2; --i ) {
			sDomain.append( m_Domain[i] );
			if ( i > 2 ) sDomain.append(1, '.');
		}
		return sDomain; 
	}	
	else						return ""; 
} 

/** Site domain (name) like 'lego.com'. Sitename is the second + top level domains 
concatenated. */
std::string url::site_domain() const
{
	std::string s( top_domain() );
	if ( !second_level_domain().empty() )	{
		s = second_level_domain() + "." + s;
	}
	return s;
}


// ----------------------------------
// --- Convenience Info Functions ---
// ----------------------------------


bool url::is_java_script() const
{ 
	bool bIsJavaScript = false;
	std::string s = string();
	if ( s.find( ".jsp" )	!= std::string::npos )	bIsJavaScript = true;
	if ( s.find( ".js" )	!= std::string::npos )	bIsJavaScript = true;
	return bIsJavaScript;
}

bool url::is_java_script_function_call() const
{
	std::string s = string();
	return s.find( ')' ) != std::string::npos;			
}


/** Link is a link to a named section. 
Html href looking like this will cause thjis function to return true:
\code
<a href="http://www.w3schools.com/html_links.asp#tips">
\endcode
*/
bool url::is_section_link() const
{
	std::string s = string();
	return s.find( '#' ) != std::string::npos;			
} 


/** Link is a "local" link to a named section. 
Html href looking like this will cause this function to return true:
\code
<a href="#tips">Jump to the Useful Tips Section</a>
\endcode
*/
bool url::is_local_section_link() const
{
	std::string s = string();
	return s[0] == '#' ;			
} 

/** */
bool url::is_mail_address()	const
{
	return boost::contains( string(), "mailto" );
}


// ----------------------
// --- Misc Functions ---
// ----------------------

/** */
boost::filesystem::path	url::save_path()		const
{
	namespace fs = boost::filesystem;
	fs::path p;				// The full save path that we return from this function. 
	if ( has_top_domain() 			)	p /= std::string("_TOPD_") + top_domain();
	if ( has_second_level_domain() 	)	p /= std::string("_SITE_") + second_level_domain();
	if ( has_sub_domains() 			)	p /= std::string("_SUBD_") + sub_domains();
	if ( has_protocol() 			)	p /= std::string("_PROT_") + protocol();
	if ( has_path() 				)	p /= path();
	if ( has_parameters()		)	{
		p /= std::string("_PARA_");
		for ( parameter_vec_t::const_iterator it = m_Parameters.begin(); it != m_Parameters.end(); ++it ) {
			p /= std::string("_EQUA_") + it->sName + "=" + it->sValue;
		}
	}
	if ( isFile() && !p.empty() ) {
		p = p.branch_path() / (p.filename().string() + "_FILE_");
	}
	return p;
	
	// TODO: Why the H... does this not work with gcc ? GCC says: "crawl::url::url::parameters’ does not have class type"
	//parameter_vec_t::const_iterator itBegin = parameters.begin();
	//parameter_vec_t::const_iterator itEnd   = parameters.end();
	// This works !
	//const crawl::url::parameter_vec_t& param = static_cast<const parameter_vec_t>(parameters());
}


url& url::operator=	(
	const url& urlIn
)
{
	m_sProtocol		= urlIn.m_sProtocol;
	m_Domain		= urlIn.m_Domain;
	m_Path			= urlIn.m_Path;
	m_Parameters	= urlIn.m_Parameters;
	m_bIsFile		= urlIn.m_bIsFile;		
	return *this;
}


// -------------------------
// --- Private Functions ---
// -------------------------

/** */
void url::assign ( 
	const std::string& sUrl			///< [in]
)
{
	using namespace std;
    using boost::format;
	namespace fs = boost::filesystem;
	
	if ( sUrl.empty() ) return;
	
	bool bMoreToParse;
	std::string sDomain;
	std::string sParameters;
	 
	string::size_type iCurPos = 0;
	m_sProtocol		= parse_protocol(sUrl, iCurPos);
	bMoreToParse	= parse_advance( sUrl, m_sProtocol, iCurPos );
	
	if (bMoreToParse) sDomain	= parse_domain(sUrl, iCurPos);
	bMoreToParse	= parse_advance( sUrl, sDomain, iCurPos );

	if (bMoreToParse) m_Path = fs::path(parse_path(sUrl, iCurPos));
	bMoreToParse	= parse_advance( sUrl, m_Path.string(), iCurPos );

	if (bMoreToParse) sParameters	= parse_parameters(sUrl, iCurPos);
	
	// Split domain and parameters into parts
	split_domain( sDomain, m_Domain);
	split_parameters( sParameters, m_Parameters );
	if ( m_Path.leaf() == "/" || m_Path.leaf() == "." || m_Path.leaf() == ".." )	m_bIsFile = false;
	else																			m_bIsFile = true;
}

// ----------------------
// --- Friend Functions ---
// ----------------------


/** Equal comparision.
\return True if URLs are equal. */
bool operator==	(const url& lhs, const url& rhs)
{
	return	( lhs.m_sProtocol	== rhs.m_sProtocol	) &&
			( lhs.m_Domain		== rhs.m_Domain		) &&
			( lhs.m_Path		== rhs.m_Path		) &&
			( lhs.m_Parameters	== rhs.m_Parameters	) &&
			( lhs.m_bIsFile		== rhs.m_bIsFile	);		
}

// ----------------------
// --- Free Functions ---
// ----------------------

/** '<<' operator, Write Url to stream. 
	\return Reference to (modified) ostream. */
std::ostream&	operator <<(std::ostream& os,		///< [out] Output stream to write to.
							const url& urlOut)			///< [in] Url to write.
{
	os << urlOut.string();
	return os;
}


}} //END namespace cpaf::net

