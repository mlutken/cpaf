#ifndef CPAF_FRAGMENTS_CLASSIFIER_H
#define CPAF_FRAGMENTS_CLASSIFIER_H

#include <semantic/fragments/fragment_matcher.h>
#include <semantic/scores/source_fields.h>
#include <nlohmann/json.hpp>

// https://github.com/sparsehash/sparsehash-c11, https://github.com/sparsehash/sparsehash
// https://www.youtube.com/watch?v=fHNmRkzxHWs

namespace cpaf::semantic
{

class pattern_matcher;
class classification_fragment_reader_base;

class fragments_classifier {
public:
    fragments_classifier();
    explicit fragments_classifier(const std::u16string& lc);

    std::u16string              language_code           () const                    { return language_code_;}
    void                        language_code           (const std::u16string& lc)  { language_code_ = lc;  }
    nlohmann::json&             all_semantic_data       () { return all_semantic_data_; }
    void                        save_semantic_data_set  (bool do_save) { save_semantic_data_ = do_save; }
    bool                        save_semantic_data      () const { return save_semantic_data_; }

    // --- Add classification rules functions ---
    classification_rules*       rules           ();
    const classification_rules* rules           () const;
    void                        rules           (classification_rules* rules_ptr) { classification_rules_ptr_ = rules_ptr; }
    void                        add_classification_rule(const std::u16string& field_name,
                                                        int min_score, int min_margin,
                                                        const std::u16string& not_detected_val);

    // --- Add functions ---

    fragment_node::vector   add             (const std::u16string& fragment);

    fragment_node::vector   add             (const std::u16string& fragment,
                                             const std::u16string& type,
                                             const std::u16string& value);

    fragment_node::vector   add             (const std::u16string& fragment,
                                             const std::u16string& type,
                                             const std::u16string& value,
                                             int weight);

    fragment_node::vector   add             (const std::u16string& fragment,
                                             const std::u16string& type,
                                             const std::u16string& value,
                                             int weight,
                                             const std::u16string& source_field);

    fragment_node::vector   add             (const std::u16string& fragment,
                                             const std::u16string& type,
                                             const std::u16string& value,
                                             int weight,
                                             const std::u16string& source_field,
                                             const std::u16string& class_name);

    void                    add_fragments    (classification_fragment_reader_base& data_reader);
    void                    add_fragments    (classification_fragment_reader_base& data_reader, const std::u16string& override_type_name);

    // --- Parse/lookup functions ---
    template <class U16STRING_MAP_TYPE>
    void                    detect_values    (U16STRING_MAP_TYPE& map,
                                              source_fields& fields,
                                              parse_mode pm) const
    {
        classify(fields, pm);
        fields.scores().assign_detected_values(map, *rules());
    }

    template <class U16STRING_MAP_TYPE>
    void                    detect_values    (U16STRING_MAP_TYPE& map,
                                              source_fields& fields,
                                              parse_mode pm,
                                              const std::vector<std::u16string>& restrict_to_types) const
    {
        classify(fields, pm, restrict_to_types);
        fields.scores().assign_detected_values(map, *rules());
    }

    template <class STRING_MAP_TYPE>
    void                    detect_values_str(STRING_MAP_TYPE& map, source_fields& fields,
                                              parse_mode pm) const
    {
        classify(fields, pm);
        fields.scores().assign_detected_values_str(map, *rules());
    }

    template <class STRING_MAP_TYPE>
    void                    detect_values_str(STRING_MAP_TYPE& map,
                                              source_fields& fields,
                                              parse_mode pm,
                                              const std::vector<std::u16string>& restrict_to_types) const
    {
        classify(fields, pm, restrict_to_types);
        fields.scores().assign_detected_values_str(map, *rules());
    }

    void                    classify         (source_fields& fields,
                                              parse_mode pm) const;
    void                    classify         (source_fields& fields,
                                              parse_mode pm,
                                              const std::vector<std::u16string>& restrict_to_attributes) const;

    void                    classify         (source_fields::source_field& field,
                                              scores_accumulator& scores,
                                              parse_mode pm) const;
    void                    classify         (source_fields::source_field& field,
                                              scores_accumulator& scores,
                                              parse_mode pm,
                                              const std::vector<std::u16string>& restrict_to_attributes) const;
    void                    dbg_mode         (bool debug_mode_active);
    bool                    dbg_mode         () const                   { return dbg_mode_;}

    const fragment_matcher&         fragment_match          () const    { return fragment_match_; }
    const fragment_matcher*         fragment_match_ptr      () const    { return &fragment_match_; }
    const pattern_matcher&          pattern_lookup          () const    { return fragment_match_.word_pattern_lookup(); }
    const pattern_matcher*          pattern_lookup_ptr      () const    { return &fragment_match_.word_pattern_lookup(); }

private:
    // --- Helper functions ---
    void                        do_save_sematic_data    (const classification_fragment_reader_base& dr);
    // --- Member data ---
    fragment_matcher            fragment_match_;
    classification_rules        classification_rules_;
    nlohmann::json              all_semantic_data_;         // For gathering all semantic data for later retrieval/output to file
    std::u16string              language_code_;
    classification_rules*       classification_rules_ptr_ = nullptr; // If this is set prefer this compared to classification_rules_
    bool                        save_semantic_data_       = false;    // FIXMENM: Change to false
    bool                        dbg_mode_ = false;
};



} //end namespace cpaf::semantic


#endif //CPAF_FRAGMENTS_CLASSIFIER_H
