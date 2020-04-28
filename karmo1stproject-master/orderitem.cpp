/*  OrderItem
 *  Programmed 6 January 2020
 *  Last updated : 23 January 2020
 *  Author: Max
 *  Version: 1.0
 */

#include "orderitem.h"

OrderItem::OrderItem(QString QRCode)
{
    //Format of str: MO号&MO制造数量&开始生产日期&机台编号&刀具编号1&刀具QTY1&刀具编号2&刀具QTY2....
    //Decode the QR code msg
    QStringList list;
    list = QRCode.split('&');

    //==============================================================================
    //Storing the Decoded Objects...
    //Storing First 4 elements
    orderID = list.at(0);
    orderQTY = list.at(1);
    prodDate = QDate::fromString(list.at(2), "yyyy/MM/dd");
    CNCID = list.at(3);

    //Looping the List
    for (int i = 4; i < list.size(); ++i) { //Skipping First 4 elements of List
        if(i%2 == 1)
        endMillSpec.append(list.at(i)); //Writing endMill Spec
        else
        endMillQTY.append(list.at(i).toInt()); //Writing endMill Spec QTY
    }


    //Looping the List
    for (int i = 4; i < list.size(); ++i) { //Skipping First 4 elements of List
        if(i != 4) //No need to add Separator before first item
            endMillSQL += "&"; //Separator
        endMillSQL += list.at(i);
    }


    //==============================================================================
}
