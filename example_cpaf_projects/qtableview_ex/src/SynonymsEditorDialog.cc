#include <camutility/SynonymsEditorDialog.h>

#include <QVBoxLayout>
#include <QPushButton>
#include <camutility/SynonymsEditor.h>

namespace camutility {


SynonymsEditorDialog::SynonymsEditorDialog(QWidget *parent) :
    QDialog(parent)
{
    m_synonyms_editor = new SynonymsEditor(this);
    QVBoxLayout *vbox_layout(new QVBoxLayout(this));

    vbox_layout->addWidget(m_synonyms_editor);
    setLayout(vbox_layout);

}

} // END namespace camutility
