#include "NavWheel.h"

#include "NavOptionCreate.h"
#include "navAttributeNamesCreate.h"
#include "QuotedStringList.h"
#include "navOptionAdd.h"

namespace phpdoc {

PHPDOC_NODE_CPP(NavWheel,Group)

NavWheel::NavWheel()
{
    appendNode( NavOptionCreate::create() );
    appendNode( navAttributeNamesCreate::create() );
    childEnabledSet(false, WHEEL_ACTION_CREATE_POS);
    childEnabledSet(false, ATTRIBUTE_NAMES_POS);
}

NavWheel* NavWheel::optionCreateSet(NavOptionCreate* optionCreate)
{
    replaceNode(optionCreate, WHEEL_ACTION_CREATE_POS);
    childEnabledSet(true, WHEEL_ACTION_CREATE_POS);
    return this;
}

NavOptionCreate* NavWheel::optionCreate() const
{
    return get<NavOptionCreate>(WHEEL_ACTION_CREATE_POS);
}

NavWheel* NavWheel::attributeNamesSet(navAttributeNamesCreate* attributeNames)
{
    replaceNode(attributeNames, ATTRIBUTE_NAMES_POS);
    childEnabledSet(true, ATTRIBUTE_NAMES_POS);
    attributeNames->attributeNames()->reserveAndAssign(4,"");
    return this;
}

navAttributeNamesCreate* NavWheel::attributeNames() const
{
    return get<navAttributeNamesCreate>(ATTRIBUTE_NAMES_POS);
}

NavWheel* NavWheel::actionNameSet(const std::string& actionName)
{
    get<NavOptionCreate>(WHEEL_ACTION_CREATE_POS)->actionNameSet(actionName);
    return this;
}

std::string NavWheel::actionName() const
{
    return get<NavOptionCreate>(WHEEL_ACTION_CREATE_POS)->actionName();
}

NavWheel* NavWheel::optionAppend(navOptionAdd* option)
{
    appendNode(option);
    return this;
}

NavWheel* NavWheel::optionSet(navOptionAdd* option, unsigned int pos)
{
    if ( optionsBeginPos() <= pos && pos < optionsEndPos()) replaceNode(option, pos);
    return this;
}

navOptionAdd* NavWheel::option(unsigned int pos) const
{
    if ( optionsBeginPos() <= pos && pos < optionsEndPos()) return get<navOptionAdd>(pos);
    return nullptr;
}

unsigned int NavWheel::optionsCount() const
{
    return size() > FIRST_OPTION_POS ? size() - FIRST_OPTION_POS : 0;
}

unsigned int NavWheel::optionsBeginPos() const
{
    return FIRST_OPTION_POS;
}

unsigned int NavWheel::optionsEndPos() const
{
    return FIRST_OPTION_POS + optionsCount();
}

} // namespace phpdoc
