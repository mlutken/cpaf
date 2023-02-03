#ifndef PHPDOC_QUOTEDSTRINGLIST_H
#define PHPDOC_QUOTEDSTRINGLIST_H

#include <string>
#include "QuotedStringBase.h"

namespace phpdoc {

class StringListElement;

class QuotedStringList : public QuotedStringBase
{
    PHPDOC_NODE_HDR(QuotedStringList,QuotedStringBase)
public:

    QuotedStringList();

    static QuotedStringList* create( const StringVecT& list);

    std::string         stringValue         ( int pos ) const;
    QuotedStringList*   stringValueSet      ( const std::string& val, int pos );
    QuotedStringList*   stringValueAppend   ( const std::string& val );
    QuotedStringList*   stringValuePrepend  ( const std::string& val );
    QuotedStringList*   stringVectorSet     ( const StringVecT& list );
    StringVecT          stringVector        () const;
    QuotedStringList*   reserveAndAssign    ( unsigned int sz, const std::string& initialVal );


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

#endif // PHPDOC_QUOTEDSTRINGLIST_H
