#include "WhiteSpace.h"

#include "phpdoc_utils.h"
#include "Space.h"

namespace phpdoc {

PHPDOC_NODE_CPP(WhiteSpace,Group)

WhiteSpace::WhiteSpace()
{
    appendNode(Space::create());
}

bool WhiteSpace::isWhiteSpace(char ch)
{
    return (ch == ' ') || (ch == '\t') || (ch == '\n');
}

bool WhiteSpace::isWhiteSpace(const std::string& ch)
{
    return (ch == " ") || (ch == "\t") || (ch == "\n");
}

void WhiteSpace::do_valueSet(const std::string& val)
{
    ParseString s = toParseString(val);
    Iterator first = s.begin();
    do_parse(first, s.end());
}

bool WhiteSpace::do_parse(Iterator& first, const Iterator& last)
{
    clear();
    if (first >= last) return false;

    while (first < last) {
        switch (*first) {
        case ' ':
            appendNode(Space::create());
            break;
        case '\t':
            appendNode(Tab::create());
            break;
        case '\n':
            appendNode(NewLine::create());
            break;
        default:
            return true;
            break;
        }
        ++first;
    }

    return true;
}



} // namespace phpdoc
