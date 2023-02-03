#include "DrawWidgetGl.h"

#include <QPainter>
#include <QTimer>

#include "PainterTest.h"

DrawWidgetGl::DrawWidgetGl(QWidget *parent) :
    QGLWidget(parent)
{
    setFixedSize(200, 200);

}


void DrawWidgetGl::animate()
{
    m_elapsed = (m_elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    repaint();
}

void DrawWidgetGl::paintEvent(QPaintEvent *event)
{
    PainterTest pt;
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    pt.paint(&painter, event, m_elapsed);
    painter.end();
}
