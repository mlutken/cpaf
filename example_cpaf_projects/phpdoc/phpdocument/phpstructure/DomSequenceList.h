#ifndef PHPDOC_DOMSEQUENCELIST_H
#define PHPDOC_DOMSEQUENCELIST_H

#include <string>
#include "QuotedStringBase.h"

namespace phpdoc {

class DomSequenceElement;

class DomSequenceList : public QuotedStringBase
{
    PHPDOC_NODE_HDR(DomSequenceList,QuotedStringBase)
public:

    DomSequenceList();

    static DomSequenceList* create( const StringVecT& list);

    std::string        stringValue         ( int pos ) const;
    DomSequenceList*   stringValueSet      ( const std::string& val, int pos );
    DomSequenceList*   stringValueAppend   ( const std::string& val );
    DomSequenceList*   stringValuePrepend  ( const std::string& val );
    DomSequenceList*   stringVectorSet     ( const StringVecT& list );
    StringVecT         stringVector        () const;
    DomSequenceList*   reserveAndAssign    ( unsigned int sz, const std::string& initialVal );


private:
    // --- PRIVATE: Functions ---
    virtual std::string     do_docStringGet () const;
    virtual void            do_valueSet     (const std::string& val);
    virtual std::string     do_valueGet     () const;
    virtual bool            do_parse        (Iterator& first, const Iterator& last) override;

private:
    char     m_separator;
};

} // namespace phpdoc

#endif // PHPDOC_DOMSEQUENCELIST_H
