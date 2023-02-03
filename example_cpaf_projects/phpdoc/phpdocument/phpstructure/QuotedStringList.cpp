#include "QuotedStringList.h"
#include "StringListElement.h"
#include "phpdoc_utils.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(QuotedStringList,QuotedStringBase)

QuotedStringList::QuotedStringList()
    : m_separator(',')
{
}

QuotedStringList* QuotedStringList::create(const StringVecT& list)
{
    QuotedStringList* qsl = new QuotedStringList();
    for ( string s : list) {
        qsl->stringValueAppend(s);
    }
    return qsl;
}

string QuotedStringList::stringValue(int pos) const
{
    return get<StringListElement>(pos)->value();
}

QuotedStringList* QuotedStringList::stringValueSet(const string &val, int pos)
{
    get<StringListElement>(pos)->valueSet(val);
    return this;
}

QuotedStringList* QuotedStringList::stringValueAppend(const string &val)
{
    StringListElement* sle = new StringListElement();
    sle->valueSet(val);
    if (sle->value().empty()) {
        delete sle;
    }
    else {
        appendNode(sle);
    }
    return this;
}

QuotedStringList* QuotedStringList::stringValuePrepend(const string &val)
{
    StringListElement* sle = new StringListElement();
    sle->valueSet(val);
    if (sle->value().empty()) {
        delete sle;
    }
    else {
        prependNode(sle);
    }
    return this;
}

QuotedStringList* QuotedStringList::stringVectorSet(const StringVecT& list)
{
    clear();
    for ( string s : list) {
        stringValueAppend(s);
    }
    return this;
}

StringVecT QuotedStringList::stringVector() const
{
    StringVecT vec;
    for (Node* node : children()) {
        if (node->enabled() && !node->value().empty()) vec.push_back(node->value());
    }
    return vec;
}

QuotedStringList* QuotedStringList::reserveAndAssign( unsigned int sz, const string& initialVal)
{
    const unsigned int toAppend = sz - size();

    for ( unsigned int n = 0; n < toAppend; ++n ) {
        StringListElement* sle = new StringListElement();
        sle->valueSet(initialVal);
        appendNode(sle);
    }

    return this;
}


std::string QuotedStringList::do_docStringGet () const
{
    string s;
    int i = 0;
    for (Node* node : children()) {
        if (node->enabled() && !node->value().empty()) {
            if ( i > 0) s += m_separator;
            i++;
            s += node->docString();
        }
    }
    if (DoubleQuoteE == quoteType())    return string("\"") + s + "\"";
    else                                return string("'") + s + "'";
}

void QuotedStringList::do_valueSet(const string &val)
{
    ParseString s = toParseString(val);
    Iterator first = s.begin();
    do_parse(first, s.end());
}

string QuotedStringList::do_valueGet() const
{
    string s;
    int i = 0;
    for (Node* node : children()) {
        if (node->enabled() && !node->value().empty()) {
            if ( i > 0) s += m_separator;
            i++;
            s += node->value();
        }
    }
    return s;
}

bool QuotedStringList::do_parse(Iterator &first, const Iterator &last)
{
    clear();
    while ( first < last ) {
        Iterator nextElem = first;
        while ( (nextElem < last) && (*nextElem != m_separator) ) ++nextElem;
        StringListElement* sle = new StringListElement();
        if ( sle->parse(first, nextElem) ) {
            appendNode(sle);
        }
        if ( (first < last) && (*first == m_separator) ) ++first;
    }
    return size() > 0;

}


} // namespace phpdoc
