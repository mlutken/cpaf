#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDesktopWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtGui>

#include "PSB.h"
#include "PSBItemBase.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{


//    auto layout = new QVBoxLayout();
////    auto psb = new PSB(this);
//    auto ib1 = new PSBItemBase(this);
////    auto ib1 = PSBItemBase::create_DBG(this, "Patient 1");
////    ib1->setPsbLayoutMode(PSBLayoutMode::vertical);
////    layout->addWidget(psb);


//    layout->addWidget(ib1);

//    this->setLayout(layout);

// //   setSizePolicy(QSizePolicy::Expanding , QSizePolicy::Expanding );
////    setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
//    resize(QDesktopWidget().availableGeometry(this).size() * 0.5);

    testLayout2();
}

MainWindow::~MainWindow()
{
}

void MainWindow::testLayout2()
{

    QLabel *l1 = new QLabel("This very long text doesn't influence "
                            "the width of the parent widget");
    l1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    // Style just to make it clear that the widget is
    // being resized to fit the parent, it doesn't "overflow"
    l1->setFrameShape(QFrame::Box);
    l1->setFrameShadow(QFrame::Raised);
    l1->setAlignment(Qt::AlignHCenter);

    QLabel *l2 = new QLabel("This influences the width");
    l2->setFrameShape(QFrame::Box);
    l2->setFrameShadow(QFrame::Raised);

    auto psb = new PSB(this);
    auto ib1 = PSBItemBase::create_DBG(this, "Hans", 3);
    ib1->setPsbLayoutMode(PSBLayoutMode::vertical);
    auto ib2 = PSBItemBase::create_DBG(this, "Gert", 3);
    ib2->setPsbLayoutMode(PSBLayoutMode::horizontal);


    auto layout = new QVBoxLayout();
    layout->addWidget(l1);
    layout->addWidget(l2);
    layout->addWidget(psb);
    layout->addWidget(ib1);
    layout->addWidget(ib2);

    m_central_widget = new QWidget(this);
    m_central_widget->setLayout(layout);
    setCentralWidget(m_central_widget);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.5);

}

void MainWindow::testLayout1()
{

    QLabel *l1 = new QLabel("This very long text doesn't influence "
                            "the width of the parent widget");
    l1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    // Style just to make it clear that the widget is
    // being resized to fit the parent, it doesn't "overflow"
    l1->setFrameShape(QFrame::Box);
    l1->setFrameShadow(QFrame::Raised);
    l1->setAlignment(Qt::AlignHCenter);

    QLabel *l2 = new QLabel("This influences the width");
    l2->setFrameShape(QFrame::Box);
    l2->setFrameShadow(QFrame::Raised);

    auto layout = new QVBoxLayout();
    layout->addWidget(l1);
    layout->addWidget(l2);
    m_central_widget = new QWidget(this);

    m_central_widget->setLayout(layout);

    setCentralWidget(m_central_widget);
}
