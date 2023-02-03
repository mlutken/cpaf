#ifndef PATIENTBROWSERSTUDYHEADERVIEW_H
#define PATIENTBROWSERSTUDYHEADERVIEW_H

#include <vector>
#include <map>
#include <QWidget>
#include <QString>
#include <PatientBrowserViewBase.h>

class QLabel;
class QGridLayout;

/**
Layout:
study_date | study_title
study_ascension_number

*/
class PatientBrowserStudyHeaderView : public QWidget,
                                      public PatientBrowserViewBase
{
    Q_OBJECT
public:
    explicit PatientBrowserStudyHeaderView(QWidget *parent = 0);
    void updateView();

    virtual void refreshView();
    virtual void setViewMode (int view_mode_mask);

signals:

public slots:
private:
    void createExpandedLayout();
    QString getLabelText (const QString& label_name);
    QString labelPosToName (unsigned int pos);
    typedef std::vector<QLabel*> WidgetVec;
    typedef std::vector<QString> StrVec;
    bool m_compact_mode;
    StrVec m_pos_to_label_name;
    WidgetVec m_label_widgets; /// We have 3 positions: Each of the positions needs a pointer to a QLabel so we can change the text

    QGridLayout* m_layout;

};

#endif // PATIENTBROWSERSTUDYHEADERVIEW_H
