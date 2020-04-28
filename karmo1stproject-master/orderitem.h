#ifndef ORDERITEM_H
#define ORDERITEM_H

#include <QObject>
#include <QDateTime>

class OrderItem
{
    Q_GADGET

public:
    explicit OrderItem(QString str);

    QString getOrderID() const {return orderID;}
    QString getOrderQTY() const {return orderQTY;}
    QDate getProdDate() const {return prodDate;}
    QString getCNCID() const {return CNCID;}

    void setAvailable(bool t){available = t;}
    bool getAvailable() const {return available;}

    void setFinish(bool t) {finish = t;}
    bool getFinish() const {return finish;}

    const QList<QString>&getEndMillSpec() const {return endMillSpec;}
    const QList<int>&getEndMillQTY() const {return endMillQTY;}

    void setPriority(int p){priority = p;}
    int getPriority() const {return priority;}

    QString getEndMillSQL() const {return endMillSQL;}

private:
    QString orderID;
    QString orderQTY;
    QDate prodDate;
    QString CNCID;
    QList<QString> endMillSpec;
    QList<int> endMillQTY;

    bool finish = false;
    bool available = true;

    int priority = 0;

    //Pass it to Database
    QString endMillSQL;

};

#endif // ORDERITEM_H
