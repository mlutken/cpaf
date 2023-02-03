#ifndef CPAF_SCORES_ACCUMULATOR_H
#define CPAF_SCORES_ACCUMULATOR_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unicode/cpaf_u16string_utils.h>
#include <semantic/config/classification_rules.h>
#include <nlohmann/json.hpp>

namespace cpaf::semantic
{
class fragment_node;
class fragment_attribute_weights;

/** For summing up scores per type.

For each type we keep a vector of values with the weight for each value.
The data looks similar to this:
@code
item_type: {city_bike: 25}, {road_bike: 15}
sex: {f: 15}, {m: 20}, ..
@endcode

When the add_score(type, value, weight) function is called we find the appropriate vector
in the top level map by looking up using the type and then we search through the vector for
the

The finalize function will sort these vectors, leaving the candidate with the highest weigt score
as first elemt in the "type score" vectors.

*/
class scores_accumulator
{
public:
    void        clear               ();
    void        add_score           (const fragment_node* node, const std::u16string& source_field);
    void        add_score           (const fragment_attribute_weights* attr_weights,
                                     const std::u16string& source_field,
                                     const fragment_node* node = nullptr);
    void        add_score           (const std::u16string& type, const std::u16string& value,
                                     int32_t weight, const std::u16string& source_field,
                                     const fragment_node* node = nullptr);
    int32_t     score               (const std::u16string& type, const std::u16string& value) const;
    void        finalize            ();

    bool        extended_debug_mode () const { return debug_scores_map_ != nullptr; }
    void        extended_debug_mode (bool enable);
    bool        add_json_debug_data () const { return source_fields_scores_ != nullptr; }
    void        add_json_debug_data (bool enable);

    nlohmann::json          as_json             () const;
    const nlohmann::json*   source_fields_scores   () const;

    template <class U16STRING_MAP_TYPE>
    void assign_detected_values (U16STRING_MAP_TYPE& map, const classification_rules& rules)
    {
        for (auto& pair : scores_) {
            const auto& field_name = pair.first;
            type_scores_vec& scores = pair.second;
            const auto& rule = rules.get_rule(field_name);
            map[field_name] = detect_field_value (scores, rule);
        }
    }

    template <class STRING_MAP_TYPE>
    void assign_detected_values_str (STRING_MAP_TYPE& map, const classification_rules& rules)
    {
        for (auto& pair : scores_) {
            const auto& field_name = pair.first;
            type_scores_vec& scores = pair.second;
            const auto& rule = rules.get_rule(field_name);
            const std::string field_name_str = cpaf::unicode::to_string(field_name);
            map[field_name_str] = cpaf::unicode::to_string(detect_field_value (scores, rule));
        }
    }


    std::u16string          debug_u16string         () const;
    std::u16string          debug_u16string         (const std::u16string& type) const;
    std::u16string          extended_debug_u16string() const;
    std::u16string          extended_debug_u16string(const std::u16string& type) const;
    std::string             debug_string            () const;
    std::string             debug_string            (const std::u16string& type) const;
    std::string             extended_debug_string   () const;
    std::string             extended_debug_string   (const std::u16string& type) const;

private:
    struct value_score {
        value_score (const std::u16string& val, int w) : value_(val), score_(w) {}
        std::u16string  value_;
        int             score_;
        bool            operator< (const value_score& rhs) const { return score_ < rhs.score_; }
        bool            operator> (const value_score& rhs) const { return score_ > rhs.score_; }
    };

    // --- PRIVATE: Helper functions ---
    using type_scores_vec   = std::vector<value_score>;
    using scores_map        = std::unordered_map<std::u16string, type_scores_vec>;
    using debug_scores_map  = std::unordered_map<std::u16string, std::u16string>;


    void                    dbg_u16string_append(std::u16string& s, const std::u16string& type,
                                                 const type_scores_vec& vec) const;

    const value_score*        find_score          (const std::u16string& type, const std::u16string& value) const;
    void                    detect_field_value  (std::u16string& value, const type_scores_vec& scores, const classification_rules::rule& rule) const;
    std::u16string          detect_field_value  (const type_scores_vec& scores, const classification_rules::rule& rule) const;

    // --- PRIVATE: Member data ---
    scores_map                          scores_;
    std::unique_ptr<debug_scores_map>   debug_scores_map_ = nullptr;
    std::unique_ptr<nlohmann::json>     source_fields_scores_ = nullptr;
};

} //end namespace cpaf::semantic

#endif //CPAF_SCORES_ACCUMULATOR_H
