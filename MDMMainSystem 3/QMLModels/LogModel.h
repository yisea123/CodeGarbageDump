#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <QObject>
#include <QAbstractTableModel>

#include "Sql.h"

class LogModel : public QAbstractTableModel
{
    Q_OBJECT
    enum TableRoles{
        TableDataRole = Qt::UserRole + 1,
        HeadingRole
    };

private:
    QVector<QVector<QString>> tableData;
    QVector<QString> heading = {tr("ID"), tr("Date"), tr("Time"), tr("Event")};

    Sql *sql;

public:
    explicit LogModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void newLog(QString id, QString date, QString time, QString event);
};


#endif // LOGMODEL_H
