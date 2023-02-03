#ifndef TASKEXECUTORTHREADBASE_H
#define TASKEXECUTORTHREADBASE_H

#include <boost/shared_ptr.hpp>
#include <QThread>
#include <QMutex>
#include <TaskBase.h>

namespace cpaf { namespace concurrent {

class TaskExecutorThreadBase : public QThread
{
    Q_OBJECT
public:
    TaskExecutorThreadBase();

    virtual void run();
    bool aboutToExit() const;



signals:
    void taskDoneSignal(TaskBasePtr task_base);

public slots:
    void stop();
    void start ( Priority priority = InheritPriority );

protected:
    mutable QMutex m_about_to_exit_guard;
    bool m_about_to_exit;

    void signalTaskDone(TaskBasePtr task_base);
    bool stopRequested () const;

private:
    virtual void doProcess() = 0;

    bool m_stop_requested;

};

}} //END namespace cpaf::concurrent


#endif // TASKEXECUTORTHREADBASE_H
