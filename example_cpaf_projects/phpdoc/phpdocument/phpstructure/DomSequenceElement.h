#ifndef PHPDOC_DOMSEQUENCEELEMENT_H
#define PHPDOC_DOMSEQUENCEELEMENT_H

#include "Group.h"

namespace phpdoc {

class WhiteSpace;

class DomSequenceElement : public Group
{
    PHPDOC_NODE_HDR(DomSequenceElement,Group)

public:
    static const unsigned int WS_0_POS          = 0;
    static const unsigned int COMPARE_FUN_POS   = 1;
    static const unsigned int STRING_POS        = 2;
    static const unsigned int WS_1_POS          = 3;

    DomSequenceElement();
    DomSequenceElement*      whiteSpaceSet              ( WhiteSpace* wsBefore, WhiteSpace* wsAfter );
    DomSequenceElement*      whiteSpaceBeforeSet        ( WhiteSpace* wsBefore);
    DomSequenceElement*      whiteSpaceAfterSet         ( WhiteSpace* wsAfter );
    DomSequenceElement*      whiteSpaceBeforeSet        ( const std::string& wsBefore );
    DomSequenceElement*      whiteSpaceAfterSet         ( const std::string& wsAfter );

    DomSequenceElement*      stringValueSet             ( const std::string& val );
    std::string              stringValue                () const;
    DomSequenceElement*      compareFunctionSet         ( const std::string& compFun );
    std::string              compareFunction            () const;


private:
    std::string              compareFunctionDocString   () const;

    virtual void        do_valueSet     (const std::string& val);
    virtual std::string do_valueGet     () const;

    virtual bool        do_parse        (Iterator& first, const Iterator& last) override;


};

} // namespace phpdoc

#endif // PHPDOC_DOMSEQUENCEELEMENT_H
