#ifndef SCRIPTABLEHPGUI_TEMPORARYTABSFORDEVELOPMENT_H
#define SCRIPTABLEHPGUI_TEMPORARYTABSFORDEVELOPMENT_H

#include <QWidget>

class QTabWidget;
class ScriptableHPGui_SeriesSelection;
class ScriptableHPGui_DisplaySetGeneration;
class ScriptableHPGui_PresentationGroupAssignment;
class ScriptableHPGui_DisplaysetHanging;

/** TODO, Temporary class with tabs for each HP pipeline step.
In general2d we already have a class holding tabs for the step and this should be used
eventually - unless we decide to make a completely new one.
*/
class ScriptableHPGui_TemporaryTabsForDevelopment : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptableHPGui_TemporaryTabsForDevelopment(QWidget *parent = 0);
    virtual ~ScriptableHPGui_TemporaryTabsForDevelopment() override;

Q_SIGNALS:

public Q_SLOTS:

private Q_SLOTS:
private:
    void constructLayout();
    void contructConnections();

    QTabWidget *m_tabs;
    ScriptableHPGui_SeriesSelection *m_series_selection;
    ScriptableHPGui_DisplaySetGeneration *m_displayset_generation;
    ScriptableHPGui_PresentationGroupAssignment *m_presentation_groups_assignment;
    ScriptableHPGui_DisplaysetHanging *m_displayset_hanging;
};

#endif // SCRIPTABLEHPGUI_TEMPORARYTABSFORDEVELOPMENT_H
