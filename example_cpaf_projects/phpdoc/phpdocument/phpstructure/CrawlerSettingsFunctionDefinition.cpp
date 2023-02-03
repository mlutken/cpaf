#include "CrawlerSettingsFunctionDefinition.h"

#include "StatementsBlock.h"
#include "ReturnQuotedStringStatement.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(CrawlerSettingsFunctionDefinition,FunctionDefinition)

CrawlerSettingsFunctionDefinition::CrawlerSettingsFunctionDefinition()
    : FunctionDefinition()
{
    StatementsBlock* sb = new StatementsBlock;
    sb->appendNode(ReturnQuotedStringStatement::create()->quoteTypeSet(QuotedString::SingleQuoteE));
    statementsSet(sb);
}

CrawlerSettingsFunctionDefinition* CrawlerSettingsFunctionDefinition::wsBeforeStatementSet(const string &ws)
{
    statement()->wsBeforeStatementSet(ws);
    return this;
}

CrawlerSettingsFunctionDefinition* CrawlerSettingsFunctionDefinition::wsBeforeSemicolonSet(const string &ws)
{
    statement()->wsBeforeSemicolonSet(ws);
    return this;
}

CrawlerSettingsFunctionDefinition* CrawlerSettingsFunctionDefinition::wsAfterSemicolonSet(const string &ws)
{
    statement()->wsAfterSemicolonSet(ws);
    return this;
}

ReturnQuotedStringStatement* CrawlerSettingsFunctionDefinition::statement() const
{
    return get<StatementsBlock>(STATEMENTS_POS)->get<ReturnQuotedStringStatement>(0);
}

QuotedString::QuoteTypeE CrawlerSettingsFunctionDefinition::quoteType() const
{
    return statement()->quoteType();
}

CrawlerSettingsFunctionDefinition* CrawlerSettingsFunctionDefinition::quoteTypeSet(const QuotedString::QuoteTypeE& quoteType)
{
    statement()->quoteTypeSet(quoteType);
    return this;
}

void CrawlerSettingsFunctionDefinition::do_valueSet(const string& val)
{
    statement()->valueSet(val);
}

} // namespace phpdoc
