QT += widgets

HEADERS       = PSBDelegate.h \
                stardelegate.h \
                stareditor.h \
                starrating.h
SOURCES       = main.cpp \
                PSBDelegate.cpp \
                stardelegate.cpp \
                stareditor.cpp \
                starrating.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/itemviews/stardelegate
INSTALLS += target

simulator: warning(This example might not fully work on Simulator platform)
