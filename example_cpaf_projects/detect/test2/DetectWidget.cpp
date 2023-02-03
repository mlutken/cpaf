#include "DetectWidget.h"

#include <iostream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

#include "DrawWidget.h"
#include "DrawWidgetGl.h"

using namespace std;

DetectWidget::DetectWidget(QWidget *parent)
    : QWidget(parent)
{
    m_btnExit       = new QPushButton( tr("Exit"), this);
    m_drawWidget    = new DrawWidget(this);
////    m_drawWidgetGl  = new DrawWidgetGl(this);
    QVBoxLayout* layoutTop = new QVBoxLayout;

    layoutTop->addWidget(m_drawWidget);
////    layoutTop->addWidget(m_drawWidgetGl);
    layoutTop->addWidget(m_btnExit);

    setLayout(layoutTop);

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), m_drawWidget, SLOT(animate()));
////    connect(timer, SIGNAL(timeout()), m_drawWidgetGl, SLOT(animate()));
    timer->start(50);


    // Signal slot connections ---
    connect(m_btnExit, SIGNAL(clicked(bool)), this, SLOT(onExit()));
}

DetectWidget::~DetectWidget()
{

}

void DetectWidget::loadImage(const QString& path)
{
    m_drawWidget->loadImage(path);
}

void DetectWidget::onExit()
{
    cout << "DetectWidget::onExit" << endl;
    close();
}
