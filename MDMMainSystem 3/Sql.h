#ifndef SQL_H
#define SQL_H

#include <QObject>
#include <QtSql>
#include <QList>
#include <QVariant>
#include <QDateTime>
#include "JobItem.h"

class Sql : public QObject
{
    Q_OBJECT

private:
    static Sql *instance;
    explicit Sql(QObject *parent = nullptr);

    QSqlDatabase database;

public:
    static Sql *getInstance();

    // Job
    void newJob(JobItem *job);
    void removeJob(long id);
    QList<QObject*> enquireJob();
    long enquireHighestJob();

    // Log
    void newLog(int event, int node = 0);
    QString eventToString(int event, int node = 0);
    QVector<QVector<QString>> enquireLog();

    // Reset
    void reset();

signals:
    void updateLogTable(QString id, QString date, QString time, QString event);
};

#endif // SQL_H
