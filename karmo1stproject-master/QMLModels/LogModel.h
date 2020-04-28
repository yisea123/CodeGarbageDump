#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <QObject>
#include <QAbstractTableModel>

#include "sql.h"

class LogModel : public QAbstractTableModel
{
    Q_OBJECT
    enum TableRoles{
        TableDataRole = Qt::UserRole + 1,
        HeadingRole
    };

private:
    //2*2 Dimensional Size to Store the table
    QVector<QVector<QString>> tableLogData;

    //QVector<QString> heading = {tr("ID"), tr("Date"), tr("Time"), tr("Event")};

    QVector<QString> logHeading = {tr("ID"), tr("QTY"), tr("CNCID"), tr("Production Date"), tr("End mill Set")};
    Sql *sql;

public:
    explicit LogModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void newLog(QString id, QString qty, QString cncid, QString productiondate, QString endMill);
};


#endif // LOGMODEL_H
