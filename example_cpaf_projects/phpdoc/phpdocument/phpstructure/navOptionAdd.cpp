#include "navOptionAdd.h"

#include "Integer.h"
#include "IntegerConstant.h"
#include "QuotedString.h"
#include "QuotedStringList.h"
#include "CallParameter.h"

namespace phpdoc {

PHPDOC_NODE_CPP(navOptionAdd,FunctionCall)

navOptionAdd::navOptionAdd()
{
    functionNameSet("navOptionAdd");
    parameterAdd ( CallParameter::create()->parameterSet(Integer::create())->whiteSpaceBeforeSet(" ") );
    parameterAdd ( CallParameter::create()->parameterSet(QuotedString::create())->whiteSpaceBeforeSet(" ") );
    parameterAdd ( CallParameter::create()->parameterSet(QuotedString::create())->whiteSpaceBeforeSet(" ") );
    parameterAdd ( CallParameter::create()->parameterSet(IntegerConstant::create()->nameSet("ATTR"))->whiteSpaceBeforeSet(" ") );
    parameterAdd ( CallParameter::create()->parameterSet(QuotedStringList::create())->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ") );
    wsBeforeStatementSet("\t");
    allParametersEnabledSet(false);
    attributeValues()->reserveAndAssign(4, "");
}

navOptionAdd* navOptionAdd::countSet(const std::string& value)
{
    CallParameter* cp = callParameter(OPTION_PARAMETER_COUNT_POS);
    cp->parameter<Integer>()->valueSet(value);
    cp->enabledSet(true);
    return this;
}

navOptionAdd* navOptionAdd::countSet(int value)
{
    CallParameter* cp = callParameter(OPTION_PARAMETER_COUNT_POS);
    cp->parameter<Integer>()->valueIntSet(value);
    cp->enabledSet(true);
    return this;
}

int navOptionAdd::count() const
{
    CallParameter* cp = callParameter(OPTION_PARAMETER_COUNT_POS);
    return cp->parameter<Integer>()->valueInt();
}

navOptionAdd* navOptionAdd::valueSet(const std::string& value)
{
    CallParameter* cp = callParameter(OPTION_PARAMETER_VALUE_POS);
    cp->parameter<QuotedString>()->valueSet(value);
    cp->enabledSet(true);
    return this;
}

std::string navOptionAdd::value() const
{
    CallParameter* cp = callParameter(OPTION_PARAMETER_VALUE_POS);
    return cp->parameter<QuotedString>()->value();
}

navOptionAdd* navOptionAdd::searchForSet(const std::string& searchFor)
{
    CallParameter* cp = callParameter(OPTION_PARAMETER_SEARCH_FOR_POS);
    cp->parameter<QuotedString>()->valueSet(searchFor);
    cp->enabledSet(true);
    return this;

}

std::string navOptionAdd::searchFor() const
{
    CallParameter* cp = callParameter(OPTION_PARAMETER_SEARCH_FOR_POS);
    return cp->parameter<QuotedString>()->value();
}


QuotedStringList* navOptionAdd::attributeValues() const
{
    CallParameter* cp = callParameter(ATTRIBUTES_LIST_POS);
    if (cp) {
        return cp->parameter<QuotedStringList>();
    }
    return nullptr;
}

navOptionAdd* navOptionAdd::attributeValuesSet(const std::string& values)
{
    CallParameter* cp = callParameter(ATTRIBUTES_LIST_POS);
    if (cp) {
        cp->parameter<QuotedStringList>()->valueSet(values);
        hasAttributesSet(true);
    }
    attributeValues()->reserveAndAssign(4, "");
    return this;
}

navOptionAdd* navOptionAdd::hasAttributesSet(bool hasAttributes)
{
    parameterEnabledSet( ATTRIBUTES_MARKER_POS, hasAttributes);
    parameterEnabledSet( ATTRIBUTES_LIST_POS, hasAttributes);
}


} // namespace phpdoc
