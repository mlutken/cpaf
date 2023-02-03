#include "PatientBrowserStudyView.h"

#include <QVBoxLayout>

#include <PatientBrowserStudyHeaderView.h>
#include <PatientBrowserSeriesView.h>

PatientBrowserStudyView::PatientBrowserStudyView(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;
    m_header = new PatientBrowserStudyHeaderView(this);
    layout->addWidget(m_header);

    layout->addWidget(new PatientBrowserSeriesView);
    layout->addWidget(new PatientBrowserSeriesView);


    setLayout(layout);
}


void PatientBrowserStudyView::refreshView()
{

}

void PatientBrowserStudyView::setViewMode(int /*view_mode_mask*/)
{

}
