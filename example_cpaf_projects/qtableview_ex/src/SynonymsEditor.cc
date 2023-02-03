#include <camutility/SynonymsEditor.h>

#include <set>
#include <iostream>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableView>

#include <evbase/utility.h>
#include <camutility/SynonymsEditorModel.h>

using namespace std;

namespace camutility {

SynonymsEditor::SynonymsEditor(QWidget *parent) :
    QWidget(parent)
{

    m_view = new QTableView(this);
    m_model = new SynonymsEditorModel(this);
    m_view->setModel(m_model);
  //  m_view->setSizePolicy();
//    m_view->setSizePolicy ( QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred) );
    m_view->setSizePolicy ( QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum) );

    m_add_synonym_btn = new QPushButton("Add synonym");
    m_remove_synonym_btn = new QPushButton("Remove selected synonym");
    QVBoxLayout *vbox_layout(new QVBoxLayout(this));

    vbox_layout->addWidget(m_add_synonym_btn);
    vbox_layout->addWidget(m_remove_synonym_btn);
    vbox_layout->addWidget(m_view);
    setLayout(vbox_layout);

    // Signal slot connnections
    EV_CONNECT(m_add_synonym_btn, SIGNAL(clicked()), this,SLOT(addSynonymSlot()));
    EV_CONNECT(m_remove_synonym_btn, SIGNAL(clicked()), this,SLOT(removeSynonymSlot()));
}

void SynonymsEditor::showEvent(QShowEvent *show_event)
{
    QWidget::showEvent(show_event);
    m_view->resizeColumnsToContents();
    updateGeometry();
    adjustSize();
}

void SynonymsEditor::addSynonymSlot()
{
    m_model->appendEmptySynonym();
}

void SynonymsEditor::removeSynonymSlot()
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




} // END namespace camutility
