#ifndef CPAF_ELASTICSEARCH_ES_UTILS_H
#define CPAF_ELASTICSEARCH_ES_UTILS_H

#include <string>
#include <vector>
#include <cpaf_libs/utils/cpaf_json_utils.h>
#include <cpaf_libs/net/cpaf_webcc_client.h>


namespace cpaf::elasticsearch
{
using json_vec = std::vector<nlohmann::json>;
// -----------------------------
// --- Convenience constants ---
// -----------------------------
inline static const std::string match_all_body = R"({"query":{"match_all":{}}})";

// --------------------------------------
// --- Response data parser functions ---
// --------------------------------------

bool            request_succeeded   (const webcc::ResponsePtr& response);
bool            request_succeeded   (const webcc::Response* response);
bool            request_succeeded   (const std::string& response_data);
bool            response_has_hits   (const std::string& response_data);
bool            response_empty_hits (const std::string& response_data);
size_t          response_hit_count  (const std::string& response_data);
std::string     response_scroll_id  (const std::string& response_data);


// -------------------------------
// --- Body composer functions ---
// -------------------------------
std::string     scroll_body         (const std::string& scroll, const std::string& scroll_id, bool pretty = false);

// ----------------------
// --- Bulk functions ---
// ----------------------
void append_to_bulk_request(
        std::string& request,
        const nlohmann::json& record,
        const std::string& index,
        const std::string& type,
        const std::string& id);

void bulk_request (
        std::string& req_out,
        const json_vec& records,
        size_t& start_from,
        size_t bulk_size,
        const std::string& index,
        const std::string& type,
        size_t reserve_hint = 10000);

} //end namespace cpaf::elasticsearch


#endif //CPAF_ELASTICSEARCH_ES_UTILS_H
