#include "ScriptableHPGui_PresentationGroupAssignment.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "HPJavascriptHookEditor.h"

using namespace std;

///Test javascript as C++11 raw string!
static const QString examples=R"JAVASCRIPT(
// Example 1
// TODO: Description and do a real example, this is just a placeholder .
dbgStdOut('ML_DBG:  assign_hanging_geometry: PGroup size: ' +  display_sets.length );
for (var i = 0; i < display_sets.length; i++) {
   dbgStdOut( "[" + i + "]: " + display_sets[i] );
   dbgStdOut("assign_hanging_geometry.getPresentationGroupDescription(): " + display_sets[i].getPresentationGroupDescription() );
   dbgStdOut("assign_hanging_geometry.getDisplaySetPresentationGroup() : " + display_sets[i].getDisplaySetPresentationGroup() );
}

// Example 2
// TODO: Description and do a real example, this is just a placeholder .
dbgStdOut('ML_DBG:  assign_hanging_geometry: PGroup size: ' +  display_sets.length );
for (var i = 0; i < display_sets.length; i++) {
   dbgStdOut( "[" + i + "]: " + display_sets[i] );
   dbgStdOut("assign_hanging_geometry.getPresentationGroupDescription(): " + display_sets[i].getPresentationGroupDescription() );
   dbgStdOut("assign_hanging_geometry.getDisplaySetPresentationGroup() : " + display_sets[i].getDisplaySetPresentationGroup() );
}
)JAVASCRIPT";


ScriptableHPGui_PresentationGroupAssignment::ScriptableHPGui_PresentationGroupAssignment(QWidget *parent)
    : ScriptableHPGui_WidgetBase(parent),
      m_assign_presentation_groups(new HPJavascriptHookEditor(this))
{
    m_assign_presentation_groups->setExamplesCode(examples);
    constructLayout();
    contructConnections();
    setEditMode("AssignPresentationGroupsModeJS");
}

ScriptableHPGui_PresentationGroupAssignment::~ScriptableHPGui_PresentationGroupAssignment()
{

}

void ScriptableHPGui_PresentationGroupAssignment::constructLayout()
{
    addWidget(m_assign_presentation_groups, "AssignPresentationGroupsModeJS", tr("Javascript: Assign exact presentation groups for each display set"));
}

void ScriptableHPGui_PresentationGroupAssignment::contructConnections()
{
}


