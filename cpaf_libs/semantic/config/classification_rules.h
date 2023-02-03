#ifndef CPAF_CLASSIFICATION_MARGINS_H
#define CPAF_CLASSIFICATION_MARGINS_H

#include <string>
#include <unordered_map>

namespace cpaf::semantic
{

/** Holds simple classification rule margins.
These are minimum required score and margin to runner up
candidate for a classification to be considered valid.
The margin rules also contain default values to use when values are not detected.

The margin rules are stored per type and there is a default rule as well.

Example:
@code
"item_type": {
    "min_required_margin": 16,
    "min_required_score": 24,
    "not_detected_value": "item_misc"
},
"sex": {
    "min_required_margin": 8,
    "min_required_score": 16,
    "not_detected_value": "m"
},
...
@endcode
*/
class classification_rules {
public:
    classification_rules();
    struct rule {
        rule()
            : min_required_score_(1),
              min_required_margin_(1),
              not_detected_value_(u"")
            {}

        rule(int min_score, int min_margin, const std::u16string& not_detected_val)
            : min_required_score_(min_score),
              min_required_margin_(min_margin),
              not_detected_value_(not_detected_val)
        {}
        int             min_required_score_;
        int             min_required_margin_;
        std::u16string  not_detected_value_;
    };


    void            add_rule    (const std::u16string& field_name,
                                 int min_score, int min_margin,
                                 const std::u16string& not_detected_val);


    const rule&     get_rule    (const std::u16string& field_name) const;

private:
    using map = std::unordered_map<std::u16string, rule>;
    map  rules_;
};

} //end namespace cpaf::semantic

#endif //CPAF_CLASSIFICATION_MARGINS_H
