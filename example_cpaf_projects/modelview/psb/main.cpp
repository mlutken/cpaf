#include "MainWindow.h"
#include <QApplication>
#include <ModelData.h>
#include <PatientBrowserDebugTree.h>

#include <PatientBrowserBaseItem.h>
#include <PatientBrowserRootItem.h>
#include <PatientBrowserYearItem.h>
#include <PatientBrowserPatientItem.h>
#include <PatientBrowserStudyItem.h>
#include <PatientBrowserSeriesItem.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ModelData md("../psb/data1.json");

//    ModelData::dbgTest1();

    PatientBrowserBaseItem* data_root_item = PatientBrowserDebugTree::createSimple();
    data_root_item = PatientBrowserDebugTree::readFromJson("../psb/data1.json");

    PatientBrowserDebugTree::print(data_root_item, 0);

    MainWindow w(data_root_item);
    w.show();

    return a.exec();
}
