#ifndef PHPDOC_GROUP_H
#define PHPDOC_GROUP_H

#include "Compound.h"

namespace phpdoc {

/** Base container type node supporting children. */
class Group  : public Compound
{
public:
    PHPDOC_NODE_HDR(Group,Compound)
    virtual ~Group();
    typedef std::vector<Node*>  VecP;

    bool    prependNode     ( Node* node );
    bool    appendNode      ( Node* node );
    bool    canAddNode      ( Node* node ) const;
    bool    insertBefore    ( Node* node, unsigned int pos );
    bool    insertAfter     ( Node* node, unsigned int pos );
    bool    insertBefore    ( Node* node, Node* positionNode );
    bool    insertAfter     ( Node* node, Node* positionNode );
    void    removeNode      ( unsigned int pos );
    bool    replaceNode     ( Node* node, unsigned int pos );
    Node*   getNode         ( unsigned int pos ) const;
    void    clear           ();
    bool    positionValid   ( unsigned int pos ) const { return pos < size(); }
    int     findPosition    ( const Node* node ) const;
    bool    childEnabled    ( unsigned int pos ) const;
    Group*  childEnabledSet ( bool enabled, unsigned int pos );

    size_t  size            () const { return m_children.size(); }

    template<typename T>
    T*      get             ( unsigned int pos ) const
    {
        Node* node = getNode(pos);
        if (!node) return nullptr;
        if ((node->docTypes() & T().docType()).any()) return static_cast<T*>(node);
        return nullptr;
    }

protected:
    const VecP&        children () const { return m_children; }


private:
        // --- PRIVATE: Functions ---
    virtual std::string do_docStringGet () const override;
    virtual bool        do_isDocumentGet() const override;
    virtual void        do_valueSet     (const std::string& ) override {}

    virtual void        do_search       ( NodeVecT& results, const NodeSearchFunT& searchFun ) override;

    virtual bool        do_canAddNode   (Node* node ) const;
    // --- PRIVATE: Data ---
    VecP        m_children;

};

} // namespace phpdoc

#endif // PHPDOC_GROUP_H
