#ifndef DRAWWIDGETGL_H
#define DRAWWIDGETGL_H

#include <QGLWidget>

class DrawWidgetGl : public QGLWidget
{
    Q_OBJECT
public:
    explicit DrawWidgetGl(QWidget *parent = 0);


public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event);

private:
    int m_elapsed;


};

#endif // DRAWWIDGETGL_H
