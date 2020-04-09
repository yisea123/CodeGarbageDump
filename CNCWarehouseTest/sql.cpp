#include "sql.h"

// Prevent LNK2001
Sql *Sql::instance;

Sql::Sql(QObject *parent) : QObject(parent)
{

}

Sql *Sql::getInstance()
{
    if(instance == nullptr)
    {
        instance = new Sql();
    }
    return instance;
}

bool Sql::connectDatabase(QString hostName, int port, QString user, QString password, QString driverVersion)
{
    database = QSqlDatabase::addDatabase("QMYSQL", "EndMillWarehouse");
    //QString connectString = "Driver={MariaDB ODBC " + driverVersion + " Driver};";
    //connectString += "TCPIP=1;";
    //connectString += "SERVER=" + hostName + ";";
    //connectString += "UID=" + user + ";";
   // connectString += "PWD=" + password + ";";
    //connectString += "PORT=" + QString::number(port);
    //database.setDatabaseName(connectString);
    database.setHostName(hostName);
    database.setPort(port);
    database.setUserName(user);
    database.setPassword(password);
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
