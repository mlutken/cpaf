#include "TestClasses.h"

#include <iostream>
#include <QThread>

using namespace std;

// -----------

int myJobFunInt()
{
    static int i = 0;
    i++;

//    cout << "Inside job function 'myJobFunInt()': " << i << endl;
    QThread::msleep(800);

    return i;
}

void myResultReadyFunInt(const int& i)
{
    cout << "myResultReadyFunInt(): " << i << endl;
}

void myTaskDoneFunIntString(const MyTaskIntString& task, const string &user_data)
{
    cout << "myTaskDoneFunIntString(), result: " << task.result() << "  User data: "  << user_data << endl;
}

// ------------



void MyObject::MySlot()
{
    static int count = 0;
    count++;
    std::cout << "slot called: " << count << std::endl;
}


void CThread1::run()
{
    std::cout << "thread 1 started\n" << std::endl;
    emit MySignal();
    int i = 0;
    while(1)
    {
        msleep( 200 );
        i +=200;
        if(i % 1000 == 0) {
            std::cout << "thread 1 emit\n" << std::endl;

            emit MySignal();
        }
    }
}


void CThread2::run()
{
    std::cout << "thread 2 started\n" << std::endl;
    exec();
}




