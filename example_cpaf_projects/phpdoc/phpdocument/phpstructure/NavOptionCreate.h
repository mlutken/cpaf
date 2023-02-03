#ifndef PHPDOC_NAVOPTIONCREATE_H
#define PHPDOC_NAVOPTIONCREATE_H

#include "FunctionCall.h"
#include "navigator_common.h"

namespace phpdoc {

class NavOptionCreate : public FunctionCall
{
    PHPDOC_NODE_HDR(NavOptionCreate,FunctionCall)
public:
    NavOptionCreate();

    bool                hasParameter        ( unsigned int parameterPos ) const { return do_hasParameter(parameterPos); }

    NavOptionCreate*    actionNameSet       ( const std::string& actionName );
    std::string         actionName          () const                            { return do_actionNameGet(); }

private:
    virtual bool        do_hasParameter     ( unsigned int parameterPos ) const;
    virtual void        do_actionNameSet    ( const std::string& actionName ) {}
    virtual std::string do_actionNameGet    () const  {}
};

} // namespace phpdoc

#endif // PHPDOC_NAVOPTIONCREATE_H
