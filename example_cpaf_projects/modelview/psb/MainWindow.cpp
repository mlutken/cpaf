#include "MainWindow.h"

#include <QDomDocument>
#include <QTreeView>
#include <QMenuBar>
#include <QFileDialog>

#include "DomModel.h"
#include "PatientBrowserModel.h"
#include <PatientBrowserBaseItem.h>
#include <PatientBrowserDebugTree.h>

#include <PatientStudyBrowserUI.h>
#include <MainWidget.h>

MainWindow::MainWindow(PatientBrowserBaseItem *data_root_item) :
    QMainWindow(), m_data_root_item(data_root_item), model(0)
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()), QKeySequence::Open);
    fileMenu->addAction(tr("E&xit"), this, SLOT(close()), QKeySequence::Quit);

    MainWidget *main_widget = new MainWidget(this);
    setCentralWidget(main_widget);


    setWindowTitle(tr("Simple DOM Model"));
}


//MainWindow::MainWindow(PatientBrowserBaseItem *data_root_item) :
//    QMainWindow(), m_data_root_item(data_root_item), model(0)
//{
//    fileMenu = menuBar()->addMenu(tr("&File"));
//    fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()), QKeySequence::Open);
//    fileMenu->addAction(tr("E&xit"), this, SLOT(close()), QKeySequence::Quit);



//    model = new PatientBrowserModel(data_root_item, this);
//    view = new QTreeView(this);
//    view->setModel(model);
//    setCentralWidget(view);

//    setWindowTitle(tr("Simple DOM Model"));
//}

//MainWindow::MainWindow(PatientBrowserBaseItem *data_root) :
//    QMainWindow(), m_data_root(data_root), model(0)
//{
//    fileMenu = menuBar()->addMenu(tr("&File"));
//    fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()), QKeySequence::Open);
//    fileMenu->addAction(tr("E&xit"), this, SLOT(close()), QKeySequence::Quit);

//    model = new PatientBrowserModel(QDomDocument(), this);
//    view = new QTreeView(this);
//    view->setModel(model);

//    QFile file("../psb/data1.xml");
//    if (file.open(QIODevice::ReadOnly)) {
//        QDomDocument document;
//        if (document.setContent(&file)) {
//            PatientBrowserModel *newModel = new PatientBrowserModel(document, this);
//            newModel->setDataTree(data_root);
//            view->setModel(newModel);
//            delete model;
//            model = newModel;
//        }
//        file.close();
//    }


//    setCentralWidget(view);
//    setWindowTitle(tr("Simple DOM Model"));
//}

MainWindow::~MainWindow()
{
    delete m_data_root_item;
}

void MainWindow::openFile()
{
//    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
//        xmlPath, tr("JSON files (*.json);;XML files (*.xml);;HTML files (*.html);;"
//                    "SVG files (*.svg);;User Interface files (*.ui)"));

//    if (!filePath.isEmpty()) {
//        QFile file(filePath);
//        if (file.open(QIODevice::ReadOnly)) {
//            QDomDocument document;
//            if (document.setContent(&file)) {
//                PatientBrowserModel *newModel = new PatientBrowserModel(document, this);
//                view->setModel(newModel);
//                delete model;
//                model = newModel;
//                xmlPath = filePath;
//            }
//            file.close();
//        }
//    }
}
