#include "sql.h"

// Prevent LNK2001
Sql *Sql::instance;

Sql *Sql::getInstance()
{
    if(instance == nullptr)
    {
        instance = new Sql();
    }
    return instance;
}

Sql::Sql(QObject *parent) : QObject(parent)
{
    //Connect to SQLite
    database = QSqlDatabase::addDatabase("QSQLITE");
    //database.setDatabaseName("/Users/max/karmo1stproject/data.sqlite"); //the location of your database and the file name
    database.setDatabaseName("./data.sqlite");
    if (!database.open()) //Open database
    {
        qDebug() << database.lastError().text();
    }

    //String for initializing table orderitem
    QString createTable = "create table orderitem (id int primary key, "
                          "qty text, productiondate text, cncid text, endmill text, available text, priority int)";

    //String for initializing log sheet
    QString createLog = "create table record (id int primary key, "
                          "qty text, productiondate text, cncid text, endmill text, available text, priority int)";


    //SQL executor (need to declare after `database.open();` is called)
    QSqlQuery query;

    if(query.exec("SELECT * FROM `orderitem`"))
    {
        qDebug() << "SQL: Database is found.";
    }
    else
    {
        qDebug() << "SQL: Database is not found or corrupted, a new database will be created.";
        query.exec(createTable); //Creating Table...
        query.exec(createLog); //Creating Logsheet...
    }

}

void Sql::fillRow(OrderItem *od)
{
    QString OrderID = od->getOrderID();
    QString OrderQTY = od->getOrderQTY();
    QDate ProdDate = od->getProdDate();
    QString CNCID = od->getCNCID();
    bool available = od->getAvailable();
    int priority = od->getPriority();
    QString endMill = od->getEndMillSQL();

    //SQL executor (need to declare after `database.open();` is called)
    QSqlQuery query;

    query.prepare("insert into orderitem (id, qty, productiondate, cncid, endmill, available, priority) "
                      "values (?,?,?,?,?,?,?)");
    query.addBindValue(OrderID);
    query.addBindValue(OrderQTY);
    query.addBindValue(ProdDate.toString());
    query.addBindValue(CNCID);
    query.addBindValue(endMill);
    query.addBindValue(QString::number(available));
    query.addBindValue(priority);
    query.exec();
}

void Sql::removeRow(OrderItem *od)
{

    //SQL executor (need to declare after `database.open();` is called)
    QSqlQuery query;

    //Add to Record Log before removing...
    fillRecordLog(od);

    //Remove Row from table "orderitem"
    QString id = od->getOrderID();
    query.prepare("DELETE FROM orderitem WHERE id = ?");
    query.addBindValue(id);
    query.exec();
}

void Sql::fillRecordLog(OrderItem *od)
{
    QString OrderID = od->getOrderID();
    QString OrderQTY = od->getOrderQTY();
    QDate ProdDate = od->getProdDate();
    QString CNCID = od->getCNCID();
    bool available = od->getAvailable();
    int priority = od->getPriority();
    QString endMill = od->getEndMillSQL();

    //SQL executor (need to declare after `database.open();` is called)
    QSqlQuery query;

    query.prepare("insert into record (id, qty, productiondate, cncid, endmill, available, priority) "
                      "values (?,?,?,?,?,?,?)");
    query.addBindValue(OrderID);
    query.addBindValue(OrderQTY);
    query.addBindValue(ProdDate.toString());
    query.addBindValue(CNCID);
    query.addBindValue(endMill);
    query.addBindValue(QString::number(available));
    query.addBindValue(priority);
    query.exec();
}

/*
void Sql::insertData(ControlUnit *cu)
{
    //Filling all rows
    for(int i = 0; i < cu->getOrderList().size(); i++)
    {
        fillRow(cu->getOrderList().at(i));
    }
}
*/

//Passing Data to Model
QVector<QVector<QString>> Sql::enquireLog()
{
    QSqlQuery query;
    query.exec("SELECT * FROM `record` ORDER BY productiondate DESC");
    QVector<QVector<QString>> table;
    while (query.next())
    {
        QVector<QString> row;
        row.append(query.value(0).toString()); //id
        row.append(query.value(1).toString()); //qty
        row.append(query.value(3).toString()); //cncid
        row.append(query.value(2).toString()); //productiondate
        row.append(query.value(4).toString()); //endmill Set

        table.append(row);
    }
    return table;
}

void Sql::newLog()
{
    QSqlQuery query;

    query.exec("SELECT * FROM `sqlite_sequence` WHERE name = \"record\"");
    QString id, qty, cncid, productiondate, endMill;

    while (query.next())
    {
        id = query.value(0).toString();
        qty = query.value(1).toString();
        cncid = query.value(2).toString();
        productiondate = query.value(3).toString();
        endMill = query.value(4).toString();
    }

    emit updateLogTable(id,qty,cncid,productiondate,endMill);
}

QVector<QVector<QString>> Sql::enquireOrder()
{
    QSqlQuery query;
    query.exec("SELECT * FROM `orderitem` ORDER BY priority DESC");
    QVector<QVector<QString>> table;
    while (query.next())
    {
        QVector<QString> row;
        row.append(query.value(0).toString()); //id
        row.append(query.value(6).toString()); //priority
        row.append(query.value(1).toString()); //qty
        row.append(query.value(3).toString()); //cncid
        row.append(query.value(2).toString()); //productiondate
        row.append(query.value(4).toString()); //endmill Set


        table.append(row);
    }
    return table;
}

void Sql::newOrder()
{
    QSqlQuery query;

    query.exec("SELECT * FROM `sqlite_sequence` WHERE name = \"orderitem\"");
    QString id, qty, cncid, productiondate, endMill, priority;

    while (query.next())
    {
        id = query.value(0).toString();
        priority = query.value(6).toString();
        qty = query.value(1).toString();
        cncid = query.value(2).toString();
        productiondate = query.value(3).toString();
        endMill = query.value(4).toString();

    }

    emit updateOrderTable(id,priority,qty,cncid,productiondate,endMill);
}


//=============================================================================

bool Sql::connectDatabase(QString hostName, int port, QString user, QString password, QString driverVersion)
{
    database = QSqlDatabase::addDatabase("QODBC3", "EndMillWarehouse");
    QString connectString = "Driver={MariaDB ODBC " + driverVersion + " Driver};";
    connectString += "TCPIP=1;";
    connectString += "SERVER=" + hostName + ";";
    connectString += "UID=" + user + ";";
    connectString += "PWD=" + password + ";";
    connectString += "PORT=" + QString::number(port);
    database.setDatabaseName(connectString);
    if(!database.open())
    {
        // To show last error
        qDebug() << database.lastError();
        QCoreApplication::quit();
        return false;
    }

    QSqlQuery query(database);
    if(query.exec("USE Warehouse"))
    {
        qDebug() << "SQL: Database connection finished.";

    }
    else
    {
        qDebug() << "SQL: Database is not found or corrupted, a new database will be created.";

        // Create database
        query.exec("CREATE DATABASE `Warehouse`;");
        query.exec("USE `Warehouse`;");

        // Create table for End Mill
        query.exec("CREATE TABLE `EndMill` (`EndMillID` CHAR(50) NOT NULL UNIQUE, `EndMillBarcode` CHAR(50) NOT NULL UNIQUE, `Category` INT, `Shape` CHAR(20), `Xlength` DOUBLE, `Ylength` DOUBLE, `Rlength` DOUBLE, `Angle` DOUBLE, `MoldSide` BOOLEAN, `Clearance` DOUBLE, `Status` INT, `Outbound` INT, `Critical` BOOLEAN, PRIMARY KEY (`EndMillID`));");

        // Create table for jig box
        query.exec("CREATE TABLE `JigBox` (`JigBoxID` CHAR(50) NOT NULL UNIQUE, `fkEndMillID` CHAR(50) UNIQUE, `BoxType` INT, PRIMARY KEY (`JigBoxID`), FOREIGN KEY (`fkEndMillID`) REFERENCES `EndMill`(`EndMillID`));");

        // Create table for warehouse storage
        query.exec("CREATE TABLE `Storage` (`StorageID` INT NOT NULL AUTO_INCREMENT, `fkEndMillID` CHAR(50) UNIQUE, `Shelf` TEXT, `Row` INT, `Column` INT, `XLocation` INT, `YLocation` INT, `AcceptBoxType` INT, PRIMARY KEY (`StorageID`), FOREIGN KEY (`fkEndMillID`) REFERENCES `EndMill`(`EndMillID`));");

        // Create table for jig box
        query.exec("CREATE TABLE `Archive` (`archiveID` INT NOT NULL AUTO_INCREMENT, `fkStorageID` INT, `fkEndMillID` CHAR(50), PRIMARY KEY (`archiveID`), FOREIGN KEY (`fkStorageID`) REFERENCES `Storage`(`StorageID`), FOREIGN KEY (`fkEndMillID`) REFERENCES `EndMill`(`EndMillID`));");

        // Create storage for A1
        for(int i = 1; i < 17; i++)
        {
            // Small upper
            for(int j = 1; j < 37; j++)
            {
                addStorage("A1", i, j, 0, 0, 0);
            }
        }
        for(int i = 1; i < 17; i++)
        {
            // Small lower
            for(int j = 1; j < 37; j++)
            {
                addStorage("A1x", i, j, 0, 0, 1);
            }
        }

        // A2
        for(int i = 1; i < 6; i++)
        {
            // big upper
            for(int j = 1; j < 15; j++)
            {
                addStorage("A2", i, j, 0, 0, 2);
            }
        }
        for(int i = 5; i < 10; i++)
        {
            // big lower
            for(int j = 1; j < 22; j++)
            {
                addStorage("A2", i, j, 0, 0, 3);
            }
        }
        for(int i = 11; i < 16; i++)
        {
            // small upper
            for(int j = 1; j < 36; j++)
            {
                addStorage("A2", i, j, 0, 0, 0);
            }
        }
        for(int i = 6; i < 16; i++)
        {
            // small lower
            for(int j = 1; j < 27; j++)
            {
                addStorage("A2x", i, j, 0, 0, 1);
            }
        }

        // B1
        for(int i = 1; i < 20; i++)
        {
            // Small upper
            for(int j = 1; j < 43; j++)
            {
                addStorage("B1", i, j, 0, 0, 0);
            }
        }
        for(int i = 1; i < 20; i++)
        {
            // Small lower
            for(int j = 1; j < 43; j++)
            {
                addStorage("B1x", i, j, 0, 0, 1);
            }
        }

        // B2
        for(int i = 1; i < 6; i++)
        {
            // big upper
            for(int j = 4; j < 20; j++)
            {
                addStorage("B2", i, j, 0, 0, 2);
            }
        }
        for(int i = 1; i < 4; i++)
        {
            // big upper, extra 3
            addStorage("B2", 5, i, 0, 0, 2);
        }
        for(int i = 5; i < 10; i++)
        {
            // big lower
            for(int j = 1; j < 24; j++)
            {
                addStorage("B2", i, j, 0, 0, 3);
            }
        }
        for(int i = 11; i < 16; i++)
        {
            // small upper
            for(int j = 1; j < 41; j++)
            {
                addStorage("B2", i, j, 0, 0, 0);
            }
        }
        for(int i = 6; i < 16; i++)
        {
            // small lower
            for(int j = 1; j < 31; j++)
            {
                addStorage("B2x", i, j, 0, 0, 1);
            }
        }
    }

    return true;
}

bool Sql::addStorage(QString shelf, int row, int column, long xLocation, long yLocation, int boxType)
{
    QSqlQuery query(database);
    if(query.exec("INSERT INTO `Storage` (`Shelf`, `Row`, `Column`, `XLocation`, `YLocation`, `AcceptBoxType`) VALUES (\'" + shelf + "\', " + QString::number(row) + ", " + QString::number(column) + ", " + QString::number(xLocation) + ", " + QString::number(yLocation) + ", " + QString::number(boxType) + ");"))
        return true;
    else
        return false;
}

QList<QVariant> Sql::enquireOrderStart(QString endMillID)
{
    // Return storage ID(int), shelf(QString), accept box(int)
    // Return lowest outbound if more than one endmill
    QList<QVariant> result;
    QSqlQuery query(database);
    query.exec("SELECT `Storage`.`StorageID`, `Shelf`, `AcceptBoxType` FROM `Storage` INNER JOIN `EndMill` ON `Storage`.`fkEndMillID` = `EndMill`.`EndMillID`  WHERE `Storage`.`fkEndMillID` LIKE \'" + endMillID + "%\' ORDER BY `EndMill`.`Outbound` ASC LIMIT 1;");
    while (query.next())
    {
        result.append(query.value(0).toInt());
        result.append(query.value(1).toString());
        result.append(query.value(2).toInt());
    }
    if(result.size() == 0)
        qDebug() << "Error: Query no result.";
    return result;
}

void Sql::sendEndMillList(int arm, QString endMillList)
{
    // arm: 0 = ST1, 1 = ST2
    QUdpSocket armComputer;
    if(arm == 0)
        armComputer.connectToHost(QHostAddress("172.0.0.1"), 8964);
    else
        armComputer.connectToHost(QHostAddress("172.0.0.1"), 7689);
    armComputer.write(endMillList.toLocal8Bit());
    armComputer.close();
}

QVector<QVector<QString>> Sql::SyncOrderList(){
    // list store QRcode String
    QVector<QVector<QString>> table;
    QSqlQuery query(database);

    query.exec("SELECT * FROM `orderitem` ORDER BY priority DESC");
    while(query.next()){
        QVector<QString> row;
        //Format of str: MO号&MO制造数量&开始生产日期&机台编号&刀具编号1&刀具QTY1&刀具编号2&刀具QTY2....
        QString qrcode = "";
        QString priority = "";
        QString available = "";

        qrcode += query.value(0).toString() + "&" + query.value(1).toString() + "&" + query.value(2).toString() + "&";
        qrcode += query.value(3).toString() + "&" + query.value(4).toString();
        priority = query.value(6).toString();
        available = query.value(5).toString();

        row.append(qrcode);
        row.append(priority);
        row.append(available);

        table.append(row);
    }
    return table;
}
