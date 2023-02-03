#ifndef TASKBASE_H
#define TASKBASE_H

#include <boost/shared_ptr.hpp>
#include <QMetaType>

namespace cpaf { namespace concurrent {

class TaskBase
{
};

typedef boost::shared_ptr<TaskBase> TaskBasePtr;

}} //END namespace cpaf::concurrent


Q_DECLARE_METATYPE(cpaf::concurrent::TaskBase)
Q_DECLARE_METATYPE(cpaf::concurrent::TaskBasePtr)

#endif // TASKBASE_H
