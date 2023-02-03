#ifndef PATIENTBROWSERSERIESVIEW_H
#define PATIENTBROWSERSERIESVIEW_H

#include <vector>
#include <map>
#include <QWidget>
#include <QString>
#include <PatientBrowserViewBase.h>

class QLabel;
class QGridLayout;

/**
Layout:
           modality | slices_count
Thumbnail
           series_title

*/
class PatientBrowserSeriesView : public QWidget,
                                 public PatientBrowserViewBase
{
    Q_OBJECT
public:
    explicit PatientBrowserSeriesView(QWidget *parent = 0);

    virtual void refreshView();
    virtual void setViewMode (int view_mode_mask);

    void updateView();

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
    QGridLayout* m_layout;
    WidgetVec m_label_widgets; /// We have TODO: positions: Each of the positions needs a pointer to a QLabel so we can change the text
    QPixmap *m_thumbnail_pixmap;
    QLabel *m_thumbnail_label;

};


#endif // PATIENTBROWSERSERIESVIEW_H
