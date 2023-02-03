#ifndef PHPDOC_CODEBLOP_H
#define PHPDOC_CODEBLOP_H

#include "Statement.h"

namespace phpdoc {

class String;

class CodeBlop : public Statement
{
    PHPDOC_NODE_HDR(CodeBlop,Statement)
public:
    static const unsigned int BLOP_POS = 1;
    CodeBlop();
    CodeBlop*   blopSet ( const std::string& val );
    std::string blop    () const;

private:
    virtual void        do_valueSet     (const std::string& val);

};

} // namespace phpdoc

#endif // PHPDOC_CODEBLOP_H
