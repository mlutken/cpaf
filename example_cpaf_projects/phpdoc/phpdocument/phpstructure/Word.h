#ifndef PHPDOC_WORD_H
#define PHPDOC_WORD_H

#include "Leaf.h"

#include <string>

namespace phpdoc {

class Word : public Leaf
{
    PHPDOC_NODE_HDR(Word,Leaf)

public:
    Word() {}

    explicit Word(const std::string& val)
      : m_val(val)
    {
    }

private:
    virtual std::string do_docStringGet () const override;
    virtual void        do_valueSet     (const std::string& val) {m_val = val;}

    std::string m_val;
};

} // namespace phpdoc

#endif // PHPDOC_WORD_H
