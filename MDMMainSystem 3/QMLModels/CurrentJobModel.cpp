#include "CurrentJobModel.h"

CurrentJobModel::CurrentJobModel(QObject *parent) : QAbstractTableModel(parent)
{
    system = System::getInstance();
    tableData.append(heading);
    QObject::connect(system, &System::currentJobChanged, this, &CurrentJobModel::updateJob);
    QObject::connect(system, &System::dmcProcedureFinished, this, &CurrentJobModel::updateProcedure);
}

int CurrentJobModel::rowCount(const QModelIndex &) const
{
    return tableData.size();//Number of rows
}

int CurrentJobModel::columnCount(const QModelIndex &) const
{
    return tableData.at(0).size();//Columns
}

QVariant CurrentJobModel::data(const QModelIndex &index, int role) const
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

QHash<int, QByteArray> CurrentJobModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TableDataRole] = "tabledata";
    roles[HeadingRole] = "heading";
    return roles;
}

void CurrentJobModel::updateJob()
{
    // Remove old data
    if(tableData.size() > 1)
    {
        beginRemoveRows(QModelIndex(), 1, tableData.size() - 1);
        tableData.remove(1, tableData.size() - 1);
        endRemoveRows();
    }
    // Insert Data
    QVector<QVector<QString>> insertData = system->getCurrentJobTable();
    beginInsertRows(QModelIndex(), 1, 1 + insertData.size());
    tableData.append(insertData);
    endInsertRows();
}

void CurrentJobModel::updateProcedure()
{
    // Remove old data
    if(tableData.size() > 1)
    {
        beginRemoveRows(QModelIndex(), 1, tableData.size() - 1);
        tableData.remove(1, 1);
        endRemoveRows();
    }
}
