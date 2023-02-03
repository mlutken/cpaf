#include "StringListElement.h"

#include "String.h"
#include "QuotedString.h"
#include "WhiteSpace.h"
#include "phpdoc_utils.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(StringListElement,Group)

StringListElement::StringListElement()
{
    appendNode(WhiteSpace::create()->valueSet(""));
    appendNode(String::create()->valueSet(""));
    appendNode(WhiteSpace::create()->valueSet(""));
}

StringListElement* StringListElement::whiteSpaceSet(WhiteSpace* wsBefore, WhiteSpace* wsAfter)
{
    replaceNode(wsBefore, WS_0_POS);
    replaceNode(wsAfter, WS_1_POS);
    return this;
}

StringListElement* StringListElement::whiteSpaceBeforeSet(WhiteSpace* wsBefore)
{
    replaceNode(wsBefore, WS_0_POS);
    return this;
}

StringListElement* StringListElement::whiteSpaceAfterSet(WhiteSpace* wsAfter)
{
    replaceNode(wsAfter, WS_1_POS);
    return this;
}

StringListElement* StringListElement::whiteSpaceBeforeSet(const string& wsBefore)
{
    return whiteSpaceBeforeSet(WhiteSpace::create()->valueSet<WhiteSpace>(wsBefore));
}

StringListElement* StringListElement::whiteSpaceAfterSet(const string& wsAfter)
{
    return whiteSpaceAfterSet(WhiteSpace::create()->valueSet<WhiteSpace>(wsAfter));
}


void StringListElement::do_valueSet(const string& val)
{
    ParseString s = toParseString(val);
    Iterator first = s.begin();
    do_parse(first, s.end());
}

string StringListElement::do_valueGet() const
{
    return get<String>(STRING_POS)->value();
}

bool StringListElement::do_parse(Iterator& first, const Iterator& last)
{
    WhiteSpace* ws0 = get<WhiteSpace>(WS_0_POS);
    String*     str = get<String>(STRING_POS);
    WhiteSpace* ws1 = get<WhiteSpace>(WS_1_POS);

    ws0->valueSet("");
    str->valueSet("");
    ws1->valueSet("");

    if (first >= last) return false;
    ws0->parse(first, last);
    if (first >= last) return false;

    string val;
    while (first < last && !WhiteSpace::isWhiteSpace(*first)) {
        val += *first;
        ++first;
    }
    bool retVal = !val.empty();
    str->valueSet(val);
    if (first >= last) return retVal;
    ws1->parse(first, last);

    return retVal;
}


} // namespace phpdoc
