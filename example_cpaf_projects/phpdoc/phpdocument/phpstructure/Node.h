#ifndef _PHPDOC_NODE_H_
#define _PHPDOC_NODE_H_

#include <loki/Visitor.h>

#include "phpdoc_types.h"

// --------------------------
// --- Convenience macros ---
// --------------------------

/** Insert this macro in the public section of the class in the header file. */
#define PHPDOC_NODE_HDR(NodeClassName, ParentClassName)\
public:\
        LOKI_DEFINE_VISITABLE()\
    static NodeClassName* create() { return new NodeClassName; }\
protected:\
   virtual doctypes::Type do_docTypesGet () const;\
private:\
   virtual doctypes::Type do_docTypeGet () const;\
   virtual std::string do_classNameGet () const;\
public:


/** Insert this macro in the cpp file. */
#define PHPDOC_NODE_CPP(NodeClassName, ParentClassName)\
std::string NodeClassName::do_classNameGet () const\
{\
    return #NodeClassName;\
}\
doctypes::Type NodeClassName::do_docTypesGet () const\
{\
    return doctypes::NodeClassName | ParentClassName::do_docTypesGet();\
}\
doctypes::Type NodeClassName::do_docTypeGet () const\
{\
    return doctypes::NodeClassName;\
}


namespace phpdoc {

enum PhpSemanticFamilyE {
    PhpSemCommentFlag = 1 << 0

};

// ------------
// --- Node ---
// ------------

/** Base class for all nodes in the php document space. */
class Node  : public Loki::BaseVisitable<void>
{
public:
    LOKI_DEFINE_VISITABLE()

    Node() : m_parent(0), m_enabled(true) {}
    virtual ~Node();
    doctypes::Type  docType     () const  { return do_docTypeGet();     }
    doctypes::Type  docTypes    () const  { return do_docTypesGet();    }
    std::string     className   () const  { return do_classNameGet();   }
    std::string     docString   () const;
    std::string     value       () const  { return do_valueGet();       }
    std::string     name        () const  { return do_nameGet();        }
    bool            isLeaf      () const  { return do_isLeafGet();      }
    bool            isDocument  () const  { return do_isDocumentGet();  }

    Node*           valueSet    (const std::string& val)    { do_valueSet(val); return this;}
    Node*           nameSet     (const std::string& name)   { do_nameSet(name); return this;}

    template <typename T>
    T*          valueSet    (const std::string& val)
    {
        do_valueSet(val);
        if ((this->docTypes() & T().docType()).any()) return static_cast<T*>(this);
        return nullptr;
    }

    template <typename T>
    T*          nameSet     (const std::string& name)
    {
        do_nameSet(name);
        if ((this->docTypes() & T().docType()).any()) return static_cast<T*>(this);
        return nullptr;
    }

    bool        parse       ( Iterator& first, const Iterator& last) { return do_parse(first, last); }
    bool        parseTry    ( const Iterator& first, const Iterator& last);

    void        search      ( NodeVecT& results, const NodeSearchFunT& searchFun ) ;

    template<typename T>
    T*          findSingle  ( const NodeSearchFunT& searchFun )
    {
        NodeVecT results;
        this->search(results, searchFun);
        if ( results.size() != 1) return 0;
        Node* const n = results.back();
        if ((n->docTypes() & T().docType()).any()) return static_cast<T*>(n);
        return nullptr;
    }

    template<typename T>
    T*          findFirst  ( const NodeSearchFunT& searchFun )
    {
        NodeVecT results;
        this->search(results, searchFun);
        if ( results.empty() ) return 0;
        Node* const n = results.front();
        if ((n->docTypes() & T().docType()).any()) return static_cast<T*>(n);
        return nullptr;
    }

    template<typename T>
    T* findLast  ( const NodeSearchFunT& searchFun )
    {
        NodeVecT results;
        this->search(results, searchFun);
        if ( results.empty() ) return 0;
        Node* const n = results.back();
        if ((n->docTypes() & T().docType()).any()) return static_cast<T*>(n);
        return nullptr;
    }


    Node*   parent () const;

    template<typename T>
    T*      parent  ()
    {
        if ((m_parent->docTypes() & T().docType()).any()) return static_cast<T*>(m_parent);
        return nullptr;
    }

    void    parentSet   (Node* parent);

    bool enabled() const;
    void enabledSet(bool enabled);

protected:
    virtual doctypes::Type do_docTypesGet () const;

private:
    virtual doctypes::Type do_docTypeGet() const;
    virtual std::string do_classNameGet () const; // = 0;
    virtual std::string do_docStringGet () const; // = 0;
    virtual bool        do_parse        (Iterator& first, const Iterator& last) {}
    virtual void        do_valueSet     (const std::string& val); // = 0;
    virtual void        do_nameSet      (const std::string&) {}
    virtual std::string do_valueGet     () const;
    virtual std::string do_nameGet      () const;
    virtual bool        do_isLeafGet    () const;
    virtual bool        do_isDocumentGet() const;

    virtual void        do_search       ( NodeVecT& results, const NodeSearchFunT& searchFun ); // = 0;

    Node* m_parent;
    bool m_enabled;
};


} // END namespace phpdoc

#endif // _PHPDOC_NODE_H_
