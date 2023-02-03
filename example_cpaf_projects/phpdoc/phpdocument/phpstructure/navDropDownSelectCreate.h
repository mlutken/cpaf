#ifndef PHPDOC_NAVDROPDOWNSELECTCREATE_H
#define PHPDOC_NAVDROPDOWNSELECTCREATE_H

#include "NavOptionCreate.h"

namespace phpdoc {

class navDropDownSelectCreate : public NavOptionCreate
{
    PHPDOC_NODE_HDR(navDropDownSelectCreate,NavOptionCreate)
public:
    static const unsigned int ACTION_NAME_POS  = 0;

    navDropDownSelectCreate();

private:
    void        do_valueSet         ( const std::string& val ) override;
    std::string do_valueGet         () const override;
    bool        do_hasParameter     ( unsigned int parameterPos ) const override;
    void        do_actionNameSet    ( const std::string& actionName ) override;
    std::string do_actionNameGet    () const override;

};

} // namespace phpdoc

#endif // PHPDOC_NAVDROPDOWNSELECTCREATE_H
