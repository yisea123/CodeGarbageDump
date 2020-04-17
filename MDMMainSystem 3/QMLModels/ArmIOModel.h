#ifndef ARMIOMODEL_H
#define ARMIOMODEL_H

#include <QObject>
#include <QAbstractTableModel>

#include "DMC.h"

class ArmIOModel : public QAbstractTableModel
{
    Q_OBJECT
    enum TableRoles{
        TableDataRole = Qt::UserRole + 1,
        HeadingRole,
        TrueRole
    };

private:
    QVector<QVector<QString>> tableData;
    QVector<QString> heading = {tr("Gripper"), "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};

    DMC *dmc;

public:
    explicit ArmIOModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void updateGripper(int node, bool open);
    void updateVibration(int node, bool on);
    void updateInput();
};

#endif // ARMIOMODEL_H
