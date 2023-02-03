#ifndef SCRIPTABLEHPGUI_DISPLAYSETGENERATION_H
#define SCRIPTABLEHPGUI_DISPLAYSETGENERATION_H

#include "ScriptableHPGui_WidgetBase.h"

class ScriptableHPGui_PerInstanceDisplaySetEditor;
class HPJavascriptHookEditor;

class ScriptableHPGui_DisplaySetGeneration : public ScriptableHPGui_WidgetBase
{
public:
    explicit ScriptableHPGui_DisplaySetGeneration(QWidget *parent = nullptr);
    virtual ~ScriptableHPGui_DisplaySetGeneration() override;
Q_SIGNALS:

public Q_SLOTS:

private Q_SLOTS:

private:
    void constructLayout();
    void contructConnections();

    ScriptableHPGui_PerInstanceDisplaySetEditor *m_per_instance_editor;
    HPJavascriptHookEditor *m_per_instance_display_sets_js;
    HPJavascriptHookEditor *m_create_display_sets_js;
};

#endif // SCRIPTABLEHPGUI_DISPLAYSETGENERATION_H
