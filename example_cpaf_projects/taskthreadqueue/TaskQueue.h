#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <iostream>
#include <boost/function.hpp>
#include <QMetaType>
#include <SingleWriterSingleReaderLocklessFifo.h>
#include <TaskQueueBase.h>
#include <Task.h>
#include <TaskQueueExecutorThread.h>

namespace cpaf { namespace concurrent {

template <typename RETURN_TYPE>
class TaskQueue : public TaskQueueBase
{
public:

    typedef Task<RETURN_TYPE> TaskType;
    typedef TaskQueueExecutorThread<RETURN_TYPE> ExecutorThread;
    typedef typename TaskType::JobFunction JobFunction;
    typedef typename TaskType::ResultReadyFunction ResultReadyFunction;
    typedef typename TaskType::TaskDoneFunction TaskDoneFunction;

    explicit TaskQueue(boost::uint32_t queue_size)
        : m_fifo(queue_size), m_executor_thread(&m_fifo)
    {
        qRegisterMetaType<TaskBasePtr>("TaskBasePtr"); // Register type for signal/slot system
        QObject::connect( &m_executor_thread, SIGNAL(taskDoneSignal(TaskBasePtr)), this, SLOT(taskDoneSlot(TaskBasePtr)) );
        m_executor_thread.start();
    }

    ~TaskQueue()
    {
        m_executor_thread.stop();
        m_executor_thread.wait();
    }

    TaskType push (const JobFunction &job_fun)
    {
        boost::shared_ptr<TaskType> task = boost::make_shared<TaskType>(job_fun);
        m_fifo.push(task);
        ensureThreadIsRunning();
        return *task;
    }

    TaskType push (const JobFunction &job_fun, const ResultReadyFunction &res_ready_fun)
    {
        boost::shared_ptr<TaskType> task = boost::make_shared<TaskType>(job_fun, res_ready_fun);
        m_fifo.push(task);
        ensureThreadIsRunning();
        return *task;
    }

    TaskType push (const JobFunction &job_fun, const TaskDoneFunction &task_done_fun)
    {
        boost::shared_ptr<TaskType> task = boost::make_shared<TaskType>(job_fun, task_done_fun);
        m_fifo.push(task);
        ensureThreadIsRunning();
        return *task;
    }

    void stop()
    {
        m_executor_thread.stop();
    }

private:
    virtual void doTaskDone( TaskBasePtr task_base)
    {
        using namespace std;
        boost::shared_ptr<TaskType> task = boost::static_pointer_cast<TaskType>(task_base);

        task->callDoneAndReadyFunctions();
        emitTaskDoneSignal(task_base);
    }

    void ensureThreadIsRunning()
    {
        if (m_executor_thread.aboutToExit()) {
            std::cout << "THREAD about to exit. So we wait for it." << std::endl;
            m_executor_thread.wait();
        }
        if (!m_executor_thread.isRunning()) m_executor_thread.start();
    }

    typedef SingleWriterSingleReaderLocklessFifo<boost::shared_ptr<TaskType> > FifoQueue;
    FifoQueue m_fifo;
    ExecutorThread m_executor_thread;
};

}} //END namespace cpaf::concurrent


#endif // TASKQUEUE_H
