#ifndef PHPDOC_BRACES_H
#define PHPDOC_BRACES_H

#include "Leaf.h"

namespace phpdoc {


// ----------------------
// --- CurlyBraceLeft ---
// ----------------------

/** Curly brace left '{'. */
class CurlyBraceLeft : public Leaf
{
public:
    PHPDOC_NODE_HDR(CurlyBraceLeft,Leaf)
private:
        // --- PRIVATE: Functions ---
    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& ) {}
};

// -----------------------
// --- CurlyBraceRight ---
// -----------------------

/** Curly brace right '}'. */
class CurlyBraceRight : public Leaf
{
public:
    PHPDOC_NODE_HDR(CurlyBraceRight,Leaf)
private:
        // --- PRIVATE: Functions ---
    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& ) {}
};


// ---------------
// --- ParLeft ---
// ---------------

/** Parentheses left '('. */
class ParLeft : public Leaf
{
public:
    PHPDOC_NODE_HDR(ParLeft,Leaf)
private:
        // --- PRIVATE: Functions ---
    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& ) {}
};

// ----------------
// --- ParRight ---
// ----------------

/** Parentheses brace right ')'. */
class ParRight : public Leaf
{
public:
    PHPDOC_NODE_HDR(ParRight,Leaf)
private:
        // --- PRIVATE: Functions ---
    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& ) {}
};

// -------------------
// --- BracketLeft ---
// -------------------

/** Square bracket left '['. */
class BracketLeft : public Leaf
{
public:
    PHPDOC_NODE_HDR(BracketLeft,Leaf)
private:
        // --- PRIVATE: Functions ---
    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& ) {}
};

// --------------------
// --- BracketRight ---
// --------------------

/** Square bracket right ']'. */
class BracketRight : public Leaf
{
public:
    PHPDOC_NODE_HDR(BracketRight,Leaf)
private:
        // --- PRIVATE: Functions ---
    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& ) {}
};


} // namespace phpdoc

#endif // PHPDOC_BRACES_H
