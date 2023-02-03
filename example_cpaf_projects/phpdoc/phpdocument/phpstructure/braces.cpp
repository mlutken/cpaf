#include "braces.h"

using namespace std;

namespace phpdoc {

// ----------------------
// --- CurlyBraceLeft ---
// ----------------------

PHPDOC_NODE_CPP(CurlyBraceLeft,Leaf)

std::string CurlyBraceLeft::do_docStringGet () const
{
    return "{";
}

// -----------------------
// --- CurlyBraceRight ---
// -----------------------

PHPDOC_NODE_CPP(CurlyBraceRight,Leaf)

std::string CurlyBraceRight::do_docStringGet () const
{
    return "}";
}


// ---------------
// --- ParLeft ---
// ---------------

PHPDOC_NODE_CPP(ParLeft,Leaf)

std::string ParLeft::do_docStringGet () const
{
    return "(";
}

// ----------------
// --- ParRight ---
// ----------------

PHPDOC_NODE_CPP(ParRight,Leaf)

std::string ParRight::do_docStringGet () const
{
    return ")";
}


// -------------------
// --- BracketLeft ---
// -------------------

PHPDOC_NODE_CPP(BracketLeft,Leaf)

std::string BracketLeft::do_docStringGet () const
{
    return "[";
}

// --------------------
// --- BracketRight ---
// --------------------

PHPDOC_NODE_CPP(BracketRight,Leaf)

std::string BracketRight::do_docStringGet () const
{
    return "]";
}

} // namespace phpdoc
