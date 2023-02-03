#ifndef PHPDOC_SEMICOLON_H
#define PHPDOC_SEMICOLON_H

#include "Leaf.h"

namespace phpdoc {

class Semicolon : public Leaf
{
    PHPDOC_NODE_HDR(Semicolon,Leaf)
public:
private:
    // --- PRIVATE: Functions ---
    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& ) {}

};

} // namespace phpdoc

#endif // PHPDOC_SEMICOLON_H
