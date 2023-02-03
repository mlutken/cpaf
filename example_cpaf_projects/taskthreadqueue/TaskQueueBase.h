#ifndef TASKQUEUEBASE_H
#define TASKQUEUEBASE_H

#include <boost/shared_ptr.hpp>
#include <QObject>
#include <TaskBase.h>

namespace cpaf { namespace concurrent {

class TaskQueueBase : public QObject
{
    Q_OBJECT
public:
    explicit TaskQueueBase(QObject *parent = 0);


signals:
    void taskDoneSignal(TaskBasePtr task_base);

protected slots:
    void taskDoneSlot(TaskBasePtr task_base);

protected:
    void emitTaskDoneSignal(TaskBasePtr task_base);

private:
    virtual void doTaskDone(TaskBasePtr task) = 0;


};

}} //END namespace cpaf::concurrent

#endif // TASKQUEUEBASE_H
