#ifndef SIMPLECOMBOBOXDELEGATE_H
#define SIMPLECOMBOBOXDELEGATE_H

#include <string>
#include <vector>

#include <QWidget>
#include <QItemDelegate>

class QModelIndex;
class QWidget;
class QVariant;
class QComboBox;

/** Simple combobox delegate for adding comboboxes to model-view containers.
Used in ScriptableHPGui_SortRulesEditor.
*/
class SimpleComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    using StringVec = std::vector<QString>;

    SimpleComboBoxDelegate(const StringVec& initial_values, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    StringVec m_items;
    mutable QComboBox *m_combo_box;
};

#endif // SIMPLECOMBOBOXDELEGATE_H
