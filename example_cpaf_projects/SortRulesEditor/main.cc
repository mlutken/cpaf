#include "MainWindow.h"

#include <iostream>
#include <QApplication>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QStandardItemModel>
#include <QTableView>
#include <QFile>
#include <QFileInfo>
#include <QDesktopWidget>

#include <qtutility/JavascriptEdit.h>
#include "SimpleComboBoxDelegate.h"
//#include "SynonymsEditorDialog.h"
#include "ScriptableHPGui_SortRulesEditor.h"
#include "HPJavascriptHookEditor.h"
#include "ScriptableHPGui_DisplaysetHanging.h"

#include "ScriptableHPGui_TemporaryTabsForDevelopment.h"

using namespace std;


static const QString create_display_sets=R"JAVASCRIPT(
dbgStdOut('ML_DBG:  create_display_sets: series.dicom("SeriesDescription"): ' +  series.dicom("SeriesDescription") );
dbgStdOut('ML_DBG:  create_display_sets: series.childCount(): ' +  series.childCount());
var x = new DisplaySet();
x.setLabel("X");
return [x];
)JAVASCRIPT";



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    ScriptableHPGui_SortRulesEditor w;
    ScriptableHPGui_TemporaryTabsForDevelopment w;
    w.setWindowTitle("ScriptableHPGui_DisplaysetHanging");
    w.resize(QApplication::desktop()->availableGeometry().size() / 2);
    w.show();

    return a.exec();
}


//int main(int argc, char *argv[])
//{
//    std::cout << "Enter." << std::endl;
//    QApplication app(argc, argv);

//    QStandardItemModel model(4, 2);
//    QTableView tableView;
//    tableView.setModel(&model);

//    ComboBoxDelegate* delegate = new ComboBoxDelegate({"Choice 1", "Choice 2"}, &tableView);
//    //tableView.setItemDelegate(delegate);
//    tableView.setItemDelegateForColumn(1, delegate); // Column 0 can take any value, column 1 can only take values up to 8.

//    for (int row = 0; row < 4; ++row)
//    {
//        for (int column = 0; column < 2; ++column)
//        {
//            QModelIndex index = model.index(row, column, QModelIndex());
//            int value = (row+1) * (column+1);
//            std::cout << "Setting (" << row << ", " << column << ") to " << value << std::endl;
//            model.setData(index, QVariant(value));
//        }
//    }

//    // Make the combo boxes always displayed.

//    for ( int col = 0; col < model.columnCount(); ++col )
//    {
//        for ( int row = 0; row < model.rowCount(); ++row )
//        {
//            tableView.openPersistentEditor( model.index(row, col) );
//        }
//    }

//    tableView.show();
//    return app.exec();
//}


//int main(int argc, char **argv)
//{
//    QApplication app(argc, argv);

//    HPJavascriptHookEditor editor;
//    editor.setWindowTitle("create_display_sets");
// //   editor.setFrameShape(JavascriptEdit::NoFrame);
////    editor.setWordWrapMode(QTextOption::NoWrap);
// //   editor.setTabStopWidth(4);
//    editor.resize(QApplication::desktop()->availableGeometry().size() / 2);

//    QStringList keywords = editor.keywords();
//    keywords << "const";
//    keywords << "let";
//    editor.setKeywords(keywords);
//    editor.setColorSchemeDark();

//    editor.setHookHeader("create_display_sets", "series");
//    editor.setHookCodeBody(create_display_sets);
//    editor.setExamplesCode("// Examples\n");
//    editor.show();

//    return app.exec();
//}

//int main(int argc, char **argv)
//{
//    QApplication app(argc, argv);

//    const char *fileName = (argc > 1) ? argv[1] : "../SortRulesEditor/hook.js";
//    QFile file(fileName);
//    file.open(QFile::ReadOnly);
//    QString contents = file.readAll();
//    file.close();
//    contents.replace('\t', "  ");

//    JavascriptEdit editor;
//    editor.setWindowTitle(QFileInfo(fileName).fileName());
//    editor.setFrameShape(JavascriptEdit::NoFrame);
//    editor.setWordWrapMode(QTextOption::NoWrap);
//    editor.setTabStopWidth(4);
//    editor.resize(QApplication::desktop()->availableGeometry().size() / 2);

//    QStringList keywords = editor.keywords();
//    keywords << "const";
//    keywords << "let";
//    editor.setKeywords(keywords);
//    editor.setColorSchemeDark();

//    editor.setPlainText(contents);
//    editor.show();

//    return app.exec();
//}


