#ifndef PHPDOC_CALLPARAMETER_H
#define PHPDOC_CALLPARAMETER_H

#include "Group.h"
#include "QuotedString.h"

namespace phpdoc {

class WhiteSpaceAndComments;

class CallParameter : public Group
{
    PHPDOC_NODE_HDR(CallParameter,Group)

public:
    static const unsigned int WS_0_POS       = 0;
    static const unsigned int PARAMETER_POS  = 1;
    static const unsigned int WS_1_POS       = 2;

    CallParameter();
    CallParameter*      parameterSet        ( Node* pNode );
    CallParameter*      whiteSpaceSet       ( WhiteSpaceAndComments* wsBefore, WhiteSpaceAndComments* wsAfter );
    CallParameter*      whiteSpaceBeforeSet ( WhiteSpaceAndComments* wsBefore);
    CallParameter*      whiteSpaceAfterSet  ( WhiteSpaceAndComments* wsAfter );
    CallParameter*      whiteSpaceBeforeSet ( const std::string& wsBefore );
    CallParameter*      whiteSpaceAfterSet  ( const std::string& wsAfter );

    CallParameter*      parameterQuotedStringSet    ( const std::string& value );
    CallParameter*      parameterQuotedStringSet    ( const std::string& value, QuotedStringBase::QuoteTypeE quoteType );
    std::string         parameterQuotedString       () const;

    CallParameter*      parameterIntSet  ( int value );
    CallParameter*      parameterIntSet  ( const std::string& value );
    int                 parameterInt     () const;

    Node*               parameter           () const;

    template<typename T>
    T*                  parameter () const
    {
        Node* node = parameter();
        if ((node->docTypes() & T().docType()).any()) return static_cast<T*>(node);
        return nullptr;
    }


private:
    virtual void        do_valueSet     (const std::string& val);

    bool                parseParameter  (Iterator& first, const Iterator& last);

};

} // namespace phpdoc

#endif // PHPDOC_CALLPARAMETER_H
