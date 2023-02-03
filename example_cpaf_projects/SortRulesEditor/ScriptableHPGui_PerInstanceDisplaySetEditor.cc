#include "ScriptableHPGui_PerInstanceDisplaySetEditor.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

using namespace std;

ScriptableHPGui_PerInstanceDisplaySetEditor::ScriptableHPGui_PerInstanceDisplaySetEditor(QWidget *parent)
    : QWidget(parent),
      m_modalities_list(new QLineEdit(this)),
      m_per_instance_threshold_spinbox(new QSpinBox(this))
{
  constructLayout();
  contructConnections();

}

ScriptableHPGui_PerInstanceDisplaySetEditor::~ScriptableHPGui_PerInstanceDisplaySetEditor()
{

}

void ScriptableHPGui_PerInstanceDisplaySetEditor::constructLayout()
{
    QVBoxLayout *top_vlayout(new QVBoxLayout());
    QHBoxLayout *topbar_hlayout(new QHBoxLayout());
    QHBoxLayout *modalities_list_hlayout(new QHBoxLayout());
    QHBoxLayout *per_instance_threshold_hlayout(new QHBoxLayout());
    topbar_hlayout->addWidget(new QLabel(tr("Specify modalities as commaseparated list and a threshold (max number of instance in series).")));
    topbar_hlayout->addStretch();
    modalities_list_hlayout->addWidget(new QLabel(tr("Modalities list (comma separated): ")));
    modalities_list_hlayout->addWidget(m_modalities_list);
    modalities_list_hlayout->addStretch();
    per_instance_threshold_hlayout->addWidget(new QLabel(tr("Number of instances threshold: ")));
    per_instance_threshold_hlayout->addWidget(m_per_instance_threshold_spinbox);
    per_instance_threshold_hlayout->addStretch();
    top_vlayout->addLayout(modalities_list_hlayout);
    top_vlayout->addLayout(per_instance_threshold_hlayout);
    top_vlayout->addStretch();
    setLayout(top_vlayout);
}

void ScriptableHPGui_PerInstanceDisplaySetEditor::contructConnections()
{
}
