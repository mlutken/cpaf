#include "MainWindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}


//int main(int argc, char **argv)
//{
//    QApplication app(argc, argv);

//    QLabel *l1 = new QLabel("This very long text doesn't influence "
//                            "the width of the parent widget");
//    l1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
//    // Style just to make it clear that the widget is
//    // being resized to fit the parent, it doesn't "overflow"
//    l1->setFrameShape(QFrame::Box);
//    l1->setFrameShadow(QFrame::Raised);
//    l1->setAlignment(Qt::AlignHCenter);

//    QLabel *l2 = new QLabel("This influences the width");
//    l2->setFrameShape(QFrame::Box);
//    l2->setFrameShadow(QFrame::Raised);

//    QWidget w;
//    QVBoxLayout layout(&w);
//    layout.addWidget(l1);
//    layout.addWidget(l2);
//    w.show();
//    return app.exec();
//}
