#ifndef SCRIPTABLEHPGUI_PRESENTATIONGROUPASSIGNMENT_H
#define SCRIPTABLEHPGUI_PRESENTATIONGROUPASSIGNMENT_H

#include "ScriptableHPGui_WidgetBase.h"

class HPJavascriptHookEditor;

class ScriptableHPGui_PresentationGroupAssignment : public ScriptableHPGui_WidgetBase
{
public:
    explicit ScriptableHPGui_PresentationGroupAssignment(QWidget *parent = nullptr);
    virtual ~ScriptableHPGui_PresentationGroupAssignment() override;
Q_SIGNALS:

public Q_SLOTS:

private Q_SLOTS:

private:
    void constructLayout();
    void contructConnections();

    HPJavascriptHookEditor *m_assign_presentation_groups;
};

#endif // SCRIPTABLEHPGUI_PRESENTATIONGROUPASSIGNMENT_H
