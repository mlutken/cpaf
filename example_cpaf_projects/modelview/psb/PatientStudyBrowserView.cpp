#include "PatientStudyBrowserView.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

#include <PatientBrowserYearView.h>
#include <PatientBrowserStudyView.h>

PatientStudyBrowserView::PatientStudyBrowserView(QWidget *parent) :
    QWidget(parent)
{
    dbgCreateContent1();
}

void PatientStudyBrowserView::refreshView()
{

}

void PatientStudyBrowserView::setViewMode(int /*view_mode_mask*/)
{

}


void PatientStudyBrowserView::dbgCreateContent1()
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(new PatientBrowserYearView("2013", this));
    layout->addWidget(new PatientBrowserStudyView(this));
    layout->addWidget(new PatientBrowserStudyView(this));
    layout->addWidget(new PatientBrowserYearView("2012", this));
    layout->addWidget(new PatientBrowserStudyView(this));
    layout->addWidget(new PatientBrowserStudyView(this));
    layout->addWidget(new PatientBrowserStudyView(this));

    setLayout(layout);

}
