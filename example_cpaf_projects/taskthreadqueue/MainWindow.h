#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>

#include <TestClasses.h>
#include <TaskQueue.h>

class MainWindow : public QWidget
{
    Q_OBJECT
    
public:
    typedef TaskQueue<int> MyTaskQueue;
    typedef MyTaskQueue::TaskType MyTask;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QPushButton* m_btnAddTask;
    QPushButton* m_btnTest;
    QPushButton* m_btnStop;

public slots:
    void testSlot();
    void stopSlot();

private slots:
    void taskDoneSlot(TaskBasePtr task_base);
    void addTaskSlot();

private:
    MyTaskQueue m_task_queue;

};

#endif // MAINWINDOW_H
