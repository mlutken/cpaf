#include "ScriptableHPGui_DisplaysetHanging.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "HPJavascriptHookEditor.h"
#include "ScriptableHPGui_DisplaySetTilingEditor.h"
#include "ScriptableHPGui_SortRulesEditor.h"

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


ScriptableHPGui_DisplaysetHanging::ScriptableHPGui_DisplaysetHanging(QWidget *parent)
    : ScriptableHPGui_WidgetBase(parent),
      m_tiling_sorting_container(new QWidget(this)),
      m_tiling_editor( new ScriptableHPGui_DisplaySetTilingEditor(this)),
      m_sort_rules_editor(new ScriptableHPGui_SortRulesEditor(this)),
      m_assign_hanging_geometry_js(new HPJavascriptHookEditor(this))
{
    m_assign_hanging_geometry_js->setExamplesCode(examples);
    constructLayout();
    contructConnections();
    setEditMode("SortingTilingMode");
}

ScriptableHPGui_DisplaysetHanging::~ScriptableHPGui_DisplaysetHanging()
{

}

void ScriptableHPGui_DisplaysetHanging::constructLayout()
{
    QVBoxLayout *top_container_vlayout(new QVBoxLayout());
    top_container_vlayout->addWidget(m_tiling_editor);
    top_container_vlayout->addStretch();
    top_container_vlayout->addWidget(m_sort_rules_editor);
    top_container_vlayout->addStretch();
    m_tiling_sorting_container->setLayout(top_container_vlayout);
    addWidget(m_tiling_sorting_container, "SortingTilingMode", tr("Specify display sorting and tiling"));
    addWidget(m_assign_hanging_geometry_js, "AssignGeometryModeJS", tr("Javascript: Assign exact geometry for each display set"));
}

void ScriptableHPGui_DisplaysetHanging::contructConnections()
{
}
