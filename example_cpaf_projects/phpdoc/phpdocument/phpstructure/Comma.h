#ifndef PHPDOC_COMMA_H
#define PHPDOC_COMMA_H

#include "Leaf.h"

namespace phpdoc {

class Comma : public Leaf
{
    PHPDOC_NODE_HDR(Comma,Leaf)
public:
private:
    // --- PRIVATE: Functions ---
    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& ) {}

};

} // namespace phpdoc

#endif // PHPDOC_COMMA_H
