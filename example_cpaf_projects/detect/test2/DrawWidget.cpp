#include "DrawWidget.h"

#include <iostream>
#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QTimer>

#include "PainterTest.h"

using namespace std;

DrawWidget::DrawWidget(QWidget *parent) :
    QWidget(parent)
{
    setMinimumSize(200, 200);
}

void DrawWidget::loadImage(const QString& path)
{
    cout << "DrawWidget::loadImage( " << path.toUtf8().data() << " )" << endl;
    m_pixmap.load(path);
}


void DrawWidget::animate()
{
    m_elapsed = (m_elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    repaint();
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    PainterTest pt;
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);


    painter.drawImage( event->rect(), m_pixmap.toImage() );
    //pt.paint(&painter, event, m_elapsed);



    painter.end();
}
