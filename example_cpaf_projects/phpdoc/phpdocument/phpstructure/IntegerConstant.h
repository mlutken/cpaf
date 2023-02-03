#ifndef PHPDOC_CONSTANT_H
#define PHPDOC_CONSTANT_H


#include "Integer.h"

namespace phpdoc {

class IntegerConstant : public Integer
{
    PHPDOC_NODE_HDR(IntegerConstant,Integer)
public:
    IntegerConstant();

private:
    virtual std::string do_docStringGet () const;
    virtual void        do_nameSet      (const std::string& name);
    virtual std::string do_nameGet      () const;

    std::string m_name;
};

} // namespace phpdoc

#endif // PHPDOC_CONSTANT_H
