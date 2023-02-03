#include "ScriptableHPGui_DisplaySetTilingEditor.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>

using namespace std;

ScriptableHPGui_DisplaySetTilingEditor::ScriptableHPGui_DisplaySetTilingEditor(QWidget *parent)
    : QWidget(parent),
      m_rows_spinbox(new QSpinBox()),
      m_columns_spinbox(new QSpinBox())
{
  constructLayout();
  contructConnections();

}

ScriptableHPGui_DisplaySetTilingEditor::~ScriptableHPGui_DisplaySetTilingEditor()
{

}

void ScriptableHPGui_DisplaySetTilingEditor::constructLayout()
{
    QVBoxLayout *top_vlayout(new QVBoxLayout());
    QHBoxLayout *topbar_hlayout(new QHBoxLayout());
    QHBoxLayout *modalities_list_hlayout(new QHBoxLayout());
    QHBoxLayout *per_instance_threshold_hlayout(new QHBoxLayout());
    topbar_hlayout->addWidget(new QLabel(tr("Specify display set tiling in rows, columns.")));
    topbar_hlayout->addStretch();
    modalities_list_hlayout->addWidget(new QLabel(tr("Rows: ")));
    modalities_list_hlayout->addWidget(m_rows_spinbox);
    modalities_list_hlayout->addStretch();
    per_instance_threshold_hlayout->addWidget(new QLabel(tr("Columns: ")));
    per_instance_threshold_hlayout->addWidget(m_columns_spinbox);
    per_instance_threshold_hlayout->addStretch();
    top_vlayout->addLayout(modalities_list_hlayout);
    top_vlayout->addLayout(per_instance_threshold_hlayout);
    top_vlayout->addStretch();
    setLayout(top_vlayout);
}

void ScriptableHPGui_DisplaySetTilingEditor::contructConnections()
{
}
