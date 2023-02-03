#include <camutility/SynonymsEditorModel.h>

#include <iostream>

using namespace std;

namespace camutility {

// http://qt-project.org/doc/qt-4.8/modelview.html

SynonymsEditorModel::SynonymsEditorModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    for(int row= 0; row < ROWS; row++)
    {
        RowVec row_data;
        for(int col= 0; col < COLS; col++)
        {
            row_data[col]  = QString::number(row) + ", " + QString::number(col);
        }
        m_grid_data.push_back(row_data);
    }

}

int SynonymsEditorModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_grid_data.size();
}

int SynonymsEditorModel::columnCount(const QModelIndex & /*parent*/) const
{
    return COLS;
}

QVariant SynonymsEditorModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return m_grid_data[index.row()][index.column()];
    }
    return QVariant();
}

QVariant SynonymsEditorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("Synonym (list) name");
            case 1:
                return QString("Synonyms (comma separated)");
            case 2:
                return QString("Group names (comma separated)");
            }
        }
        else
            return QAbstractTableModel::headerData(section, orientation, role);
    }
    return QVariant();
}

bool SynonymsEditorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        // Save value from editor to member m_gridData
        const QString str_data = value.toString();
        m_grid_data[index.row()][index.column()] = str_data;
        Q_EMIT editCompleted(str_data);
    }
    return true;
}

Qt::ItemFlags SynonymsEditorModel::flags(const QModelIndex & /*index*/) const
{
    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}

bool SynonymsEditorModel::insertRows(int row, int count, const QModelIndex &)
{
    beginInsertRows(QModelIndex(), row, row+count-1);

    const auto it_insert = m_grid_data.begin()+row;
    if (it_insert > m_grid_data.end())
        return false;
    m_grid_data.insert( it_insert, count, RowVec() );
    endInsertRows();
    return true;
}



bool SynonymsEditorModel::removeRows(int row, int count, const QModelIndex &)
{

    const auto it_end = m_grid_data.end();
    const auto it_first = m_grid_data.begin()+row;
    if (it_first >= it_end)
        return false;
    auto it_last = it_first + count;
    if (it_last > it_end)
        it_last = it_end;

    beginRemoveRows(QModelIndex(), row, row+count-1);
    m_grid_data.erase(it_first, it_last);
    endRemoveRows();
    return true;
}

void SynonymsEditorModel::appendEmptySynonym()
{
    insertRow(rowCount());
//    int row = rowCount();
//    QModelIndex idx = index(row, 0);
//    beginInsertRows(idx, row, row );

//    m_grid_data.push_back({"1", "2", "3"});
//////    m_gridData.erase(m_gridData.begin()+2);
//    // insert your data

    //    endInsertRows();
}




} // END namespace camutility
