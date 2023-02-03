#include "ReturnQuotedStringStatement.h"


#include "Semicolon.h"
#include "WhiteSpaceAndComments.h"
#include "Word.h"

namespace phpdoc {

PHPDOC_NODE_CPP(ReturnQuotedStringStatement,ReturnStatement)


ReturnQuotedStringStatement::ReturnQuotedStringStatement()
{
    appendNode(Word::create()->valueSet("return"));
    appendNode(WhiteSpaceAndComments::create()->valueSet(" "));
    appendNode(QuotedString::create()->valueSet(""));
}

QuotedString::QuoteTypeE ReturnQuotedStringStatement::quoteType() const
{
    return get<QuotedString>(QUOTED_STRING_POS)->quoteType();
}

ReturnQuotedStringStatement* ReturnQuotedStringStatement::quoteTypeSet(const QuotedString::QuoteTypeE& quoteType)
{
    get<QuotedString>(QUOTED_STRING_POS)->quoteTypeSet(quoteType);
    return this;
}

void ReturnQuotedStringStatement::do_valueSet(const std::string& val)
{
    get<QuotedString>(QUOTED_STRING_POS)->valueSet(val);
}

} // namespace phpdoc
