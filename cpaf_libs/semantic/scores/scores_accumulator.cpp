#include <iostream>
#include <cpaf_libs/utils/cpaf_stl_utils.h>
#include <cpaf_libs/semantic/fragments/fragment_node.h>
#include "scores_accumulator.h"

using namespace cpaf::unicode::osops;
using namespace cpaf::unicode;
using namespace std;

namespace cpaf::semantic
{
void scores_accumulator::clear()
{
    scores_.clear();
    if (debug_scores_map_) {
        debug_scores_map_->clear();
    }
    if (source_fields_scores_) {
        source_fields_scores_->clear();
    }
}

void scores_accumulator::add_score(const fragment_node* node, const u16string& source_field)
{
    const auto it_end = node->attributes_end();
    for (auto it = node->attributes_begin(); it != it_end; ++it) {
        const fragment_attribute_weights* attr_weights = &(it->second);
        add_score(attr_weights, source_field, node);
    }
}

void scores_accumulator::add_score(const fragment_attribute_weights* attr_weights,
                                   const u16string& source_field,
                                   const fragment_node* node)
{
    if (attr_weights) {
        auto it = attr_weights->weights_begin();
        auto it_end = attr_weights->weights_end();
        for (; it != it_end; ++it ) {
            int weight = 0; // Default fallback weight
            const std::u16string& attribute_value = it->first;
            const fragment_attribute_weights::source_fields_weights& src_fields_weights = it->second;
            const auto it_end = src_fields_weights.end();
            auto it = src_fields_weights.find(source_field);
            if (it == it_end) {
                it = src_fields_weights.find(u"*");
            }
            if (it != it_end) {
                weight = it->second;
            }
            add_score(attr_weights->type(), attribute_value, weight, source_field, node);
        }
    }
}

void scores_accumulator::add_score(
        const u16string& type,
        const u16string& value,
        int32_t weight,
        const u16string& source_field,
        const fragment_node* node)
{
    if (debug_scores_map_) {
        const std::u16string s = u"[" + value + u"] "
                + u"{"
                + source_field + u":" + cpaf::unicode::to_u16string(weight)
                + (node ? u" '" + node->u16string() + u"'" : u"")
                + u"}";
        std::u16string& dbg_str = (*debug_scores_map_)[type];
        if (!dbg_str.empty()) {
            dbg_str.append(u", ");
        }
        dbg_str.append(s);
    }
    if (source_fields_scores_) {
        using namespace cpaf::unicode;
        nlohmann::json jo;
        jo["value"] = to_string(value);
        jo["weight"] = weight;
        jo["match_string"] = to_string(node->u16string());
        (*source_fields_scores_)[to_string(type)][to_string(source_field)].push_back(jo);
    }

    auto& vec = scores_[type];
    const auto pred = [&value](const value_score& scr) { return value == scr.value_; };
    auto it = std::find_if(vec.begin(), vec.end(), pred);
    if (it != vec.end()) {
        value_score& scores = *it;
        scores.score_ += weight;
    }
    else {
        vec.emplace_back(value, weight);
    }
}

int32_t scores_accumulator::score(const u16string& type, const u16string& value) const
{
    const value_score* ptr = find_score(type, value);
    return ptr ? ptr->score_ : 0;
}

void scores_accumulator::finalize()
{
    for (auto& pair : scores_) {
        type_scores_vec& vec = pair.second;
        std::sort(vec.begin(), vec.end(), std::greater<>());
    }
}

void scores_accumulator::extended_debug_mode(bool enable)
{
    if (enable) {
        if (!debug_scores_map_) {
            debug_scores_map_ = std::make_unique<debug_scores_map>();
        }
    }
    else {
        debug_scores_map_.reset();
    }
}

void scores_accumulator::add_json_debug_data(bool enable)
{
    if (enable) {
        if (!source_fields_scores_) {
            source_fields_scores_ = std::make_unique<nlohmann::json>();
        }
    }
    else {
        debug_scores_map_.reset();
    }
}

nlohmann::json scores_accumulator::as_json() const
{
    using namespace cpaf::unicode;
    nlohmann::json jo;
    for (const auto& pair : scores_) {
        const std::string type = to_string(pair.first);
        const type_scores_vec& vec = pair.second;
        auto jo_vec = nlohmann::json();
        for (const value_score& vw : vec) {
            jo_vec.push_back(nlohmann::json{{"value", to_string(vw.value_)}, {"score", vw.score_}} );
        }
        jo[type] = jo_vec;
    }
    return jo;
}

const nlohmann::json* scores_accumulator::source_fields_scores() const
{
    return source_fields_scores_.get();
}


std::u16string scores_accumulator::debug_u16string() const
{
    std::u16string s;
    for (const auto& pair : scores_) {
        const std::u16string& type = pair.first;
        const type_scores_vec& vec = pair.second;
        dbg_u16string_append(s, type, vec);
    }
    return s;
}

std::u16string scores_accumulator::debug_u16string(const u16string& type) const
{
    std::u16string s;
    if (auto it = scores_.find(type); it != scores_.end()) {
        const std::u16string& type = it->first;
        const type_scores_vec& vec = it->second;
        dbg_u16string_append(s, type, vec);
    }
    return s;
}

u16string scores_accumulator::extended_debug_u16string() const
{
    if (debug_scores_map_){
        std::u16string s;
        for (const auto& pair : scores_) {
            const std::u16string& type = pair.first;
            s.append(extended_debug_u16string(type) + u"\n");
        }
        return s;
    }

    return debug_u16string(); // Fallback to normal debug string!
}


u16string scores_accumulator::extended_debug_u16string(const u16string& type) const
{
    if (debug_scores_map_){
        const auto it = debug_scores_map_->find(type);
        if (it != debug_scores_map_->end()) {
            std::u16string s(type + u": ");
            const auto& vec = scores_.at(type);
            const auto num_values = std::min<size_t>(vec.size(), 4);
            for (size_t i = 0; i < num_values; ++i) {
                if (i != 0) s.append(u", ");
                const value_score& vw = vec[i];
                s.append(u"[" + vw.value_ + u"]");
                s.append(u"(" + cpaf::unicode::to_u16string(vw.score_) + u")");
            }
            s.append( u" => " + it->second);
            return s;
        }
    }

    return debug_u16string(type); // Fallback to normal debug string!
}

std::string scores_accumulator::debug_string() const
{
    return cpaf::unicode::to_string(debug_u16string());
}

void scores_accumulator::dbg_u16string_append(
        u16string& s, const u16string& type,
        const scores_accumulator::type_scores_vec& vec) const
{
    // To improve debugging we locate the maximum score here.
    // We could sort, but the finalize function is mutable so instead of
    // making it const and add mutable to selected members we do this instead.
    // Anyway the top scoring value might not "win" as the rules are not considered
    // here at all;
    auto it_max_score = std::max_element(vec.begin(), vec.end());
    s += type;
    if (it_max_score != vec.end() ) {
        s += u": " + it_max_score->value_;
    }
    s += u" { ";
    for (const value_score& vw : vec) {
        s += u"(" + vw.value_ + u":" + unicode::to_u16string(vw.score_) + u")";
    }
    s += u" }\n";
}

std::string scores_accumulator::debug_string(const u16string& type) const
{
    return cpaf::unicode::to_string(debug_u16string(type));
}

string scores_accumulator::extended_debug_string() const
{
    return cpaf::unicode::to_string(extended_debug_u16string());
}

string scores_accumulator::extended_debug_string(const u16string& type) const
{
    return cpaf::unicode::to_string(extended_debug_u16string(type));
}

const scores_accumulator::value_score*
scores_accumulator::find_score(const std::u16string& type, const std::u16string& value) const
{
    const auto it_scores = scores_.find(type);
    if (it_scores == scores_.end()) {
        return nullptr;
    }
    const auto& vec = it_scores->second;
    const auto pred = [&value](const value_score& scr) { return value == scr.value_; };
    auto it = std::find_if(vec.begin(), vec.end(), pred);
    if (it != vec.end()) {
        return &(*it);
    }
    return nullptr;
}

void scores_accumulator::detect_field_value(
        u16string& value,
        const scores_accumulator::type_scores_vec& scores,
        const classification_rules::rule& rule) const
{
    if (scores.empty()) {
        value = rule.not_detected_value_;
        return;
    }
    if (scores.size() == 1) {
        const auto& candidate = scores.front();
        if ( candidate.score_ >= rule.min_required_score_) {
            value = candidate.value_;
        }
        else {
            value = rule.not_detected_value_;
        }
    }
    else {
        const auto& candidate = scores.front();
        const auto& runner_up = scores[1];
        if (  candidate.score_ >= rule.min_required_score_ &&
             (candidate.score_ - runner_up.score_) >= rule.min_required_margin_ ) {
            value = candidate.value_;
        }
        else {
            value = rule.not_detected_value_;
        }
    }
}

u16string scores_accumulator::detect_field_value(const scores_accumulator::type_scores_vec& scores,
                                                 const classification_rules::rule& rule) const
{
    u16string value;
    detect_field_value(value, scores, rule);
    return value;
}
} //end namespace cpaf::semantic
