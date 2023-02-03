#ifndef PATIENTBROWSERVIEWBASE_H
#define PATIENTBROWSERVIEWBASE_H

class PatientBrowserViewBase
{
public:
    const static int vmStudyHeadersCompact = 1 << 0;

    virtual void refreshView() = 0;
    virtual void setViewMode (int view_mode_mask) = 0;
};

#endif // PATIENTBROWSERVIEWBASE_H
