#ifndef SCRIPTABLEHPGUI_SORTRULESEDITOR_H
#define SCRIPTABLEHPGUI_SORTRULESEDITOR_H

#include <QWidget>


/** Implements a model-view with combo boxes etc. The Model created should
use a DisplaySetSortRules as data source. */
class ScriptableHPGui_SortRulesEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptableHPGui_SortRulesEditor(QWidget *parent = 0);
    virtual ~ScriptableHPGui_SortRulesEditor() override;

Q_SIGNALS:

public Q_SLOTS:

private Q_SLOTS:

private:
    void constructLayout();
    void contructConnections();
};

#endif // SCRIPTABLEHPGUI_SORTRULESEDITOR_H
