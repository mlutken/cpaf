#include "Node.h"
#include <phpstructure/Globals.h>

using namespace std;

namespace phpdoc {

// ------------
// --- Node ---
// ------------


Node::~Node()
{
}

string Node::docString() const
{
    if (enabled()) return do_docStringGet();
    return "";
}


bool Node::parseTry(const Iterator& first, const Iterator& last)
{
    Iterator it = first;
    return do_parse(it, last);
}

void Node::search(NodeVecT& results, const NodeSearchFunT& searchFun)
{
    do_search(results, searchFun);
}

Node* Node::parent() const
{
    return m_parent;
}

void Node::parentSet(Node* parent)
{
    m_parent = parent;
}

bool Node::enabled() const
{
    return m_enabled;
}

void Node::enabledSet(bool enabled)
{
    m_enabled = enabled;
}


doctypes::Type Node::do_docTypeGet() const
{
    return doctypes::Node;
}

//PHPDOC_NODE_CPP(Node)
// We can't use the PHPDOC_NODE_CPP macro for the base Node
std::string Node::do_classNameGet () const
{
    return "Node";
}

std::string Node::do_docStringGet () const
{
    return "";
}

void Node::do_valueSet(const string &/*val*/)
{

}

string Node::do_valueGet() const
{
    return do_docStringGet();
}

string Node::do_nameGet() const
{
    return "";
}


bool Node::do_isLeafGet () const
{
    return false;
}

bool Node::do_isDocumentGet() const
{
    return false;
}

void Node::do_search(NodeVecT &/*results*/, const NodeSearchFunT &/*searchFun*/)
{

}


doctypes::Type Node::do_docTypesGet() const
{
    return doctypes::Node;
}








} // END namespace phpdoc
