#-------------------------------------------------
#
# Project created by QtCreator 2014-01-20T09:16:02
#
#-------------------------------------------------

QT       += core gui xml widgets

TARGET = psb
TEMPLATE = app

INCLUDEPATH += /usr/include/boost/

SOURCES += main.cpp\
    DomItem.cpp \
    DomModel.cpp \
    MainWindow.cpp \
    ModelData.cpp \
    PatientBrowserBaseItem.cpp \
    PatientBrowserItemFactory.cpp \
    PatientBrowserDebugTree.cpp \
    PatientBrowserModel.cpp \
    PatientBrowserPatientItem.cpp \
    PatientBrowserStudyItem.cpp \
    PatientBrowserSeriesItem.cpp \
    PatientBrowserYearItem.cpp \
    PatientBrowserRootItem.cpp \
    PatientBrowserModelItem.cpp \
    PatientStudyBrowserUI.cpp \
    VerticalScrollArea.cpp \
    PatientStudyBrowserView.cpp \
    PatientBrowserPatientView.cpp \
    PatientBrowserYearView.cpp \
    PatientBrowserStudyView.cpp \
    PatientBrowserSeriesView.cpp \
    PatientBrowserStudyHeaderView.cpp \
    MainWidget.cpp \
    PatientBrowserViewBase.cpp

HEADERS  += DomItem.h \
    DomModel.h \
    MainWindow.h \
    ModelData.h \
    PatientBrowserBaseItem.h \
    PatientBrowserDebugTree.h \
    PatientBrowserItemFactory.h \
    PatientBrowserModel.h \
    PatientBrowserPatientItem.h \
    PatientBrowserStudyItem.h \
    PatientBrowserSeriesItem.h \
    PatientBrowserYearItem.h \
    PatientBrowserRootItem.h \
    PatientBrowserModelItem.h \
    PatientStudyBrowserUI.h \
    VerticalScrollArea.h \
    PatientStudyBrowserView.h \
    PatientBrowserPatientView.h \
    PatientBrowserYearView.h \
    PatientBrowserStudyView.h \
    PatientBrowserSeriesView.h \
    PatientBrowserStudyHeaderView.h \
    MainWidget.h \
    PatientBrowserViewBase.h

OTHER_FILES += \
    data1.json
