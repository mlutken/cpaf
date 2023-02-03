#include "FunctionCall.h"

#include <phpstructure/Globals.h>
#include "braces.h"
#include "WhiteSpaceAndComments.h"
#include "Identifier.h"
#include "Integer.h"
#include "ParametersCallList.h"
#include "Semicolon.h"

using namespace std;


namespace phpdoc {

PHPDOC_NODE_CPP(FunctionCall,Statement)

FunctionCall::FunctionCall()
{
    appendNode(WhiteSpaceAndComments::create()->valueSet(""));
    appendNode(Identifier::create()->valueSet("newFunctionCall"));
    appendNode(WhiteSpaceAndComments::create()->valueSet(" "));
    appendNode(ParLeft::create());
    appendNode(WhiteSpaceAndComments::create()->valueSet(""));
    appendNode(ParametersCallList::create());
    appendNode(WhiteSpaceAndComments::create()->valueSet(""));
    appendNode(ParRight::create());
    appendNode(WhiteSpaceAndComments::create()->valueSet(""));
}

bool FunctionCall::hasSemiColon() const
{
    return size() > SEMICOLON_POS;
}

void FunctionCall::functionNameSet(const string& name)
{
    Identifier* id = get<Identifier>(FUNCTION_NAME_POS);
    id->valueSet(name);
}

string FunctionCall::functionName() const
{
    return get<Identifier>(FUNCTION_NAME_POS)->value();

}

ParametersCallList* FunctionCall::parameters() const
{
    return get<ParametersCallList>(PARAMETERS_POS);
}

FunctionCall* FunctionCall::parametersSet(ParametersCallList* callParams)
{
    replaceNode(callParams, PARAMETERS_POS);
    return this;
}

FunctionCall* FunctionCall::parameterAdd(CallParameter* callParam)
{
    parameters()->parameterAdd(callParam);
    return this;
}

FunctionCall* FunctionCall::parameterSet(CallParameter* callParam, unsigned int pos)
{
    parameters()->parameterSet(callParam, pos);
    return this;
}

CallParameter* FunctionCall::callParameter(unsigned int pos) const
{
    return parameters()->callParameter(pos);
}

FunctionCall* FunctionCall::parameterQuotedStringSet(const string& value, unsigned int pos)
{
    parameters()->parameterQuotedStringSet(value, pos);
    return this;
}

FunctionCall* FunctionCall::parameterQuotedStringSet(const string& value, unsigned int pos, QuotedStringBase::QuoteTypeE quoteType )
{
    parameters()->parameterQuotedStringSet(value, pos, quoteType);
    return this;
}


string FunctionCall::parameterQuotedString( unsigned int pos ) const
{
    return parameters()->parameterQuotedString(pos);
}

FunctionCall* FunctionCall::parameterIntSet(int value, unsigned int pos)
{
    parameters()->parameterIntSet(value, pos);
    return this;
}

FunctionCall* FunctionCall::parameterIntSet(const string& value, unsigned int pos)
{
    parameters()->parameterIntSet(value, pos);
    return this;
}

int FunctionCall::parameterInt(unsigned int pos) const
{
    return parameters()->parameterInt(pos);
}

FunctionCall* FunctionCall::allParametersEnabledSet(bool enabled)
{
    parameters()->allParametersEnabledSet(enabled);
    return this;
}

FunctionCall* FunctionCall::parameterEnabledSet(unsigned int pos, bool enabled)
{
    parameters()->parameterEnabledSet(pos, enabled);
    return this;
}

bool FunctionCall::parameterEnabled(unsigned int pos) const
{
    return parameters()->parameterEnabled(pos);
}

unsigned int FunctionCall::enabledParametersCount() const
{
    return parameters()->enabledParametersCount();
}


void FunctionCall::do_nameSet(const string& name)
{
    functionNameSet(name);
}

string FunctionCall::do_nameGet() const
{
    return functionName();
}


} // namespace phpdoc
