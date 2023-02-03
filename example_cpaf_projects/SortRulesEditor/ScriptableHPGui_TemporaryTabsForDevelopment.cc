#include "ScriptableHPGui_TemporaryTabsForDevelopment.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTabWidget>

#include "ScriptableHPGui_DisplaysetHanging.h"
#include "ScriptableHPGui_SeriesSelection.h"
#include "ScriptableHPGui_PresentationGroupAssignment.h"
#include "ScriptableHPGui_DisplaySetGeneration.h"

using namespace std;

ScriptableHPGui_TemporaryTabsForDevelopment::ScriptableHPGui_TemporaryTabsForDevelopment(QWidget *parent)
    : QWidget(parent),
      m_tabs(new QTabWidget(this)),
      m_series_selection(new ScriptableHPGui_SeriesSelection(this)),
      m_displayset_generation(new ScriptableHPGui_DisplaySetGeneration()),
      m_presentation_groups_assignment(new ScriptableHPGui_PresentationGroupAssignment()),
      m_displayset_hanging(new ScriptableHPGui_DisplaysetHanging(this))
{
    m_tabs->addTab(m_series_selection, tr("Series selection"));
    m_tabs->addTab(m_displayset_generation, tr("Display Set Generation"));
    m_tabs->addTab(m_presentation_groups_assignment, tr("Assign presentation groups"));
    m_tabs->addTab(m_displayset_hanging, tr("Hanging"));

    constructLayout();
    contructConnections();

}

ScriptableHPGui_TemporaryTabsForDevelopment::~ScriptableHPGui_TemporaryTabsForDevelopment()
{

}



void ScriptableHPGui_TemporaryTabsForDevelopment::constructLayout()
{

    QVBoxLayout *top_vlayout(new QVBoxLayout());
    QHBoxLayout *topbar_hlayout(new QHBoxLayout());
    topbar_hlayout->addWidget(new QLabel("FIXMENM, TODO: Temporary class with tabs for each HP pipeline step."));
    topbar_hlayout->addStretch();
    top_vlayout->addLayout(topbar_hlayout);
    top_vlayout->addWidget(m_tabs);
    setLayout(top_vlayout);

}

void ScriptableHPGui_TemporaryTabsForDevelopment::contructConnections()
{
}
