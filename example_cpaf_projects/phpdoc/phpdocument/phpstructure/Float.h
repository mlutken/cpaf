#ifndef PHPDOC_FLOAT_H
#define PHPDOC_FLOAT_H

#include "Leaf.h"

namespace phpdoc {

class Float : public Leaf
{
    PHPDOC_NODE_HDR(Float,Leaf)
public:
    static int registerClass();
    static const unsigned int DUMMY_REGISTER_CLASS;
    Float();

    float   val () const            { return m_val; }
    void    valSet (float val)      { m_val = val;  }

private:

    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& val);
    virtual std::string do_valueGet     () const;

    float m_val;
};

} // namespace phpdoc

#endif // PHPDOC_FLOAT_H
