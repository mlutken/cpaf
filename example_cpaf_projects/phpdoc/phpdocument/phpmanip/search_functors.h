#ifndef SEARCH_FUNCTORS_H
#define SEARCH_FUNCTORS_H

#include <boost/algorithm/string.hpp>
#include <phpstructure/phpdoc_types.h>
#include <phpstructure/Node.h>

namespace phpdoc {

namespace search {

struct DocTypes
{
    explicit DocTypes( const doctypes::Type& docTypes );
    bool operator() (const Node* node);

    doctypes::Type  m_docTypes;
};


struct ValueContains
{
    explicit ValueContains( const std::string& searchFor );
    ValueContains( const std::string& searchFor, const doctypes::Type& docTypes );
    bool operator() (const Node* node);

    std::string     m_searchFor;
    doctypes::Type  m_docTypes;
};

struct NameContains
{
    explicit NameContains( const std::string& searchFor );
    NameContains( const std::string& searchFor, const doctypes::Type& docTypes );
    bool operator() (const Node* node);

    std::string     m_searchFor;
    doctypes::Type  m_docTypes;
};

struct DocContains
{
    explicit DocContains( const std::string& searchFor );
    DocContains( const std::string& searchFor, const doctypes::Type& docTypes );
    bool operator() (const Node* node);

    std::string     m_searchFor;
    doctypes::Type  m_docTypes;
};


struct ValueEquals
{
    explicit ValueEquals( const std::string& searchFor );
    ValueEquals( const std::string& searchFor, const doctypes::Type& docTypes );
    bool operator() (const Node* node);

    std::string     m_searchFor;
    doctypes::Type  m_docTypes;
};

struct NameEquals
{
    explicit NameEquals( const std::string& searchFor );
    NameEquals( const std::string& searchFor, const doctypes::Type& docTypes );
    bool operator() (const Node* node);

    std::string     m_searchFor;
    doctypes::Type  m_docTypes;
};

struct DocEquals
{
    explicit DocEquals( const std::string& searchFor );
    DocEquals( const std::string& searchFor, const doctypes::Type& docTypes );
    bool operator() (const Node* node);

    std::string     m_searchFor;
    doctypes::Type  m_docTypes;
};


} // END namespace search


} // END namespace phpdoc

#endif // SEARCH_FUNCTORS_H
