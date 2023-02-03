#include "Statement.h"

#include <iostream>

#include "Semicolon.h"
#include "WhiteSpaceAndComments.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(Statement,Group)

Statement::Statement()
    : m_endsWithSemicolon           (true)
    , m_endingSemicolon             (Semicolon::create())
    , m_whiteSpaceBeforeSemicolon   (WhiteSpaceAndComments::create()->whiteSpaceSet(""))
    , m_whiteSpaceAfterSemicolon    (WhiteSpaceAndComments::create()->whiteSpaceSet("\n"))
    , m_whiteSpaceBeforeStatement   (WhiteSpaceAndComments::create()->whiteSpaceSet(""))
{
}

Statement::~Statement()
{
    delete m_endingSemicolon;
    delete m_whiteSpaceBeforeSemicolon;
    delete m_whiteSpaceAfterSemicolon;
    delete m_whiteSpaceBeforeStatement;
}

Statement* Statement::wsBeforeStatementSet(const string& ws)
{
    m_whiteSpaceBeforeStatement->whiteSpaceSet(ws);
    return this;
}

bool Statement::endsWithSemicolon() const
{
    return m_endsWithSemicolon;
}

Statement* Statement::endsWithSemicolonSet(bool endsWithSemicolon)
{
    m_endsWithSemicolon = endsWithSemicolon;
    return this;
}

Statement* Statement::wsBeforeSemicolonSet(const string& ws)
{
    m_whiteSpaceBeforeSemicolon->whiteSpaceSet(ws);
    return this;
}

Statement* Statement::wsAfterSemicolonSet(const string& ws)
{
    endsWithSemicolonSet(true);
    m_whiteSpaceAfterSemicolon->whiteSpaceSet(ws);
    return this;
}

string Statement::do_docStringGet() const
{
    string s;
    s += m_whiteSpaceBeforeStatement->docString();
    for (Node* pNode: children() ) {
        s += pNode->docString();
    }
    s += m_whiteSpaceBeforeSemicolon->docString();
    if (endsWithSemicolon()) {
        s += m_endingSemicolon->docString();
        s += m_whiteSpaceAfterSemicolon->docString();
    }
    return s;
}

} // namespace phpdoc



