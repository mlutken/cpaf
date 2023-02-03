#ifndef PHPDOC_NAVURLLOADCREATE_H
#define PHPDOC_NAVURLLOADCREATE_H

#include "NavOptionCreate.h"

namespace phpdoc {

class navUrlLoadCreate : public NavOptionCreate
{
    PHPDOC_NODE_HDR(navUrlLoadCreate,NavOptionCreate)
public:
    static const unsigned int ACTION_NAME_POS    = 0;
    static const unsigned int BASE_URL_POS       = 1;

    navUrlLoadCreate();

    navUrlLoadCreate*    baseUrlSet     ( const std::string& baseUrl );
    std::string          baseUrl        () const;

private:
    void        do_valueSet         ( const std::string& val ) override;
    std::string do_valueGet         () const override;
    bool        do_hasParameter     ( unsigned int parameterPos ) const override;
    void        do_actionNameSet    ( const std::string& actionName ) override;
    std::string do_actionNameGet    () const override;
};

} // namespace phpdoc

#endif // PHPDOC_NAVURLLOADCREATE_H
