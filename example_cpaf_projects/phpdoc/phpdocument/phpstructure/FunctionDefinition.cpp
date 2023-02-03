#include "FunctionDefinition.h"

#include <iostream>

#include "braces.h"
#include "Identifier.h"
#include "ParametersDefinitionList.h"
#include "StatementsBlock.h"
#include "Statement.h"
#include "Word.h"
#include "WhiteSpaceAndComments.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(FunctionDefinition,Definition)

FunctionDefinition* FunctionDefinition::createDbg1()
{
    FunctionDefinition* f = new FunctionDefinition;
    f->functionNameSet("myFunction");
    return f;
}

FunctionDefinition::FunctionDefinition()
{
    appendNode(WhiteSpaceAndComments::create()->valueSet(""));      // WS_0_POS = 0;
    appendNode(Word::create()->valueSet("function"));
    appendNode(WhiteSpaceAndComments::create()->valueSet(" "));     // WS_1_POS = 2;
    appendNode(Identifier::create()->valueSet("newFunction"));
    appendNode(WhiteSpaceAndComments::create()->valueSet(" "));     // WS_2_POS = 4;
    appendNode(ParLeft::create());
    appendNode(WhiteSpaceAndComments::create()->valueSet(""));      // WS_3_POS = 6;
    appendNode(ParametersDefinitionList::create());
    appendNode(WhiteSpaceAndComments::create()->valueSet(""));      // WS_4_POS = 8;
    appendNode(ParRight::create());
    appendNode(WhiteSpaceAndComments::create()->valueSet("\n"));    // WS_5_POS = 10;
    appendNode(CurlyBraceLeft::create());
    appendNode(WhiteSpaceAndComments::create()->valueSet("\n"));    // WS_6_POS = 12;
    appendNode(StatementsBlock::create());
    appendNode(WhiteSpaceAndComments::create()->valueSet(""));      // WS_7_POS = 14;
    appendNode(CurlyBraceRight::create());
    appendNode(WhiteSpaceAndComments::create()->valueSet("\n"));    // WS_8_POS = 16;
}

FunctionDefinition* FunctionDefinition::functionNameSet(const string& functionName)
{
    Identifier* id = get<Identifier>(FUNCTION_NAME_POS);
    id->valueSet(functionName);
    return this;
}

FunctionDefinition* FunctionDefinition::statementsSet(StatementsBlock* statements)
{
    replaceNode(statements, STATEMENTS_POS);
    return this;
}

/** If true the function will format itself to use a single line in the source file only.
If set to false the function will use normal formatting style.
\note This only works if the StatementsBlock has only one statement - typically a return statement.*/
FunctionDefinition* FunctionDefinition::oneLinerSet(bool oneLinerFunction)
{
    StatementsBlock* sb = statements();
    if ( sb->size() > 1 ) return this;

    if (oneLinerFunction) {
        replaceNode(WhiteSpaceAndComments::create()->valueSet(" "), WS_5_POS);
        replaceNode(WhiteSpaceAndComments::create()->valueSet(" "), WS_6_POS);
        if (sb->size() > 0) {
            sb->get<Statement>(0)->wsAfterSemicolonSet(" ");
        }
    }
    else {
        replaceNode(WhiteSpaceAndComments::create()->valueSet("\n"), WS_5_POS);
        replaceNode(WhiteSpaceAndComments::create()->valueSet("\n"), WS_6_POS);
        if (sb->size() > 0) sb->get<Statement>(0)->wsAfterSemicolonSet("\n");
    }
    return this;
}

string FunctionDefinition::functionName() const
{
    return get<Identifier>(FUNCTION_NAME_POS)->value();
}

StatementsBlock* FunctionDefinition::statements() const
{
    return get<StatementsBlock>(STATEMENTS_POS);
}

void FunctionDefinition::do_nameSet(const string& name)
{
    Identifier* id = get<Identifier>(FUNCTION_NAME_POS);
    id->valueSet(name);
}

string FunctionDefinition::do_nameGet() const
{
    return get<Identifier>(FUNCTION_NAME_POS)->value();
}

} // namespace phpdoc
