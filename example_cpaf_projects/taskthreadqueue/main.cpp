#include <iostream>
#include <string>

#include "MainWindow.h"
#include <QApplication>
#include <QThread>

#include <TestClasses.h>
#include <TaskQueue.h>

using namespace std;


typedef TaskQueue<int> MyTaskQueue;
typedef MyTaskQueue::ExecutorThread MyTaskQueueExecutor;
typedef MyTaskQueue::TaskType MyTask;



int main3(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}


int main(int argc, char *argv[])
{
    return main3(argc, argv);
}
