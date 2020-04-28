#include "mytcpsocket.h"

MyTcpSocket::MyTcpSocket(QObject *parent) :
    QObject(parent)
{
}

void MyTcpSocket::doConnect()
{
    socket = new QTcpSocket(this);

    socket->connectToHost("google.com", 80); //tbh

    if(socket->waitForConnected(5000))
    {
        qDebug() << "Connected!";

        /*
        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(3000);

        qDebug() << "Reading: " << socket->bytesAvailable();

        // get the data
        qDebug() << socket->readAll();
        */

    }
    else
    {
        //qDebug() << "Not connected!";
        qDebug() << "Error: " << socket->errorString();
    }
}

void MyTcpSocket::doDisconnect(){
    // close the connection
    socket->close();
}

void MyTcpSocket::writeMsg(QString msg){

    QByteArray br = msg.toUtf8();
    socket->write(br);
}
