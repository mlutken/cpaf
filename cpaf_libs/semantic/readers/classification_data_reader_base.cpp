#include "classification_data_reader_base.h"

#include <iostream>
#include <utils/cpaf_string_utils.hpp>


namespace ba = boost::algorithm;

namespace cpaf::semantic
{

const classification_fragment_reader_base::weights_map
classification_fragment_reader_base::default_weights = {{"*", 5}};

classification_fragment_reader_base::classification_fragment_reader_base()
    : global_weights_per_source_field_(default_weights),
      language_code_("en")
{

}

classification_fragment_reader_base::classification_fragment_reader_base(const std::string& resource_path_or_data)
    : global_weights_per_source_field_(default_weights),
      resource_(resource_path_or_data)
{
}

classification_fragment_reader_base::classification_fragment_reader_base(const boost::filesystem::path& resource_path_or_data)
    : global_weights_per_source_field_(default_weights),
      resource_(resource_path_or_data.string())
{
}

const std::string& classification_fragment_reader_base::resource() const
{
    return resource_;
}

void classification_fragment_reader_base::resource(const std::string& resource_path_or_data)
{
    resource_ = resource_path_or_data;
    should_read_data_ = true;
}

void classification_fragment_reader_base::resource(const boost::filesystem::path& path)
{
    resource_ = path.string();
    should_read_data_ = true;
}

void classification_fragment_reader_base::read_data()
{
    if (should_read_data_) {
        do_read_data();
        should_read_data_ = false;
    }
}

void classification_fragment_reader_base::begin_iterating()
{
    read_data();
    do_begin_iterating();
}

// --------------------------------
// --- Reader utility functions ---
// --------------------------------
std::string parse_fragment_weights(std::map<std::string, int>& weights_map, const std::string& fragment)
{
    using namespace cpaf;
    const auto weights_start_pos = fragment.find('[');
    if (weights_start_pos == std::string::npos)     { return fragment; }
    const auto weights_end_pos = fragment.find(']');
    if (weights_end_pos == std::string::npos)       { return fragment; }
    const auto it_start = fragment.begin() + static_cast<int>(weights_start_pos) +1;
    const auto it_end = fragment.begin() + static_cast<int>(weights_end_pos);

    str_const_range range(it_start, it_end);
    const std::string main_str(range.begin(), range.end());
    auto field_weight_pairs_vec = cpaf::splitv_trim(main_str, ",");
    for(const auto& field_weight: field_weight_pairs_vec){
        auto vec = cpaf::splitv_trim(field_weight, ":");
        if (vec.size() == 2) {
            const std::string& source_field = vec[0];
            const int weight = std::stoi(vec[1]);
            weights_map[source_field] = weight;
        }
    }
    std::string rest;
    rest.append(fragment.begin(), it_start -1);
    rest.append(it_end+1, fragment.end());
    ba::trim(rest);
    return rest;
}

nlohmann::json fragment_to_json(const std::string& raw_fragment)
{
    std::map<std::string, int> fragment_weights;
    const std::string fragment = cpaf::semantic::parse_fragment_weights(fragment_weights, raw_fragment);
    if (fragment.empty()) return nlohmann::json::object();

    if (!fragment_weights.empty()) {
        nlohmann::json fragment_obj = nlohmann::json::object();
        fragment_obj[fragment] = nlohmann::json(fragment_weights);
        return fragment_obj;
    }

    return nlohmann::json(fragment);
}


std::string parse_fragment_weights_iter_not_working(std::map<std::string, int>& weights_map, const std::string& fragment)
{
    using namespace cpaf;
    const auto weights_start_pos = fragment.find('[');
    if (weights_start_pos == std::string::npos)     { return ""; }
    const auto weights_end_pos = fragment.find(']');
    if (weights_end_pos == std::string::npos)       { return ""; }
    const auto it_start = fragment.begin() + static_cast<int>(weights_start_pos) +1;
    const auto it_end = fragment.begin() + static_cast<int>(weights_end_pos);

    str_const_range range(it_start, it_end);
    const std::string main_str_DEBUG(range.begin(), range.end());
    std::cerr << "main_str_DEBUG: '" << main_str_DEBUG << "'\n";
    const auto split_iterator_end = str_split_const_iterator();
    auto pair_it  = mk_str_split_const_iterator (range, ",");
    for(; pair_it != split_iterator_end; ++pair_it){
        const str_const_range pair_str = *pair_it;
        const std::string src_weight_DEBUG(pair_str.begin(), pair_str.end());
        std::cerr << "src_weight_DEBUG: '" << src_weight_DEBUG << "'\n"; // This seems wrong in secon iteration with this input: "[*:3, item_title:6, breadcrumbs_text:4]"
        auto it  = mk_str_split_const_iterator (pair_str, ":");
        std::string source_field = "";
        int weight = -1;
        if (it != split_iterator_end) {
            source_field = boost::trim_copy(std::string(it->begin(), it->end()));
            ++it;
            if (it != split_iterator_end) {
                const std::string weight_str_DEBUG(it->begin(), it->end());
                std::cerr << "weight_str_DEBUG: '" << weight_str_DEBUG << "'\n";
                cpaf::parse_int(weight,it->begin(), it->end(), 0);
                weights_map[source_field] = weight;
            }
        }
    }
    return ba::trim_copy(std::string(it_end+1, fragment.end())); // Return rest of fragment
//    return ba::trim_copy(std::string(it_end+2, fragment.end())); // Return rest of fragment
}


} //end namespace cpaf::semantic
