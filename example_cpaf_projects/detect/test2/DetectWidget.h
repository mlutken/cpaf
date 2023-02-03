#ifndef DETECTWIDGET_H
#define DETECTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QPixmap>

class DrawWidget;
class DrawWidgetGl;

class DetectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DetectWidget(QWidget *parent = 0);
    virtual ~DetectWidget();

    void loadImage(const QString& path);

signals:

public slots:
    void onExit();

private:
    QPushButton*    m_btnExit;
    DrawWidget*     m_drawWidget;
////    DrawWidgetGl*   m_drawWidgetGl;
    QPixmap         m_pixmap;
};

#endif // DETECTWIDGET_H
