#include "System.h"

// Prevent LNK2001
System *System::instance;

System::System(QObject *parent) : QObject(parent)
{
    currentPath = QDir::currentPath();
}

System *System::getInstance()
{
    if(instance == nullptr)
    {
        instance = new System();
    }
    return instance;
}

void System::start()
{
    // Initialze SQL
    sql = Sql::getInstance();

    // Get the size of the job
    jobCount = sql->enquireHighestJob();

    // Initialze DMC
    dmc = DMC::getInstance();
    dmc->moveToThread(&dmcThread);

    // DMC signal and slot
    // Job
    qRegisterMetaType<QList<QPointF>>("QList<QPointF>");
    qRegisterMetaType<QList<QList<int>>>("QList<QList<int>>");
    qRegisterMetaType<WORD>("WORD");
    QObject::connect(this, &System::dmcPerformOperation, dmc, &DMC::autoOperation);
    QObject::connect(dmc, &DMC::opeartionFinished, this, &System::autoOperationFinished);

    // Manual
    QObject::connect(this, &System::dmcManualGripper, dmc, &DMC::gripper);
    QObject::connect(this, &System::dmcManualVibration, dmc, &DMC::manualVibration);
    QObject::connect(this, &System::dmcManualMove, dmc, &DMC::manualMove);

    // Signal from DMC
    QObject::connect(dmc, &DMC::gripperStatusChanged, this, &System::receiveGripperStatusChanged);
    QObject::connect(dmc, &DMC::vibrationStatusChanged, this, &System::receiveVibrationStatusChanged);
    QObject::connect(dmc, &DMC::checkArmFailed, this, &System::receiveCheckArmFailed);
    QObject::connect(dmc, &DMC::structureAlarm, this, &System::receiveStructureAlarm);
    QObject::connect(dmc, &DMC::motorFailed, this, &System::receiveMotorFail);

    // Voltage
    QObject::connect(dmc, &DMC::regularJobFinished, this, &System::receiveRegularJobFinished);

    // Other
    QObject::connect(this, &System::dmcSwitchLight, dmc, &DMC::light);
    QObject::connect(this, &System::dmcResetMotor, dmc, &DMC::resetMotor);
    QObject::connect(dmc, &DMC::motorResetFinished, this, &System::receiveMotorResetFinished);
    QObject::connect(this, &System::dmcEmergencyStop, dmc, &DMC::emergencyStop);
    QObject::connect(this, &System::dmcUpdateAutoSetting, dmc, &DMC::updateSetting);

    // Box suplier
    boxesSupplier = new QUdpSocket();
    boxesSupplier->bind(QHostAddress("192.168.1.101"), 3001);
    boxesSupplier->connectToHost(QHostAddress("192.168.1.254"), 3000);
    QObject::connect(boxesSupplier, &QUdpSocket::readyRead, this, &System::boxSuplierReceive);

    dmcThread.start();
    emit dmcSwitchLight(1);

    // Initialze values
    for(int i = 0; i < 6; i++)
        laserFinder.append(0);

    // Get jobs from sql
    jobList.append(sql->enquireJob());
    emit jobListChanged();

    // Print the details of the first job
    if(jobList.size() > 0)
    {
        // Get job status
        settings.beginGroup("System");
        currentJobProcedure = settings.value("currentJobProcedure").toInt();
        currentOpeartion = settings.value("currentOpeartion").toInt();
        settings.endGroup();
        // Get the first item
        JobItem *item = qobject_cast<JobItem *>(jobList.first());
        // Chane the current job
        currentJobID = item->id();
        currentJobFilePath = item->file();
        currentJobProcedureSize = item->step();
        CurrentBoxType = item->boxType();
        CurrentPanelHeight = item->panelHeight();
        systemStatus = tr("Auto mode paused.");
        emit systemStatusChanged();
    }
}

void System::runAuto()
{
    if(!autoMode)
    {
        emit dmcSwitchLight(2);
        autoMode = true;
        emit autoModeChanged();
        systemStatus = tr("Box supplier in manual mode.");
        emit systemStatusChanged();
        // bypass supplier
        //supplierAuto = true;
        if(supplierAuto)
        {
            autoRunning = true;
            if(jobList.size() > 0)
                autoJobStart();
        }
    }
}

void System::pauseAuto()
{
    if(autoMode)
    {
        autoRunning = false;
        emit dmcSwitchLight(1);
        autoMode = false;
        emit autoModeChanged();
        if(supplierAuto)
            systemStatus = tr("Auto mode paused.");
        else
            systemStatus = tr("Box supplier in manual mode.");
        emit systemStatusChanged();
    }
}

void System::newJob(QString file, QList<QPointF> armCoordinate, QList<QList<int>> boxArrayList, double panelHeight, QString boxType)
{
    JobItem *job = new JobItem(QString::number(++jobCount), file, armCoordinate, boxArrayList, panelHeight, boxType);
    // Add the Job into list
    jobList.append(job);
    emit jobListChanged();

    // Add job to sql
    sql->newJob(job);

    if(autoRunning && jobList.size() == 1)
        autoJobStart();
}

void System::autoJobStart()
{
    // Get the first item
    JobItem *item = qobject_cast<JobItem *>(jobList.first());
    currentArmCoordinate.clear();
    currentArmCoordinate.append(item->armCoordinate());
    currentBoxArrayList.clear();
    currentBoxArrayList.append(item->boxArrayList());

    // Change the current job
    currentJobID = item->id();
    currentJobFilePath = item->file();
    currentJobProcedureSize = item->step();
    CurrentBoxType = item->boxType();
    CurrentPanelHeight = item->panelHeight();

    // Change the table in home page
    currentJobTable.clear();
    for(int i = 0; i < currentArmCoordinate.size(); i++)
    {
        QVector<QString> row;
        // Append waiting procedure
        if(i >= currentJobProcedure)
        {
            // ID
            row.append(QString::number(i + 1));
            // Center
            row.append("(" + QString::number(currentArmCoordinate.at(i).x()) + ", " + QString::number(currentArmCoordinate.at(i).y()) + ")");
            for(int j = 0; j < 15; j++)
            {
                if(currentBoxArrayList.at(i).at(j))
                    row.append("Y");
                else
                    row.append("");
            }
            currentJobTable.append(row);
        }
    }
    emit currentJobChanged();

    // Move dmc
    dmcOperationStart();
}

void System::autoJobsFinished()
{
    qDebug() << SYSTEM_NAME << "Job finished";

    JobItem *first = qobject_cast<JobItem *>(jobList.first());

    /*
    // Write to csv
    QFile file("history.csv");
    if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        // We're going to streaming text to the file
        QTextStream stream(&file);
        stream << first->id() << "," << first->file() << "," << first->createTime().toString("yyyy年MM月dd日 (dddd) HH:mm:ss") << "," << QDateTime::currentDateTime().toString("yyyy年MM月dd日 (dddd) HH:mm:ss");
        file.close();
    }
    */

    // Remove from sql
    sql->removeJob(first->id().toLong());

    // Remove img
    QFile::remove(first->id() + ".png");

    // Remove the first item
    if(jobList.size() > 0)
        jobList.removeFirst();
    emit jobListChanged();

    currentJobProcedure = 0;
    settings.setValue("System/currentJobProcedure", currentJobProcedure);
    emit dmcProcedureFinished();

    qDebug() << SYSTEM_NAME << "The size of job list is " << jobList.size();

    // Complete the next Job
    if(jobList.size() > 0)
        autoJobStart();
    else{
        // idle
        autoRunning = false;
        currentJobID = "";
        currentJobFilePath = "";
        currentJobProcedureSize = 0;
        currentJobProcedure = 0;
        CurrentBoxType = "";
        CurrentPanelHeight = 0;
        currentJobTable.clear();
        systemStatus = tr("Idle.");
        emit currentJobChanged();
        emit systemStatusChanged();
        pauseAuto();
    }
}

void System::autoOperationFinished(int operation)
{
    if(autoRunning && autoMode)
    {
        qDebug() << SYSTEM_NAME << "operation finished " << operation << " out of " << DMC_MAX_OPERATION;

        // Procedure finished
        if(currentOpeartion >= DMC_MAX_OPERATION)
        {
            qDebug() << SYSTEM_NAME << "PROCEDURE FINISHED " << currentJobProcedure << " OUT OF " << currentJobProcedureSize;

            currentOpeartion = 0;
            currentJobProcedure++;
            settings.setValue("System/currentJobProcedure", currentJobProcedure);
            emit dmcProcedureFinished();

            // Job Finished
            if(currentJobProcedure >= currentJobProcedureSize)
            {
                autoJobsFinished();
                return;
            }
        }
        else
            currentOpeartion++;

        // Change the str
        systemStatus = QString::number(currentOpeartion);
        emit systemStatusChanged();
        settings.setValue("System/currentOpeartion", currentOpeartion);
        // Perform other operation
        dmcOperationStart();
    }
    /*
    else
    {
        // Change the current operation string
        systemStatus = "Auto mode paused.";
        emit systemStatusChanged();
        qDebug() << SYSTEM_NAME << " Auto mode paused";
    }
    */
}

void System::dmcOperationStart()
{
    // Change the str
    systemStatus = QString::number(currentOpeartion);
    emit systemStatusChanged();

    // Supplier
    if(currentOpeartion == 0)
    {
        QString boxStr = "";
        // First 8 boxes
        for(int i = 0; i < 8; i++)
        {
            if(currentBoxArrayList.at(currentJobProcedure).at(i) == 1)
                boxStr += "1";
            else
                boxStr += "0";
        }
        // 9 is unused
        boxStr += "0";
        // 15 to 9
        for(int i = 14; i > 8; i--)
        {
            if(currentBoxArrayList.at(currentJobProcedure).at(i) == 1)
                boxStr += "1";
            else
                boxStr += "0";
        }
        //qDebug()<< boxStr;
        if(currentJobProcedure == 0)
            boxesSupplierSend(BoxRequest, boxStr);

        // Old shit
        QVariantList *parameter = new QVariantList();
        parameter->append(boxStr);
        emit dmcPerformOperation(currentOpeartion, parameter);
    }
    // Move arm to supplier
    else if(currentOpeartion == 3)
    {
        // Supplier is operating, block the system
        if(supplierOperating)
        {
            autoBlocking = true;
            systemStatus = tr("Waiting for box suppling finished.");
            emit systemStatusChanged();
            return;
        }
        else
        {
            // Not Allow box supplier to operate
            //boxesSupplierSend(PauseSupplier);
            emit dmcPerformOperation(currentOpeartion);
        }
    }
    // Move arm to embed
    else if(currentOpeartion == 8)
    {
        QVariantList *parameter = new QVariantList();
        parameter->append(currentArmCoordinate.at(currentJobProcedure).x());
        parameter->append(currentArmCoordinate.at(currentJobProcedure).y());
        emit dmcPerformOperation(currentOpeartion, parameter);

    }
    else if(currentOpeartion == 9)
    {
        // Ask for next box
        QString boxStr = "";
        // First 8 boxes
        for(int i = 0; i < 8; i++)
        {
            if(currentBoxArrayList.at(currentJobProcedure).at(i) == 1)
                boxStr += "1";
            else
                boxStr += "0";
        }
        // 9 is unused
        boxStr += "0";
        // 15 to 9
        for(int i = 14; i > 8; i--)
        {
            if(currentBoxArrayList.at(currentJobProcedure).at(i) == 1)
                boxStr += "1";
            else
                boxStr += "0";
        }
        //qDebug()<< boxStr;
        if(currentJobProcedure != 0)
            boxesSupplierSend(BoxRequest, boxStr);

        emit dmcPerformOperation(currentOpeartion);
    }
    else
        emit dmcPerformOperation(currentOpeartion);
}

void System::emergencyStop()
{
    emergencyMode = true;
    emit emergencyModeChanged();
    sql->newLog(4);
    // Stop auto mode
    pauseAuto();
    // Stop motor
    emit dmcEmergencyStop();
    // Ask Supplier to stop
    boxesSupplierSend(EmergencyStop);
    systemStatus = tr("Emergency stop pressed.");
    emit systemStatusChanged();
    // Light
    emit dmcSwitchLight(0);
}

void System::emergencyStopOff()
{
    emergencyMode = false;
    emit emergencyModeChanged();
    systemStatus = tr("Idle.");
    emit systemStatusChanged();
    boxesSupplierSend(ResumeSupplier);
    // Light
    emit dmcSwitchLight(1);
}

void System::resetMotor()
{
    emit dmcResetMotor();
}

void System::receiveMotorResetFinished()
{
    emit dmcMotorResetFinished();
}

void System::manualGripper(int node, bool open, bool skip)
{
    emit dmcManualGripper(WORD(node), open, skip);
}

void System::manualVibration(int node, bool open)
{
    emit dmcManualVibration(WORD(node), open);
}

void System::manualMove(int axis, QString distance)
{
    long ld = 0;
    if(!distance.isEmpty())
        ld = long(distance.toDouble());
    emit dmcManualMove(WORD(axis), ld);
}

void System::receiveGripperStatusChanged(int node, bool open)
{
    emit gripperStatusChanged();
}

void System::receiveVibrationStatusChanged(int node, bool on)
{
    emit vibrationStatusChanged();
}

void System::receiveRegularJobFinished(double laser0, double laser1, double laser2)
{
    laserFinder[0] = laser0;
    laserFinder[1] = laser0;
    laserFinder[2] = laser1;
    laserFinder[3] = laser1;
    laserFinder[4] = laser2;
    laserFinder[5] = laser2;

    emit regularJobFinished();
}

void System::receiveCheckArmFailed(int event, int node)
{
    sql->newLog(event, node);
}

void System::receiveStructureAlarm(int node)
{
    sql->newLog(3, node);
}

void System::receiveMotorFail(int node)
{
    sql->newLog(5, node);

    emergencyStop();

    systemStatus = tr("Motor %1 failed").arg(QString::number(node));
    emit systemStatusChanged();
}

void System::boxesSupplierRequest(QString requestBoxes)
{
    boxesSupplierSend(BoxRequest, requestBoxes);
}

void System::boxesSupplierSend(UdpSend command, QString requestBoxes)
{
    QByteArray request;
    // BoxRequest
    quint16 requestBoxesInt = 0;
    switch (command)
    {
        case BoxRequest:
            // Convert into integer
            requestBoxesInt = requestBoxes.toUShort(nullptr, 2);
            qDebug() << "System: Requesting string: " + requestBoxes + " in hex: " + QString::number(requestBoxesInt, 16);
            request = QByteArray::fromHex(QString::number(requestBoxesInt, 16).toLocal8Bit());
            request.resize(2);
            // Resize
            request += QByteArray::fromHex("000000000000");
            break;
        case PauseSupplier:
            request = QByteArray::fromHex("0000FFFF00000000");
            break;
        case ResumeSupplier:
            request = QByteArray::fromHex("0000000000000000");
            break;
        case EmergencyStop:
            request = QByteArray::fromHex("0000FFFF0000EEEE");
            break;
    }
    qDebug() << "System: sending: " + request.toHex(0).toUpper();
    boxesSupplier->write(request);
}

void System::boxSuplierReceive()
{
    // when data comes in
    QByteArray buffer;
    buffer.resize(boxesSupplier->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    boxesSupplier->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);
    QString bufferStr = buffer.toHex();
    bufferStr = bufferStr.toUpper();

    #ifdef DEBUG_MODE
    //QDateTime local(QDateTime::currentDateTime());
    //qDebug() << local << ": " << bufferStr;
    #endif

    // Analyze the string
    // D0
    if(bufferStr.mid(0, 2) == "00")
    {
        // PLC Normal startup
    }
    else if(bufferStr.mid(0, 2) == "FF")
    {
        // PLC emergency
        emergencyStop();
    }
    // D1
    if(bufferStr.mid(4, 4) == "0000")
    {
        // Box supplier is idle
        supplierOperating = false;
        // Recover from blocking
        if(autoBlocking)
        {
            autoBlocking = false;
            dmcOperationStart();
        }
    }
    else if(bufferStr.mid(4, 4) == "FFFF")
    {
        // Box supplier is operating
        supplierOperating = true;
    }
    // D2
    if(bufferStr.mid(8, 4) == "0000")
    {
        // Plc decide box
    }
    else if(bufferStr.mid(8, 4) == "AAAA")
    {
        // Box A
    }
    else if(bufferStr.mid(8, 4) == "BBBB")
    {
        // Box B
    }
    // D3
    if(bufferStr.mid(12, 4) == "0000")
    {
        // Plc decide box gripping
    }
    // D4
    if(bufferStr.mid(16, 4) == "0000")
    {
        // Box supplier is idle (Auto)
        supplierOperating = false;
        // Recover from blocking
        if(autoBlocking)
        {
            autoBlocking = false;
            dmcOperationStart();
        }
    }
    else if(bufferStr.mid(16, 4) == "FFFF")
    {
        // Box supplier is operating (Auto)
        supplierOperating = true;
    }
    // D5
    if(bufferStr.mid(20, 4) == "0000")
    {
        // PLC in manual
        supplierAuto = false;
        // Stop the PC
        if(autoRunning)
        {
            pauseAuto();
        }
    }
    else if(bufferStr.mid(20, 4) == "FFFF")
    {
        // PLC in auto
        supplierAuto = true;
        // Allow PC to run
        if(autoMode)
        {
            if(!autoRunning)
            {
                autoRunning = true;
                if(jobList.size() > 0)
                    autoJobStart();
            }
        }
    }
    // D6
    if(bufferStr.mid(24, 4) == "0000")
    {
        //
    }
    else if(bufferStr.mid(24, 4) == "EEEE")
    {
        // PLC emergency return
    }
}

void System::newLog(int event)
{
    sql->newLog(event);
}

void System::updateSetting()
{
    emit dmcUpdateAutoSetting();
}

void System::resetApplication()
{
    if(autoRunning)
        pauseAuto();
    foreach(QString key, settings.allKeys())
        settings.remove(key);
    sql->reset();
    QCoreApplication::quit();
}
