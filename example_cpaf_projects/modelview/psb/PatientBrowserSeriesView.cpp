#include "PatientBrowserSeriesView.h"

#include <boost/foreach.hpp>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>

PatientBrowserSeriesView::PatientBrowserSeriesView(QWidget *parent) :
    QWidget(parent), m_compact_mode(true)
{
    // --- Set default positions of labels ---
    m_pos_to_label_name.push_back("modality");
    m_pos_to_label_name.push_back("slices_count");
    m_pos_to_label_name.push_back("series_title");

    m_label_widgets.push_back(new QLabel("0", this)); // 0
    m_label_widgets.push_back(new QLabel("1", this)); // 1
    m_label_widgets.push_back(new QLabel("2", this)); // 2

    m_layout = new QGridLayout;

    m_thumbnail_pixmap = new QPixmap("../psb/thumbnail1.png");
    m_thumbnail_label = new QLabel();
    m_thumbnail_label->setPixmap(*m_thumbnail_pixmap);
    m_layout->addWidget(m_thumbnail_label, 0, 0, 2, 2);
    m_layout->addWidget(m_label_widgets[0], 0, 2);
    m_layout->addWidget(m_label_widgets[1], 0, 3);
    m_layout->addWidget(m_label_widgets[2], 1, 2, 1, 2);

    setLayout(m_layout);

    updateView();
}

void PatientBrowserSeriesView::refreshView()
{

}

void PatientBrowserSeriesView::setViewMode(int /*view_mode_mask*/)
{

}


void PatientBrowserSeriesView::updateView()
{
    //QPixmap QPixmap::scaled(const QSize & size, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio, Qt::TransformationMode transformMode = Qt::FastTransformation) const
    m_thumbnail_label->setPixmap(m_thumbnail_pixmap->scaled(QSize(100,100), Qt::KeepAspectRatio));
    const unsigned int N = m_label_widgets.size();
    for ( unsigned int i = 0; i < N; ++i) {
        QLabel* const label = m_label_widgets[i];
        label->setText( getLabelText(labelPosToName(i)));
    }
    if (!m_compact_mode) m_label_widgets[2]->hide();
}

QString PatientBrowserSeriesView::getLabelText(const QString &label_name)
{
    if (label_name == "modality") return "MR";
    else if (label_name == "slices_count") return "622";
    return label_name;
}

QString PatientBrowserSeriesView::labelPosToName(unsigned int pos)
{
    return m_pos_to_label_name[pos];
}


