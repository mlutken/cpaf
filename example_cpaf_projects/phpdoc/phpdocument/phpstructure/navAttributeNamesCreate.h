#ifndef PHPDOC_NAVATTRIBUTENAMESCREATE_H
#define PHPDOC_NAVATTRIBUTENAMESCREATE_H

#include "FunctionCall.h"

namespace phpdoc {

class QuotedStringList;

class navAttributeNamesCreate : public FunctionCall
{
    PHPDOC_NODE_HDR(navAttributeNamesCreate,FunctionCall)
public:
    static const unsigned int ATTRIBUTES_LIST_POS   = 0;
    navAttributeNamesCreate();

    QuotedStringList* attributeNames() const;

private:
    void        do_valueSet     (const std::string& val) override;
    std::string do_valueGet     () const override;

};

} // namespace phpdoc

#endif // PHPDOC_NAVATTRIBUTENAMESCREATE_H
