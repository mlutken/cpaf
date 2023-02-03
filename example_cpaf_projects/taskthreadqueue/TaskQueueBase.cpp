#include "TaskQueueBase.h"

#include <iostream>

using namespace std;

namespace cpaf { namespace concurrent {

TaskQueueBase::TaskQueueBase(QObject *parent) :
    QObject(parent)
{
}


void TaskQueueBase::taskDoneSlot(TaskBasePtr task_base)
{
    doTaskDone(task_base);
}

void TaskQueueBase::emitTaskDoneSignal(TaskBasePtr task_base)
{
    emit taskDoneSignal(task_base);
}

}} //END namespace cpaf::concurrent
