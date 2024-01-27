#include "cpaf_http_convenience.h"

////#include <boost/process.hpp>

      
namespace cpaf { namespace net {


// -------------------------
// --- "Private" helpers ---
// -------------------------
size_t curl_write_file_data_helper(void *ptr, size_t size, size_t nmemb, FILE *stream ) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

static CURLcode curl_do_http_download_file_impl(const std::string& sSrcUrl, const std::string& sDstFilePath )
{
    CURLcode    res = CURLE_FAILED_INIT;
    CURL*       curl;
    FILE*       fp;

    curl = curl_easy_init();
    if (curl) {
        fp = fopen( sDstFilePath.c_str(), "wb" );
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // enable redirect following
        curl_easy_setopt(curl, CURLOPT_URL, sSrcUrl.c_str() );
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_file_data_helper);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    return res;
}

static size_t curl_write_string_data_helper(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string curl_do_http_download_to_string_impl(const std::string sSrcUrl)
{
    CURLcode    res = CURLE_FAILED_INIT;
    CURL*       curl;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // enable redirect following
        curl_easy_setopt(curl, CURLOPT_URL, sSrcUrl.c_str() );
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    //    curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_string_data_helper);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    if (res != CURLE_OK) {
        return "";
    }
    return readBuffer;
}


// -----------------
// --- Functions ---
// -----------------

/** Download file via HTTP protocol. Typically from a webserver.
\todo Implement ftp download function using curl too. */
CURLcode curl_http_download_file (
          const std::string sSrcUrl         ///< [in] Full path to source URL
        , const std::string sDstFilePath    ///< [in] Full path to destination file
        )
{
    return curl_do_http_download_file_impl( sSrcUrl, sDstFilePath );
}

/** Download file via HTTP protocol. Typically from a webserver.
 * File contents is returned as a string.
\todo Implement ftp download function using curl too. */
std::string curl_http_download_to_string (
          const std::string sSrcUrl         ///< [in] Full path to source URL
        )
{
    return curl_do_http_download_to_string_impl(sSrcUrl);
}

/**
Upload a single file using HTTP POST, no pasword, user only the file.
\see http://curl.haxx.se/libcurl/c/multi-post.html
\see http://linux.die.net/man/3/libcurl-tutorial
\return Curl result code: http://curl.haxx.se/libcurl/c/libcurl-errors.html E.g: CURLE_OK, CURLE_UPLOAD_FAILED, CURLE_READ_ERROR
*/
CURLcode curl_post_upload_file_simple   (
          const std::string& sURL
        , const std::string& sFieldName
        , const std::filesystem::path& filePath
        )
{
    if ( sURL == "" ) return CURLE_READ_ERROR;
    using namespace std;

    CURL *curl;
    CURLcode res = CURLE_FAILED_INIT;


    struct curl_slist* headerlist=NULL;
    struct curl_httppost* post=NULL;
    struct curl_httppost* last=NULL;
    static const char buf[] = "Expect:";



    curl = curl_easy_init();
    if(curl) {
        headerlist 	= curl_slist_append(headerlist, buf);

        curl_formadd(&post, &last,
                    CURLFORM_COPYNAME, sFieldName.c_str(),
                    CURLFORM_FILE, filePath.string().c_str(),
                    CURLFORM_CONTENTTYPE, "multipart/form-data", //
                    CURLFORM_END);

        curl_easy_setopt(curl, CURLOPT_URL, sURL.c_str() );

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, post );

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_formfree(post);
        curl_slist_free_all (headerlist);
    }
    return res;
}

/**
Upload a single file using HTTP POST, no pasword, user only the file. 
\see http://curl.haxx.se/libcurl/c/multi-post.html
\see http://linux.die.net/man/3/libcurl-tutorial
\return Curl result code: http://curl.haxx.se/libcurl/c/libcurl-errors.html E.g: CURLE_OK, CURLE_UPLOAD_FAILED, CURLE_READ_ERROR 
*/
CURLcode curlPostUploadFileSimple( const std::string& sURL, const std::string& sFieldName, const std::filesystem::path& filePath ) 
{
    if ( sURL == "" ) return CURLE_READ_ERROR;
	using namespace std;
	
	CURL *curl;
    CURLcode res = CURLE_FAILED_INIT;
	

// 	struct curl_slist* headers=NULL;
	struct curl_slist* headerlist=NULL;
	struct curl_httppost* post=NULL;
	struct curl_httppost* last=NULL;
	static const char buf[] = "Expect:";

	
	
	curl = curl_easy_init();
	if(curl) {
// 		headers 	= curl_slist_append(headers, "Content-Type: multipart/form-data");
		headerlist 	= curl_slist_append(headerlist, buf);

		curl_formadd(&post, &last,
					CURLFORM_COPYNAME, sFieldName.c_str(),
					CURLFORM_FILE, filePath.string().c_str(), 
					CURLFORM_CONTENTTYPE, "multipart/form-data", // 
					CURLFORM_END);
		
// 		curl_easy_setopt ( curl, CURLOPT_VERBOSE, 1L );
		curl_easy_setopt(curl, CURLOPT_URL, sURL.c_str() );
		
 		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
  		curl_easy_setopt(curl, CURLOPT_HTTPPOST, post );
				
		res = curl_easy_perform(curl);
	
 		curl_easy_cleanup(curl);
		curl_formfree(post);
		curl_slist_free_all (headerlist);
// 		curl_slist_free_all (headers);
	}
	return res;
}








/** Download content of URL (page) to a string. */


}} //END namespace cpaf::net


#if 0
/**
Upload a single file using HTTP POST, no pasword, user only the file.
\see http://curl.haxx.se/libcurl/c/multi-post.html
\see http://linux.die.net/man/3/libcurl-tutorial
\return Curl result code: http://curl.haxx.se/libcurl/c/libcurl-errors.html E.g: CURLE_OK, CURLE_UPLOAD_FAILED, CURLE_READ_ERROR
*/
CURLcode curlPostUploadFileSimple( const std::string& sURL, const std::string& sFieldName, const std::filesystem::path& filePath )
{
    if ( sURL == "" ) return CURLE_READ_ERROR;
    using namespace std;

    CURL *curl;
    CURLcode res;


// 	struct curl_slist* headers=NULL;
    struct curl_slist* headerlist=NULL;
    struct curl_httppost* post=NULL;
    struct curl_httppost* last=NULL;
    static const char buf[] = "Expect:";



    curl = curl_easy_init();
    if(curl) {
// 		headers 	= curl_slist_append(headers, "Content-Type: multipart/form-data");
        headerlist 	= curl_slist_append(headerlist, buf);

        curl_formadd(&post, &last,
                    CURLFORM_COPYNAME, sFieldName.c_str(),
                    CURLFORM_FILE, filePath.string().c_str(),
                    CURLFORM_CONTENTTYPE, "multipart/form-data", //
                    CURLFORM_END);

// 		curl_easy_setopt ( curl, CURLOPT_VERBOSE, 1L );
        curl_easy_setopt(curl, CURLOPT_URL, sURL.c_str() );

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, post );

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_formfree(post);
        curl_slist_free_all (headerlist);
// 		curl_slist_free_all (headers);
    }
    return res;
}


int	http_download_wget	(
    std::string const & sUrl, 		///< [in] Url to download
    std::string& sData )			///< [out] String to place resulting/downloaded data ind
{
/*
    namespace bp = ::boost::process;
    bp::command_line cl("wget");
    cl.argument( std::string("-q"));
    cl.argument( std::string("-T"));
    cl.argument( std::string("10"));
    cl.argument( std::string("-t"));
    cl.argument( std::string("3"));
    cl.argument( std::string("-O"));
    cl.argument( std::string("-"));
    cl.argument( sUrl );

    bp::launcher l;
    l.set_stdout_behavior(bp::redirect_stream);
    l.set_merge_out_err(true);
    bp::child c = l.start(cl);

    bp::pistream& is = c.get_stdout();
    std::string line;
    while (std::getline(is, line)) {
        sData.append( line + "\n" );
    }
    bp::status s = c.wait();
    return s.exit_status();
*/
return 0;
}


#endif

