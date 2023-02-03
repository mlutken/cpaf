#ifndef PATIENTBROWSERPATIENTITEM_H
#define PATIENTBROWSERPATIENTITEM_H

#include <PatientBrowserBaseItem.h>

class PatientBrowserPatientItem : public PatientBrowserBaseItem
{
public:
    static PatientBrowserBaseItem* create() { return new PatientBrowserPatientItem();}

    PatientBrowserPatientItem () : PatientBrowserBaseItem() {}
    explicit PatientBrowserPatientItem(PatientBrowserBaseItem *parent) : PatientBrowserBaseItem(parent) {}

private:
    virtual QString doDisplayText () const;

};

#endif // PATIENTBROWSERPATIENTITEM_H
