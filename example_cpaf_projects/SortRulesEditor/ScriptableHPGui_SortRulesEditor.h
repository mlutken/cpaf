#ifndef CAMUTILITY_SYNONYMSEDITOR_H
#define CAMUTILITY_SYNONYMSEDITOR_H

#include <QWidget>

class QTableView;
class QPushButton;

class ScriptableHP_SortRulesEditorModel;

class ScriptableHPGui_SortRulesEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptableHPGui_SortRulesEditor(QWidget *parent = 0);


protected:
    virtual void showEvent(QShowEvent *show_event) override;


Q_SIGNALS:

public Q_SLOTS:

private Q_SLOTS:
    void addSynonymSlot();
    void removeSynonymSlot();


private:
    QTableView *m_view;
    ScriptableHP_SortRulesEditorModel *m_model;
    QPushButton *m_add_sortrule_btn;
    QPushButton *m_remove_sortrule_btn;
};

#endif // CAMUTILITY_SYNONYMSEDITOR_H
