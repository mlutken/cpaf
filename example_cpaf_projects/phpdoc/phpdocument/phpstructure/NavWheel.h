#ifndef PHPDOC_NAVWHEEL_H
#define PHPDOC_NAVWHEEL_H

#include "Group.h"

namespace phpdoc {

class NavOptionCreate;
class navAttributeNamesCreate;
class navOptionAdd;

class NavWheel : public Group
{
    PHPDOC_NODE_HDR(NavWheel,Group)
public:
    static const unsigned int WHEEL_ACTION_CREATE_POS   = 0;
    static const unsigned int ATTRIBUTE_NAMES_POS       = 1;
    static const unsigned int FIRST_OPTION_POS          = 2;

    NavWheel();

    NavWheel*                   optionCreateSet     ( NavOptionCreate* optionCreate );
    NavOptionCreate*            optionCreate        () const;

    NavWheel*                   attributeNamesSet   ( navAttributeNamesCreate* attributeNames );
    navAttributeNamesCreate*    attributeNames      () const;

    NavWheel*                   actionNameSet       ( const std::string& actionName );
    std::string                 actionName          () const;

    NavWheel*                   optionAppend        ( navOptionAdd* option );
    NavWheel*                   optionSet           ( navOptionAdd* option, unsigned int pos );
    navOptionAdd*               option              ( unsigned int pos ) const;

    unsigned int                optionsCount        () const;
    unsigned int                optionsBeginPos     () const;
    unsigned int                optionsEndPos       () const;

private:


};

} // namespace phpdoc

#endif // PHPDOC_NAVWHEEL_H
