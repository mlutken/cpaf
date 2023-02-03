#include "TaskExecutorThreadBase.h"

#include <iostream>

using namespace std;


namespace cpaf { namespace concurrent {

TaskExecutorThreadBase::TaskExecutorThreadBase()
    : m_about_to_exit_guard(), m_about_to_exit(false), m_stop_requested(false)
{

}

void TaskExecutorThreadBase::run()
{
    cout << "Thread START" << endl;
    m_about_to_exit = false;
    m_stop_requested = false;
    doProcess();
    cout << "Thread STOPPED" << endl;
}

bool TaskExecutorThreadBase::aboutToExit() const
{
    QMutexLocker lock(&m_about_to_exit_guard);
    return m_about_to_exit;
}

void TaskExecutorThreadBase::stop()
{
    m_stop_requested = true;
}

void TaskExecutorThreadBase::start(QThread::Priority priority)
{
    m_stop_requested = false;
    QThread::start(priority);
}

void TaskExecutorThreadBase::signalTaskDone(TaskBasePtr task_base)
{
    emit taskDoneSignal(task_base);
}

bool TaskExecutorThreadBase::stopRequested() const
{
    return m_stop_requested;
}

}} //END namespace cpaf::concurrent
