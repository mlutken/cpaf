#include "classification_rules.h"

using namespace std;

namespace cpaf::semantic
{
classification_rules::classification_rules()
{
    rules_[u"*"] = {20, 10, u""};
}

void classification_rules::add_rule(
        const std::u16string& field_name,
        int min_score, int min_margin,
        const std::u16string& not_detected_val)
{
    rules_[field_name] = rule{min_score, min_margin, not_detected_val};
}

const classification_rules::rule& classification_rules::get_rule(const u16string& field_name) const
{
    const auto it = rules_.find(field_name);
    if (it != rules_.end()) {
        return it->second;
    }
    return rules_.at(u"*");
}


} //end namespace cpaf::semantic
