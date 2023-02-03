#ifndef PAINTERTEST_H
#define PAINTERTEST_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>

class PainterTest
{
public:
    PainterTest();
    void paint(QPainter *painter, QPaintEvent *event, int elapsed);

private:
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
};

#endif // PAINTERTEST_H
