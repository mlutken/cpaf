#ifndef PHPDOC_QUOTEDSTRING_H
#define PHPDOC_QUOTEDSTRING_H

#include "QuotedStringBase.h"

namespace phpdoc {

class QuotedString : public QuotedStringBase
{
    PHPDOC_NODE_HDR(QuotedString,QuotedStringBase)
public:
    static const unsigned int STRING_POS     = 1;

    QuotedString();
    int     valueInt    () const;
    void    valueIntSet (int val);


private:
    // --- PRIVATE: Functions ---
    virtual std::string     do_docStringGet () const;
    virtual void            do_valueSet     (const std::string& val);
    virtual std::string     do_valueGet     () const;

    virtual bool            do_parse        (Iterator& first, const Iterator& last) override;

};

} // namespace phpdoc

#endif // PHPDOC_QUOTEDSTRING_H
