#include "ScriptableHPGui_SortRulesEditor.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

using namespace std;

ScriptableHPGui_SortRulesEditor::ScriptableHPGui_SortRulesEditor(QWidget *parent)
    : QWidget(parent)
{
    constructLayout();
    contructConnections();

}

ScriptableHPGui_SortRulesEditor::~ScriptableHPGui_SortRulesEditor()
{

}



void ScriptableHPGui_SortRulesEditor::constructLayout()
{
    QVBoxLayout *top_vlayout(new QVBoxLayout());
    QHBoxLayout *topbar_hlayout(new QHBoxLayout());
    topbar_hlayout->addWidget(new QLabel("FIXMENM, TODO: Implement the sort rules editor"));
    topbar_hlayout->addStretch();
    top_vlayout->addLayout(topbar_hlayout);
    setLayout(top_vlayout);
}

void ScriptableHPGui_SortRulesEditor::contructConnections()
{
}

