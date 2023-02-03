#include "PatientBrowserYearView.h"

#include <QVBoxLayout>
#include <QLabel>

PatientBrowserYearView::PatientBrowserYearView(const QString &year, QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(new QLabel(year, this));
    setLayout(layout);
}

void PatientBrowserYearView::refreshView()
{

}

void PatientBrowserYearView::setViewMode(int /*view_mode_mask*/)
{

}
