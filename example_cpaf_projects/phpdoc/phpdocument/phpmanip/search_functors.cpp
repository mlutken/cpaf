#include "search_functors.h"
#include <phpstructure/globals_utils.h>

namespace phpdoc {

namespace search {


// ---------------------
// --- DocTypes ---
// ---------------------

DocTypes::DocTypes(const doctypes::Type& docTypes)
    : m_docTypes (docTypes)
{}

bool DocTypes::operator()(const Node* node)
{
    return (m_docTypes & node->docTypes()).any();
}


// ---------------------
// --- ValueContains ---
// ---------------------

ValueContains::ValueContains(const std::string& searchFor)
    : m_searchFor(searchFor)
    , m_docTypes(doctypes::All)
{}

ValueContains::ValueContains(const std::string& searchFor, const doctypes::Type& docTypes)
    : m_searchFor(searchFor)
    , m_docTypes (docTypes)
{}

bool ValueContains::operator()(const Node* node)
{
    if ( (m_docTypes & node->docTypes()).none() ) return false;
    return boost::algorithm::contains(node->value(), m_searchFor);
}


// --------------------
// --- NameContains ---
// --------------------

NameContains::NameContains(const std::string& searchFor)
    : m_searchFor(searchFor)
    , m_docTypes(doctypes::All)
{}

NameContains::NameContains(const std::string& searchFor, const doctypes::Type& docTypes)
    : m_searchFor(searchFor)
    , m_docTypes (docTypes)
{}

bool NameContains::operator()(const Node* node)
{
    if ( (m_docTypes & node->docTypes()).none() ) return false;
    return boost::algorithm::contains(node->name(), m_searchFor);
}

// --------------------
// --- DocContains ---
// --------------------

DocContains::DocContains(const std::string& searchFor)
    : m_searchFor(searchFor)
    , m_docTypes(doctypes::All)
{}

DocContains::DocContains(const std::string& searchFor, const doctypes::Type& docTypes)
    : m_searchFor(searchFor)
    , m_docTypes (docTypes)
{}

bool DocContains::operator()(const Node* node)
{
    if ( (m_docTypes & node->docTypes()).none() ) return false;
    return boost::algorithm::contains(node->docString(), m_searchFor);
}



// ---------------------
// --- ValueEquals ---
// ---------------------

ValueEquals::ValueEquals(const std::string& searchFor)
    : m_searchFor(searchFor)
    , m_docTypes(doctypes::All)
{}

ValueEquals::ValueEquals(const std::string& searchFor, const doctypes::Type& docTypes)
    : m_searchFor(searchFor)
    , m_docTypes (docTypes)
{}

bool ValueEquals::operator()(const Node* node)
{
    if ( (m_docTypes & node->docTypes()).none() ) return false;
    return boost::algorithm::equals(node->value(), m_searchFor);
}


// --------------------
// --- NameEquals ---
// --------------------

NameEquals::NameEquals(const std::string& searchFor)
    : m_searchFor(searchFor)
    , m_docTypes(doctypes::All)
{}

NameEquals::NameEquals(const std::string& searchFor, const doctypes::Type& docTypes)
    : m_searchFor(searchFor)
    , m_docTypes (docTypes)
{}

bool NameEquals::operator()(const Node* node)
{
    if ( (m_docTypes & node->docTypes()).none() ) return false;
    return boost::algorithm::equals(node->name(), m_searchFor);
}

// --------------------
// --- DocEquals ---
// --------------------

DocEquals::DocEquals(const std::string& searchFor)
    : m_searchFor(searchFor)
    , m_docTypes(doctypes::All)
{}

DocEquals::DocEquals(const std::string& searchFor, const doctypes::Type& docTypes)
    : m_searchFor(searchFor)
    , m_docTypes (docTypes)
{}

bool DocEquals::operator()(const Node* node)
{
    if ( (m_docTypes & node->docTypes()).none() ) return false;
    return boost::algorithm::equals(node->docString(), m_searchFor);
}


} // END namespace search


} // END namespace phpdoc
