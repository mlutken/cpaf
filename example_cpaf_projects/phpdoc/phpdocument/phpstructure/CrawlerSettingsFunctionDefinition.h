#ifndef PHPDOC_CRAWLERSETTINGSFUNCTIONDEFINITION_H
#define PHPDOC_CRAWLERSETTINGSFUNCTIONDEFINITION_H

#include "FunctionDefinition.h"
#include "QuotedString.h"

namespace phpdoc {

class ReturnQuotedStringStatement;

class CrawlerSettingsFunctionDefinition : public FunctionDefinition
{
    PHPDOC_NODE_HDR(CrawlerSettingsFunctionDefinition,FunctionDefinition)
public:
    CrawlerSettingsFunctionDefinition();

    CrawlerSettingsFunctionDefinition*  wsBeforeStatementSet    ( const std::string& ws );
    CrawlerSettingsFunctionDefinition*  wsBeforeSemicolonSet    ( const std::string& ws );
    CrawlerSettingsFunctionDefinition*  wsAfterSemicolonSet     ( const std::string& ws );

    ReturnQuotedStringStatement*        statement() const;

    QuotedString::QuoteTypeE            quoteType       () const;
    CrawlerSettingsFunctionDefinition*  quoteTypeSet    (const QuotedString::QuoteTypeE& quoteType);


private:
    virtual void        do_valueSet     (const std::string& val);
};

} // namespace phpdoc

#endif // PHPDOC_CRAWLERSETTINGSFUNCTIONDEFINITION_H
