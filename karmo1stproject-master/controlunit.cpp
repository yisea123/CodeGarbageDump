/*  Control Unit
 *  Programmed 6 January 2020
 *  Last updated : 30 January 2020
 *  Author: Max
 *  Version: 1.0
 */

#include "controlunit.h"



ControlUnit::ControlUnit(QObject *parent) : QObject(parent)
{   
    sql = Sql::getInstance();

    QVector<QVector<QString>> temp = sql->SyncOrderList();

    //Retrieve ordersheet from SQL
    for(int i = 0; i < temp.size(); i++){
        int priority = temp.at(i).at(1).toInt();
        QString qrcode = temp.at(i).at(0);
        btnInsert(priority, qrcode);
    }



    //========================================================================

    //Connect TCP
    tcpSocket.doConnect();

    //Connect Database(QString hostName, int port, QString user, QString password, QString driverVersion)
    connectDatabase();

    //========================================================================


}

//Button Implementation===========================================================
bool ControlUnit::btnInsert(int priority, QString QRcode){

    if(QRcode == ""){
        qDebug() << "You haven't enter QRcode!";
        return false;
    }

    if(!QRcode.contains("&")){
        qDebug() << "The QRCode is Invalid!";
        return false;
    }

    OrderItem *odItem = new OrderItem(QRcode);

    int itemOrder = 1; //Default Order is 1 (insert to first if It is highest priority)

    odItem->setPriority(priority);
    for( int i=0; i<orderList.count(); ++i )
    {
        // Are there Items that have higher priority than the one we just added?
        if(orderList.at(i)->getPriority() >= odItem->getPriority()) //First come First serve if same priority
            itemOrder++;
    }
    orderList.insert(itemOrder-1,odItem); //List starts from 0

    //Throw Data into
    sql->fillRow(odItem);

    return true;
}



OrderItem ControlUnit::orderGenerate(QString QRcode){

    OrderItem *order = new OrderItem(QRcode);
    return *order;
}

void ControlUnit::insertOrder(int priority, OrderItem &odItem){ //Workable

    int itemOrder = 1; //Default Order is 1 (insert to first if It is highest priority)

    odItem.setPriority(priority);
    for( int i=0; i<orderList.count(); ++i )
    {
        // Are there Items that have higher priority than the one we just added?
        if(orderList.at(i)->getPriority() >= odItem.getPriority()) //First come First serve if same priority
            itemOrder++;
    }
    orderList.insert(itemOrder-1,&odItem); //List starts from 0

    //Throw Data into
    sql->fillRow(&odItem);
}

void ControlUnit::finishOrder(OrderItem &odItem){
    odItem.setFinish(true);

    removeFinishedOrder();

    sql->removeRow(&odItem);
}


//Debug=========================================================================================

bool ControlUnit::debugAddDummies(){

    //==============================================================================
    //Testing Dummy
    //Format: MO号&MO制造数量&开始生产日期&机台编号&刀具编号1&刀具QTY1&刀具编号2&刀具QTY2....
    //==============================================================================

    OrderItem *order1 = new OrderItem("MOCNA9A121111&33PCS&2019/10/31&CPD5&K-RE9.6*1.4-17&1&K-RE9.6*1.4-2&1&K-RE6.6*1.4-0.15-1&1&K-RE9.6*1.4-0.15-1&1&K-OB9.5*4.34-1K-OB9.5*4.34-2&1&K-OB9.5*4.3-0.15-1&1&K-OB9.5*4.3-0.15-2&1");
    OrderItem *order2 = new OrderItem("MOCNA9A122222&34PCS&2019/10/30&CPD5&K-CR6.5*R0.75-1&1&K-CR5*R1-0.25-2&1&K-CR7*R1-0.25-1&1&K-CR7*R1-1&1");
    OrderItem *order3 = new OrderItem("MOCNA9A123333&43PCS&2019/10/29&CPD4&K-CR6*R1.5-0.25-1&2&K-CR6*R1.5-0.25-2&1&K-CR6*R1.5-0.25-3&1&K-CR8*R1.5-0.25-1&1&K-CR8*R1.5-1&1");
    OrderItem *order4 = new OrderItem("MOCNA9A124444&23PCS&2019/10/28&CPD3&K-CR7*R2-0.08-1&1&K-RE14*12.5-0.15-1&1&K-CR9*R2-0.15-1&1&K-CR2.54*R2.54-1&2&K-CR8*R2.5-1&1");
    OrderItem *order5 = new OrderItem("MOCNA9A125555&41PCS&2019/10/27&CPD2&K-CS7.0*3.2-3&1&K-CS7.4*3.2-1&2&K-CS7.8*3.7-1&1&K-CS8.2*4.5-1&1&K-CS9.1*5.3-1&1&K-DD6.65*5.25-0.3-1&1");

    insertOrder(2,*order1);
    insertOrder(3,*order2);
    insertOrder(2,*order3);
    insertOrder(4,*order4);
    insertOrder(5,*order5);

    qDebug() << "5 Dummies Generated.";

    finishOrder(*order1);
    finishOrder(*order3);

    qDebug() << "2 Finished.";

    return true;
}

bool ControlUnit::debugAddExtraDummies(){

    OrderItem *order6 = new OrderItem("MOCNA9A126666&33PCS&2019/10/31&CPD5&K-RE9.6*1.4-17&1&K-RE9.6*1.4-2&1&K-RE6.6*1.4-0.15-1&1&K-RE9.6*1.4-0.15-1&1&K-OB9.5*4.34-1K-OB9.5*4.34-2&1&K-OB9.5*4.3-0.15-1&1&K-OB9.5*4.3-0.15-2&1");
    OrderItem *order7 = new OrderItem("MOCNA9A127777&34PCS&2019/10/30&CPD5&K-CR6.5*R0.75-1&1&K-CR5*R1-0.25-2&1&K-CR7*R1-0.25-1&1&K-CR7*R1-1&1");
    OrderItem *order8 = new OrderItem("MOCNA9A128888&43PCS&2019/10/29&CPD4&K-CR6*R1.5-0.25-1&2&K-CR6*R1.5-0.25-2&1&K-CR6*R1.5-0.25-3&1&K-CR8*R1.5-0.25-1&1&K-CR8*R1.5-1&1");
    OrderItem *order9 = new OrderItem("MOCNA9A129999&23PCS&2019/10/28&CPD3&K-CR7*R2-0.08-1&1&K-RE14*12.5-0.15-1&1&K-CR9*R2-0.15-1&1&K-CR2.54*R2.54-1&2&K-CR8*R2.5-1&1");
    OrderItem *order10 = new OrderItem("MOCNA9A12AAAA&41PCS&2019/10/27&CPD2&K-CS7.0*3.2-3&1&K-CS7.4*3.2-1&2&K-CS7.8*3.7-1&1&K-CS8.2*4.5-1&1&K-CS9.1*5.3-1&1&K-DD6.65*5.25-0.3-1&1");

    insertOrder(2,*order6);
    insertOrder(3,*order7);
    insertOrder(2,*order8);
    insertOrder(4,*order9);
    insertOrder(5,*order10);

    qDebug() << "5 Dummies Generated.";

    finishOrder(*order7);
    finishOrder(*order9);

    qDebug() << "2 Finished.";

    return true;
}


bool ControlUnit::debugShowOrderList(){

    if(orderList.size() == 0)
    {
        qDebug() << "Nothing inside the order list yet!";
        return false;
    }

    for( int i = 0; i < orderList.size(); i++)
    {
        qDebug() << "Priority: " << orderList.at(i)->getPriority() << "   ID: " << orderList.at(i)->getOrderID() << "   QTY: " << orderList.at(i)->getOrderQTY() << "   Availability: " << orderList.at(i)->getAvailable() << "   Finished: " << orderList.at(i)->getFinish();
    }

    return true;
}


bool ControlUnit::debugFinishAll(){

    if(orderList.size() > 0){
        for( int i = 0; i < orderList.size(); i++)
        {
            finishOrder(*orderList.at(i));
        }
        qDebug() << "All items finished.";
        return true;
    }
    qDebug() << "No Item in the list yet!!";
    return false;
}


bool ControlUnit::connectDatabase(QString hostName, int port, QString user, QString password, QString driverVersion){
    //if(sql->connectDatabase("karmo.synology.me", 3307, "root", "EIENoMoney$4", "3.1"))
    if(sql->connectDatabase(hostName, port, user, password, driverVersion))
        return true;
    else
        return false;
}

bool ControlUnit::sendTCPsignal(){

    QList<QString> fakeOrderList;
    fakeOrderList.append("K-OB10*2-0.15");
    fakeOrderList.append("K-OB10*4-0.15");
    fakeOrderList.append("K-OB10*6.4-0.25");
    fakeOrderList.append("K-OB10*6.5-0.1");
    fakeOrderList.append("K-OB10*6.5");
    fakeOrderList.append("K-OB10*7.05");
    fakeOrderList.append("K-OB10*7.5-0.15");
    fakeOrderList.append("K-OB10*8-0.2");
    fakeOrderList.append("K-OB10*8-0.25");
    fakeOrderList.append("K-OB10.35*6.85-0.15");
    fakeOrderList.append("K-OB10.4*7.1");
    fakeOrderList.append("K-OB10.5*1.5");
    fakeOrderList.append("K-OB10.5*7.3");
    fakeOrderList.append("K-OB10.9*7.1-0.35");
    fakeOrderList.append("K-OB16*12");
    fakeOrderList.append("K-OB17*7.2");
    fakeOrderList.append("K-OB20*2-0.15");
    fakeOrderList.append("K-OB20*5-0.1");
    fakeOrderList.append("K-OB24.8*5.75-0.25");
    fakeOrderList.append("K-OB25.4*6.35-0.15");
    fakeOrderList.append("K-OB25.4*6.35");
    fakeOrderList.append("K-OB4.8*3.2-0.25");
    fakeOrderList.append("K-OB6*4.03-0.15");
    fakeOrderList.append("K-OB6.8*4.3-0.25");
    fakeOrderList.append("K-OB7.6*5.2-0.15");
    fakeOrderList.append("K-RE13*7.5-0.15");
    fakeOrderList.append("K-RE13*8-0.3");
    fakeOrderList.append("K-RE13.2*1.2-0.25");
    fakeOrderList.append("K-RE13.2*1.2");
    fakeOrderList.append("K-RE13.2*8.9-0.35");

    // Divide into 2 lists
    // 0: small, 1: big
    QList<int> endMillListA[2];
    QList<int> endMillListB[2];
    for(int i = 0; i < fakeOrderList.size(); i++)
    {
        QList<QVariant> loc = sql->enquireOrderStart(fakeOrderList.at(i));
        qDebug() << "End mill: " << fakeOrderList.at(i) << " in shelf " << loc.at(1).toString() << " id: " << loc.at(0).toInt();
        if(loc.at(1).toString().contains("A"))
        {
            // Arm A
            if(loc.at(2).toInt() == 0 || loc.at(2).toInt() == 1)
                // Small
                endMillListA[0].append(loc.at(0).toInt());
            else
                // Big
                endMillListA[1].append(loc.at(0).toInt());
        }
        else
        {
            // Arm B
            if(loc.at(2).toInt() == 0 || loc.at(2).toInt() == 1)
                // Small
                endMillListB[0].append(loc.at(0).toInt());
            else
                // Big
                endMillListB[1].append(loc.at(0).toInt());
        }
    }
    QString armAStr, armBStr;
    // Convert to str
    // End mill list
    for(int i = 0; i < 2; i++)
    {
        // Arm A
        if(endMillListA[i].size() > 0)
        {
            for(int j = 0; j < endMillListA[i].size(); j++)
            {
                if(j > 0)
                    armAStr += ",";
                armAStr += QString::number(endMillListA[i].at(j));
            }
        }
        else
            armAStr += "NO";
        armAStr += ";";

        // Arm b
        if(endMillListB[i].size() > 0)
        {
            for(int j = 0; j < endMillListB[i].size(); j++)
            {
                if(j > 0)
                    armBStr += ",";
                armBStr += QString::number(endMillListB[i].at(j));
            }
        }
        else
            armBStr += "NO";
        armBStr += ";";
    }
    // Exchange end mill list
    armAStr += "NO;NO;NO;NO";
    armBStr += "NO;NO;NO;NO";

    qDebug() << armAStr << endl << armBStr;

    //========================================================================

    //Send TCP msg
    tcpSocket.writeMsg(armAStr);
    tcpSocket.writeMsg(armBStr);

    //========================================================================

    return true;
}

//Not So Important==============================================================================

void ControlUnit::sortPriority(){ //In case you enter multiple items at once

    //bubble sorting (descending order)
    int i, j=0;
    for(i = 0; i<orderList.count(); i++) {
       for(j = i+1; j<orderList.count(); j++)
       {
          if(orderList.at(j)->getPriority() > orderList.at(i)->getPriority()) {
              orderList.swapItemsAt(i,j);
          }
       }
    }
    qDebug() <<"Sorted Element List ...\n";
    for(i = 0; i<orderList.count(); i++) {
       qDebug() <<orderList.at(i)->getPriority()<< orderList.at(i)->getOrderID();
    }
}


void ControlUnit::moveOrder(int fromPos, int toPos){ //User start counting from 1

    int modFromPos = fromPos-1;
    int modToPos = toPos-1;

    //If Both modFromPos and toPos are inside the List (>= 0 and < size of list)
    if((modFromPos >= 0 && modFromPos < orderList.size())&&(modToPos >= 0 && modToPos < orderList.size())){
        orderList.move(modFromPos, modToPos);

        // Simulated Situation
        // When user call this function
        //moveOrder(2, 5);
        // What it actually does :
        //orderList.move(1, 4);
        //  Example :
        //  Before Moving : list: ["A", "B", "C", "D", "E", "F"]
        //  After Moving : list: ["A", "C", "D", "E", "B", "F"]
    }
    else
    {
        //Return Error message
        qDebug() << "[moveOrder]Wrong Position Input Parameter, Action abort";
    }
}

void ControlUnit::removeFinishedOrder(){

    //Loop through the list, throw it away if the bill is finished
    for( int i=0; i<orderList.count(); ++i)
    {
        if(orderList.at(i)->getFinish() == true)
        {
            orderList.removeAt(i);
        }
    }
}


//WIP===========================================================================================

void ControlUnit::executeOrder(OrderItem &odItem){

    qDebug() << "Order ID" << odItem.getOrderID() + "is being executed.";


    /*
     *
     *
     *
     */


    qDebug() << "Order ID" << odItem.getOrderID() + "done.";

    finishOrder(odItem);
}
