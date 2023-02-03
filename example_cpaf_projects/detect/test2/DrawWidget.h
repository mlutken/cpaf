#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include <QPixmap>

class PainterTest;

class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWidget(QWidget *parent = 0);

    void loadImage(const QString& path);

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event);

private:
    int         m_elapsed;
    QPixmap     m_pixmap;


};

#endif // DRAWWIDGET_H
