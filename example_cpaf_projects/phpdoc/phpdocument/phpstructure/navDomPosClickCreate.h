#ifndef PHPDOC_NAVDOMPOSCLICKCREATE_H
#define PHPDOC_NAVDOMPOSCLICKCREATE_H

#include "NavOptionCreate.h"

namespace phpdoc {

class navDomPosClickCreate : public NavOptionCreate
{
    PHPDOC_NODE_HDR(navDomPosClickCreate,NavOptionCreate)
public:
    static const unsigned int ACTION_NAME_POS    = 0;
    static const unsigned int SEQUENCE_POS       = 1;

    navDomPosClickCreate();

    navDomPosClickCreate*   findNextSequenceSet( const std::string& sequence );
    std::string             findNextSequence   () const;

private:
    void        do_valueSet         ( const std::string& val ) override;
    std::string do_valueGet         () const override;
    bool        do_hasParameter     ( unsigned int parameterPos ) const override;
    void        do_actionNameSet    ( const std::string& actionName ) override;
    std::string do_actionNameGet    () const override;
};

} // namespace phpdoc

#endif // PHPDOC_NAVDOMPOSCLICKCREATE_H
