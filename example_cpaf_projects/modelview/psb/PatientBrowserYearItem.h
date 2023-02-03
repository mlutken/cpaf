#ifndef PATIENTBROWSERYEARITEM_H
#define PATIENTBROWSERYEARITEM_H

#include <PatientBrowserBaseItem.h>

class PatientBrowserYearItem : public PatientBrowserBaseItem
{
public:
    static PatientBrowserBaseItem* create() { return new PatientBrowserYearItem();}

    PatientBrowserYearItem () : PatientBrowserBaseItem() {}
    explicit PatientBrowserYearItem(PatientBrowserBaseItem *parent) : PatientBrowserBaseItem(parent) {}

private:
    virtual QString doDisplayText () const;
};

#endif // PATIENTBROWSERYEARITEM_H
