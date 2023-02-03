#ifndef CPAF_FRAGMENT_ATTRIBUTE_WEIGHTS_H
#define CPAF_FRAGMENT_ATTRIBUTE_WEIGHTS_H

#include <string>
#include <map>
//#include <unordered_map>

// https://github.com/sparsehash/sparsehash-c11, https://github.com/sparsehash/sparsehash
// https://www.youtube.com/watch?v=fHNmRkzxHWs

namespace cpaf::semantic
{

/** Holds the attribute_types and canonical values for a sentence node.
    For this attribute_type/value pair we also associate weights per source field.

    To top it all each fragment_node can have a number of these attributes.
*/
class fragment_attribute_weights
{
public:
    using source_fields_weights     = std::map<std::u16string, int, std::less<> >;
    using attribute_value_weights   = std::map<std::u16string, source_fields_weights, std::less<> >;
    using const_iterator            = attribute_value_weights::const_iterator;

    fragment_attribute_weights  () = default;
    ~fragment_attribute_weights () = default;

    const std::u16string&   type            () const                        { return attribute_type_;     }
    void                    type            (const std::u16string& t)       { attribute_type_ = t;        }
    void                    set_weight      (const std::u16string& attr_value,
                                             const std::u16string& source_field,
                                             int32_t weight);

    const_iterator          weights_begin   () const { return attribute_value_weights_.begin(); }
    const_iterator          weights_end     () const { return attribute_value_weights_.end();   }
private:
    attribute_value_weights attribute_value_weights_;        ///< Keys = attribute_value, source_field
    std::u16string          attribute_type_;
};

} //end namespace cpaf::semantic


#endif //CPAF_FRAGMENT_ATTRIBUTE_WEIGHTS_H

