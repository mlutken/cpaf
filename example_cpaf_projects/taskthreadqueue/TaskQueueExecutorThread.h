#ifndef TASKQUEUEEXECUTOR_H
#define TASKQUEUEEXECUTOR_H

#include <iostream>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <SingleWriterSingleReaderLocklessFifo.h>
#include <TaskExecutorThreadBase.h>
#include <Task.h>

namespace cpaf { namespace concurrent {


template <typename RETURN_TYPE>
class TaskQueueExecutorThread : public TaskExecutorThreadBase
{
public:
    typedef Task<RETURN_TYPE> TaskType;
    typedef SingleWriterSingleReaderLocklessFifo<boost::shared_ptr<TaskType> > FifoQueue;

    explicit TaskQueueExecutorThread(FifoQueue* fifo)
        : m_fifo(fifo)
    {

    }



private:
//    virtual void doProcess1()
//    {
//        while (!m_fifo->empty() && !stopRequested()){
//            boost::shared_ptr<TaskType> task = m_fifo->front();
//            task->runJobFunction();
//            m_fifo->pop();
//            signalTaskDone(task);
//        }
//    }

    virtual void doProcess()
    {
        while (!m_about_to_exit) {
            if(stopRequested()) return;
            if (!m_fifo->empty()) {
                boost::shared_ptr<TaskType> task = m_fifo->front();
                task->runJobFunction();
                m_fifo->pop();
                signalTaskDone(task);
            }

            {   // Lock scope
                QMutexLocker lock(&m_about_to_exit_guard);
                if (m_fifo->empty()) {
                    std::cout << "EMPTY 1" << std::endl;
//                    sleep(3);
                    std::cout << "EMPTY 2" << std::endl;
                    m_about_to_exit = true;
                }
            }
        }
        std::cout << "Thread STOPPING, fifo empty: " << m_fifo->empty() << std::endl;
//        sleep(5);
    }


    // Data
    FifoQueue* m_fifo;
};

}} //END namespace cpaf::concurrent

#endif // TASKQUEUEEXECUTOR_H
