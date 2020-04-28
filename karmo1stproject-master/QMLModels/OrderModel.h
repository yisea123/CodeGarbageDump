#ifndef ORDERMODEL_H
#define ORDERMODEL_H

#include <QObject>
#include <QAbstractTableModel>

#include "sql.h"

class OrderModel : public QAbstractTableModel
{
    Q_OBJECT
    enum TableRoles{
        TableDataRole = Qt::UserRole + 1,
        HeadingRole
    };

private:
    //2*2 Dimensional Size to Store the table
    QVector<QVector<QString>> tableData;


    QVector<QString> orderHeading = {tr("ID"), tr("Priority"), tr("QTY"), tr("CNCID"), tr("Production Date"), tr("End mill Set")};
    Sql *sql;

public:
    explicit OrderModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void newOrder(QString id, QString priority, QString qty, QString cncid, QString productiondate, QString endMill);
};


#endif // ORDERMODEL_H
