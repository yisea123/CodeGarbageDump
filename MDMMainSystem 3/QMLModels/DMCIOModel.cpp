#include "DMCIOModel.h"

QString DMCIOModel::bitToStr(bool bit)
{
    if(bit)
        return QString(tr("Normal"));
    else
        return QString(tr("Abnormal"));
}

DMCIOModel::DMCIOModel(QObject *parent) : QAbstractTableModel(parent)
{
    dmc = DMC::getInstance();
    tableData.append(heading);
    QObject::connect(dmc, &DMC::inputStatus, this, &DMCIOModel::updateModel);
}

int DMCIOModel::rowCount(const QModelIndex &) const
{
    return tableData.size();//Number of rows
}

int DMCIOModel::columnCount(const QModelIndex &) const
{
    return tableData.at(0).size();//Columns
}

QVariant DMCIOModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case TableDataRole:
    {
        return  tableData.at(index.row()).at(index.column());
    }
    case HeadingRole:
    {
        if( index.row()==0 || index.column() == 0){
            return true;
        }else{
            return false;
        }
    }
    case TrueRole:
    {
        if(tableData.at(index.row()).at(index.column()) == "Normal")
            return true;
        else
            return false;
    }
    case NARole:
    {
        if(tableData.at(index.row()).at(index.column()) == "/")
            return true;
        else
            return false;
    }
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> DMCIOModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TableDataRole] = "tabledata";
    roles[HeadingRole] = "heading";
    roles[TrueRole] = "truerole";
    roles[NARole] = "narole";
    return roles;
}

void DMCIOModel::updateModel(QString in0, QString in1)
{
    beginInsertRows(QModelIndex(), 0, 1);
    // ORG
    //tableData[1][1] = bitToStr(in1.at(24));
    //tableData[2][1] = bitToStr(in1.at(25));
    //tableData[3][1] = bitToStr(in1.at(26));
    // EL+
   // tableData[1][2] = bitToStr(!in0.at(8));
    //tableData[2][2] = bitToStr(!in0.at(9));
    //tableData[3][2] = bitToStr(!in0.at(10));
    // EL-
    //tableData[1][3] = bitToStr(!in0.at(0));
    //tableData[2][3] = bitToStr(!in0.at(1));
    //tableData[3][3] = bitToStr(!in0.at(2));
    // Safe Area
    tableData[1][4] = "/";
    tableData[2][4] = "Normal";
    tableData[3][4] = "/";
    endInsertRows();
}
