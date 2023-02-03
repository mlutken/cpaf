#ifndef PHPDOC_WHITESPACE_H
#define PHPDOC_WHITESPACE_H

#include "Group.h"

namespace phpdoc {

class WhiteSpace : public Group
{
    PHPDOC_NODE_HDR(WhiteSpace,Group)
public:
    WhiteSpace();

    static bool isWhiteSpace    ( char ch );
    static bool isWhiteSpace    ( const std::string& ch );

private:
    virtual void        do_valueSet     (const std::string& val);
    virtual bool        do_parse        (Iterator& first, const Iterator& last) override;

};

} // namespace phpdoc

#endif // PHPDOC_WHITESPACE_H
