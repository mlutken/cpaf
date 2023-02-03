#ifndef PHPDOC_INTEGER_H
#define PHPDOC_INTEGER_H

#include "Leaf.h"

namespace phpdoc {

class Integer : public Leaf
{
    PHPDOC_NODE_HDR(Integer,Leaf)
public:
    Integer();

    int         valueInt     () const    { return m_val; }
    Integer*    valueIntSet  (int val);

private:

    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& val);
    virtual std::string do_valueGet     () const;


    int m_val;
};

} // namespace phpdoc

#endif // PHPDOC_INTEGER_H
