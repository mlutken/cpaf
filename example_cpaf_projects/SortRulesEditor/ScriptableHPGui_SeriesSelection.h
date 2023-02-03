#ifndef SCRIPTABLEHPGUI_SERIESSELECTION_H
#define SCRIPTABLEHPGUI_SERIESSELECTION_H

#include "ScriptableHPGui_WidgetBase.h"
class HPJavascriptHookEditor;

class ScriptableHPGui_SeriesSelection : public ScriptableHPGui_WidgetBase
{
public:
    explicit ScriptableHPGui_SeriesSelection(QWidget *parent = nullptr);
    virtual ~ScriptableHPGui_SeriesSelection() override;

Q_SIGNALS:

public Q_SLOTS:

private Q_SLOTS:

private:
    void constructLayout();
    void contructConnections();

    HPJavascriptHookEditor *m_accept_series_js;
};

#endif // SCRIPTABLEHPGUI_SERIESSELECTION_H
