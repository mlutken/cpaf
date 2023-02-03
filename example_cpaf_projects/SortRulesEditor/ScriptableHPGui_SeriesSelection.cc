#include "ScriptableHPGui_SeriesSelection.h"

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


ScriptableHPGui_SeriesSelection::ScriptableHPGui_SeriesSelection(QWidget *parent)
    : ScriptableHPGui_WidgetBase(parent),
      m_accept_series_js(new HPJavascriptHookEditor(this))
{
    m_accept_series_js->setExamplesCode(examples);
    constructLayout();
    contructConnections();
    setEditMode("AcceptSeriesModeJS");
}

ScriptableHPGui_SeriesSelection::~ScriptableHPGui_SeriesSelection()
{

}

void ScriptableHPGui_SeriesSelection::constructLayout()
{
    addWidget(m_accept_series_js, "AcceptSeriesModeJS", tr("Javascript: Assign accept or reject each image series"));
}

void ScriptableHPGui_SeriesSelection::contructConnections()
{
}
