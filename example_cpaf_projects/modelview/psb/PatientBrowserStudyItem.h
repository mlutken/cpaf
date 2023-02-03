#ifndef PATIENTBROWSERSTUDYITEM_H
#define PATIENTBROWSERSTUDYITEM_H

#include <PatientBrowserBaseItem.h>

class PatientBrowserStudyItem : public PatientBrowserBaseItem
{
public:
    static PatientBrowserBaseItem* create() { return new PatientBrowserStudyItem();}

    PatientBrowserStudyItem () : PatientBrowserBaseItem() {}
    explicit PatientBrowserStudyItem(PatientBrowserBaseItem *parent) : PatientBrowserBaseItem(parent) {}

private:
    virtual QString doDisplayText () const;
};

#endif // PATIENTBROWSERSTUDYITEM_H
