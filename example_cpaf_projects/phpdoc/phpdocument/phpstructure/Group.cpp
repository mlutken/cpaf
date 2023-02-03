#include "Group.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(Group,Compound)

Group::~Group()
{
    clear();
}

bool Group::prependNode(Node *node)
{
    return insertBefore(node, static_cast<unsigned int>(0) );
}

bool Group::appendNode ( Node* node )
{
    if (!canAddNode(node)) return false;
    m_children.push_back(node);
    node->parentSet(this);
    return true;
}

bool Group::canAddNode(Node *node) const
{
    return do_canAddNode(node);
}


bool Group::insertBefore(Node* node, unsigned int pos)
{
    if ( pos > m_children.size() ) return false;
    if (!canAddNode(node)) return false;
    const VecP::iterator it = m_children.begin() + pos;
    m_children.insert(it, 1, node );
    node->parentSet(this);
    return true;
}

bool Group::insertAfter(Node *node, unsigned int pos)
{
    if (!canAddNode(node)) return false;
    const VecP::iterator it = m_children.begin() + pos + 1;
    if ( it > m_children.end() ) return false;
    m_children.insert(it, 1, node );
    node->parentSet(this);
    return true;
}

bool Group::insertBefore(Node *node, Node *positionNode)
{
    return insertBefore(node, findPosition(positionNode));
}

bool Group::insertAfter(Node *node, Node *positionNode)
{
    return insertAfter(node, findPosition(positionNode));
}

void Group::removeNode(unsigned int pos)
{
    Node* node = getNode(pos);
    if ( node ) {
        delete node;
        m_children.erase(m_children.begin()+pos);
    }
}

bool Group::replaceNode(Node* node, unsigned int pos)
{
    if (!canAddNode(node)) return false;
    delete getNode(pos);
    m_children[pos] = node;
    node->parentSet(this);
    return true;
}

Node* Group::getNode(unsigned int pos) const
{
    if ( pos >= m_children.size() ) return nullptr;
    else                            return m_children[pos];

}

void Group::clear()
{
    for (Node* pNode: m_children) {
        delete pNode;
    }
    m_children.clear();
}

/** Find postion in group of a given node. If not found size() is returned*/
int Group::findPosition(const Node *node) const
{
    int pos = 0;
    const int SIZE = size();
    for ( ; pos < SIZE; ++pos) {
        if ( m_children[pos] == node ) return pos;
    }
    return size(); // Not found!

}

bool Group::childEnabled(unsigned int pos) const
{
    Node* node = getNode(pos);
    if ( node ) return node->enabled();
    return false;
}

Group* Group::childEnabledSet(bool enabled, unsigned int pos)
{
    Node* node = getNode(pos);
    if ( node ) node->enabledSet(enabled);
    return this;
}


std::string Group::do_docStringGet () const
{
    string s;
    for (Node* pNode: m_children) {
        s += pNode->docString();
    }
    return s;
}

bool Group::do_isDocumentGet() const
{
    return true;
}

void Group::do_search(NodeVecT& results, const NodeSearchFunT& searchFun)
{
    if (searchFun(this)) results.push_back(this);
    for (Node* node: m_children) {
        node->search(results, searchFun);
    }
}


bool Group::do_canAddNode   ( Node* /*node*/ ) const
{
    return true;
}

} // namespace phpdoc
