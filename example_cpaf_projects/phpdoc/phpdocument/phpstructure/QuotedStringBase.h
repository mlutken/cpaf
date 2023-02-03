#ifndef PHPDOC_QUOTEDSTRINGBASE_H
#define PHPDOC_QUOTEDSTRINGBASE_H

#include "Group.h"

namespace phpdoc {

class QuotedStringBase : public Group
{
    PHPDOC_NODE_HDR(QuotedStringBase,Group)
public:
    enum QuoteTypeE { SingleQuoteE, DoubleQuoteE };
public:
    QuotedStringBase();

    QuoteTypeE          quoteType       () const;
    QuotedStringBase*   quoteTypeSet    (const QuoteTypeE& quoteType);

//    static bool

private:
    QuoteTypeE      m_quoteType;

};

} // namespace phpdoc

#endif // PHPDOC_QUOTEDSTRINGBASE_H
