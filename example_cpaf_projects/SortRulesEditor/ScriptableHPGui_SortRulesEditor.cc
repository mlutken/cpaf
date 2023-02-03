#include "ScriptableHPGui_SortRulesEditor.h"

#include <set>
#include <iostream>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableView>

#include "ScriptableHP_SortRulesEditorModel.h"
#include "SimpleComboBoxDelegate.h"

using namespace std;


ScriptableHPGui_SortRulesEditor::ScriptableHPGui_SortRulesEditor(QWidget *parent) :
    QWidget(parent)
{

    m_view = new QTableView(this);
    m_model = new ScriptableHP_SortRulesEditorModel(this);
    m_view->setModel(m_model);
  //  m_view->setSizePolicy();
//    m_view->setSizePolicy ( QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred) );
    m_view->setSizePolicy ( QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum) );

    m_add_sortrule_btn = new QPushButton("Add sorting rule");
    m_remove_sortrule_btn = new QPushButton("Remove selected sorting rule");
    QVBoxLayout *vbox_layout(new QVBoxLayout(this));

    vbox_layout->addWidget(m_add_sortrule_btn);
    vbox_layout->addWidget(m_remove_sortrule_btn);
    vbox_layout->addWidget(m_view);
    setLayout(vbox_layout);

    // Signal slot connnections
    connect(m_add_sortrule_btn, SIGNAL(clicked()), this,SLOT(addSynonymSlot()));
    connect(m_remove_sortrule_btn, SIGNAL(clicked()), this,SLOT(removeSynonymSlot()));
    SimpleComboBoxDelegate* delegate = new SimpleComboBoxDelegate({"item1", "item2", "item3", "item4"}, m_view);
    m_view->setItemDelegateForColumn(2, delegate); // Column 0 can take any value, column 1 can only take values up to 8.


    std::vector<int> columns_with_persistent_editor = {2};
    for ( int col : columns_with_persistent_editor )
    {
        for ( int row = 0; row < m_model->rowCount(); ++row )
        {
            m_view->openPersistentEditor( m_model->index(row, col) );
        }
    }

}

void ScriptableHPGui_SortRulesEditor::showEvent(QShowEvent *show_event)
{
    QWidget::showEvent(show_event);
    m_view->resizeColumnsToContents();
    updateGeometry();
    adjustSize();
    resize(800, 600);
}

void ScriptableHPGui_SortRulesEditor::addSynonymSlot()
{
    m_model->appendEmptySynonym();
}

void ScriptableHPGui_SortRulesEditor::removeSynonymSlot()
{
    std::set<int> rows_to_remove;
    for (QModelIndex index : m_view->selectionModel()->selection().indexes()) {
        rows_to_remove.insert(index.row());
    }

    // Note: We need to remove from the en in order not to mess up the row numbers as we erase one row at a time.
    for (auto it = rows_to_remove.rbegin(); it != rows_to_remove.rend(); ++it ) {
        const int row = *it;
        m_model->removeRow(row);
    }

    //m_model->remove();
}
