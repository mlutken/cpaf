#ifndef PHPDOC_WHITESPACEANDCOMMENTS_H
#define PHPDOC_WHITESPACEANDCOMMENTS_H

#include "Group.h"

namespace phpdoc {

class WhiteSpaceAndComments : public Group
{
    PHPDOC_NODE_HDR(WhiteSpaceAndComments,Group)
public:
    WhiteSpaceAndComments();

    WhiteSpaceAndComments* whiteSpaceSet (const std::string& val);
private:
    virtual void        do_valueSet     (const std::string& val);
    virtual bool        do_parse        (Iterator& first, const Iterator& last) override;

};

} // namespace phpdoc

#endif // PHPDOC_WHITESPACEANDCOMMENTS_H
