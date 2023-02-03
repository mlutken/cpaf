#ifndef PHPDOC_VARIABLE_H
#define PHPDOC_VARIABLE_H

#include "Identifier.h"

namespace phpdoc {

class Variable : public Identifier
{
    PHPDOC_NODE_HDR(Variable,Identifier)
public:

private:

    virtual std::string do_docStringGet () const override;
};

} // namespace phpdoc

#endif // PHPDOC_VARIABLE_H
