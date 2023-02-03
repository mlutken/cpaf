#ifndef PATIENTBROWSERYEARVIEW_H
#define PATIENTBROWSERYEARVIEW_H

#include <QWidget>
#include <PatientBrowserViewBase.h>

class PatientBrowserYearView : public QWidget,
                               public PatientBrowserViewBase
{
    Q_OBJECT
public:
    explicit PatientBrowserYearView(const QString& year, QWidget *parent = 0);

    virtual void refreshView();
    virtual void setViewMode (int view_mode_mask);

signals:

public slots:

};

#endif // PATIENTBROWSERYEARVIEW_H
