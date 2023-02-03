#ifndef PHPDOC_FUNCTIONCALL_H
#define PHPDOC_FUNCTIONCALL_H

#include "Statement.h"
#include "CallParameter.h"
#include "QuotedString.h"

namespace phpdoc {

class ParametersCallList;

class FunctionCall : public Statement
{
PHPDOC_NODE_HDR(FunctionCall,Statement)

public:
    static const unsigned int FUNCTION_NAME_POS  = 1;
    static const unsigned int PARAMETERS_POS     = 5;
    static const unsigned int SEMICOLON_POS      = 9;

    FunctionCall                        ();
    bool                hasSemiColon    () const;
    void                functionNameSet (const std::string& name);
    std::string         functionName    () const;

    ParametersCallList* parameters      () const;
    FunctionCall*       parametersSet   ( ParametersCallList* callParams );
    FunctionCall*       parameterAdd    ( CallParameter* callParam );
    FunctionCall*       parameterSet    ( CallParameter* callParam, unsigned int pos );
    CallParameter*      callParameter   ( unsigned int pos ) const;


    FunctionCall*       parameterQuotedStringSet    ( const std::string& value, unsigned int pos );
    FunctionCall*       parameterQuotedStringSet    ( const std::string& value, unsigned int pos, QuotedStringBase::QuoteTypeE quoteType );
    std::string         parameterQuotedString       ( unsigned int pos ) const;

    FunctionCall*       parameterIntSet         ( int value, unsigned int pos );
    FunctionCall*       parameterIntSet         ( const std::string& value, unsigned int pos );
    int                 parameterInt            ( unsigned int pos) const;

    FunctionCall*       allParametersEnabledSet ( bool enabled );
    FunctionCall*       parameterEnabledSet     (unsigned int pos , bool enabled);
    bool                parameterEnabled        (unsigned int pos) const;

    unsigned int        enabledParametersCount  () const;

    template<typename T>
    T* parameter ( unsigned int pos ) const
    {
        CallParameter*  cp = callParameter ( pos ) ;
        return cp->parameter<T>();
    }


private:
    virtual void        do_nameSet  (const std::string& name) override;
    virtual std::string do_nameGet  () const override;

};

} // namespace phpdoc

#endif // PHPDOC_FUNCTIONCALL_H
