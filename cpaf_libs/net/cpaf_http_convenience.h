#pragma once
 
#include <string>
#include <filesystem>
#include <chrono>
#include <curl/curl.h>


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

CURLcode curl_http_download_file        ( const std::string sSrcUrl, const std::string sDstFilePath,
                                          std::chrono::milliseconds timeout = std::chrono::minutes(2) );
std::string curl_http_download_to_string( const std::string sSrcUrl,
                                         std::chrono::milliseconds timeout = std::chrono::minutes(2));
CURLcode curl_post_upload_file_simple   ( const std::string& sURL,
                                          const std::string& sFieldName,
                                          const std::filesystem::path& filePath,
                                          std::chrono::milliseconds timeout = std::chrono::minutes(2)) ;
CURLcode curlPostUploadFileSimple       ( const std::string& sURL,
                                          const std::string& sFieldName,
                                          const std::filesystem::path& filePath,
                                          std::chrono::milliseconds timeout = std::chrono::minutes(2)) ;


int	http_download_wget	( std::string const& sUrl, std::string& sData );
//int	http_download		( std::string const& sUrl, std::string& sData );

// -------------------------------------------------------------
// --- Functions that needs platform specific implementation ---
// -------------------------------------------------------------


}} //END namespace cpaf::net



