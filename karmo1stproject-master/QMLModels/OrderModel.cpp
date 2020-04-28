#include "OrderModel.h"

OrderModel::OrderModel(QObject *parent) : QAbstractTableModel(parent)
{
    tableData.append(orderHeading);
    sql = Sql::getInstance();
    tableData.append(sql->enquireOrder());
    QObject::connect(sql, &Sql::updateOrderTable, this, &OrderModel::newOrder);
}

int OrderModel::rowCount(const QModelIndex &) const
{
    return tableData.size();//Number of rows
}

int OrderModel::columnCount(const QModelIndex &) const
{
    return tableData.at(0).size();//Columns
}

QVariant OrderModel::data(const QModelIndex &index, int role) const
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

QHash<int, QByteArray> OrderModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TableDataRole] = "tabledata";
    roles[HeadingRole] = "heading";
    return roles;
}

void OrderModel::newOrder(QString id, QString priority, QString qty, QString cncid, QString productiondate, QString endMill)
{
    // Insert Data
    QVector<QString> insertOrderData;
    insertOrderData.append(id);
    insertOrderData.append(priority);
    insertOrderData.append(qty);
    insertOrderData.append(cncid);
    insertOrderData.append(productiondate);
    insertOrderData.append(endMill);
    beginInsertRows(QModelIndex(), 1, 1);
    tableData.insert(1, insertOrderData);
    endInsertRows();
}
