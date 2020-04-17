#include "LogModel.h"

LogModel::LogModel(QObject *parent) : QAbstractTableModel(parent)
{
    tableData.append(heading);
    sql = Sql::getInstance();
    tableData.append(sql->enquireLog());
    QObject::connect(sql, &Sql::updateLogTable, this, &LogModel::newLog);
}

int LogModel::rowCount(const QModelIndex &) const
{
    return tableData.size();//Number of rows
}

int LogModel::columnCount(const QModelIndex &) const
{
    return tableData.at(0).size();//Columns
}

QVariant LogModel::data(const QModelIndex &index, int role) const
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

QHash<int, QByteArray> LogModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TableDataRole] = "tabledata";
    roles[HeadingRole] = "heading";
    return roles;
}

void LogModel::newLog(QString id, QString date, QString time, QString event)
{
    // Insert Data
    QVector<QString> insertData;
    insertData.append(id);
    insertData.append(date);
    insertData.append(time);
    insertData.append(event);
    beginInsertRows(QModelIndex(), 1, 1);
    tableData.insert(1, insertData);
    endInsertRows();
}
