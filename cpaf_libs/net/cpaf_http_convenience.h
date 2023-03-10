#ifndef CPAF_HTTP_CONVENIENCE_H
#define CPAF_HTTP_CONVENIENCE_H

 
#include <string>
#include <boost/filesystem/path.hpp>
#include <curl/curl.h>

//#include <iostream>         // NOTE: ML For debug only


namespace cpaf { namespace net {

/** \unit cpaf_http_convenience.h
! Functions for primitive http support. 
Functions include easy download of a http page to a std::string.
\author Martin Lutken 
*/


// -----------------------------
// --- Types and Definitions ---
// -----------------------------

// -----------------
// --- Functions ---
// -----------------

CURLcode curl_http_download_file        ( const std::string sSrcUrl, const std::string sDstFilePath );
std::string curl_http_download_to_string( const std::string sSrcUrl);
CURLcode curl_post_upload_file_simple   ( const std::string& sURL, const std::string& sFieldName, const boost::filesystem::path& filePath ) ;
CURLcode curlPostUploadFileSimple       ( const std::string& sURL, const std::string& sFieldName, const boost::filesystem::path& filePath ) ;


int	http_download_wget	( std::string const& sUrl, std::string& sData );
//int	http_download		( std::string const& sUrl, std::string& sData );

// -------------------------------------------------------------
// --- Functions that needs platform specific implementation ---
// -------------------------------------------------------------


}} //END namespace cpaf::net


#endif //CPAF_HTTP_CONVENIENCE_H


