
#include "DxfImport.h"

// Prevent LNK2001
DxfImport *DxfImport::instance;

DxfImport::DxfImport(QObject *parent) : QObject (parent)
{
    system = System::getInstance();
}

DxfImport *DxfImport::getInstance()
{
    if(instance == nullptr)
    {
        instance = new DxfImport();
    }
    return instance;
}

int DxfImport::nameCheck(QUrl file)
{
    // Get file name
    QString fileStr = file.toString();
    QStringList fileStrList = fileStr.split("/");
    //qDebug() << fileStrList.at(fileStrList.size() - 1);
    QString fileName = fileStrList.at(fileStrList.size() - 1);
    QStringList fileNameList = fileName.split("_");

    QString boxTypeStr = fileNameList.at(1).toUpper();
    QString panelHeightStr = fileNameList.at(2);
    panelHeightStr.chop(4);
    //qDebug() << boxType << panelHeight;
    boxType = boxTypeStr;
    if(fileNameList.size() != 3)
    {
        // incorrect name format
        return 1;
    }
    if(boxTypeStr.toUpper() != "A" && boxTypeStr.toUpper() != "B")
    {
        // incorrect box type
        return 2;
    }
    bool ok;
    panelHeight = panelHeightStr.toDouble(&ok);
    if(!ok)
    {
        // incorrect height
        return 3;
    }
    return 0;
}

void DxfImport::import(QUrl file)
{
    DxfLib f;
    DL_Dxf dxf;
    QString fileStr = file.toString();
    dxfFile = fileStr;
    if (!dxf.in(fileStr.right(fileStr.size() - 8).toStdString(), &f))
    {
        qDebug() << fileStr.right(fileStr.size() - 8) << " could not be opened.";
    }

    // Read file
    QPointF minimum;

    foreach(const QString &layer, f.layers)
    {
        //  qDebug() << "This layer is: " << layer;

        /*QList<DL_CircleData> circles = f.circles.values(layer);
        for(int i = 0; i < circles.size(); i++)
        {
            qDebug() << "Coordinate X: " << circles.at(i).cx << "Coordinate Y: " << circles.at(i).cy << circles.at(i).radius;;
        }*/
        //"A-GENM"

        if (layer == "A-GENM"){                                                       //in layer A-GENM

            QList<DL_LineData> lines= f.lines.values(layer);
            QPointF StartingPoint (lines.at(0).x1,lines.at(0).y1);
            QPointF EndingPoint (lines.at(0).x2,lines.at(0).y2);

            for(int i = 0; i < lines.size(); i++)                               // Loop for finding zero position (minium) (most negative point)
            {
                if(lines.at(i).x1 <= StartingPoint.rx()){                       // Compare points in starting point and ending point
                    if(lines.at(i).y1 <= StartingPoint.ry()){
                        StartingPoint.setX(lines.at(i).x1);
                        StartingPoint.setY(lines.at(i).y1);
                    }
                }
                if(lines.at(i).x2 <= EndingPoint.rx()){
                    if(lines.at(i).y2 <= EndingPoint.ry()){
                        EndingPoint.setX(lines.at(i).x2);
                        EndingPoint.setY(lines.at(i).y2);
                    }
                }

            }

            if(StartingPoint.rx()<EndingPoint.rx()){                            // compare between starting and ending point
                minimum = StartingPoint;
            }
            else{
                minimum = EndingPoint;
            }

            qDebug() << minimum;
            dxfOrigin = minimum;
            //     finding the edge of squares to detect boxes
            //     temp1(a,b) --- temp3(a,b)
            //          |
            //          |
            //          |
            //     temp2(a,b) ---


            //     tempa(a,b)
            //          |
            //          |
            //          |
            //     tempb(a,b)


            // parameters for finding all boxes' coordinates.

            QPointF temp1;
            QPointF temp2;
            QPointF temp3;
            QPointF tempC;
            QList<QPointF> count;
            QList<QPointF> center;
            QPointF tempa;
            QPointF tempb;

            for(int i = 0; i < lines.size(); i++)
            {
                if(lines.at(i).x1 == lines.at(i).x2){
                    //qDebug() << "vertical line!"<<i;

                    if(lines.at(i).y1 < lines.at(i).y2){       //find highest point in line
                        tempa.setX(lines.at(i).x2);
                        tempa.setY(lines.at(i).y2);
                        tempb.setX(lines.at(i).x1);
                        tempb.setY(lines.at(i).y1);
                        //count << minimum;
                        //qDebug() << "setaa" << tempa << tempb;
                    }
                    else if(lines.at(i).y1 > lines.at(i).y2) {
                        tempa.setX(lines.at(i).x1);
                        tempa.setY(lines.at(i).y1);
                        tempb.setX(lines.at(i).x2);
                        tempb.setY(lines.at(i).y2);
                        //count << minimum;
                        //qDebug() << "setbb" << tempa << tempb;
                    }                                           //find all vertical lines
                    int j = 0;
                    for (j; j < lines.size();j++) {
                        if(lines.at(j).y1 == lines.at(j).y2){
                            //qDebug() << "Found the horizontal line";
                            if(tempa.rx() == lines.at(j).x1 & tempa.ry() == lines.at(j).y1 & tempa.rx() < lines.at(j).x2){
                                temp3.setX(lines.at(j).x2);
                                temp3.setY(lines.at(j).y2);
                                temp1.setX(tempa.rx());
                                temp1.setY(tempa.ry());
                                temp2.setX(tempb.rx());
                                temp2.setY(tempb.ry());
                                count << minimum;
                                if(temp1.ry()-temp2.ry() <300){
                                    tempC.setX((temp3.rx()-temp2.rx())/2+temp2.rx()-minimum.rx());
                                    tempC.setY((temp1.ry()-temp2.ry())/2+temp2.ry()-minimum.ry());
                                    center << tempC;
                                }
                            }
                            else if(tempa.rx() == lines.at(j).x2 & tempa.ry() == lines.at(j).y2 & tempa.rx() < lines.at(j).x1){
                                temp3.setX(lines.at(j).x1);
                                temp3.setY(lines.at(j).y1);
                                temp1.setX(tempa.rx());
                                temp1.setY(tempa.ry());
                                temp2.setX(tempb.rx());
                                temp2.setY(tempb.ry());
                                count << minimum;
                                if(temp1.ry()-temp2.ry() <300){
                                    //count<<minimum;
                                    tempC.setX((temp3.rx()-temp2.rx())/2+temp2.rx()-minimum.rx());
                                    tempC.setY((temp1.ry()-temp2.ry())/2+temp2.ry()-minimum.ry());
                                    center << tempC;
                                }
                            }
                        }
                    }
                }
            }
            //qDebug() << center.size() << center <<endl;
            for (int k = 1; k < center.size();k++) {
                //qDebug()<<"YO"<<k << center.size();
                for (int l = k-1;l >= 0;l--) {
                    //qDebug()<<"yeah"<<l;
                    if(center.at(k) == center.at(l)){
                        center.removeAt(k);
                        k--;
                    }
                }
            }


            //qDebug() << count.size()<< lines.size() << center.size() << endl;
            //qDebug() << center.size() << endl << center;

            //     corner --- rangert
            //        |          |
            //        |          |
            //        |          |
            //     rangelb --- corner

            // parameters for arranging and planning for mapping coordinates for robot arm.

            QPointF rangelb = center.at(0);
            QPointF rangert = center.at(0);

            for (int m = 0;m < center.size();m++) {
                if(center.at(m).x() < rangelb.x()){
                    rangelb.setX(center.at(m).x());
                }
                if(center.at(m).y() < rangelb.y()){
                    rangelb.setY(center.at(m).y());
                }
                if(center.at(m).x() > rangert.x()){
                    rangert.setX(center.at(m).x());
                }
                if(center.at(m).y() > rangert.y()){
                    rangert.setY(center.at(m).y());
                }
            }

            int point_counter;
            QList<QPointF> centre_order;
            int abc=0;

            for (point_counter = 0; point_counter < center.size();point_counter++) {
                //qDebug()<<"YO"<<k << center.size();
                for (int l = point_counter;l >= 0; l--) {
                    //qDebug()<<"yeah"<<l;
                    if(center.at(point_counter).x() >= center.at(l).x()){
                        if(center.at(point_counter).y() >= center.at(l).y()){
                            centre_order.append(center.at(l));
                            dxfBox.append(center.at(l));
                            center.removeAt(l);

                            abc ++;
                            point_counter--;
                        }
                    }
                }
            }
           // qDebug() << "-------------------------------------------------------";

            //qDebug() << centre_order << abc;

            //box distribution
            int n,m;
            int X,Y;
            QList<QPointF> Arm_Coordinate;
            QPointF temp_armc;
            int x,y;
            int j,k;
            bool empty = true;
            double Box_Increment = 300;

            X = qCeil((rangert.rx()-rangelb.rx()+300)/(3* Box_Increment));
            Y = qCeil((rangert.ry()-rangelb.ry()+300)/(5* Box_Increment));

            //qDebug() << X << Y;
            int Arm_Cycles = 0;
            QList<int> Valid_Cycles;
            int Box_Array [X*Y][5][3];
            QList<QList<int>> boxArrayList;
            QPointF tempPoint;

            tempPoint = rangelb;
            // two loops to
            for (y=0; y<X; y++){
                for (x=0; x<Y; x++){
                    // two for loops to create 2d array for box
                    for (n=0; n<3; n++){
                        for (m=0; m<5; m++){
                            // << tempPoint << m << n << x << y;
                            for (j= 0; j < centre_order.size(); j++) {
                                if (tempPoint == centre_order.at(j)){
                                    Box_Array[Arm_Cycles][m][n]=1;
                                    break;
                                }
                                else {
                                    Box_Array[Arm_Cycles][m][n]=0;
                                }
                            }

                            tempPoint.setY(tempPoint.ry()+ Box_Increment);
                        }
                        tempPoint.setY(rangelb.ry());
                        tempPoint.setX(tempPoint.rx()+Box_Increment);
                    }
                    temp_armc.setX(rangelb.rx()+1*Box_Increment);
                    temp_armc.setY(rangelb.ry()+2*Box_Increment);
                    Arm_Coordinate << temp_armc;
                    rangelb.setY(rangelb.ry()+5*Box_Increment);
                    tempPoint = rangelb;
                    Arm_Cycles++;
                }
                rangelb.setX(rangelb.rx()+3*Box_Increment);
                rangelb.setY(rangelb.ry()-10*Box_Increment);
                tempPoint = rangelb;
            }
            //qDebug() << Arm_Cycles;

            //---------------------------------------output------------------------------------------------------

            for(int i=0;i<Arm_Cycles;i++){
                empty = true;
                for(j=4;j>=0;j--){
                    for(k=0;k<3;k++)
                    {
                        //printf("%d\t",Box_Array[i][j][k]);
                        if(Box_Array[i][j][k]==1){
                            empty = false;
                        }
                    }
                    //qDebug("\n");
                }
                if(empty == false){
                    Valid_Cycles << i;
                }
            }

            //qDebug() << Valid_Cycles;
            for (int i=0;i<Valid_Cycles.size();i++) {
                //qDebug() << "Center " << Arm_Coordinate.at(Valid_Cycles.at(i)) << endl;
                QList<int> oneCycle;
                for(j=4;j>=0;j--){
                    for(k=0;k<3;k++)
                    {
                        //printf("%d\t",Box_Array[Valid_Cycles.at(i)][j][k]);
                        oneCycle.append(Box_Array[Valid_Cycles.at(i)][j][k]);
                    }
                    //qDebug("\n");
                }
                mBoxArrayList.append(oneCycle);
            }

            // Not DXF lib data
            validCycles.append(Valid_Cycles);
            ArmCoordinate.append(Arm_Coordinate);
            mBoxArrayList.append(mBoxArrayList);
            // Convert DXF to standard
            standardBoxArrayList.clear();
            for(int i = 0; i < mBoxArrayList.size(); i++)
            {
                QList<int> box;
                for(int j = 0; j < 15; j++)
                    box.append(0);

                // Don't use loop
                box[0] = mBoxArrayList.at(i).at(12);
                box[1] = mBoxArrayList.at(i).at(9);
                box[2] = mBoxArrayList.at(i).at(6);
                box[3] = mBoxArrayList.at(i).at(3);
                box[4] = mBoxArrayList.at(i).at(0);
                box[5] = mBoxArrayList.at(i).at(13);
                box[6] = mBoxArrayList.at(i).at(10);
                box[7] = mBoxArrayList.at(i).at(7);
                box[8] = mBoxArrayList.at(i).at(4);
                box[9] = mBoxArrayList.at(i).at(1);
                box[10] = mBoxArrayList.at(i).at(14);
                box[11] = mBoxArrayList.at(i).at(11);
                box[12] = mBoxArrayList.at(i).at(8);
                box[13] = mBoxArrayList.at(i).at(5);
                box[14] = mBoxArrayList.at(i).at(2);

                standardBoxArrayList.append(box);
            }
        }
    }
    // Generate table
    for (int i = 0; i < validCycles.size(); i++)
    {
        QVector<QString> record;
        record.append(QString::number(i + 1));
        record.append("(" + QString::number(ArmCoordinate.at(validCycles.at(i)).x()) + ", " + QString::number(ArmCoordinate.at(validCycles.at(i)).y()) + ")");
        finalArmCoordinate.append(ArmCoordinate.at(validCycles.at(i)));
        for(int j = 0; j < 15; j++)
        {
            if(standardBoxArrayList.at(i).at(j))
                record.append("Y");
            else
                record.append("");
        }
        //qDebug() << record.size();
        table.append(record);
    }
    emit importFinished();
}

void DxfImport::newJob()
{
    system->newJob(dxfFile, finalArmCoordinate, standardBoxArrayList, panelHeight, boxType);
}

void DxfImport::clear()
{
    dxfFile = "";
    table.clear();
    dxfBox.clear();
    validCycles.clear();
    ArmCoordinate.clear();
    mBoxArrayList.clear();
    finalArmCoordinate.clear();
    //finalBoxArrayList.clear();
    emit importFinished();
}
