#ifndef PHPDOC_FUNCTIONDEFINITION_H
#define PHPDOC_FUNCTIONDEFINITION_H

#include "Definition.h"


namespace phpdoc {
    class StatementsBlock;

class FunctionDefinition : public Definition
{
PHPDOC_NODE_HDR(FunctionDefinition,Definition)

public:
    static const unsigned int FUNCTION_NAME_POS  = 3;
    static const unsigned int PARAMETERS_POS     = 7;
    static const unsigned int STATEMENTS_POS     = 13;

    static const unsigned int WS_0_POS = 0;
    static const unsigned int WS_1_POS = 2;
    static const unsigned int WS_2_POS = 4;
    static const unsigned int WS_3_POS = 6;
    static const unsigned int WS_4_POS = 8;
    static const unsigned int WS_5_POS = 10;
    static const unsigned int WS_6_POS = 12;
    static const unsigned int WS_7_POS = 14;
    static const unsigned int WS_8_POS = 16;

    static FunctionDefinition* createDbg1();
    FunctionDefinition();

    FunctionDefinition* functionNameSet (const std::string& functionName);
    FunctionDefinition* statementsSet   (StatementsBlock* statements);
    FunctionDefinition* oneLinerSet     (bool oneLinerFunction);

    std::string         functionName    () const;
    StatementsBlock*    statements      () const;

private:
    virtual void        do_nameSet      (const std::string& name);
    virtual std::string do_nameGet      () const;

};

} // namespace phpdoc

#endif // PHPDOC_FUNCTIONDEFINITION_H
