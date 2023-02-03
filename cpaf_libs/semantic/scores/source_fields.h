#ifndef CPAF_SOURCE_FIELDS_H
#define CPAF_SOURCE_FIELDS_H

#include <nlohmann/json.hpp>
#include <semantic/primitives/pattern_matcher.h>
#include <semantic/fragments/fragment_matcher.h>
#include <semantic/scores/scores_accumulator.h>


// https://github.com/sparsehash/sparsehash-c11, https://github.com/sparsehash/sparsehash
// https://www.youtube.com/watch?v=fHNmRkzxHWs

namespace cpaf::semantic
{
class fragments_classifier;

/** Holds a map of source_field(s) and the associated scores accumulator. */
class source_fields {
public:
    friend class fragments_classifier;

    /** Holds a source field with original string, field name and the tokenized version */
    struct source_field {
        friend class source_fields;
        const std::u16string&   field_name      () const                        { return field_name_;   }
        void                    field_name      (const std::u16string& name)    { field_name_ = name;   }
        const token::vector&    tokens          () const                        { return tokens_;       }
        const std::u16string&   source          () const                        { return str_;          }
    private:
        std::u16string      str_;
        std::u16string      field_name_;
        token::vector       tokens_;
    };

    template <class MAP_TYPE, class VECTOR_TYPE>
    void add_fields (const MAP_TYPE& source_fields, const VECTOR_TYPE& field_names)
    {
        const auto it_end = source_fields.end();
        for (const auto& field_name : field_names) {
            const auto it = source_fields.find(field_name);
            if (it != it_end) {
                add(field_name, it->second);
            }
        }
    }

    /** Add all fields as source fields */
    template <class MAP_TYPE>
    void add_fields (const MAP_TYPE& rec)
    {
        for (const auto& [field_name, value] : rec) {
            add(field_name, value);
        }
    }

    template <class VECTOR_TYPE>
    void add_fields (const nlohmann::json& source_fields, const VECTOR_TYPE& field_names)
    {
        const auto it_end = source_fields.end();
        for (const auto& field_name : field_names) {
            const auto it = source_fields.find(field_name);
            if (it != it_end) {
                const nlohmann::json& jo = *it;
                if (jo.is_null()) {
                    continue;
                }
                if (jo.is_string()) {
                    add(field_name, jo.get<std::string>());
                }
            }
        }
    }

    /** Add all fields as source fields */
    void add_fields (const nlohmann::json& rec)
    {
        for (const auto& [field_name, value] : rec.items()) {
            add(field_name, value);
        }
    }

    void                        add             (std::u16string&& field_name, const std::u16string& str);
    void                        add             (const std::u16string& field_name, const std::u16string& str);
    void                        add             (const std::string& field_name, const std::string& str);
    source_field&               field           (const std::u16string& field_name)  { return fields_map_[field_name];}
    const token::vector&        tokens          (const std::u16string& field_name) const;
    scores_accumulator&         scores          ()                                  { return all_scores_;}
    const scores_accumulator&   scores          () const                            { return all_scores_;}

    bool                        extended_debug_mode() const         { return all_scores_.extended_debug_mode(); }
    void                        extended_debug_mode(bool enable)    { all_scores_.extended_debug_mode(enable);  }

    std::u16string              dbg_u16string   () const;
    std::string                 dbg_string      () const;
    void                        dbg_print       () const;
    bool                        convert_to_lowercase_on_add() const;
    void                        convert_to_lowercase_on_add(bool convert_to_lowercase_on_add);

private:
    using fields_map   = std::unordered_map<std::u16string, source_field>;
    mutable fields_map          fields_map_;
    scores_accumulator          all_scores_;
    bool                        convert_to_lowercase_on_add_ = true;
    bool                        dbg_mode_ = false;
};

} //end namespace cpaf::semantic

#endif //CPAF_SOURCE_FIELDS_H
