#include "PatientBrowserModelItem.h"

#include <QtXml>

//! [0]
PatientBrowserModelItem::PatientBrowserModelItem(
        PatientBrowserBaseItem *base_item, int row, PatientBrowserModelItem *parent)
{
    m_base_item = base_item;
//! [0]
    // Record the item's location within its parent.
//! [1]
    m_row_number = row;
    m_parent_model_item = parent;
}
//! [1]

//! [2]
PatientBrowserModelItem::~PatientBrowserModelItem()
{
    QHash<int,PatientBrowserModelItem*>::iterator it;
    for (it = m_child_items.begin(); it != m_child_items.end(); ++it)
        delete it.value();
}
//! [2]

//! [3]
PatientBrowserBaseItem *PatientBrowserModelItem::baseItem() const
{
    return m_base_item;
}
//! [3]

//! [4]
PatientBrowserModelItem *PatientBrowserModelItem::parent()
{
    return m_parent_model_item;
}
//! [4]

//! [5]
PatientBrowserModelItem *PatientBrowserModelItem::child(int i)
{
    if (m_child_items.contains(i))
        return m_child_items[i];

    unsigned int index = static_cast<unsigned int>(i);
    if ( index < m_base_item->children().size()) {
        PatientBrowserBaseItem *child_data_item = m_base_item->children()[index];
        PatientBrowserModelItem *child_item = new PatientBrowserModelItem(child_data_item, index, this);
        m_child_items[index] = child_item;
        return child_item;
    }
    return 0;
}
//! [5]

//! [6]
int PatientBrowserModelItem::row()
{
    return m_row_number;
}
//! [6]
