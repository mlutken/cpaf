#pragma once
 
#include <string>
#include <filesystem>
#include <chrono>
#include <functional>
#include <atomic>
#include <curl/curl.h>


namespace cpaf { namespace net {

/** \unit cpaf_http_convenience.h
! Functions for primitive http support. 
Functions include easy download of a http page to a std::string.
\author Martin Lutken 
*/


// ------------------
// --- curl class ---
// ------------------

/**
 * Class to hanlde up/download using cURL library.
 * @see https://curl.se/libcurl/c/CURLOPT_XFERINFOFUNCTION.html
 *
 * Examples:
 * ---------
 * @code

    cpaf::net::curl mycurl;
    const std::string download_url = "https://my.domain.com/some_file.txt";
    const std::filesystem::path local_download_path = "https://my.domain.com/some_file.txt";

    auto cb = [](float progress) -> bool {
        std::cerr << "Progress " << progress * 100 << " %\n";
        return false; // NOTE: To cancel operation return true!
    };


    // 3 Different ways of downloading a file
    // Note: The CURLOPT_FOLLOWLOCATION option is by default set to 1L
    auto res = mycurl.timeout(timeout).download_file(download_url, local_download_path_);
    res = mycurl.timeout(timeout).init_url(download_url).file_path(local_download_path_).opt(CURLOPT_FOLLOWLOCATION, 0L).download_file();
    res = mycurl.timeout(timeout).init_url(download_url).file_path(local_download_path_).progress_callback(std::move(cb)).download_file();
 * @endcode
*/
class curl {
public:
    using progress_cb = std::function<bool (float)>;
    enum class state_t : uint8_t { ready, transferring, cancel_requested, cancelled, error };
    enum class mode_t : uint8_t { download, upload };

    curl();
    ~curl();

    const std::filesystem::path&    file_path           () const { return file_path_; }
    std::chrono::milliseconds       timeout             () const { return timeout_; }
    std::chrono::seconds            timeout_seconds     () const { return std::chrono::duration_cast<std::chrono::seconds>(timeout_); }
    state_t                         state               () const { return state_; }
    bool                            is_transferring     () const;
    bool                            can_start_new       () const;
    CURLcode                        last_curlcode       () const { return last_curlcode_; }

    CURLcode                        download_file       (const std::string& url, const std::string& dst_file_path);
    CURLcode                        upload_file         (const std::string& url, const std::filesystem::path& src_file_path, const std::string& field_name);
    std::string                     download_string     (const std::string& url);


    CURLcode                        download_file       ();
    CURLcode                        upload_file         (const std::string& field_name);
    std::string                     download_string     ();
    curl&                           init_url            (const std::string& url);
    curl&                           file_path           (std::filesystem::path p)       { file_path_ = std::move(p); return *this; }
    curl&                           timeout             (std::chrono::milliseconds to)  { timeout_ = to; return *this; }
    curl&                           progress_callback   (progress_cb&& cb)              { progress_cb_ = std::move(cb);  return *this; }
    curl&                           progress_callback   (const progress_cb& cb)         { progress_cb_ = cb;  return *this; }

    void                            cancel_transfer     ();

    int64_t                         total_size_bytes    () const;
    int64_t                         current_size_bytes  () const;
    float                           progress            () const;

    template<typename OPT, typename TYPE>
    curl&                           opt          (OPT opt, TYPE value)
    {
        curl_easy_setopt(curl_, opt, value);
        return *this;
    }

private:
    static size_t curl_progress_callback(void *clientp,
                                        curl_off_t dltotal,
                                        curl_off_t dlnow,
                                        curl_off_t ultotal,
                                        curl_off_t ulnow);

    void                        init                (const std::string& url);
    void                        set_default_options (const std::string& url);
    void                        cleanup             ();

    CURL*                       curl_               = nullptr;
    FILE*                       write_file_         = nullptr;
    std::filesystem::path       file_path_          {};
    std::chrono::milliseconds   timeout_            = std::chrono::seconds(60);
    progress_cb                 progress_cb_        {nullptr};
    std::atomic<state_t>        state_              = state_t::ready;
    mode_t                      mode_               = mode_t::download;
    CURLcode                    last_curlcode_      = CURLE_OK;
    int64_t                     total_download_size_= 0;
    int64_t                     total_upload_size_  = 0;
    int64_t                     cur_download_size_  = 0;
    int64_t                     cur_upload_size_    = 0;
};

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



