#ifndef PATIENTBROWSERMODELITEM_H
#define PATIENTBROWSERMODELITEM_H

#include <PatientBrowserBaseItem.h>
#include <QDomNode>
#include <QHash>

//! [0]
class PatientBrowserModelItem
{
public:
    PatientBrowserModelItem(PatientBrowserBaseItem *base_item, int row, PatientBrowserModelItem *parent = 0);
    ~PatientBrowserModelItem();
    PatientBrowserModelItem *child(int i);
    PatientBrowserModelItem *parent();
    PatientBrowserBaseItem* baseItem() const;
    int row();

private:
    PatientBrowserBaseItem* m_base_item;
    QHash<int,PatientBrowserModelItem*> m_child_items;
    PatientBrowserModelItem *m_parent_model_item;
    int m_row_number;
};
//! [0]

#endif // PATIENTBROWSERMODELITEM_H
