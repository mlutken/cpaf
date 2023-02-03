#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <ModelData.h>

class DomModel;
class PatientBrowserModel;
class PatientBrowserBaseItem;

QT_BEGIN_NAMESPACE
class QMenu;
class QTreeView;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(PatientBrowserBaseItem* data_root_item);
    virtual ~MainWindow();

public slots:
    void openFile();

private:
    PatientBrowserBaseItem *m_data_root_item;
    PatientBrowserModel *model;
    QMenu *fileMenu;
    QString xmlPath;
    QTreeView *view;
};

#endif // MAINWINDOW_H
