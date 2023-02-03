#include "SimpleComboBoxDelegate.h"

#include <QComboBox>
#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>

#include <iostream>

SimpleComboBoxDelegate::SimpleComboBoxDelegate(const StringVec &initial_values, QObject *parent)
    :QItemDelegate(parent), m_items(initial_values)
{
//    m_items.push_back("1 dfgf");
//    m_items.push_back("2 dfgf");
}


QWidget *SimpleComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    QComboBox* combo_box = new QComboBox(parent);
    for(unsigned int i = 0; i < m_items.size(); ++i)
    {
        combo_box->addItem(m_items[i]);
    }
    return combo_box;
}

void SimpleComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    int value = index.model()->data(index, Qt::EditRole).toUInt();
    comboBox->setCurrentIndex(value);
}

void SimpleComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    model->setData(index, comboBox->currentIndex(), Qt::EditRole);
}

void SimpleComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

void SimpleComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);
    return;
//    QStyleOptionViewItemV4 myOption = option;
////    QComboBox *combo_box = static_cast<QComboBox*>(editor);
//    QString text = m_combo_box->currentText();
////    QString text = m_items[index.row()];

//    myOption.text = text;

//    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}

