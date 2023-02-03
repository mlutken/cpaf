#ifndef SEMANTIC_COLLAPSE_STRING_H
#define SEMANTIC_COLLAPSE_STRING_H
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <unicode/cpaf_u16string_utils.h>

namespace cpaf::semantic
{

class fragment_matcher;
class fragment_node;
class token_parser;

class collapse_string
{
public:
    using only_remove_if_map = std::unordered_map<std::u16string, std::unordered_set<std::u16string> >;
    static only_remove_if_map       create_only_remove_if_map(const std::u16string& attribute, const std::u16string& only_remove_if_value);

    collapse_string() = default;
    explicit collapse_string(const fragment_matcher* fm);

    std::u16string collapse_title               (const std::u16string& src_string,
                                                 const std::vector<std::u16string>& attributes_collapse,
                                                 const only_remove_if_map& only_remove_if);
    std::u16string remove_matching_attributes   (const std::u16string& src_string,
                                                 const std::vector<std::u16string>& attributes_collapse,
                                                 const only_remove_if_map& only_remove_if);

    void fragment_matcher_set               (const fragment_matcher* fm)                    { fragments_match_ = fm; }
    void remove_matching_attributes         (std::u16string& dst_string,
                                             const std::u16string& src_string,
                                             const std::vector<std::u16string>& attributes_collapse,
                                             const only_remove_if_map& only_remove_if);
    void remove_matching_attributes         (std::u16string& dst_string,
                                             const token_parser& parsed_tokens,
                                             const std::u16string& src_string,
                                             const std::vector<std::u16string>& attributes_collapse,
                                             const only_remove_if_map& only_remove_if) const;

    std::string collapse_title             (const std::string& src_string,
                                            const std::vector<std::string>& attributes_collapse,
                                            const only_remove_if_map& only_remove_if);
    std::string remove_matching_attributes (const std::string& src_string,
                                            const std::vector<std::string>& attributes_collapse,
                                            const only_remove_if_map& only_remove_if);

private:
    bool                                should_remove_substr                (const fragment_node* node_ptr,
                                                                             const std::vector<std::u16string>& remove_if_node_has_attributes,
                                                                             const only_remove_if_map& only_remove_if) const;
    bool                                should_remove_attribute_value_pair  (const std::u16string& attribute,
                                                                             const std::u16string& value,
                                                                             const only_remove_if_map& only_remove_if) const;
    const fragment_matcher&             fragments_match                     () const  {return *fragments_match_; }

    void static                         copy_slice                          (std::u16string& dst_string,
                                                                             const std::u16string& src_string,
                                                                             const unicode::u16_index_pair& slice);
    const fragment_matcher*             fragments_match_ = nullptr;
};

} //end namespace cpaf::semantic


#endif // SEMANTIC_COLLAPSE_STRING_H
