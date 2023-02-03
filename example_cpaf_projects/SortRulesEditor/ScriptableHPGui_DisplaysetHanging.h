#ifndef SCRIPTABLEHPGUI_DISPLAYSETHANGING_H
#define SCRIPTABLEHPGUI_DISPLAYSETHANGING_H

#include "ScriptableHPGui_WidgetBase.h"

class ScriptableHPGui_DisplaySetTilingEditor;
class ScriptableHPGui_SortRulesEditor;
class HPJavascriptHookEditor;

class ScriptableHPGui_DisplaysetHanging : public ScriptableHPGui_WidgetBase
{
public:
    explicit ScriptableHPGui_DisplaysetHanging(QWidget *parent = nullptr);
    virtual ~ScriptableHPGui_DisplaysetHanging() override;
Q_SIGNALS:

public Q_SLOTS:

private Q_SLOTS:

private:
    void constructLayout();
    void contructConnections();

    QWidget *m_tiling_sorting_container;
    ScriptableHPGui_DisplaySetTilingEditor *m_tiling_editor;
    ScriptableHPGui_SortRulesEditor *m_sort_rules_editor;
    HPJavascriptHookEditor *m_assign_hanging_geometry_js;
};

#endif // SCRIPTABLEHPGUI_DISPLAYSETHANGING_H
