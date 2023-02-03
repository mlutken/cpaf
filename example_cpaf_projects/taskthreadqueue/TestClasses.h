#ifndef TESTCLASSES_H
#define TESTCLASSES_H

#include <string>
#include <QObject>
#include <QThread>
#include <Task.h>

using namespace cpaf::concurrent;

typedef Task<int> MyTaskIntString;

int myJobFunInt();
void myResultReadyFunInt( const int& i);
void myTaskDoneFunIntString( const MyTaskIntString& task, const std::string& user_data);

////typedef boost::function<void (const Task&)> TaskDoneFunction;



class MyObject : public QObject
{
    Q_OBJECT
public slots:
    void MySlot( void );
};


class CThread1 : public QThread
{
    Q_OBJECT
public:
    void run( void );
signals:
    void MySignal( void );
};


class CThread2 : public QThread
{
    Q_OBJECT
public:
    void run( void );
};

#endif // TESTCLASSES_H
