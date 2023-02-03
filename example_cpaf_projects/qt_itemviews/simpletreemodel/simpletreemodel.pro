QT += widgets

HEADERS     = treeitem.h \
              treemodel.h \
              PSBDelegate.h
RESOURCES   = simpletreemodel.qrc
SOURCES     = treeitem.cpp \
              treemodel.cpp \
              PSBDelegate.cpp \
              main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/itemviews/simpletreemodel
INSTALLS += target
