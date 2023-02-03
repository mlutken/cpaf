// main.cpp
#include <QtGui/QApplication>
#include <QtGui/QTableView>
#include <camutility/SynonymsEditor.h>
#include <camutility/SynonymsEditorDialog.h>

using namespace camutility;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SynonymsEditor syn_dlg;
    syn_dlg.show();
    return a.exec();
}
