#include "settingSet.h"

#include <phpstructure/CallParameter.h>
#include <phpstructure/QuotedString.h>

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(settingSet,FunctionCall)


settingSet::settingSet()
{
    functionNameSet("settingSet");
    parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet(""))->whiteSpaceBeforeSet(" ")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet(""))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
            )->wsBeforeStatementSet("\t")
            ;
}

void settingSet::do_valueSet(const std::string& val)
{
    CallParameter* cp = callParameter(SETTING_VALUE_POS);
    if (cp) {
        cp->parameter<QuotedString>()->valueSet(val);
    }
}

void settingSet::do_nameSet(const std::string& name)
{
    CallParameter* cp = callParameter(SETTING_NAME_POS);
    if (cp) {
        cp->parameter<QuotedString>()->valueSet(name);
    }
}

std::string settingSet::do_valueGet() const
{
    CallParameter* cp = callParameter(SETTING_VALUE_POS);
    if (cp) {
        return cp->parameter<QuotedString>()->value();
    }
    return "";
}

std::string settingSet::do_nameGet() const
{
    CallParameter* cp = callParameter(SETTING_NAME_POS);
    if (cp) {
        return cp->parameter<QuotedString>()->value();
    }
    return "";
}

} // namespace phpdoc
