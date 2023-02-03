#include "fragment_attribute_weights.h"

using namespace std;

namespace cpaf::semantic
{

void fragment_attribute_weights::set_weight(
        const u16string& attr_value,
        const u16string& source_field,
        int32_t weight)
{
    if (source_field.empty()) {
        attribute_value_weights_[attr_value][u"*"] = weight;
    }
    else {
        attribute_value_weights_[attr_value][source_field] = weight;
    }
}

} //end namespace cpaf::semantic
