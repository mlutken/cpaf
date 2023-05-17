#include "json_classification_data_reader.h"

#include <iostream>
#include <regex>
#include <cpaf_libs/utils/cpaf_json_utils.h>
#include <cpaf_libs/utils/cpaf_string_utils.hpp>
#include <cpaf_libs/utils/cpaf_debug_utils.h>
#include <cpaf_libs/streams/cpaf_json_streams.h>



using namespace std;
namespace ba = boost::algorithm;
namespace cpaf::semantic
{

json_classification_data_reader::json_classification_data_reader(const std::string& path)
    : classification_fragment_reader_base(path)
{
}

json_classification_data_reader::json_classification_data_reader(
        const std::filesystem::path& path)
    : classification_fragment_reader_base(path)
{
}


const nlohmann::json& json_classification_data_reader::values() const
{
    const auto it = json_data_.find("values");
    if (it == json_data_.end()) {
        return empty_json_;
    }
    return *it;
}

std::string json_classification_data_reader::dbg_default_weights() const
{
    return cpaf::dbg::dict_container_str(default_fragment_weights_);
}

// --------------------------------------------------
// --- PRIVATE: Virtual functions overridden here ---
// --------------------------------------------------

void json_classification_data_reader::do_read_data()
{
    std::regex rx( R"(\s*(\{|\[))" );
    if (std::regex_search(resource(), rx, std::regex_constants::match_continuous) ) {
        json_data_ = nlohmann::json::parse(resource());
    }
    else {
        json_data_ = cpaf::streams::to_json(resource());
    }

    default_fragment_weights_.clear();
    const nlohmann::json& jweights = default_json_weights();
    if (!jweights.contains("*")) {
        default_fragment_weights_ = global_weights();
    }
    for (const auto& [src_field, wj] : jweights.items()) {
        const float w = cpaf::json_value_float(wj, 0);
        default_fragment_weights_[src_field] = w;
    }
}

void json_classification_data_reader::do_begin_iterating()
{
    language_codes_.clear();
    fragment_ = "";
    fragments_iterator_ = empty_json_.end();
    fragments_iterator_end_ = empty_json_.end();
    const std::string languages_add = "names_"s + language_code() +  "_add_from_language"s;
    language_codes_.push_back("all");
    language_codes_.push_back(language_code());
    for (const auto& jo : json_data_.value(languages_add, nlohmann::json())) {
        language_codes_.push_back(jo.get<std::string>());
    }
    values_iterator_    = values().begin();
    languages_iterator_ = language_codes_.begin();
    if (values_iterator_ != values().end()) {
        static_cast<void>(seek_first_valid_value());
    }
}

bool json_classification_data_reader::do_iterating_done()
{
    return values_iterator_ == values().end();
}

void json_classification_data_reader::do_next_fragment()
{
    ++weights_iterator_;
    if (weights_iterator_ != weights_iterator_end_) {
        return;
    }

    fragment_ = "";
    ++fragments_iterator_;
    if (fragments_iterator_ != fragments_iterator_end_) {
        create_weights_iterators();
        return;
    }

    ++languages_iterator_ ;
    if (seek_first_valid_value()) {
        return;
    }

    if (values_iterator_ == values().end()) {
        return;
    }

    ++values_iterator_;
    languages_iterator_ = language_codes_.begin();
    seek_first_valid_value();
}

std::string json_classification_data_reader::do_custom_field(
        const string& key,
        const string& default_value) const
{
    return values_iterator_->value(key, default_value);
}

std::string json_classification_data_reader::do_fragment() const
{
    if (!fragment_.empty()) {
        return fragment_;
    }
    return fragment_iterator_value();
}

std::string json_classification_data_reader::do_type() const
{
    return values_iterator_->value("type", "");
}

std::string json_classification_data_reader::do_value() const
{
    const auto& jo = values_iterator_->value("val", nlohmann::json());
    return cpaf::simple_json_to_string(jo);
}

int json_classification_data_reader::do_weight() const
{
    return weights_iterator_->second;
}

std::string json_classification_data_reader::do_source_field() const
{
    return weights_iterator_->first;
}

// ---------------------------------
// --- PRIVATE: Helper functions ---
// ---------------------------------

bool json_classification_data_reader::seek_first_valid_value()
{
    while (values_iterator_ != values().end())
    {
        if (seek_first_valid_fragments()) {
            return true;
        }
        ++values_iterator_;
        languages_iterator_ = language_codes_.begin(); // Start over on language codes
    }
    return false;
}


bool json_classification_data_reader::seek_first_valid_fragments()
{
    while (languages_iterator_ != language_codes_.end())
    {
        if (create_fragments_iterators()) {
            create_weights_iterators();
            return true;
        }
        ++languages_iterator_;
    }
    return false;
}

bool json_classification_data_reader::create_fragments_iterators()
{
    fragment_ = "";
    json_fragments_ptr_ = nullptr;
    const std::string lc = *languages_iterator_;
    const nlohmann::json& value_jo = *values_iterator_;
    const auto key = "names_"s + lc;
    const auto jo_it = value_jo.find(key);
    if (jo_it != value_jo.end()) {
        json_fragments_ptr_ = &(*jo_it);
        fragments_iterator_ = jo_it->begin();
        fragments_iterator_end_ = jo_it->end();
        return fragments_iterator_ != fragments_iterator_end_;
    }
    return false;
}


void json_classification_data_reader::create_weights_iterators()
{
    weights_map fragment_weights;
    if (fragments_iterator_->is_object()) {
        for (const auto& [src_field, w] : fragments_iterator_.value().items()) {
            fragment_weights[src_field] = w;
        }
    }
    else {
        fragment_= cpaf::semantic::parse_fragment_weights(fragment_weights, fragment_iterator_value());
    }

    current_fragment_weights_.clear();
    if (!fragment_weights.contains("*")) {
        current_fragment_weights_ = default_fragment_weights_;
    }
    for (const auto& [src_field, w] : fragment_weights) {
        current_fragment_weights_[src_field] = w;
    }
    weights_iterator_ = current_fragment_weights_.begin();
    weights_iterator_end_ = current_fragment_weights_.end();
}

std::string json_classification_data_reader::fragment_iterator_value() const
{
    if (!json_fragments_ptr_) return "";
    std::string fragment;
    if (json_fragments_ptr_->is_object()) {
        fragment = fragments_iterator_.key();
    }
    else if (json_fragments_ptr_->is_array()) {
        fragment = cpaf::simple_json_to_string(*fragments_iterator_);
    }
    return fragment;
}

const nlohmann::json& json_classification_data_reader::default_json_weights() const
{
    const auto it = json_data_.find("default_weights");
    if (it == json_data_.end()) {
        return empty_json_;
    }
    return *it;
}

} //end namespace cpaf::semantic
