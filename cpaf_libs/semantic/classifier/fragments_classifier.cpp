#include "fragments_classifier.h"
#include <iostream>
#include <unicode/cpaf_u16string_utils.h>
#include <semantic/readers/classification_data_reader_base.h>
#include <cpaf_libs/semantic/fragments/skipped_tokens.h>

using namespace cpaf::unicode::osops;
using namespace cpaf::unicode;
using namespace std;


namespace cpaf::semantic
{

fragments_classifier::fragments_classifier()
    : language_code_(u"en")
{
}

fragments_classifier::fragments_classifier(const u16string& lc)
    : language_code_(lc)
{

}

// ------------------------------------------
// --- Add classification rules functions ---
// ------------------------------------------
classification_rules* fragments_classifier::rules()
{
    if (classification_rules_ptr_) { return classification_rules_ptr_; }
    return &classification_rules_;
}

const classification_rules* fragments_classifier::rules() const
{
    if (classification_rules_ptr_) { return classification_rules_ptr_; }
    return &classification_rules_;
}

void fragments_classifier::add_classification_rule(
        const u16string& field_name,
        int min_score,
        int min_margin,
        const u16string& not_detected_val)
{
    rules()->add_rule(field_name, min_score, min_margin, not_detected_val);
}

// ---------------------
// --- Add functions ---
// ---------------------

fragment_node::vector fragments_classifier::add(const u16string& fragment)
{
    return fragment_match_.add(fragment);
}

fragment_node::vector fragments_classifier::add(const u16string& fragment,
        const u16string& type,
        const u16string& value)
{
    return add(fragment, type, value, 1, u"*", u"");
}

fragment_node::vector fragments_classifier::add(const u16string& fragment,
        const u16string& type,
        const u16string& value,
        int weight)
{
    return add(fragment, type, value, weight, u"*", u"");
}

fragment_node::vector fragments_classifier::add(
        const u16string& fragment,
        const u16string& type,
        const u16string& value,
        int weight,
        const u16string& source_field)
{
    return add(fragment, type, value, weight, source_field, u"");
}

fragment_node::vector fragments_classifier::add(
        const u16string& fragment,
        const u16string& type,
        const u16string& value,
        int weight,
        const u16string& source_field,
        const u16string& class_name)
{
    const auto nodes = fragment_match_.add(fragment);
    for (const auto& node : nodes) {
        node->add_attribute(type, value, weight, source_field, class_name);
    }
    return nodes;
}

/** Add classification fragments from a classsification data reader.

The classification data reader typically reads json files with classification
data.
*/
void fragments_classifier::add_fragments(classification_fragment_reader_base& data_reader)
{
    data_reader.language_code(unicode::to_string(language_code()));
    data_reader.begin_iterating();
    while (!data_reader.iterating_done()) {
        const auto& fragment = data_reader.u16_fragment();
        if (!fragment.empty()) {
            add(fragment,
                data_reader.u16_type(),
                data_reader.u16_value(),
                data_reader.weight(),
                data_reader.u16_source_field()
            );
        }
        if (save_semantic_data_) {
            do_save_sematic_data(data_reader);
        }
        data_reader.next_fragment();
    }
}

void fragments_classifier::add_fragments(classification_fragment_reader_base& data_reader,
                                         const std::u16string& override_type_name)
{
    data_reader.language_code(unicode::to_string(language_code()));
    data_reader.begin_iterating();
    const std::string str_override_type_name = unicode::to_string(override_type_name);
    while (!data_reader.iterating_done()) {
        const auto& fragment = data_reader.u16_fragment();
        const std::u16string value = data_reader.u16_custom_field(str_override_type_name);
        if (!fragment.empty()) {
            if (!value.empty()) {
                add(fragment,
                    override_type_name,
                    value,
                    data_reader.weight(),
                    data_reader.u16_source_field()
                );
            }
            else {
                add(data_reader.u16_fragment(),
                    data_reader.u16_type(),
                    data_reader.u16_value(),
                    data_reader.weight(),
                    data_reader.u16_source_field()
                );
            }
        }
        if (save_semantic_data_) {
            do_save_sematic_data(data_reader);
        }
        data_reader.next_fragment();
    }
}

void fragments_classifier::classify(source_fields& fields,
                                    parse_mode pm) const
{
    scores_accumulator& scores = fields.scores();
    for (auto& pair : fields.fields_map_) {
        source_fields::source_field& field = pair.second;
        classify(field, scores, pm);
    }
    scores.finalize();
}

void fragments_classifier::classify(source_fields& fields,
                                    parse_mode pm,
                                    const std::vector<u16string>& restrict_to_attributes) const
{
    scores_accumulator& scores = fields.scores();
    for (auto& pair : fields.fields_map_) {
        source_fields::source_field& field = pair.second;
        classify(field, scores, pm, restrict_to_attributes);
    }
    scores.finalize();
}

/** Main classify function
 *
 * Will do a complete classification and detection of type/value pairs.
 *
 * @todo The current support for running extra passes to classify further
 * from any ( due to wildcard ) skipped tokens is limited to one extra pass.
 * So it's not recursive in nature. This should not be hard to fix, but for now
 * this simple approach is expected to be adequate.
*/
void fragments_classifier::classify(source_fields::source_field& field,
                                    scores_accumulator& scores,
                                    parse_mode pm) const
{
    const auto& tokens = field.tokens();
    auto begin = tokens.begin();
    const auto end = tokens.end();
    skipped_tokens skipped;

    const std::vector<std::u16string> restrict_to_attributes;

    fragment_matcher::nodes_vector nodes = fragment_match_.parse_all(skipped, begin, end, pm, restrict_to_attributes, 4);

    // Do one extra run if any skipped ranges found and we are parsing in multi pass mode
    if (pm == parse_mode::longest_sentence_multi_pass) {
        [[maybe_unused]] skipped_tokens skipped_1; // Note: We only do one extra pass for now, so we do not use this!
        for (const auto& range: skipped.ranges()) {
            if (!range.range_completed_with_skipped_tokens) continue;
            const auto skip_nodes = fragment_match_.parse_all(skipped_1, range.first, range.second, pm, restrict_to_attributes, 4);
            nodes.insert(nodes.end(), skip_nodes.begin(), skip_nodes.end());
        }
    }

    // Add the scores
    for (const fragment_node* n : nodes ) {
        if (dbg_mode_) cerr << "DBG: Parsed : " << n->dbg_string() << "\n";
        scores.add_score(n, field.field_name());
    }
}

/** Main classify function
 *
 * Will do a complete classification and detection of type/value pairs.
 *
 * @todo The current support for running extra passes to classify further
 * from any ( due to wildcard ) skipped tokens is limited to one extra pass.
 * So it's not recursive in nature. This should not be hard to fix, but for now
 * this simple approach is expected to be adequate.
*/
void fragments_classifier::classify(source_fields::source_field& field,
                                   scores_accumulator& scores,
                                   parse_mode pm,
                                   const std::vector<std::u16string>& restrict_to_attributes) const
{
    const auto& tokens = field.tokens();
    auto begin = tokens.begin();
    const auto end = tokens.end();

    skipped_tokens skipped;
    fragment_matcher::nodes_vector nodes = fragment_match_.parse_all(skipped, begin, end, pm, restrict_to_attributes, 4);

    // Do one extra run if any skipped ranges found and we are parsing in multi pass mode
    if (pm == parse_mode::longest_sentence_multi_pass) {
        [[maybe_unused]] skipped_tokens skipped_1; // Note: We only do one extra pass for now, so we do not use this!
        for (const auto& range: skipped.ranges()) {
            const auto skip_nodes = fragment_match_.parse_all(skipped_1, range.first, range.second, pm, restrict_to_attributes, 4);
            nodes.insert(nodes.end(), skip_nodes.begin(), skip_nodes.end());
        }
    }

    // Add the scores
    for (const fragment_node* n : nodes ) {
        if (dbg_mode_) cerr << "DBG: Parsed : " << n->dbg_string() << "\n";
        if (n->has_any_of_attributes(restrict_to_attributes)) {
            scores.add_score(n, field.field_name());
        }
    }
}


void fragments_classifier::dbg_mode(bool debug_mode_active)
{
    dbg_mode_ = debug_mode_active;
    fragment_match_.dbg_mode(debug_mode_active);
    /// pattern_matcher_.dbg_mode(debug_mode_active);
}

// ---------------------------------
// --- PRIVATE: Helper functions ---
// ---------------------------------

void fragments_classifier::do_save_sematic_data(const classification_fragment_reader_base& dr)
{
    all_semantic_data_[dr.fragment()][dr.type()][dr.value()];
//    ,
//                    dr.type(),
//                    dr.value(),
//                    dr.weight(),
//                    dr.source_field()
}

} //end namespace cpaf::semantic


