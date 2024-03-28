#include "cpaf_http_convenience.h"

#include <iostream>


using namespace std::chrono;
      
namespace cpaf { namespace net {

// -------------------------
// --- "Private" helpers ---
// -------------------------
static size_t curl_write_file_data_helper(void *ptr, size_t size, size_t nmemb, FILE *stream ) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

static size_t curl_write_string_data_helper(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}



static CURLcode curl_do_http_download_file_impl(const std::string& sSrcUrl,
                                                const std::string& sDstFilePath,
                                                std::chrono::milliseconds timeout)
{
    CURLcode    res = CURLE_FAILED_INIT;
    CURL*       curl;
    FILE*       fp;

    curl = curl_easy_init();
    if (curl) {
        fp = fopen( sDstFilePath.c_str(), "wb" );
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, duration_cast<seconds>(timeout).count());
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



// ------------------
// --- curl class ---
// ------------------

curl::curl()
{
}

curl::~curl()
{
    cleanup();
}

bool curl::transfer_in_progress() const
{
    const auto progr = state();
    return progr == state_t::transferring || progr == state_t::cancel_requested;
}

bool curl::can_start_new() const
{
    const auto progr = state();
    return progr != state_t::transferring && progr != state_t::cancel_requested;
}

CURLcode curl::download_file(const std::string& url, const std::string& dst_file_path)
{
    if (dst_file_path.empty()) { return CURLE_READ_ERROR; }
    mode_ = mode_t::download;
    last_curlcode_ = CURLE_FAILED_INIT;

    if (!url.empty()) {
        init(url);
    }
    write_file_ = fopen(dst_file_path.c_str(), "wb");
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, curl_write_file_data_helper);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, write_file_);

    state_ = state_t::transferring;
    last_curlcode_ = curl_easy_perform(curl_);
    cleanup();
    return last_curlcode_;
}

CURLcode curl::upload_file(const std::string& url, const std::filesystem::path& src_file_path, const std::string& field_name)
{
    last_curlcode_ = CURLE_FAILED_INIT;
    if (src_file_path.empty()) { return CURLE_READ_ERROR; }
    mode_ = mode_t::upload;

    if (!url.empty()) {
        init(url);
    }

    struct curl_slist* headerlist = nullptr;
    struct curl_httppost* post = nullptr;
    struct curl_httppost* last = nullptr;
    static const char buf[] = "Expect:";

    headerlist 	= curl_slist_append(headerlist, buf);

    curl_formadd(&post, &last,
                 CURLFORM_COPYNAME, field_name.c_str(),
                 CURLFORM_FILE, src_file_path.string().c_str(),
                 CURLFORM_CONTENTTYPE, "multipart/form-data", //
                 CURLFORM_END);


    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl_, CURLOPT_HTTPPOST, post );

    state_ = state_t::transferring;
    last_curlcode_ = curl_easy_perform(curl_);

    cleanup();
    curl_formfree(post);
    curl_slist_free_all (headerlist);

    return last_curlcode_;
}

std::string curl::download_string(const std::string& url)
{
    std::string read_buffer;
    mode_ = mode_t::download;

    if (!url.empty()) {
        init(url);
    }
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, curl_write_string_data_helper);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &read_buffer);
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, curl_write_string_data_helper);

    state_ = state_t::transferring;
    last_curlcode_ = curl_easy_perform(curl_);
    cleanup();
    return read_buffer;
}

CURLcode curl::download_file()
{
    return download_file("", file_path_);
}

CURLcode curl::upload_file(const std::string& field_name)
{
    return upload_file("", file_path_, field_name);
}

std::string curl::download_string()
{
    return download_string("");
}

curl& curl::init_url(const std::string& url)
{
    init(url);
    return *this;
}

void curl::cancel_transfer()
{
    if (state_ == state_t::transferring) {
        state_ = state_t::cancel_requested;
    }
}

int64_t curl::total_size_bytes() const
{
    return mode_ == mode_t::download ? total_download_size_ : total_upload_size_;
}

int64_t curl::current_size_bytes() const
{
    return mode_ == mode_t::download ? cur_download_size_ : cur_upload_size_;
}

float curl::progress() const
{
    const auto total = total_size_bytes();
    if (total == 0) { return 0; }

    const auto cur = current_size_bytes();
    return static_cast<float>(cur) / static_cast<float>(total);
}


void curl::init(const std::string& url)
{
    cleanup();
    state_ = state_t::ready;
    curl_ = curl_easy_init();
    set_default_options(url);
}

void curl::set_default_options(const std::string& url)
{
    if (curl_) {
        curl_easy_setopt(curl_, CURLOPT_TIMEOUT, duration_cast<seconds>(timeout_).count());
        curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L); // enable redirect following
        curl_easy_setopt(curl_, CURLOPT_URL, url.c_str() );


        curl_easy_setopt(curl_, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl_, CURLOPT_XFERINFODATA, this);
        curl_easy_setopt(curl_, CURLOPT_XFERINFOFUNCTION, curl::curl_progress_callback);
    }
}

void curl::cleanup()
{
    if (state_ == state_t::transferring) {
        if (last_curlcode_ == CURLE_OK) {
            state_ = state_t::ready;
        }
        else {
            state_ = state_t::error;
        }
    }
    if (curl_) {
        curl_easy_cleanup(curl_);
    }
    if (write_file_) {
        fclose(write_file_);
    }
    curl_ = nullptr;
    write_file_ = nullptr;
}

size_t curl::curl_progress_callback(void *clientp,
                                    curl_off_t dltotal,
                                    curl_off_t dlnow,
                                    curl_off_t ultotal,
                                    curl_off_t ulnow)
{
    auto this_curl = static_cast<curl*>(clientp);
    this_curl->total_download_size_ = dltotal;
    this_curl->cur_download_size_ = dlnow;
    this_curl->total_upload_size_ = ultotal;
    this_curl->cur_upload_size_ = ulnow;

    // Call user callback if any assigned!
    if (this_curl->progress_cb_) {
        bool cancel_requested = this_curl->progress_cb_(this_curl->progress());
        if (cancel_requested) {
            this_curl->state_ = state_t::cancel_requested;
        }
    }

    // Handle cancel transfer.
    if (this_curl->state_ == state_t::cancel_requested) {
        this_curl->state_ = state_t::cancelled;
        return 1;   // Note: Anything but 0 will cancel the current transfer!
    }

    return 0; // All is good - continue transfer
}

// ----------------------
// --- Free functions ---
// ----------------------

std::string curl_do_http_download_to_string_impl(const std::string sSrcUrl,
                                                std::chrono::milliseconds timeout)
{
    CURLcode    res = CURLE_FAILED_INIT;
    CURL*       curl;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, duration_cast<seconds>(timeout).count());
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
CURLcode curl_http_download_file (const std::string sSrcUrl         ///< [in] Full path to source URL
                                 , const std::string sDstFilePath    ///< [in] Full path to destination file
                                 , std::chrono::milliseconds timeout)
{
    return curl_do_http_download_file_impl( sSrcUrl, sDstFilePath, timeout);
}

/** Download file via HTTP protocol. Typically from a webserver.
 * File contents is returned as a string.
\todo Implement ftp download function using curl too. */
std::string curl_http_download_to_string (const std::string sSrcUrl         ///< [in] Full path to source URL
                                         , std::chrono::milliseconds timeout)
{
    return curl_do_http_download_to_string_impl(sSrcUrl, timeout);
}

/**
Upload a single file using HTTP POST, no pasword, user only the file.
\see http://curl.haxx.se/libcurl/c/multi-post.html
\see http://linux.die.net/man/3/libcurl-tutorial
\return Curl result code: http://curl.haxx.se/libcurl/c/libcurl-errors.html E.g: CURLE_OK, CURLE_UPLOAD_FAILED, CURLE_READ_ERROR
*/
CURLcode curl_post_upload_file_simple   (const std::string& sURL
                                      , const std::string& sFieldName
                                      , const std::filesystem::path& filePath
                                      , std::chrono::milliseconds timeout)
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
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, duration_cast<seconds>(timeout).count());

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
CURLcode curlPostUploadFileSimple(const std::string& sURL,
                                  const std::string& sFieldName,
                                  const std::filesystem::path& filePath,
                                  std::chrono::milliseconds timeout)
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
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, duration_cast<seconds>(timeout).count());
		
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
CURLcode curlPostUploadFileSimple( const std::string& sURL,
                                   const std::string& sFieldName,
                                   const std::filesystem::path& filePath )
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

