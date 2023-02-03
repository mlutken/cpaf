#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class QPushButton;
class PatientStudyBrowserUI;

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);

signals:

public slots:

private:
    PatientStudyBrowserUI *m_psb_ui;
    QPushButton *m_btn1;
    QPushButton *m_btn2;
    QPushButton *m_btn3;
    QPushButton *m_btn4;
    QPushButton *m_btn5;
};

#endif // MAINWIDGET_H
