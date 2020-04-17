#include "Sql.h"

// Prevent LNK2001
Sql *Sql::instance;


Sql::Sql(QObject *parent) : QObject(parent)
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("data.sqlite");
    database.open();

    QSqlQuery query;
    if(query.exec("SELECT * FROM `System`"))
    {
        qDebug() << "SQL: Database is found.";
    }
    else
    {
        qDebug() << "SQL: Database is not found or corrupted, a new database will be created.";
        query.exec("CREATE TABLE \"JobList\" (\"ID\" INTEGER PRIMARY KEY AUTOINCREMENT, \"File\" TEXT, \"Procedure\" INTEGER, \"ArmCoordinate\" TEXT, \"BoxArrayList\" TEXT, \"PanelHeight\" REAL, \"BoxType\" TEXT, \"CreateTime\" INTEGER)");
        query.exec("CREATE TABLE \"Log\" (\"ID\" INTEGER PRIMARY KEY AUTOINCREMENT, \"DateTime\" INTEGER, \"Event\" INTEGER)");
    }
}

Sql *Sql::getInstance()
{
    if(instance == nullptr)
    {
        instance = new Sql();
    }
    return instance;
}

void Sql::newJob(JobItem *job)
{
    QString file = job->file();
    int procedure = job->step();
    double panelHeight = job->panelHeight();
    QString boxType = job->boxType();
    QString armCoordinate;
    for(int i = 0; i < procedure; i++)
    {
        armCoordinate += QString::number(job->armCoordinate().at(i).x()) + "," + QString::number(job->armCoordinate().at(i).y());
        if(i != (procedure-1))
            armCoordinate += ";";
    }
    QString boxArrayList;
    for(int i = 0; i < procedure; i++)
    {
        for(int j = 0; j < 15; j++)
        {
            if(job->boxArrayList().at(i).at(j))
                boxArrayList += "1";
            else
                boxArrayList += "0";
            if(j != 14)
                boxArrayList += ",";
        }
        if(i != (procedure-1))
            boxArrayList += ";";
    }
    QSqlQuery query;
    query.exec("INSERT INTO `JobList` (File, Procedure, ArmCoordinate, BoxArrayList, PanelHeight, BoxType, CreateTime) VALUES (\"" + file + "\", \"" + QString::number(procedure) + "\" ,\"" + armCoordinate + "\" , \"" + boxArrayList + "\", " + QString::number(panelHeight) + ", \"" + boxType + "\", " + QString::number(job->createTime().toSecsSinceEpoch()) + ")");
    qDebug() << query.lastQuery();
}

void Sql::removeJob(long id)
{
    QSqlQuery query;
    query.exec("DELETE FROM `JobList` WHERE ID = " + QString::number(id));
}

QList<QObject *> Sql::enquireJob()
{
    QSqlQuery query;
    query.exec("SELECT * FROM `JobList`");

    QList<QObject *> jobList;
    while (query.next())
    {
        QString id = query.value(0).toString();
        QString file = query.value(1).toString();
        //int procedure = query.value(2).toInt();

        QString armCoordinateStr = query.value(3).toString();
        QList<QPointF> armCoordinate;
        QStringList coordList = armCoordinateStr.split(";");
        foreach (QString coordStr, coordList)
        {
            QStringList coord = coordStr.split(",");
            QPointF point(coord.at(0).toDouble(), coord.at(1).toDouble());
            armCoordinate.append(point);
        }

        QString boxArrayListStr = query.value(4).toString();
        QStringList boxList = boxArrayListStr.split(";");
        QList<QList<int>> boxArrayList;
        foreach (QString boxStr, boxList)
        {
            QStringList box = boxStr.split(",");
            QList<int> boxInt;
            foreach (QString str, box)
            {
                if(str == "1")
                    boxInt.append(1);
                else
                    boxInt.append(0);
            }
            boxArrayList.append(boxInt);
        }
        double panelHeight = query.value(5).toDouble();
        QString boxType = query.value(6).toString();
        qint64 createDate = query.value(7).toLongLong();

        jobList.append(new JobItem(id, file, armCoordinate, boxArrayList, panelHeight, boxType, createDate));
    }
    return jobList;
}

long Sql::enquireHighestJob()
{
    QSqlQuery query;
    query.exec("SELECT * FROM `sqlite_sequence` WHERE name = \"JobList\"");
    while (query.next())
    {
        long value = long(query.value(1).toLongLong());
        return value;
    }
    return 0;
}

void Sql::newLog(int event, int node)
{
    QDateTime current = QDateTime::currentDateTime();
    qint64 datetime = current.toSecsSinceEpoch();
    QSqlQuery query;
    query.exec("INSERT INTO `Log` (DateTime, Event) VALUES (" + QString::number(datetime) + ", " + QString::number(event) + ")");

    query.exec("SELECT * FROM `sqlite_sequence` WHERE name = \"Log\"");
    long value = 0;
    while (query.next())
    {
        value = long(query.value(1).toLongLong());
    }
    emit updateLogTable(QString::number(value), current.toString("yyyy/MMMM/dd (dddd)"), current.time().toString(), eventToString(event));
}

QString Sql::eventToString(int event, int node)
{
    switch (event)
    {
        case 0:
            return QString(tr("Debug Log."));
        case 1:
            return QString(tr("Node %1 open failed.").arg(QString::number(node)));
        case 2:
            return QString(tr("Node %1 close failed.").arg(QString::number(node)));
        case 3:
            return QString(tr("Axis %1 reaxh limit.").arg(QString::number(node)));
        case 4:
            return QString(tr("Emergency button pressed."));
        case 5:
            return QString(tr("Motor %1 failed").arg(QString::number(node)));
        default:
            return QString("");
    }
}

QVector<QVector<QString>> Sql::enquireLog()
{
    QSqlQuery query;
    query.exec("SELECT * FROM `Log` ORDER BY ID DESC");
    QVector<QVector<QString>> table;
    while (query.next())
    {
        QVector<QString> row;
        row.append(query.value(0).toString());
        QDateTime dateTime = QDateTime::fromSecsSinceEpoch(query.value(1).toLongLong());
        row.append(dateTime.date().toString("yyyy/MMMM/dd (dddd)"));
        row.append(dateTime.time().toString());
        int event = query.value(2).toInt();
        row.append(eventToString(event));
        table.append(row);
    }
    return table;
}

void Sql::reset()
{
    QSqlQuery query;
    query.exec("DROP TABLE \"JobList\"");
    query.exec("DROP TABLE \"Log\"");
}
