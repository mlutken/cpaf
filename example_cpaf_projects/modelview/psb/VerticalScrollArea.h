#ifndef VERTICALSCROLLAREA_H
#define VERTICALSCROLLAREA_H

#include <QScrollArea>

class QEvent;

class VerticalScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit VerticalScrollArea(QWidget *parent = 0);
    virtual bool eventFilter(QObject *o, QEvent *e);
signals:

public slots:

};

#endif // VERTICALSCROLLAREA_H
