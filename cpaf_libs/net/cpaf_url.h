#ifndef CPAF_URL_H
#define CPAF_URL_H

#include <vector>
#include <filesystem>

#include "cpaf_url_string_functions.h"

namespace cpaf { namespace net {

/** 
XXX Brief description XXX.
XXX Detailed description here ... XXX
\author Martin Lütken */
class url
{
public:

    // ----------------
    // --- Typedefs ---
    // ----------------
	

    // ------------------------------------
    // --- Constructor / Init Functions ---
    // ------------------------------------
	url				() {};
	url				( const std::string& sUrl );
	url				( const std::string& sUrl, bool bIsFile );

    // ----------------------------
    // --- Inline Get Functions ---
    // ----------------------------
	
	/// Get protocol
	std::string				protocol()	const	{ return m_sProtocol;	}

	/// Get path
	std::filesystem::path	path()		const	{ return m_Path;		}

	/// Get domain parts vector
	string_vec_t&			domain()			{ return m_Domain;		}
	const string_vec_t&		domain()	const	{ return m_Domain;		}

	/// Get parameters
	parameter_vec_t&		parameters() 		{ return m_Parameters;	}
	const parameter_vec_t&	parameters() const 	{ return m_Parameters;	}

	/// Is this URL representing something that is/can-besaved-to a file? As opposed to a directory. 
	bool					isFile()	 const	{ return m_bIsFile; 	}

    // ----------------------------
    // --- Inline Set Functions ---
    // ----------------------------
	
	/// Set path part of Url.
	void					path		( const std::filesystem::path& path )		///< [in] Path to assign.
		{ m_Path = path; }
	/// Set file/directory property. 
	void					bIsFile		( bool bIsFile )							///< [in] Set to true if this URL represents a file. Set to true if it's a directory.	 
		{ m_bIsFile = bIsFile; 	}
    // ---------------------
    // --- Set Functions ---
    // ---------------------

	void					protocol	( const std::string& sProtocol		);
	void					domain		( const std::string& sSite			);
	void					domain		( const string_vec_t& site			)
		{ m_Domain = site; }
	void					parameters	( const std::string& sParameters	);

    // ---------------------
    // --- Get Functions ---
    // ---------------------
	std::string				string()				const; 
	std::string				domain_as_string()		const; 
	std::string				parameters_as_string()	const; 
	std::string				top_domain()			const; 
	std::string				second_level_domain()	const; 
	std::string				sub_domains()			const; 
	std::string 			site_domain() 			const;


    // ----------------------------------
    // --- Convenience Info Functions ---
    // ----------------------------------
	bool					has_protocol()					const	{ return !protocol().empty(); 				}
	bool					has_path()						const	{ return !path().empty(); 					}
	bool					has_domain()					const	{ return !domain().empty(); 				}
	bool					has_parameters()				const	{ return !parameters().empty(); 			} 
	bool					has_top_domain()				const	{ return !top_domain().empty();				}  
	bool					has_second_level_domain()		const	{ return !second_level_domain().empty();	} 
	bool					has_sub_domains()				const	{ return !sub_domains().empty();			} 
	bool					is_java_script()				const; 
	bool					is_java_script_function_call()	const; 
	bool					is_section_link()				const;
	bool					is_local_section_link()			const;
	bool					is_mail_address()				const;
	bool					empty()							const	{ return !(has_protocol() || has_path() || has_domain() || has_parameters()); }
	

    // ----------------------
    // --- Misc Functions ---
    // ----------------------
	std::filesystem::path	save_path()			const;
	
	url&					operator=	(const url& Url);
	
    // ----------------------
    // --- Friend Functions ---
    // ----------------------
	friend bool				operator==	(const url& lhs, const url& rhs);

protected:

private:
    // -------------------------
    // --- Private Functions ---
    // -------------------------

	void	assign	( const std::string& sUrl );

    // -------------------
    // --- Member Data ---
    // -------------------
	std::string				m_sProtocol;
	string_vec_t			m_Domain;
	std::filesystem::path	m_Path;
	parameter_vec_t			m_Parameters;
	bool					m_bIsFile;		///< True if this URL represents a file, as opposed to
};
// ----------------------------
// --- Convenience Typedefs ---
// ----------------------------

typedef std::vector<url>	url_vec_t;

// ----------------------
// --- Free Functions ---
// ----------------------


/// Not equal comparison. \return True if URLs are not eaqual.
inline bool				operator!=	(const url& lhs, const url& rhs)
{ return !(lhs==rhs);}


std::ostream&	operator <<( std::ostream& os, const url& urlOut );


//void copy_if ( ) 


}} //END namespace cpaf::net

#endif //CPAF_URL_H

