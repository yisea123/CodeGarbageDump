#ifndef CURRENTJOBMODEL_H
#define CURRENTJOBMODEL_H

#include <QObject>
#include <QAbstractTableModel>

#include "System.h"

class CurrentJobModel : public QAbstractTableModel
{
    Q_OBJECT
    enum TableRoles{
        TableDataRole = Qt::UserRole + 1,
        HeadingRole
    };

private:
    QVector<QVector<QString>> tableData;
    QVector<QString> heading = {tr("ID"), tr("Arm Coordinate"), "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "B10", "B11", "B12", "B13", "B14", "B15"};

    System *system;

public:
    explicit CurrentJobModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void updateJob();
    void updateProcedure();
};

#endif // CURRENTJOBMODEL_H
