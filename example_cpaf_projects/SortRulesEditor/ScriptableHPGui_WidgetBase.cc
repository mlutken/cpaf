#include "ScriptableHPGui_WidgetBase.h"
#include <iostream>
#include <QStackedWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

using namespace std;

ScriptableHPGui_WidgetBase::ScriptableHPGui_WidgetBase(QWidget *parent)
    : QWidget(parent),
      m_edit_mode_combo(new QComboBox()),
      m_widget_stack(new QStackedWidget())

{
    constructLayout();
    contructConnections();

}

ScriptableHPGui_WidgetBase::~ScriptableHPGui_WidgetBase()
{

}

/** Add widget to the internal stacked layout.
You must specify a string designating the 'edit_mode'. This is the string
that is used in the combobox to select the different viewmodes and show
the corresponding widget. */
void ScriptableHPGui_WidgetBase::addWidget(QWidget *widget, const QString &edit_mode_string_id, const QString &edit_mode_user_text)
{
    m_widget_stack->addWidget(widget);
    m_edit_mode_combo->addItem(edit_mode_user_text, edit_mode_string_id);
    m_edit_modes.push_back(edit_mode_string_id);
}

void ScriptableHPGui_WidgetBase::setEditMode(const QString &edit_mode_string_id)
{
    cout << "FIXMENM, ScriptableHPGui_WidgetBase::setEditMode: '" << edit_mode_string_id.toUtf8().data() << endl;
    const auto it = std::find(std::begin(m_edit_modes), std::end(m_edit_modes), edit_mode_string_id);
    if (it == m_edit_modes.end()) return;
    const int index = it - m_edit_modes.begin();
    m_edit_mode_combo->setCurrentIndex(index);

}

void ScriptableHPGui_WidgetBase::currentComboIndexChangedSlot(int index)
{
    m_widget_stack->setCurrentIndex(index);
}

void ScriptableHPGui_WidgetBase::constructLayout()
{
    QVBoxLayout *top_vlayout(new QVBoxLayout());
    QHBoxLayout *topbar_hlayout(new QHBoxLayout());
    topbar_hlayout->addWidget(new QLabel(tr("Select edit mode")));
    topbar_hlayout->addWidget(m_edit_mode_combo);
    topbar_hlayout->addStretch();
    top_vlayout->addLayout(topbar_hlayout);
    top_vlayout->addWidget(m_widget_stack);
    setLayout(top_vlayout);

}

void ScriptableHPGui_WidgetBase::contructConnections()
{
    connect(m_edit_mode_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(currentComboIndexChangedSlot(int)));
}

