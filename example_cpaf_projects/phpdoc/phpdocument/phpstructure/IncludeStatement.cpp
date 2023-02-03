#include "IncludeStatement.h"

#include "Semicolon.h"
#include "WhiteSpaceAndComments.h"
#include "Word.h"

namespace phpdoc {

PHPDOC_NODE_CPP(IncludeStatement,Statement)

IncludeStatement::IncludeStatement()
{
    appendNode(Word::create()->valueSet(""));
    appendNode(Word::create()->valueSet(""));
    appendNode(WhiteSpaceAndComments::create()->valueSet(" "));
    appendNode(QuotedString::create()->valueSet(""));

    isOnceSet(true);
    isRequireSet(true);
    quoteTypeSet(QuotedString::SingleQuoteE);
}

bool IncludeStatement::isOnce() const
{
    return m_isOnce;
}

IncludeStatement* IncludeStatement::isOnceSet(bool isOnce)
{
    m_isOnce = isOnce;
    replaceNode(Word::create()->valueSet( m_isOnce ? "_once" : ""), IS_ONCE_POS );
    return this;
}

bool IncludeStatement::isRequire() const
{
    return m_isRequire;
}

IncludeStatement* IncludeStatement::isRequireSet(bool isRequire)
{
    m_isRequire = isRequire;
    replaceNode(Word::create()->valueSet( m_isRequire ? "require" : "include"), INCLUDE_WORD_POS );
    return this;
}

QuotedString::QuoteTypeE IncludeStatement::quoteType() const
{
    return get<QuotedString>(INCLUDE_FILE_POS)->quoteType();
}

IncludeStatement* IncludeStatement::quoteTypeSet(const QuotedString::QuoteTypeE& quoteType)
{
    get<QuotedString>(INCLUDE_FILE_POS)->quoteTypeSet(quoteType);
    return this;
}

void IncludeStatement::do_valueSet(const std::string& val)
{
    get<QuotedString>(INCLUDE_FILE_POS)->valueSet(val);
}


} // namespace phpdoc

