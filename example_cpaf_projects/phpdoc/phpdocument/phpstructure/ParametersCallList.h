#ifndef PHPDOC_PARAMETERSCALLLIST_H
#define PHPDOC_PARAMETERSCALLLIST_H

#include "Group.h"
#include "QuotedString.h"
#include "CallParameter.h"

namespace phpdoc {

class ParametersCallList : public Group
{
PHPDOC_NODE_HDR(ParametersCallList,Group)

public:
    ParametersCallList();

    ParametersCallList*     parameterAdd            ( CallParameter* pCallParameter );
    ParametersCallList*     parameterSet            ( CallParameter* pCallParameter, unsigned int pos );
    CallParameter*          callParameter           ( unsigned int pos ) const;

    ParametersCallList*     parameterQuotedStringSet( const std::string& value, unsigned int pos );
    ParametersCallList*     parameterQuotedStringSet( const std::string& value, unsigned int pos, QuotedStringBase::QuoteTypeE quoteType );
    std::string             parameterQuotedString   ( unsigned int pos ) const;

    ParametersCallList*     parameterIntSet         ( int value, unsigned int pos );
    ParametersCallList*     parameterIntSet         ( const std::string& value, unsigned int pos );
    int                     parameterInt            ( unsigned int pos ) const;


    ParametersCallList*     allParametersEnabledSet ( bool enabled );
    ParametersCallList*     parameterEnabledSet     ( unsigned int pos , bool enabled );
    bool                    parameterEnabled        ( unsigned int pos ) const;

    unsigned int            enabledParametersCount  () const;
    unsigned int            parametersCount         () const { return size(); }


    template<typename T>
    T* parameter ( unsigned int pos ) const
    {
        CallParameter*  cp = callParameter ( pos ) ;
        return cp->parameter<T>();
    }

private:
    virtual std::string do_docStringGet () const override;

};

} // namespace phpdoc

#endif // PHPDOC_PARAMETERSCALLLIST_H
