#ifndef SQL_H
#define SQL_H

#include <QObject>
#include <QtSql>
#include <QList>
#include <QVariant>
#include <QUdpSocket>
#include <QHostAddress>

class Sql : public QObject
{
    Q_OBJECT

private:
    static Sql *instance;
    explicit Sql(QObject *parent = nullptr);
    QSqlDatabase database;

public:
    static Sql *getInstance();
    bool connectDatabase(QString hostName, int port, QString user, QString password, QString driverVersion);

    // Warehouse operation
    bool addStorage(QString shelf, int row, int column, long xLocation, long yLocation, int boxType);

    // Enquire
    QList<QVariant> enquireOrderStart(QString endMillID);
    //int locateEndMillArm(QString endMillID);

    // Other
    void sendEndMillList(int arm, QString endMillList);
};

#endif // SQL_H
