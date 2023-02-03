#include "collapse_string.h"

#include <iostream>
#include <semantic/tokens/token_parser.h>
#include <semantic/fragments/fragment_matcher.h>

namespace cu = cpaf::unicode;
using namespace cpaf::unicode::osops;

namespace cpaf::semantic
{

collapse_string::only_remove_if_map collapse_string::create_only_remove_if_map(
        const std::u16string& attribute,
        const std::u16string& only_remove_if_value)
{
    collapse_string::only_remove_if_map only_remove_if;
    only_remove_if.emplace(attribute, std::unordered_set<std::u16string>{only_remove_if_value});
    return only_remove_if;
}

collapse_string::collapse_string(const fragment_matcher* fm)
    : fragments_match_(fm)
{
}

std::u16string collapse_string::collapse_title(const std::u16string& src_string,
                                               const std::vector<std::u16string>& attributes_collapse,
                                               const only_remove_if_map& only_remove_if)
{
    auto str = cu::simplify_title_copy(src_string);
    return remove_matching_attributes(str, attributes_collapse, only_remove_if);
}

std::u16string collapse_string::remove_matching_attributes(const std::u16string& src_string,
                                                           const std::vector<std::u16string>& attributes_collapse,
                                                           const only_remove_if_map& only_remove_if)
{
    std::u16string dst_string;
    dst_string.reserve(src_string.size());
    remove_matching_attributes(dst_string, src_string, attributes_collapse, only_remove_if);
    return dst_string;
}

void collapse_string::remove_matching_attributes (
        std::u16string& dst_string,
        const std::u16string& src_string,
        const std::vector<std::u16string>& attributes_collapse,
        const only_remove_if_map& only_remove_if)
{
    token_parser parsed_tokens(cpaf::unicode::to_lower_copy(src_string));
    parsed_tokens.parse();
    remove_matching_attributes(dst_string, parsed_tokens, src_string, attributes_collapse, only_remove_if);
}

void collapse_string::remove_matching_attributes (
        std::u16string& dst_string,
        const token_parser& parsed_tokens,
        const std::u16string& src_string,
        const std::vector<std::u16string>& attributes_collapse,
        const only_remove_if_map& only_remove_if) const
{
    const auto candidates = fragments_match().parse_and_mark_all_tokens(
                parsed_tokens.tokens(), parse_mode::longest_sentence,
                attributes_collapse,
                6
                );
    if (candidates.empty()) {
        dst_string = src_string;
        return;
    }
    size_t i = 0;
    const size_t size = candidates.size();
    const size_t last = size -1;
    for ( ; i < size; ++i) {
        const fragment_matcher::match_candidate& candidate = candidates[i];
        const auto node_ptr = candidate.node();
        if (node_ptr && should_remove_substr(node_ptr, attributes_collapse, only_remove_if)) {
            continue;
        }
        const auto slice = candidate.token_u16_indices(*parsed_tokens.source());
        copy_slice(dst_string, src_string, slice);
        if (i < last && candidate.space_after()) {
            dst_string.push_back(cu::chars16::space);
        }
    }
    // We might have added a space too much if all following candidates are skipped
    if (!dst_string.empty() && cu::chars16::is_space(dst_string.back()) ) {
        dst_string.pop_back();
    }
}

std::string collapse_string::collapse_title(const std::string& src_string,
                                            const std::vector<std::string>& attributes_collapse,
                                            const only_remove_if_map& only_remove_if)
{
    using namespace unicode;
    const auto& u16s = collapse_title(to_u16string(src_string), to_u16string(attributes_collapse), only_remove_if);
    return to_string(u16s);
}

std::string collapse_string::remove_matching_attributes(const std::string& src_string,
                                                        const std::vector<std::string>& attributes_collapse,
                                                        const only_remove_if_map& only_remove_if)
{
    using namespace unicode;
    const auto& u16s = remove_matching_attributes(to_u16string(src_string), to_u16string(attributes_collapse), only_remove_if);
    return to_string(u16s);
}

bool collapse_string::should_remove_substr(
        const fragment_node* node_ptr,
        const std::vector<std::u16string>& remove_if_node_has_attributes,
        const only_remove_if_map& only_remove_if) const
{
    const auto& value = node_ptr->u16string();
    for (const auto& attribute: remove_if_node_has_attributes) {
        if (node_ptr->has_attribute(attribute)) {
            const bool should_remove = should_remove_attribute_value_pair(attribute, value, only_remove_if);
            return should_remove;
        }
    }
    return false;
}

bool collapse_string::should_remove_attribute_value_pair(
        const std::u16string& attribute,
        const std::u16string& value,
        const only_remove_if_map& only_remove_if) const
{
    auto it = only_remove_if.find(attribute);
    if ( it != only_remove_if.end()) {
        const bool should_remove = it->second.count(value) != 0; // contains
        return should_remove;
    }
    return true;
}

void collapse_string::copy_slice (
        std::u16string& dst_string,
        const std::u16string& src_string,
        const cpaf::unicode::u16_index_pair& slice)
{
    const auto begin = src_string.begin() + static_cast<int>(slice.first);
    const auto end = src_string.begin() + static_cast<int>(slice.second);
    std::copy (begin, end, std::back_inserter(dst_string));
}

}
