#ifndef CAMUTILITY_SYNONYMSEDITOR_H
#define CAMUTILITY_SYNONYMSEDITOR_H

#include <QWidget>
#include <camutility/config.h>

class QTableView;
class QPushButton;

namespace camutility {

class SynonymsEditorModel;

class CAMUTILITY_EXPORT SynonymsEditor : public QWidget
{
    Q_OBJECT
public:
    explicit SynonymsEditor(QWidget *parent = 0);


protected:
    virtual void showEvent(QShowEvent *show_event) override;


Q_SIGNALS:

public Q_SLOTS:

private Q_SLOTS:
    void addSynonymSlot();
    void removeSynonymSlot();


private:
    QTableView *m_view;
    SynonymsEditorModel *m_model;
    QPushButton *m_add_synonym_btn;
    QPushButton *m_remove_synonym_btn;
};

} // END namespace camutility


#endif // CAMUTILITY_SYNONYMSEDITOR_H
