#ifndef SPACE_H
#define SPACE_H

#include "Leaf.h"

namespace phpdoc {


// -------------
// --- Empty ---
// -------------

/** Empty  (Space). */
class Empty  : public Leaf
{
public:
    PHPDOC_NODE_HDR(Empty,Leaf)
private:
        // --- PRIVATE: Functions ---
    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& ) {}
};

// -------------
// --- Space ---
// -------------

/** Single space, as in ASCII = 32 for example. */
class Space  : public Leaf
{
public:
    PHPDOC_NODE_HDR(Space,Leaf)
private:
        // --- PRIVATE: Functions ---
    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& ) {}
};


// -------------
// --- Tab ---
// -------------

/** Single tabulator. */
class Tab  : public Leaf
{
public:
    PHPDOC_NODE_HDR(Tab,Leaf)
private:
        // --- PRIVATE: Functions ---
    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& ) {}
};


// ---------------
// --- NewLine ---
// ---------------

/** Single tabulator. */
class NewLine  : public Leaf
{
public:
    PHPDOC_NODE_HDR(NewLine,Leaf)
private:
        // --- PRIVATE: Functions ---
    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& ) {}
};

} // END namespace phpdoc

#endif // SPACE_H
