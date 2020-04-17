#include "ArmIOModel.h"

ArmIOModel::ArmIOModel(QObject *parent) : QAbstractTableModel(parent)
{
    dmc = DMC::getInstance();

    tableData.append(heading);
    // Open
    QVector<QString> open;
    open.append(tr("Open"));
    QList<bool> gripperStatus = dmc->getGripperStatus();
    for(int i = 0; i < 15; i++)
    {
        if(gripperStatus.at(i))
            open.append("True");
        else
            open.append("False");
    }
    tableData.append(open);
    // Box sensor
    QVector<QString> box;
    box.append(tr("Box sensor"));
    for(int i = 0; i < 15; i++)
        box.append("");
    tableData.append(box);
    // Open limit
    QVector<QString> limit;
    limit.append(tr("Open limit"));
    for(int i = 0; i < 15; i++)
        limit.append("");
    tableData.append(limit);
    // Vibration
    QList<bool> vibrationStatus = dmc->getVibrationStatus();
    QVector<QString> vibration;
    vibration.append(tr("Vibration"));
    for(int i = 0; i < 15; i++)
    {
        if(vibrationStatus.at(i))
            vibration.append("True");
        else
            vibration.append("False");
    }
    tableData.append(vibration);

    QObject::connect(dmc, &DMC::gripperStatusChanged, this, &ArmIOModel::updateGripper);
    QObject::connect(dmc, &DMC::vibrationStatusChanged, this, &ArmIOModel::updateVibration);
    QObject::connect(dmc, &DMC::checkArmFinished, this, &ArmIOModel::updateInput);
}

int ArmIOModel::rowCount(const QModelIndex &) const
{
    return tableData.size();//Number of rows
}

int ArmIOModel::columnCount(const QModelIndex &) const
{
    return tableData.at(0).size();//Columns
}

QVariant ArmIOModel::data(const QModelIndex &index, int role) const
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
        if(tableData.at(index.row()).at(index.column()) == "True")
            return true;
        else
            return false;
    }
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> ArmIOModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TableDataRole] = "tabledata";
    roles[HeadingRole] = "heading";
    roles[TrueRole] = "truerole";
    return roles;
}

void ArmIOModel::updateGripper(int node, bool open)
{
    beginInsertRows(QModelIndex(), 1, 1);
    if(open)
        tableData[1][node] = "True";
    else
        tableData[1][node] = "False";
    endInsertRows();
}

void ArmIOModel::updateVibration(int node, bool on)
{
    beginInsertRows(QModelIndex(), 1, 1);
    if(on)
        tableData[4][node] = "True";
    else
        tableData[4][node] = "False";
    endInsertRows();
}

void ArmIOModel::updateInput()
{
    beginInsertRows(QModelIndex(), 2, 2);
    QList<bool> boxSensor = dmc->getBoxSensor();
    QList<bool> openLimit = dmc->getOpenLimit();
    for(int i = 0; i < 15; i++)
    {
        if(boxSensor.at(i))
            tableData[2][i + 1] = "True";
        else
            tableData[2][i + 1] = "False";
        if(openLimit.at(i))
            tableData[3][i + 1] = "True";
        else
            tableData[3][i + 1] = "False";
    }
    endInsertRows();
}
