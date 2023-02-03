#ifndef CAMUTILITY_SYNONYMSEDITORMODEL_H
#define CAMUTILITY_SYNONYMSEDITORMODEL_H

#include <array>
#include <vector>
#include <QAbstractTableModel>
#include <camutility/config.h>

namespace camutility {

const int COLS= 3;
const int ROWS= 4;


class CAMUTILITY_EXPORT SynonymsEditorModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SynonymsEditorModel(QObject *parent);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    virtual Qt::ItemFlags flags(const QModelIndex & index) const override;
    virtual bool insertRows ( int row, int count, const QModelIndex & parent = QModelIndex() ) override;
    virtual bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() ) override;

    void appendEmptySynonym();
    void removeSelectedRow();

Q_SIGNALS:
    void editCompleted(const QString &);

public Q_SLOTS:

private:
    using RowVec = std::array<QString,COLS>;
    using SynonymsMatrix = std::vector<RowVec>;
    SynonymsMatrix m_grid_data;  // holds text entered into QTableView
//    QString m_gridData[ROWS][COLS];  // holds text entered into QTableView


};

} // END namespace camutility


#endif // CAMUTILITY_SYNONYMSEDITORMODEL_H
