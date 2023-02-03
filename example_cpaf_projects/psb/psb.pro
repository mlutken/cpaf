#-------------------------------------------------
#
# Project created by QtCreator 2016-05-26T11:04:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = psb
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    PSB.cpp \
    PSBPatient.cpp \
    PatientLevelItem.cpp \
    PSBVerticalView.cpp \
    PSBItemBase.cpp

HEADERS  += MainWindow.h \
    PSB.h \
    PSBPatient.h \
    PatientLevelItem.h \
    PSBVerticalView.h \
    PSBItemBase.h

FORMS    += MainWindow.ui
