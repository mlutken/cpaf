#include "Space.h"

using namespace std;

namespace phpdoc {

// -------------
// --- Empty ---
// -------------
PHPDOC_NODE_CPP(Empty,Leaf)

std::string Empty::do_docStringGet () const
{
    return "";
}

// -------------
// --- Space ---
// -------------
PHPDOC_NODE_CPP(Space,Leaf)

std::string Space::do_docStringGet () const
{
    return " ";
}

// -----------
// --- Tab ---
// -----------
PHPDOC_NODE_CPP(Tab,Leaf)

std::string Tab::do_docStringGet () const
{
    return "\t";
}

// ---------------
// --- NewLine ---
// ---------------
PHPDOC_NODE_CPP(NewLine,Leaf)

std::string NewLine::do_docStringGet () const
{
    return "\n";
}

} // END namespace phpdoc
