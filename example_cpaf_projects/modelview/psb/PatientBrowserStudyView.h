#ifndef PATIENTBROWSERSTUDYVIEW_H
#define PATIENTBROWSERSTUDYVIEW_H

#include <vector>
#include <QWidget>
#include <PatientBrowserViewBase.h>

class PatientBrowserStudyHeaderView;
class PatientBrowserSeriesView;

class PatientBrowserStudyView : public QWidget,
                                public PatientBrowserViewBase
{
    Q_OBJECT
public:
    explicit PatientBrowserStudyView(QWidget *parent = 0);

    virtual void refreshView();
    virtual void setViewMode (int view_mode_mask);

signals:

public slots:

private:
    typedef std::vector<PatientBrowserSeriesView*> SeriesViewVec;
    PatientBrowserStudyHeaderView* m_header;
    SeriesViewVec m_series_vec;
};

#endif // PATIENTBROWSERSTUDYVIEW_H
