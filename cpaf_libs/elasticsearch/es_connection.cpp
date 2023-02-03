#include "es_connection.h"
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <elasticsearch/es_utils.h>

using namespace std;
using json = nlohmann::json;


namespace cpaf::elasticsearch
{


// --------------------------------
// --- PUBLIC: Basic operations ---
// --------------------------------

es_connection::es_connection()
    : server_location_("http://localhost:9200")
{
    session().SetHeader("Content-Type", "application/json");
    session().set_ssl_verify(false);
}

es_connection::es_connection(const string& srv_location)
    : server_location_(srv_location)
{
}

webcc::RequestBuilder& es_connection::request()
{
    if (!request_) { request_.reset(new webcc::RequestBuilder{}); }
    return *request_;
}

webcc::ResponsePtr es_connection::send(bool stream)
{
    return session().Send(request()(), stream);
}

webcc::RequestBuilder& es_connection::delb(const string& path, const string& body)
{
    request_.reset(new webcc::RequestBuilder{});
    webcc::RequestBuilder& req_builder = request();
    const auto p = es_path(path);
    req_builder.Delete(p);
    update_builder(req_builder, 0);
    if (!body.empty()) {
        req_builder.Body(body);
    }
    return req_builder;
}

webcc::ResponsePtr es_connection::del(const string& path, const string& body)
{
    webcc::RequestBuilder& req = delb(path, body);
    return session().Send((req)());
}

webcc::RequestBuilder& es_connection::putb(const string& path, const string& body)
{
    request_.reset(new webcc::RequestBuilder{});
    webcc::RequestBuilder& req_builder = request();
    req_builder.Put(es_path(path));
    update_builder(req_builder, 0);
    req_builder.Body(body);
    return req_builder;
}

webcc::ResponsePtr es_connection::put(const string& path, const string& body)
{
    webcc::RequestBuilder& req = putb(path, body);
    return session().Send((req)());
}

webcc::RequestBuilder& es_connection::getb(const std::string& path, std::size_t size, const std::string& body)
{
    request_.reset(new webcc::RequestBuilder{});
    webcc::RequestBuilder& req_builder = request();
    req_builder.Get(es_path(path));
    update_builder(req_builder, size);
    req_builder.Body(body);
    return req_builder;
}

webcc::ResponsePtr es_connection::get(const string& path, size_t size, const string& body)
{
    webcc::RequestBuilder& req = getb(path, size, body);
    return session().Send((req)());
}

webcc::RequestBuilder& es_connection::postb(const string& path, size_t size, const string& body)
{
    request_.reset(new webcc::RequestBuilder{});
    webcc::RequestBuilder& req_builder = request();
    req_builder.Post(es_path(path));
    update_builder(req_builder, size);
    req_builder.Body(body);
    return req_builder;
}

webcc::ResponsePtr es_connection::post(const string& path, size_t size, const string& body)
{
    webcc::RequestBuilder& req = postb(path, size, body);
    return session().Send((req)());
}

// ---------------------------------
// --- PUBLIC: Scroll operations ---
// ---------------------------------

webcc::RequestBuilder& es_connection::scroll_postb(
        const string& path,
        size_t size,
        const string& scroll,
        const string& body)
{
    auto& req = postb(path, size, body);
    req.Query("scroll", scroll);
    scroll_ = scroll;
    return req;
}

webcc::ResponsePtr es_connection::scroll_post(
        const string& path,
        size_t size,
        const string& scroll,
        const string& body)
{
    webcc::RequestBuilder& req = scroll_postb(path, size, scroll, body);
    auto response = session().Send((req)());
    scroll_id_ = response_scroll_id(response->data());
    return response;
}

webcc::ResponsePtr es_connection::scroll_post_next(const string& scroll, const string& scroll_id)
{
    const auto scrl = scroll.empty() ? scroll_ : scroll;
    const auto scrl_id = scroll_id.empty() ? scroll_id_ : scroll_id;
    const auto body = scroll_body(scrl, scrl_id, false);
    auto response = post("/_search/scroll", 0, body);
    scroll_id_ = response_scroll_id(response->data());
    return response;
}

webcc::ResponsePtr es_connection::scroll_delete(const string& scroll_id)
{
    const auto scrl_id = scroll_id.empty() ? scroll_id_ : scroll_id;
    const std::string path = "/_search/scroll/" + scrl_id;
    return del(path, "");
}

// ----------------------------------------
// --- PUBLIC: Read/Write single record ---
// ----------------------------------------

bool es_connection::write_record(const string& path, const string& record_json_data)
{
    bool ok = true;
    try {
        webcc::ResponsePtr r = put(path, record_json_data);
        ok = request_succeeded(r->data());
    }  catch (...) {
        ok = false;
    }
    return ok;
}

string es_connection::read_record(const string& path)
{
    return get(path, 0, "")->data();
}

// ---------------------------------
// --- PUBLIC: Indices functions ---
// ---------------------------------
bool es_connection::delete_index(const string& index)
{
    const auto path = "/"s + index;
    return del(path, "")->status() == 200;
}

bool es_connection::delete_non_matching(const string& index, const string& field, int64_t value_to_keep)
{
    const auto path = "/" + index + "*/_delete_by_query?pretty=1&conflicts=proceed";

    const json body = {
        { "query",
            {{ "bool",
               {{ "must_not",
                  {
                      {{ "term",
                         {{field, value_to_keep}}
                      }}
                  }
               }}
            }}
        }
    };
    webcc::ResponsePtr response_ptr = post(path, 0, body.dump());
    return request_succeeded(response_ptr);
}

bool es_connection::index_exist(const string& index)
{
    const auto path = es_path("/"s + index);
    auto req = webcc::RequestBuilder{};
    req.Head(path);
    webcc::ResponsePtr r = session_.Send((req)());
    return r->status() == 200;
}

bool es_connection::create_index(const string& index, const string& create_data)
{
    const auto path = "/"s + index;
    return put(path, create_data)->status() == 200;
}

void es_connection::ensure_index_exists(const string& index, const string& create_data)
{
    if (!index_exist(index)) {
        create_index(index, create_data);
    }
}

// ---------------------------------------
// --- PUBLIC: Setter/Getter functions ---
// ---------------------------------------

const std::string& es_connection::server_location() const
{
    return server_location_;
}

void es_connection::server_location(const std::string& srv_location)
{
    server_location_ = srv_location;
}

// ------------------------------------------------
// --- PUBLIC: Bulk write convenience functions ---
// ------------------------------------------------

/*
void bulk_request (
        std::string& req_out,
        const json_vec& records,
        size_t& start_from,
        size_t bulk_size,
        const std::string& index,
        const std::string& type,
        size_t reserve_hint = 10000);

*/

bool es_connection::bulk_write(
        const json_vec& records,
        const std::string& index,
        const std::string& type,
        size_t bulk_size,
        size_t reserve_hint)
{

    size_t start_from = 0;
    const size_t records_count = records.size();

    std::string bulk_data;
    bool ok = true;
    while (start_from < records_count) {
        bulk_request (bulk_data, records, start_from, bulk_size, index, type, reserve_hint);
        post("/_bulk", 0, bulk_data);
    }
    return ok;
}

// ---------------------------------
// --- PRIVATE: Helper functions ---
// ---------------------------------

void es_connection::update_builder(webcc::RequestBuilder& request, std::size_t size) const
{
    request.Header("Accept", "application/json");
    request.Header("content-type","application/json; charset=UTF-8");
    if (pretty_) {
        request.Query("pretty", "1");
    }
    if (size > 0) {
        request.Query("size", std::to_string(size));
    }
}

const string es_connection::es_path(const string& path) const
{
    if (!path.empty()) {
        return server_location() + path;
    }
    return server_location() + path_;
}


/*
-----------------------
--- Getting started ---
-----------------------
https://www.elastic.co/guide/en/elasticsearch/reference/5.6/search-request-scroll.html
https://www.elastic.co/guide/en/elasticsearch/reference/7.9/paginate-search-results.html#scroll-search-results
curl -X PUT "localhost:9200/customer/external/1?pretty" -H 'Content-Type: application/json' -d'
{
  "name": "John Doe"
}
'
curl -XGET 'localhost:9200/customer/external/1?pretty=1'

curl -XPUT 'localhost:9200/customer?pretty=1'
curl -XDELETE 'localhost:9200/customer/external/1?pretty=1'


curl -X POST "localhost:9200/customer/external/1/_update?pretty&pretty" -H 'Content-Type: application/json' -d'
{
  "doc": { "name": "Jane Doe" }
}
'

curl -XGET 'localhost:9200/cycling_business/_search?pretty' --data '{
    "query" : {
        "term" : { "shop_id" : "fribikeshop.dk" }
    }
}'

curl -XGET 'localhost:9200/cycling_business/_search?pretty' --data '{
    "query" : {
        "match_all" : {}
    }
}'

curl -XGET 'localhost:9200/cycling_business/_search/?scroll=3m&size=2&pretty=1' --data '{
    "query" : {
        "match_all" : {}
    }
}'

curl -XGET 'localhost:9200/cycling_localized__da/staticpage/_search/?scroll=3m&size=2&pretty=1' --data '{
    "query" : {
        "match_all" : {}
    }
}'

curl -X POST "localhost:9200/_search/scroll?pretty=1" -H 'Content-Type: application/json' -d'
{
    "scroll" : "3m",
    "scroll_id" : "DnF1ZXJ5VGhlbkZldGNoCgAAAAAAAACNFjE5eWdLQTVpUVdhaVQ2bmt5cF9mTGcAAAAAAAAAjhYxOXlnS0E1aVFXYWlUNm5reXBfZkxnAAAAAAAAAI8WMTl5Z0tBNWlRV2FpVDZua3lwX2ZMZwAAAAAAAACQFjE5eWdLQTVpUVdhaVQ2bmt5cF9mTGcAAAAAAAAAkRYxOXlnS0E1aVFXYWlUNm5reXBfZkxnAAAAAAAAAJIWMTl5Z0tBNWlRV2FpVDZua3lwX2ZMZwAAAAAAAACTFjE5eWdLQTVpUVdhaVQ2bmt5cF9mTGcAAAAAAAAAlBYxOXlnS0E1aVFXYWlUNm5reXBfZkxnAAAAAAAAAJUWMTl5Z0tBNWlRV2FpVDZua3lwX2ZMZwAAAAAAAACWFjE5eWdLQTVpUVdhaVQ2bmt5cF9mTGc="
}
'

curl -X DELETE "localhost:9200/_search/scroll/DXF1ZXJ5QW5kRmV0Y2gBAAAAAAAAAD4WYm9laVYtZndUQlNsdDcwakFMNjU1QQ==,DnF1ZXJ5VGhlbkZldGNoBQAAAAAAAAABFmtSWWRRWUJrU2o2ZExpSGJCVmQxYUEAAAAAAAAAAxZrUllkUVlCa1NqNmRMaUhiQlZkMWFBAAAAAAAAAAIWa1JZZFFZQmtTajZkTGlIYkJWZDFhQQAAAAAAAAAFFmtSWWRRWUJrU2o2ZExpSGJCVmQxYUEAAAAAAAAABBZrUllkUVlCa1NqNmRMaUhiQlZkMWFB?pretty"


*/
} //end namespace cpaf::elasticsearch

