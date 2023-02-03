#include "MainWindow.h"

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <QVBoxLayout>
#include <QHBoxLayout>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent), m_task_queue(1000)
{
    QHBoxLayout* layout = new QHBoxLayout(this);

    m_btnAddTask = new QPushButton("Add task", this);
    m_btnTest = new QPushButton("Test", this);
    m_btnStop = new QPushButton("Stop", this);

    connect( m_btnTest, SIGNAL( clicked() ), this, SLOT( testSlot() ) );
    connect( m_btnStop, SIGNAL( clicked() ), this, SLOT( stopSlot() ) );
    connect( m_btnAddTask, SIGNAL( clicked() ), this, SLOT( addTaskSlot() ) );
    connect( &m_task_queue, SIGNAL( taskDoneSignal(TaskBasePtr) ), this, SLOT( taskDoneSlot(TaskBasePtr) ) );



//    for ( int n = 0; n < 2; n++) {
//        MyTask task = m_task_queue.push(job_fun, task_done_fun);
//    }

    // Layout
    layout->addWidget(m_btnAddTask);
    layout->addWidget(m_btnTest);
    layout->addWidget(m_btnStop);
    setLayout(layout);
}

MainWindow::~MainWindow()
{
}

void MainWindow::testSlot()
{
    cout << "MainWindow::testSlot() called" << endl;

}

void MainWindow::stopSlot()
{
    cout << "MainWindow::stopSlot() called" << endl;
    m_task_queue.stop();
}

void MainWindow::taskDoneSlot(TaskBasePtr task_base)
{
    boost::shared_ptr<MyTask> task = boost::static_pointer_cast<MyTask>(task_base);

//    cout << "MainWindow::taskDoneSlot() , result: " << task->result() << endl;

}

void MainWindow::addTaskSlot()
{
    static int n = 0;
    n++;

    string user_data = "User data #";
    user_data += boost::lexical_cast<string>(n);

    cout << "Adding new task... " << user_data << endl;
    MyTask::JobFunction job_fun = &myJobFunInt;
    MyTask::ResultReadyFunction ready_fun = &myResultReadyFunInt;

    MyTask::TaskDoneFunction task_done_fun =
        boost::bind(&myTaskDoneFunIntString, _1, user_data);

//    MyTask::TaskDoneFunction task_done_fun = &myTaskDoneFunIntString;

    MyTask task = m_task_queue.push(job_fun, task_done_fun);
}
