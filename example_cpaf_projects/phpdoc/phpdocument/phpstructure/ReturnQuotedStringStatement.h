#ifndef PHPDOC_RETURNQUOTEDSTRINGSTATEMENT_H
#define PHPDOC_RETURNQUOTEDSTRINGSTATEMENT_H

#include "QuotedString.h"
#include "ReturnStatement.h"

namespace phpdoc {

class ReturnQuotedStringStatement : public ReturnStatement
{
PHPDOC_NODE_HDR(ReturnQuotedStringStatement,ReturnStatement)
public:
    static const unsigned int QUOTED_STRING_POS  = 2;

    ReturnQuotedStringStatement();

    QuotedString::QuoteTypeE        quoteType       () const;
    ReturnQuotedStringStatement*    quoteTypeSet    (const QuotedString::QuoteTypeE& quoteType);

private:
    virtual void        do_valueSet     (const std::string& val);

};

} // namespace phpdoc

#endif // PHPDOC_RETURNQUOTEDSTRINGSTATEMENT_H
