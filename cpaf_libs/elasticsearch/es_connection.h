#ifndef CPAF_ELASTICSEARCH_ES_CONNECTION_H
#define CPAF_ELASTICSEARCH_ES_CONNECTION_H

#include <string>
#include <cpaf_libs/net/cpaf_webcc_client.h>
#include <cpaf_libs/utils/cpaf_json_utils.h>

namespace cpaf::elasticsearch
{

/**
Elastic search class for handling basic operations
Uses the webcc::ClientSession object to perform the requests.

@see https://github.com/sprinfall/webcc
*/
class es_connection {
public:

    es_connection();
    explicit es_connection(const std::string& srv_location);
    ~es_connection() = default;

    // --------------------------------
    // --- PUBLIC: Basic operations ---
    // --------------------------------
    webcc::ClientSession&       session ()          { return session_; }
    const webcc::ClientSession& session () const    { return session_; }
    webcc::RequestBuilder&      request ();

    webcc::ResponsePtr          send    (bool stream = false);

    webcc::RequestBuilder&      delb    (const std::string& path, const std::string& body);
    webcc::ResponsePtr          del     (const std::string& path, const std::string& body);

    webcc::RequestBuilder&      putb    (const std::string& path, const std::string& body);
    webcc::ResponsePtr          put     (const std::string& path, const std::string& body);

    webcc::RequestBuilder&      getb    (const std::string& path, std::size_t size, const std::string& body);
    webcc::ResponsePtr          get     (const std::string& path, std::size_t size, const std::string& body);

    webcc::RequestBuilder&      postb   (const std::string& path, std::size_t size, const std::string& body);
    webcc::ResponsePtr          post    (const std::string& path, std::size_t size, const std::string& body);

    // ---------------------------------
    // --- PUBLIC: Scroll operations ---
    // ---------------------------------
    webcc::RequestBuilder&      scroll_postb    (const std::string& path, std::size_t size, const std::string& scroll, const std::string& body);
    webcc::ResponsePtr          scroll_post     (const std::string& path, std::size_t size, const std::string& scroll, const std::string& body);
    webcc::ResponsePtr          scroll_post_next(const std::string& scroll = "", const std::string& scroll_id = "");

    webcc::ResponsePtr          scroll_delete   (const std::string& scroll_id = "");
    // ----------------------------------------
    // --- PUBLIC: Read/Write single record ---
    // ----------------------------------------
    bool                write_record    (const std::string& path, const std::string& record_json_data);
    std::string         read_record     (const std::string& path);

    // ---------------------------------
    // --- PUBLIC: Indices functions ---
    // ---------------------------------
    bool                delete_index        (const std::string& index);
    bool                delete_non_matching (const std::string& index, const std::string& field, int64_t value_to_keep);
    bool                index_exist         (const std::string& index);
    bool                create_index        (const std::string& index, const std::string& create_data);
    void                ensure_index_exists (const std::string& index, const std::string& create_data);

    // ---------------------------------------
    // --- PUBLIC: Setter/Getter functions ---
    // ---------------------------------------
    const std::string&  server_location () const;
    void                server_location (const std::string& srv_location);

    bool                pretty          () const { return pretty_; }
    void                pretty          (bool return_pretty_json) { pretty_ = return_pretty_json; }
    const std::string&  scroll          () const { return scroll_; }
    void                scroll          (const std::string& scroll_size) { scroll_ = scroll_size; }
    const std::string&  scroll_id       () const { return scroll_id_; }

    // ------------------------------------------------
    // --- PUBLIC: Bulk write convenience functions ---
    // ------------------------------------------------

    bool bulk_write(const cpaf::json_vec& records,
                                         const std::string& index,
                                         const std::string& type,
                                         size_t bulk_size,
                                         size_t reserve_hint = 10000);

private:
    // ---------------------------------
    // --- PRIVATE: Helper functions ---
    // ---------------------------------
    void                    update_builder  (webcc::RequestBuilder& request, std::size_t size) const;
    const std::string       es_path         (const std::string& path) const;
    webcc::ClientSession                    session_;
    std::unique_ptr<webcc::RequestBuilder>  request_;
    std::string                             server_location_;   // For example "http://localhost:9200
    std::string                             path_;              // For example "http://localhost:9200
    std::string                             scroll_;            // Set to for example '3m'
    std::string                             scroll_id_;         // Will keep the scroll_id in case of scroll search active
    bool                                    pretty_ = false;    // For example "http://localhost:9200
};

} //end namespace cpaf::elasticsearch


#endif //CPAF_ELASTICSEARCH_ES_CONNECTION_H
