#ifndef PATIENTBROWSERPATIENTVIEW_H
#define PATIENTBROWSERPATIENTVIEW_H

#include <QWidget>
#include <PatientBrowserViewBase.h>

class PatientBrowserPatientView : public QWidget,
                                  public PatientBrowserViewBase
{
    Q_OBJECT
public:
    explicit PatientBrowserPatientView(QWidget *parent = 0);

    virtual void refreshView();
    virtual void setViewMode (int view_mode_mask);

signals:

public slots:

};

#endif // PATIENTBROWSERPATIENTVIEW_H
