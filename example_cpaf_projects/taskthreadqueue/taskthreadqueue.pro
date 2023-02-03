#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T15:20:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = taskthreadqueue
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    TestClasses.cc \
    TaskQueueBase.cpp \
    TaskQueue.cpp \
    TaskBase.cpp \
    Task.cpp \
    TaskExecutorThreadBase.cc

HEADERS  += MainWindow.h \
    SingleWriterSingleReaderLocklessFifo.h \
    TestClasses.h \
    TaskQueueBase.h \
    TaskQueue.h \
    TaskBase.h \
    Task.h \
    align_macros.h \
    TaskExecutorThreadBase.h \
    TaskQueueExecutorThread.h

FORMS    +=
