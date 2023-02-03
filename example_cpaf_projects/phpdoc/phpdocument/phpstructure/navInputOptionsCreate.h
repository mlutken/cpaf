#ifndef PHPDOC_NAVINPUTOPTIONSCREATE_H
#define PHPDOC_NAVINPUTOPTIONSCREATE_H

#include "NavOptionCreate.h"

namespace phpdoc {

class navInputOptionsCreate : public NavOptionCreate
{
    PHPDOC_NODE_HDR(navInputOptionsCreate,NavOptionCreate)
public:
    static const unsigned int ACTION_NAME_POS  = 0;

    navInputOptionsCreate();


private:
    void        do_valueSet         ( const std::string& val ) override;
    std::string do_valueGet         () const override;
    bool        do_hasParameter     ( unsigned int parameterPos ) const override;
    void        do_actionNameSet    ( const std::string& actionName ) override;
    std::string do_actionNameGet    () const override;

};

} // namespace phpdoc

#endif // PHPDOC_NAVINPUTOPTIONSCREATE_H
