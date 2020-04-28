#include "LogModel.h"

LogModel::LogModel(QObject *parent) : QAbstractTableModel(parent)
{
    tableLogData.append(logHeading);
    sql = Sql::getInstance();
    tableLogData.append(sql->enquireLog());
    QObject::connect(sql, &Sql::updateLogTable, this, &LogModel::newLog);
}

int LogModel::rowCount(const QModelIndex &) const
{
    return tableLogData.size();//Number of rows
}

int LogModel::columnCount(const QModelIndex &) const
{
    return tableLogData.at(0).size();//Columns
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case TableDataRole:
    {
        return  tableLogData.at(index.row()).at(index.column());
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

void LogModel::newLog(QString id, QString qty, QString cncid, QString productiondate, QString endMill)
{
    // Insert Data
    QVector<QString> insertLogData;
    insertLogData.append(id);
    insertLogData.append(qty);
    insertLogData.append(cncid);
    insertLogData.append(productiondate);
    insertLogData.append(endMill);
    beginInsertRows(QModelIndex(), 1, 1);
    tableLogData.insert(1, insertLogData);
    endInsertRows();
}
