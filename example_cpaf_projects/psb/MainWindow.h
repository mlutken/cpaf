#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void testLayout2();
    void testLayout1();

private:
    QWidget *m_central_widget;
};

#endif // MAINWINDOW_H
