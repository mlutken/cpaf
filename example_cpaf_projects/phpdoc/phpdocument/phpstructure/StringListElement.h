#ifndef PHPDOC_STRINGLISTELEMENT_H
#define PHPDOC_STRINGLISTELEMENT_H

#include "Group.h"

namespace phpdoc {

class WhiteSpace;

class StringListElement : public Group
{
    PHPDOC_NODE_HDR(StringListElement,Group)

public:
    static const unsigned int WS_0_POS       = 0;
    static const unsigned int STRING_POS     = 1;
    static const unsigned int WS_1_POS       = 2;

    StringListElement();
    StringListElement*      whiteSpaceSet       ( WhiteSpace* wsBefore, WhiteSpace* wsAfter );
    StringListElement*      whiteSpaceBeforeSet ( WhiteSpace* wsBefore);
    StringListElement*      whiteSpaceAfterSet  ( WhiteSpace* wsAfter );
    StringListElement*      whiteSpaceBeforeSet ( const std::string& wsBefore );
    StringListElement*      whiteSpaceAfterSet  ( const std::string& wsAfter );



private:
    virtual void        do_valueSet     (const std::string& val);
    virtual std::string do_valueGet     () const;

    virtual bool        do_parse        (Iterator& first, const Iterator& last) override;

};

} // namespace phpdoc

#endif // PHPDOC_STRINGLISTELEMENT_H
