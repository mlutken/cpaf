TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
#QMAKE_CXXFLAGS += -o3

SOURCES += main.cpp \
    tprintf.cpp \
    make_unique.cpp \
    emplace_back.cpp \
    TestMe.cpp \
    parameter_pack.cpp \
    processing.cpp \
    mytuple.cpp \
    Delegate.cpp

HEADERS += \
    tprintf.h \
    make_unique.h \
    emplace_back.h \
    TestMe.h \
    parameter_pack.h \
    processing.h \
    mytuple.h \
    Delegate.h \
    vector_for_slides.h

