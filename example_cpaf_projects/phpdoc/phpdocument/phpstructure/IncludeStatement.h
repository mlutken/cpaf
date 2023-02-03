#ifndef PHPDOC_INCLUDESTATEMENT_H
#define PHPDOC_INCLUDESTATEMENT_H

#include "Statement.h"
#include "QuotedString.h"

namespace phpdoc {

class IncludeStatement : public Statement
{
    PHPDOC_NODE_HDR(IncludeStatement,Statement)
public:
    static const unsigned int INCLUDE_WORD_POS   = 0;
    static const unsigned int IS_ONCE_POS        = 1;
    static const unsigned int INCLUDE_FILE_POS   = 3;

    IncludeStatement();
    QuotedString::QuoteTypeE    quoteType       () const;
    IncludeStatement*           quoteTypeSet    (const QuotedString::QuoteTypeE& quoteType);

    bool                        isOnce          () const;
    IncludeStatement*           isOnceSet       (bool isOnce);
    bool                        isRequire       () const;
    IncludeStatement*           isRequireSet    (bool isOnce);

private:

    virtual void        do_valueSet     (const std::string& val);

    bool    m_isOnce;
    bool    m_isRequire;
};

} // namespace phpdoc

#endif // PHPDOC_INCLUDESTATEMENT_H
