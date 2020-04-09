#include <QCoreApplication>
#include "sql.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Sql *sql = Sql::getInstance();
    sql->connectDatabase("192.168.0.2", 3307, "root", "EIENoMoney$4", "3.1");

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

    return a.exec();
}
