#include "QuotedStringBase.h"


using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(QuotedStringBase,Group)

QuotedStringBase::QuotedStringBase()
    : m_quoteType(DoubleQuoteE)
{
}

QuotedStringBase::QuoteTypeE QuotedStringBase::quoteType() const
{
    return m_quoteType;
}

QuotedStringBase* QuotedStringBase::quoteTypeSet(const QuoteTypeE& quoteType)
{
    m_quoteType = quoteType;
    return this;
}


} // namespace phpdoc
