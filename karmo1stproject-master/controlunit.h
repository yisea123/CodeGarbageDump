#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include <QObject>
#include <QList>
#include <QDebug>
#include "orderitem.h"
#include "sql.h"
#include "mytcpsocket.h"

class ControlUnit : public QObject
{
    Q_OBJECT

    //Q_PROPERTY(type name READ name WRITE setname NOTIFY nameChanged)


public slots:

    //Combination of orderGenerate() and insertOrder()
    bool btnInsert(int priority,QString QRcode);

    //Debug
    bool debugAddDummies();
    bool debugShowOrderList();
    bool debugAddExtraDummies();

    bool connectDatabase(QString hostName = "karmo.synology.me", int port = 3307, QString user = "root", QString password = "EIENoMoney$4", QString driverVersion = "3.1");
    bool sendTCPsignal();

    //WIP
    bool debugFinishAll();

public:
    ControlUnit(QObject *parent = nullptr);

    OrderItem orderGenerate(QString QRcode);
    void insertOrder(int priority, OrderItem &odItem);

    const QList<OrderItem*>&getOrderList() const {return orderList;}

    //Function Implemented
    void finishOrder(OrderItem &odItem);


    //Not so-related (Support Function)
    void sortPriority();
    void moveOrder(int fromPos, int toPos);
    void removeFinishedOrder();

    //WIP
    void executeOrder(OrderItem &odItem);
    void receiveAGVCall(); //收到PLC叫車通知
    void AGVtoCNC(); //叫AGC到CNC機
    bool isMillInWarehouse();//知道有刀具入倉
    bool checkSequence(); //檢查入倉順序
    void RobotArmToWarehouse(); //通知機械手入倉




private:
    QList<OrderItem*> orderList; //List of Orders
    Sql *sql;
    MyTcpSocket tcpSocket;

    int selectedPriority;

};


#endif // CONTROLUNIT_H
