#ifndef PHPDOC_STATEMENT_H
#define PHPDOC_STATEMENT_H

#include "Group.h"

namespace phpdoc {

class Semicolon;
class WhiteSpaceAndComments;

class Statement : public Group
{
    PHPDOC_NODE_HDR(Statement,Group)
public:
    Statement();
    virtual ~Statement() override;


    Statement*  wsBeforeStatementSet    ( const std::string& ws );
    Statement*  wsBeforeSemicolonSet    ( const std::string& ws );
    Statement*  wsAfterSemicolonSet     ( const std::string& ws );

    bool        endsWithSemicolon           () const;
    Statement*  endsWithSemicolonSet        ( bool endsWithSemicolon);

private:
    virtual std::string do_docStringGet () const override;

    bool                    m_endsWithSemicolon;
    Semicolon*              m_endingSemicolon;
    WhiteSpaceAndComments*  m_whiteSpaceBeforeSemicolon;
    WhiteSpaceAndComments*  m_whiteSpaceAfterSemicolon;
    WhiteSpaceAndComments*  m_whiteSpaceBeforeStatement;
};

} // namespace phpdoc

#endif // PHPDOC_STATEMENT_H
