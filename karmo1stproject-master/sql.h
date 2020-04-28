#ifndef SQL_H
#define SQL_H

#include <QObject>
#include <QtSql>
#include <QList>
#include <QVariant>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDateTime>
#include "orderitem.h"

class Sql : public QObject
{
    Q_OBJECT

private:
    // Prevent LNK2001
    static Sql *instance;


    explicit Sql(QObject *parent = nullptr);

    //Connect to SQLite and Initializing Table
    QSqlDatabase database;




public:
    static Sql *getInstance();

    //Filling 1 row
    void fillRow(OrderItem *od);
    //Remove Finished Job
    void removeRow(OrderItem *od);

    // Record Log for finished job
    void fillRecordLog(OrderItem *od);

    //Filling all rows
    //void insertData(ControlUnit *cu);

    //Passing Data to Model
    QVector<QVector<QString>> enquireLog();

    void newLog();

    QVector<QVector<QString>> enquireOrder();

    void newOrder();

    //Connect Database
    bool connectDatabase(QString hostName, int port, QString user, QString password, QString driverVersion);

    // Warehouse operation
    bool addStorage(QString shelf, int row, int column, long xLocation, long yLocation, int boxType);

    // Enquire
    QList<QVariant> enquireOrderStart(QString endMillID);
    //int locateEndMillArm(QString endMillID);

    // Other
    void sendEndMillList(int arm, QString endMillList);


    //synchronize List in ControlUnit.cpp
    QVector<QVector<QString>> SyncOrderList();


signals:
    //void updateLogTable(QString id, QString date, QString time, QString event);
    void updateLogTable(QString id, QString qty, QString cncid, QString productiondate, QString endMill);

    void updateOrderTable(QString id, QString qty, QString cncid, QString productiondate, QString endMill, QString priority);

};

#endif // SQL_H
