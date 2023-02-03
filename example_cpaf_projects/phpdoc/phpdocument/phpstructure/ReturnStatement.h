#ifndef PHPDOC_RETURNSTATEMENT_H
#define PHPDOC_RETURNSTATEMENT_H

#include "Statement.h"

namespace phpdoc {

class ReturnStatement : public Statement
{
PHPDOC_NODE_HDR(ReturnStatement,Statement)

public:
    ReturnStatement();
};

} // namespace phpdoc

#endif // PHPDOC_RETURNSTATEMENT_H
