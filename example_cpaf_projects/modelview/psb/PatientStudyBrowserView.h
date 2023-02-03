#ifndef PATIENSTUDYBROWSERVIEW_H
#define PATIENSTUDYBROWSERVIEW_H

#include <QWidget>
#include <vector>

#include <PatientBrowserViewBase.h>

class PatientBrowserStudyView;
class PatientBrowserYearView;

class PatientStudyBrowserView : public QWidget,
                                public PatientBrowserViewBase
{
    Q_OBJECT
public:
    explicit PatientStudyBrowserView(QWidget *parent = 0);

    virtual void refreshView();
    virtual void setViewMode (int view_mode_mask);

signals:

public slots:

private:
    void dbgCreateContent1();
    typedef std::vector<PatientBrowserStudyView*> StudyViewVec;
    typedef std::vector<PatientBrowserYearView*> YearViewVec;


};

#endif // PATIENSTUDYBROWSERVIEW_H
