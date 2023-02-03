#include "ScriptableHPGui_DisplaySetGeneration.h"

#include "HPJavascriptHookEditor.h"
#include "ScriptableHPGui_PerInstanceDisplaySetEditor.h"

using namespace std;

///Test javascript as C++11 raw string!
static const QString examples_per_instance_display_sets=R"JAVASCRIPT(
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

///Test javascript as C++11 raw string!
static const QString examples_create_display_sets=R"JAVASCRIPT(
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


ScriptableHPGui_DisplaySetGeneration::ScriptableHPGui_DisplaySetGeneration(QWidget *parent)
    : ScriptableHPGui_WidgetBase(parent),
      m_per_instance_editor(new ScriptableHPGui_PerInstanceDisplaySetEditor()),
      m_per_instance_display_sets_js(new HPJavascriptHookEditor()),
      m_create_display_sets_js(new HPJavascriptHookEditor())
{
    m_per_instance_display_sets_js->setExamplesCode(examples_per_instance_display_sets);
    m_create_display_sets_js->setExamplesCode(examples_create_display_sets);
    constructLayout();
    contructConnections();
    setEditMode("PerInstanceMode");
}



ScriptableHPGui_DisplaySetGeneration::~ScriptableHPGui_DisplaySetGeneration()
{

}

void ScriptableHPGui_DisplaySetGeneration::constructLayout()
{
    addWidget(m_per_instance_editor, "PerInstanceMode", tr("Specify modalities and threshold for per instance display sets"));
    addWidget(m_per_instance_display_sets_js, "PerInstanceModeJS", tr("Javascript: Simple function that if you return true creates per instance display sets"));
    addWidget(m_create_display_sets_js, "CreateDisplaySetsModeJS", tr("Javascript: Freely create display sets"));
}

void ScriptableHPGui_DisplaySetGeneration::contructConnections()
{
}
