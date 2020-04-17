#include "DxfImportModel.h"

DxfImportModel::DxfImportModel(QObject *parent) : QAbstractTableModel(parent)
{
    dxfImport = DxfImport::getInstance();
    tableData.append(heading);
    QObject::connect(dxfImport, &DxfImport::importFinished, this, &DxfImportModel::updateModel);
}

int DxfImportModel::rowCount(const QModelIndex &) const
{
    return tableData.size();//Number of rows
}

int DxfImportModel::columnCount(const QModelIndex &) const
{
    return tableData.at(0).size();//Columns
}

QVariant DxfImportModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case TableDataRole:
    {
        return  tableData.at(index.row()).at(index.column());
    }
    case HeadingRole:
    {
        if( index.row()==0){
            return true;
        }else{
            return false;
        }
    }
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> DxfImportModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TableDataRole] = "tabledata";
    roles[HeadingRole] = "heading";
    return roles;
}

void DxfImportModel::updateModel()
{
    // Remove old data
    if(tableData.size() > 1)
    {
        beginRemoveRows(QModelIndex(), 1, tableData.size() - 1);
        tableData.remove(1, tableData.size() - 1);
        endRemoveRows();
    }
    // Insert Data
    QVector<QVector<QString>> insertData = dxfImport->getTable();
    beginInsertRows(QModelIndex(), 1, 1 + insertData.size());
    tableData.append(insertData);
    endInsertRows();
}
