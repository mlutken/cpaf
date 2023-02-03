#include "PatientBrowserBaseItem.h"
#include <boost/foreach.hpp>

PatientBrowserBaseItem::PatientBrowserBaseItem()
    : m_parent(0)
{
}

PatientBrowserBaseItem::PatientBrowserBaseItem(PatientBrowserBaseItem *parent)
{
    parent->appendChild(this);
}

PatientBrowserBaseItem::~PatientBrowserBaseItem()
{
    BOOST_FOREACH(PatientBrowserBaseItem* base_item, m_children) {
        delete base_item;
    }
}

void PatientBrowserBaseItem::setParent(PatientBrowserBaseItem *parent)
{
    m_parent = parent;
}

void PatientBrowserBaseItem::appendChild(PatientBrowserBaseItem *child)
{
    child->setParent(this);
    m_children.push_back(child);
}

QString PatientBrowserBaseItem::dbgText() const
{
    return displayText() + ": " + m_dbg_text;
}
