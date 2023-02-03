#include "DomSequenceElement.h"

#include "String.h"
#include "QuotedString.h"
#include "WhiteSpace.h"
#include "phpdoc_utils.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(DomSequenceElement,Group)

DomSequenceElement::DomSequenceElement()
{
    appendNode(WhiteSpace::create()->valueSet(""));
    appendNode(String::create()->valueSet(""));
    appendNode(String::create()->valueSet(""));
    appendNode(WhiteSpace::create()->valueSet(""));
}

DomSequenceElement* DomSequenceElement::whiteSpaceSet(WhiteSpace* wsBefore, WhiteSpace* wsAfter)
{
    replaceNode(wsBefore, WS_0_POS);
    replaceNode(wsAfter, WS_1_POS);
    return this;
}

DomSequenceElement* DomSequenceElement::whiteSpaceBeforeSet(WhiteSpace* wsBefore)
{
    replaceNode(wsBefore, WS_0_POS);
    return this;
}

DomSequenceElement* DomSequenceElement::whiteSpaceAfterSet(WhiteSpace* wsAfter)
{
    replaceNode(wsAfter, WS_1_POS);
    return this;
}

DomSequenceElement* DomSequenceElement::whiteSpaceBeforeSet(const string& wsBefore)
{
    return whiteSpaceBeforeSet(WhiteSpace::create()->valueSet<WhiteSpace>(wsBefore));
}

DomSequenceElement* DomSequenceElement::whiteSpaceAfterSet(const string& wsAfter)
{
    return whiteSpaceAfterSet(WhiteSpace::create()->valueSet<WhiteSpace>(wsAfter));
}

DomSequenceElement*DomSequenceElement::stringValueSet(const string& val)
{
    get<String>(STRING_POS)->valueSet(val);
    return this;
}

string DomSequenceElement::stringValue() const
{
    return get<String>(STRING_POS)->value();
}

DomSequenceElement* DomSequenceElement::compareFunctionSet(const string& compFun)
{
    get<String>(COMPARE_FUN_POS)->valueSet(compFun);
    return this;
}

string DomSequenceElement::compareFunction() const
{
    return get<String>(COMPARE_FUN_POS)->value();
}

string DomSequenceElement::compareFunctionDocString() const
{
    const string compFun = compareFunction();
    if (compFun.empty())    return "";
    else                    return string("#") + compFun + ":";
}


void DomSequenceElement::do_valueSet(const string& val)
{
    ParseString s = toParseString(val);
    Iterator first = s.begin();
    do_parse(first, s.end());
}

string DomSequenceElement::do_valueGet() const
{
    return compareFunctionDocString() + stringValue();
}

bool DomSequenceElement::do_parse(Iterator& first, const Iterator& last)
{
    WhiteSpace* ws0         = get<WhiteSpace>(WS_0_POS);
    String*     commpareFun = get<String>(COMPARE_FUN_POS);
    String*     strElem     = get<String>(STRING_POS);
    WhiteSpace* ws1         = get<WhiteSpace>(WS_1_POS);

    ws0->valueSet("");
    strElem->valueSet("");
    ws1->valueSet("");

    if (first >= last) return false;
    ws0->parse(first, last);
    if (first >= last) return false;

    // Parse normal string element
    string valStrElem;
    while (first < last && !WhiteSpace::isWhiteSpace(*first)) {
        valStrElem += *first;
        ++first;
    }
    bool retVal = !valStrElem.empty();
    strElem->valueSet(valStrElem);
    if (first >= last) return retVal;
    ws1->parse(first, last);

    return retVal;
}


} // namespace phpdoc
