#include "es_utils.h"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <nlohmann/json.hpp>
#include <cpaf_libs/utils/cpaf_string_utils.hpp>
#include <cpaf_libs/unicode/cpaf_u8string_utils.h>


using namespace std;
using json = nlohmann::json;
namespace cu = cpaf::unicode;
namespace ba = boost::algorithm;


namespace cpaf::elasticsearch
{
// -----------------------------
// --- Convenience constants ---
// -----------------------------

// --------------------------------------
// --- Response data parser functions ---
// --------------------------------------
bool request_succeeded(const webcc::ResponsePtr& response)
{
    return request_succeeded(response.get());
}


bool request_succeeded(const webcc::Response* response)
{
    if (!response) return false;
    if (response->status() != webcc::kOK) return false;
    return request_succeeded(response->data());
}


bool request_succeeded(const std::string& response_data)
{
    return ba::contains(response_data, "\"failed\":0") ||
           ba::contains(response_data, "\"failed\" : 0") ;
}

bool response_has_hits(const string& response_data)
{
    return !response_empty_hits(response_data);
}

bool response_empty_hits(const string& response_data)
{
    return ba::contains(response_data, "\"hits\":[]") ||
           ba::contains(response_data, "\"hits\" : [ ]") ;
}

size_t response_hit_count(const string& response_data)
{
    auto count_str = cu::substring_between(response_data, R"("hits":{"total":)", R"(,)");
    if (count_str.empty()) {
        count_str = cu::substring_between(response_data, R"("hits" : {)", R"(,)");
        count_str = cu::substring_between(count_str, R"("total" :)", "");
    }
    return static_cast<size_t>(cpaf::to_int_default(count_str, 0));
}


/**
scroll_id_get Gets scrool_id from an elaticsearch response text.

@code
curl -XGET 'localhost:9200/cycling_business/_search/?scroll=3m&size=2&pretty=0' --data '{
    "query" : {
        "match_all" : {}
    }
}'

# Will get you a scroll_id similar to this:
{"_scroll_id":"DnF1ZXJ5VGhlbkZldGNoBQAA...", ... }
In this case the scroll_id returned will be the 'DnF1ZXJ5VGhlbkZldGNoBQAA...' part.
@endcode
@param response_data Response text from initial ElasticSearch scroll search
@return The scroll id,
 */
string response_scroll_id(const string& response_data)
{
    auto scroll_id = cu::substring_between(response_data, R"("_scroll_id":")", R"(")");
    if (scroll_id.empty()) {
        scroll_id = cu::substring_between(response_data, R"("_scroll_id" : ")", R"(")");
    }
    return scroll_id;
}

// -------------------------------
// --- Body composer functions ---
// -------------------------------

/** Create scroll-next body from scroll and scroll_id

Creates an ES body like this:
@code
{
    "scroll" : "3m",
    "scroll_id" : "DnF1ZXJ5VGhlb..."
}
@endcode
*/
string scroll_body(const string& scroll, const string& scroll_id, bool pretty)
{
    json jo;
    jo["scroll"] = scroll;
    jo["scroll_id"] = scroll_id;
    int indent = pretty ? 4 : -1;
    return jo.dump(indent);
}

// ----------------------
// --- Bulk functions ---
// ----------------------
void append_to_bulk_request(std::string& request,
                            const nlohmann::json& record,
                            const string& index,
                            const string& type,
                            const std::string& id)
{
    //    { "index" : { "_index" : "myindex", "_type": "mytype", "_id" : "myid" } }
    request.append("{ \"index\": { \"_index\" : \"");
    request.append(index);
    request.append("\", \"_type\": \"");
    request.append(type);
    if (!id.empty()) {
        request.append("\", \"_id\": \"");
        request.append(id);
    }
    request.append("\" } }\n");
    request.append(record.dump());
    request.append("\n");
}

/** Create bulk request string from a slice of json records. */
void bulk_request(  std::string& req_out,
                    const json_vec& records,
                    size_t& start_from,
                    size_t bulk_size,
                    const string& index,
                    const string& type,
                    size_t reserve_hint)
{
    req_out = "";
    const auto stop_at_index = std::min(records.size(), start_from + bulk_size);
    if (reserve_hint > 0) {
        req_out.reserve(reserve_hint);
    }
    for (; start_from < stop_at_index; ++start_from) {
        const nlohmann::json& rec = records[start_from];
        const std::string id = cpaf::json_value_str(rec, "id", "");
        append_to_bulk_request(req_out, rec, index, type, id);
    }
}




} //end namespace cpaf::elasticsearch

