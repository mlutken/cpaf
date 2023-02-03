#ifndef TASK_H
#define TASK_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <TaskBase.h>

namespace cpaf { namespace concurrent {

template <typename RETURN_TYPE> class Task : public TaskBase
{
private:
    class TaskData;

public:
    typedef boost::function<RETURN_TYPE()> JobFunction;
    typedef boost::function<void(const RETURN_TYPE &)> ResultReadyFunction;
    typedef boost::function<void(const Task &)> TaskDoneFunction;

    // NOTE: The Fifo queue requires a default contructor, since it allocates a contiguous array
    Task()
    {
    }

    Task(const JobFunction &job_fun)
        : m_task_data(boost::make_shared<TaskData>(job_fun))
    {
    }

    Task(const JobFunction &job_fun, const ResultReadyFunction &res_ready_fun)
        : m_task_data(boost::make_shared<TaskData>(job_fun, res_ready_fun))
    {
    }

    Task(const JobFunction &job_fun, const TaskDoneFunction &task_done_fun)
        : m_task_data(boost::make_shared<TaskData>(job_fun, task_done_fun))
    {
    }

    void runJobFunction()
    {
        m_task_data->m_result = m_task_data->m_job_function();
        m_task_data->m_task_done = true;
    }

    const RETURN_TYPE &result() const
    {
        return m_task_data->m_result;
    }

    bool taskDone() const
    {
        return m_task_data->m_task_done;
    }

    void callDoneAndReadyFunctions()
    {
        if (m_task_data->m_result_ready_function){
            m_task_data->m_result_ready_function(m_task_data->m_result);
        }
        if (m_task_data->m_task_done_function){
            m_task_data->m_task_done_function(*this);
        }
    }


private:
    // --- TaskData private helper class ---
    class TaskData
    {
    public:

        TaskData(const JobFunction &job_fun)
            : m_job_function(job_fun), m_task_done(false)
        {
        }

        TaskData(const JobFunction &job_fun, const ResultReadyFunction &res_ready_fun)
            : m_job_function(job_fun), m_result_ready_function(res_ready_fun), m_task_done(false)
        {
        }

        TaskData(const JobFunction &job_fun, const TaskDoneFunction &task_done_fun)
            : m_job_function(job_fun), m_task_done_function(task_done_fun), m_task_done(false)
        {
        }


        JobFunction m_job_function;
        ResultReadyFunction m_result_ready_function;
        TaskDoneFunction m_task_done_function;
        RETURN_TYPE m_result;
        bool m_task_done;
    };

    // --- Other private definitions, functions and data ---
    typedef boost::shared_ptr<TaskData> TaskDataPtr;

    TaskDataPtr m_task_data;
};

}} //END namespace cpaf::concurrent

#endif // TASK_H
