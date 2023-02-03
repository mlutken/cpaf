#ifndef CPAF_CONFIGURATION_H
#define CPAF_CONFIGURATION_H

#include <semantic/config/classification_rules.h>
#include <semantic/config/semantic_data_resources.h>

namespace cpaf::semantic
{

/** Holds all semantic configuration data.
*/
class configuration {
public:
    classification_rules&           rules       ()         { return rules_; }
    const classification_rules&     rules       () const   { return rules_; }
    classification_rules*           rules_ptr   ()         { return &rules_; }
    const classification_rules*     rules_ptr   () const   { return &rules_; }

    semantic_data_resources&        data_resources   ()         { return data_resources_; }
    const semantic_data_resources&  data_resources   () const   { return data_resources_; }
private:

    classification_rules        rules_;
    semantic_data_resources     data_resources_;
};

} //end namespace cpaf::semantic

#endif //CPAF_CONFIGURATION_H
