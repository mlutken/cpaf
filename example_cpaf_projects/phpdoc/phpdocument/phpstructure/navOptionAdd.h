#ifndef PHPDOC_NAVOPTIONADD_H
#define PHPDOC_NAVOPTIONADD_H

#include "FunctionCall.h"
#include "navigator_common.h"

namespace phpdoc {

class QuotedStringList;

class navOptionAdd : public FunctionCall
{
    PHPDOC_NODE_HDR(navOptionAdd,FunctionCall)
public:
    static const unsigned int OPTION_PARAMETER_COUNT_POS            = NavigatorConstants::OPTION_PARAMETER_COUNT_POS;
    static const unsigned int OPTION_PARAMETER_VALUE_POS            = NavigatorConstants::OPTION_PARAMETER_VALUE_POS;
    static const unsigned int OPTION_PARAMETER_SEARCH_FOR_POS       = NavigatorConstants::OPTION_PARAMETER_SEARCH_FOR_POS;
    static const unsigned int ATTRIBUTES_MARKER_POS                 = 3;
    static const unsigned int ATTRIBUTES_LIST_POS                   = 4;

    navOptionAdd();

    navOptionAdd*       countSet            ( const std::string& value );
    navOptionAdd*       countSet            ( int value );
    int                 count               () const;

    navOptionAdd*       valueSet            ( const std::string& value );
    std::string         value               () const;

    navOptionAdd*       searchForSet        ( const std::string& searchFor );
    std::string         searchFor           () const;

    QuotedStringList*   attributeValues     () const;
    navOptionAdd*       attributeValuesSet  (const std::string& values);

    navOptionAdd*       hasAttributesSet    (bool hasAttributes);

};

} // namespace phpdoc

#endif // PHPDOC_NAVOPTIONADD_H
