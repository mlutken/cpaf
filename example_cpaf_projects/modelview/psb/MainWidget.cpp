#include "MainWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include <PatientStudyBrowserUI.h>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout;

    m_psb_ui = new PatientStudyBrowserUI(this);
    m_btn1 = new QPushButton("Btn1 Expand test", this);
    m_btn2 = new QPushButton("Btn2 Collapse test", this);
    m_btn3 = new QPushButton("Btn3 Compact mode", this);
    m_btn4 = new QPushButton("Btn4 Large mode", this);
    m_btn5 = new QPushButton("Btn5", this);

    layout->addWidget(m_psb_ui);

    QVBoxLayout* layoutV = new QVBoxLayout;

    layoutV->addWidget(m_btn1);
    layoutV->addWidget(m_btn2);
    layoutV->addWidget(m_btn3);
    layoutV->addWidget(m_btn4);
    layoutV->addWidget(m_btn5);

    layout->addLayout(layoutV);
    setLayout(layout);


    // Connect signal/slots
    connect(m_btn1, SIGNAL(clicked(bool)), m_psb_ui, SLOT(dbgSlot1()));
    connect(m_btn2, SIGNAL(clicked(bool)), m_psb_ui, SLOT(dbgSlot2()));
    connect(m_btn3, SIGNAL(clicked(bool)), m_psb_ui, SLOT(dbgSlot3()));
    connect(m_btn4, SIGNAL(clicked(bool)), m_psb_ui, SLOT(dbgSlot4()));
    connect(m_btn5, SIGNAL(clicked(bool)), m_psb_ui, SLOT(dbgSlot5()));
}
