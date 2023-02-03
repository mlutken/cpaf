#ifndef PHPDOC_IDENTIFIER_H
#define PHPDOC_IDENTIFIER_H

#include "Leaf.h"

#include <string>

namespace phpdoc {

class Identifier : public Leaf
{
    PHPDOC_NODE_HDR(Identifier,Leaf)

public:
    Identifier() {}
    explicit Identifier(const std::string& val)
      : m_val(val)
    {
    }

    std::string identifierName () const { return m_val; }

private:
    virtual std::string do_docStringGet () const override;
    virtual void        do_valueSet     (const std::string& val) { m_val = val; }

    std::string m_val;
};

} // namespace phpdoc

#endif // PHPDOC_IDENTIFIER_H
