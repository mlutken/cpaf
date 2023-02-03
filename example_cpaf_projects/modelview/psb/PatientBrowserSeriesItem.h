#ifndef PATIENTBROWSERSERIESITEM_H
#define PATIENTBROWSERSERIESITEM_H

#include <PatientBrowserBaseItem.h>

class PatientBrowserSeriesItem : public PatientBrowserBaseItem
{
public:
    static PatientBrowserBaseItem* create() { return new PatientBrowserSeriesItem();}

    PatientBrowserSeriesItem () : PatientBrowserBaseItem() {}
    explicit PatientBrowserSeriesItem(PatientBrowserBaseItem *parent) : PatientBrowserBaseItem(parent) {}

private:
    virtual QString doDisplayText () const;
};

#endif // PATIENTBROWSERSERIESITEM_H
