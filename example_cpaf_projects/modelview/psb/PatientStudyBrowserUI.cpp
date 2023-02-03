#include "PatientStudyBrowserUI.h"

#include <QScrollArea>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

#include <VerticalScrollArea.h>
#include <PatientStudyBrowserView.h>

PatientStudyBrowserUI::PatientStudyBrowserUI(QWidget *parent) :
    QWidget(parent)
{

    dbgCreateContent1();
}

void PatientStudyBrowserUI::dbgSlot1()
{
    qWarning() << "PatientStudyBrowserUI::dbgSlot1()";
}


void PatientStudyBrowserUI::dbgSlot2()
{
    qWarning() << "PatientStudyBrowserUI::dbgSlot2()";
}

void PatientStudyBrowserUI::dbgSlot3()
{
    qWarning() << "PatientStudyBrowserUI::dbgSlot3()";
}

void PatientStudyBrowserUI::dbgSlot4()
{
    qWarning() << "PatientStudyBrowserUI::dbgSlot4()";
}

void PatientStudyBrowserUI::dbgSlot5()
{
    qWarning() << "PatientStudyBrowserUI::dbgSlot5()";
}

void PatientStudyBrowserUI::dbgCreateContent1()
{
    PatientStudyBrowserView* view = new PatientStudyBrowserView(this);
    QVBoxLayout* layout = new QVBoxLayout;

    VerticalScrollArea* scrollArea = new VerticalScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(view);
    layout->addWidget(scrollArea);
    setLayout(layout);
}

