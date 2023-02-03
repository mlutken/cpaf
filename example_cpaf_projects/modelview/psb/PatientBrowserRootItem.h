#ifndef PATIENTBROWSERROOTITEM_H
#define PATIENTBROWSERROOTITEM_H

#include <PatientBrowserBaseItem.h>

class PatientBrowserRootItem : public PatientBrowserBaseItem
{
public:
    static PatientBrowserBaseItem* create() { return new PatientBrowserRootItem();}

    PatientBrowserRootItem () : PatientBrowserBaseItem() {}
    explicit PatientBrowserRootItem(PatientBrowserBaseItem *parent) : PatientBrowserBaseItem(parent) {}

private:
    virtual QString doDisplayText () const;
};

#endif // PATIENTBROWSERROOTITEM_H
