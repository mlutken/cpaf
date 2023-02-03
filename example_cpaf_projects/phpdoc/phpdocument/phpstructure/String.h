#ifndef PHPDOC_STRING_H
#define PHPDOC_STRING_H

#include "Leaf.h"

namespace phpdoc {

class String : public Leaf
{
    PHPDOC_NODE_HDR(String,Leaf)
public:
    String() {}
    explicit String(const std::string& val)
      : m_val(val)
    {
    }

private:
    virtual std::string do_docStringGet () const override;
    virtual void        do_valueSet     (const std::string& val) { m_val = val; }
    virtual bool        do_parse        (Iterator& first, const Iterator& last) override;

    std::string m_val;
};

} // namespace phpdoc

#endif // PHPDOC_STRING_H
