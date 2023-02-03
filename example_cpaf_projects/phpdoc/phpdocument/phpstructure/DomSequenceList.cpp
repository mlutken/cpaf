#include "DomSequenceList.h"
#include "DomSequenceElement.h"
#include "phpdoc_utils.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(DomSequenceList,QuotedStringBase)

DomSequenceList::DomSequenceList()
    : m_separator(',')
{
}

DomSequenceList* DomSequenceList::create(const StringVecT& list)
{
    DomSequenceList* qsl = new DomSequenceList();
    for ( string s : list) {
        qsl->stringValueAppend(s);
    }
    return qsl;
}

string DomSequenceList::stringValue(int pos) const
{
    return get<DomSequenceElement>(pos)->value();
}

DomSequenceList* DomSequenceList::stringValueSet(const string &val, int pos)
{
    get<DomSequenceElement>(pos)->valueSet(val);
    return this;
}

DomSequenceList* DomSequenceList::stringValueAppend(const string &val)
{
    DomSequenceElement* sle = new DomSequenceElement();
    sle->valueSet(val);
    if (sle->value().empty()) {
        delete sle;
    }
    else {
        appendNode(sle);
    }
    return this;
}

DomSequenceList* DomSequenceList::stringValuePrepend(const string &val)
{
    DomSequenceElement* sle = new DomSequenceElement();
    sle->valueSet(val);
    if (sle->value().empty()) {
        delete sle;
    }
    else {
        prependNode(sle);
    }
    return this;
}

DomSequenceList* DomSequenceList::stringVectorSet(const StringVecT& list)
{
    clear();
    for ( string s : list) {
        stringValueAppend(s);
    }
    return this;
}

StringVecT DomSequenceList::stringVector() const
{
    StringVecT vec;
    for (Node* node : children()) {
        if (node->enabled() && !node->value().empty()) vec.push_back(node->value());
    }
    return vec;
}

DomSequenceList* DomSequenceList::reserveAndAssign( unsigned int sz, const string& initialVal)
{
    const unsigned int toAppend = sz - size();

    for ( unsigned int n = 0; n < toAppend; ++n ) {
        DomSequenceElement* sle = new DomSequenceElement();
        sle->valueSet(initialVal);
        appendNode(sle);
    }

    return this;
}


std::string DomSequenceList::do_docStringGet () const
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

void DomSequenceList::do_valueSet(const string &val)
{
    ParseString s = toParseString(val);
    Iterator first = s.begin();
    do_parse(first, s.end());
}

string DomSequenceList::do_valueGet() const
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

bool DomSequenceList::do_parse(Iterator &first, const Iterator &last)
{
    clear();
    while ( first < last ) {
        Iterator nextElem = first;
        while ( (nextElem < last) && (*nextElem != m_separator) ) ++nextElem;
        DomSequenceElement* sle = new DomSequenceElement();
        if ( sle->parse(first, nextElem) ) {
            appendNode(sle);
        }
        if ( (first < last) && (*first == m_separator) ) ++first;
    }
    return size() > 0;

}


} // namespace phpdoc
