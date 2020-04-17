#include "DMC.h"

// Prevent LNK2001
DMC *DMC::instance;

DMC::DMC(QObject *parent) : QObject(parent)
{
    // Start the board
    short dmcCard = dmc_board_init();
    #ifdef DEBUG_MODE
    qDebug() << DMC_SYSTEM_NAME << "The card is " << dmcCard;
    #endif

    // Get setting
    for(int i = 0; i < DOF_COUNT; i++)
    {
        struct MotorSetting a;
        motorsetting.append(a);
        struct MotorSetting b;
        motorresetsetting.append(b);
    }
    for(int i = 0; i < BOX_TYPE; i++)
    {
        struct AutoSetting a;
        autosetting.append(a);
    }
    updateSetting();
    setting.beginGroup("DMC");
    for (int i = 0; i < GRIPPER_COUNT; ++i)
    {
        gripperStatus.append(setting.value("gripper" + QString::number(i)).toBool());
        vibrationStatus.append(setting.value("vibration" + QString::number(i)).toBool());
    }
    for(WORD i = 1; i < 16; i++)
    {
        bool skip = true;
        if(i == 15)
            skip = false;
        gripper(i, gripperStatus[i - 1], skip);
        manualVibration(i, vibrationStatus[i - 1]);
    }
    setting.endGroup();

    // Connect to I/O board 1 and 2
    // nmc_set_connect_state(WORD CardNo,WORD NodeNum,WORD state,WORD Baud)
    nmc_set_connect_state(cardNo, 1, 1, 0);
    nmc_set_connect_state(cardNo, 2, 1, 0);

    // Initalize motor
    // Lazy
    dmc_set_inp_mode( 0 , 0 , 0 , 0 );
    dmc_set_inp_mode( 0 , 1 , 0 , 0 );
    dmc_set_inp_mode( 0 , 2 , 0 , 0 );
    dmc_set_alm_mode( 0 , 0 , 1 , 0 , 0 );
    dmc_set_alm_mode( 0 , 1 , 1 , 0 , 0 );
    dmc_set_alm_mode( 0 , 2 , 1 , 0 , 0 );
    dmc_set_emg_mode( 0 , 0 , 0 , 0 );
    dmc_set_emg_mode( 0 , 1 , 0 , 0 );
    dmc_set_emg_mode( 0 , 2 , 0 , 0 );
    dmc_set_el_mode( 0 , 0 , 1 , 1 , 1 );
    dmc_set_el_mode( 0 , 1 , 1 , 1 , 1 );
    dmc_set_el_mode( 0 , 2 , 1 , 1 , 1 );
    dmc_set_softlimit( 0 , 0 , 0 , 0 , 0 , 0 , 0 );
    dmc_set_softlimit( 0 , 1 , 0 , 0 , 0 , 0 , 0 );
    dmc_set_softlimit( 0 , 2 , 0 , 0 , 0 , 0 , 0 );
    dmc_set_counter_inmode( 0 , 0 , 0 );
    dmc_set_counter_inmode( 0 , 1 , 0 );
    dmc_set_counter_inmode( 0 , 2 , 0 );
    dmc_set_ez_mode( 0 , 0 , 0 , 0 , 0 );
    dmc_set_ez_mode( 0 , 1 , 0 , 0 , 0 );
    dmc_set_ez_mode( 0 , 2 , 0 , 0 , 0 );
    dmc_set_homemode( 0 , 0 , 0 , 0 , 2 , 1 );
    dmc_set_home_pin_logic( 0 , 0 , 0 , 0 );
    dmc_set_homemode( 0 , 1 , 0 , 0 , 2 , 1 );
    dmc_set_home_pin_logic( 0 , 1 , 0 , 0 );
    dmc_set_homemode( 0 , 2 , 0 , 0 , 2 , 1 );
    dmc_set_home_pin_logic( 0 , 2 , 0 , 0 );
    dmc_set_s_profile( 0 , 0 , 0 , 0 );
    dmc_set_s_profile( 0 , 1 , 0 , 0 );
    dmc_set_s_profile( 0 , 2 , 0 , 0 );
    dmc_set_profile( 0 , 0 , 6000 , 6000 , 0.001 , 0.5 , 500 );
    dmc_set_profile( 0 , 1 , 9000 , 9000 , 0.001 , 0.5 , 500 );
    dmc_set_profile( 0 , 2 , 9000 , 9000 , 0.001 , 0.5 , 500 );
    dmc_set_pulse_outmode( 0 , 0 , 3 );
    dmc_set_pulse_outmode( 0 , 1 , 3 );
    dmc_set_pulse_outmode( 0 , 2 , 3 );
    dmc_set_dec_stop_time( 0 , 0 , 0.004 );
    dmc_set_dec_stop_time( 0 , 1 , 0.004 );
    dmc_set_dec_stop_time( 0 , 2 , 0.004 );
    dmc_set_axis_io_map( 0 , 0 , 0 , 0 , 0 , 0 );
    dmc_set_axis_io_map( 0 , 1 , 0 , 0 , 1 , 0 );
    dmc_set_axis_io_map( 0 , 2 , 0 , 0 , 2 , 0 );
    dmc_set_axis_io_map( 0 , 0 , 1 , 1 , 0 , 0 );
    dmc_set_axis_io_map( 0 , 1 , 1 , 1 , 1 , 0 );
    dmc_set_axis_io_map( 0 , 2 , 1 , 1 , 2 , 0 );
    dmc_set_axis_io_map( 0 , 0 , 2 , 2 , 0 , 0 );
    dmc_set_axis_io_map( 0 , 1 , 2 , 2 , 1 , 0 );
    dmc_set_axis_io_map( 0 , 2 , 2 , 2 , 2 , 0 );
    dmc_set_axis_io_map( 0 , 0 , 3 , 4 , 0 , 0 );
    dmc_set_axis_io_map( 0 , 1 , 3 , 4 , 0 , 0 );
    dmc_set_axis_io_map( 0 , 2 , 3 , 4 , 0 , 0 );
    dmc_set_axis_io_map( 0 , 0 , 4 , 4 , 0 , 0 );
    dmc_set_axis_io_map( 0 , 1 , 4 , 4 , 0 , 0 );
    dmc_set_axis_io_map( 0 , 2 , 4 , 4 , 0 , 0 );
    dmc_set_axis_io_map( 0 , 0 , 5 , 4 , 0 , 0 );
    dmc_set_axis_io_map( 0 , 1 , 5 , 4 , 1 , 0 );
    dmc_set_axis_io_map( 0 , 2 , 5 , 4 , 2 , 0 );
    dmc_set_axis_io_map( 0 , 0 , 7 , 3 , 0 , 0 );
    dmc_set_axis_io_map( 0 , 1 , 7 , 3 , 1 , 0 );
    dmc_set_axis_io_map( 0 , 2 , 7 , 3 , 2 , 0 );
    dmc_set_io_dstp_mode( 0 , 0 , 0 , 0 );
    dmc_set_io_dstp_mode( 0 , 1 , 0 , 0 );
    dmc_set_io_dstp_mode( 0 , 2 , 0 , 0 );
    dmc_set_inp_mode( 0 , 0 , 0 , 0 );
    dmc_set_inp_mode( 0 , 1 , 0 , 0 );
    dmc_set_inp_mode( 0 , 2 , 0 , 0 );

    // Son
    dmc_set_sevon_enable(cardNo, 0, 1);
    dmc_set_sevon_enable(cardNo, 1, 1);
    dmc_set_sevon_enable(cardNo, 2, 1);

    // Start
    dmc_set_counter_inmode(cardNo, 0, 3);
    dmc_set_counter_inmode(cardNo, 1, 3);
    dmc_set_counter_inmode(cardNo, 2, 3);

    // Setup the regular job
    regularJob = new QTimer(this);
    connect(regularJob, &QTimer::timeout, this, &DMC::regularJobTimeOut);
    regularJob->start(regularJobMs);
}

DMC *DMC::getInstance()
{
    if(instance == nullptr)
    {
        instance = new DMC();
    }
    return instance;
}

void DMC::autoOperation(int operation, QVariantList *parameter)
{

    switch (operation)
    {
        case 0:
            // Request boxes from supplier
            break;
        case 1:
            // Move arm to waiting area (Move up)
            moveZ(long(10), autosetting[currentBox].boxGrabSpeed);
            break;
        case 2:
            // Move arm to waiting area
            moveXY(autosetting[currentBox].boxWaitX, autosetting[currentBox].boxWaitY);
            #ifdef DEBUG_MODE
                QThread::msleep(ioWaitMs);
            #endif
            break;
        case 3:
            // Move arm to sipplier
            moveXY(autosetting[currentBox].boxSupplyX, autosetting[currentBox].boxSupplyY);
            break;
        case 4:
            // Open all gripper and check IO
            for(WORD j = 1; j < 16; j++)
            {
                gripper(j, true, true);
            }
            QThread::msleep(ioWaitMs);
            checkArmIO();
            break;
        case 5:
            // Move down to grab boxes
            moveZ(autosetting[currentBox].boxSupplyZ, autosetting[currentBox].boxGrabSpeed);
            break;
        case 6:
            // Close all gripper and check IO
            for(WORD j = 1; j < 16; j++)
            {
                gripper(j, false, true);
            }
            QThread::msleep(ioWaitMs);
            checkArmIO();
            break;
        case 7:
            // Move up arm
            moveZ(long(10), autosetting[currentBox].boxGrabSpeed);
            break;
        case 8:
            // Move to embedding position
            if(parameter->size() == 2)
                moveXY(long(parameter->at(0).toDouble()), long(parameter->at(1).toDouble()), true);
            else
                qDebug() << "Parameter error";
            break;
        case 9:
            // Vibrate
            autoVibration(true);
            break;
        case 10:
            // Move down to embed boxes
            moveZ(autosetting[currentBox].boxEmbedHeight, autosetting[currentBox].boxEmbedSpeed);
            break;
        case 11:
            // Wait
            QThread::msleep(1000);
            break;
        case 12:
            // Open all gripper
            for(WORD j = 1; j < 16; j++)
            {
                gripper(j, true, true);
            }
            QThread::msleep(ioWaitMs);
            checkArmIO();
            // Stop Vibrate
            autoVibration(false);
            break;
        case 13:
            // Move up
            moveZ(long(10), autosetting[currentBox].boxEmbedSpeed);
            break;
        case 14:
            // Close gripper
            for(WORD j = 1; j < 16; j++)
            {
                gripper(j, false, true);
            }
            QThread::msleep(ioWaitMs);
            checkArmIO();
            break;
        default:
            break;
    }
    emit opeartionFinished(operation);
}

void DMC::gripper(WORD node, bool open, bool skipCheck)
{
    #ifdef DEBUG_MODE
    if(open)
        qDebug() << DMC_SYSTEM_NAME << "gripper " << node << " open";
    else
        qDebug() << DMC_SYSTEM_NAME << "gripper " << node << " close";
    #endif

    // nmc_write_outbit(WORD CardNo,WORD NodeID,WORD IoBit,WORD IoValue)
    switch (node)
    {
        case 1:
            if(open)
                nmc_write_outbit(cardNo, 1, 13, 1);
            else
                nmc_write_outbit(cardNo, 1, 13, 0);
            break;
        case 2:
            if(open)
                nmc_write_outbit(cardNo, 1, 10, 1);
            else
                nmc_write_outbit(cardNo, 1, 10, 0);
            break;
        case 3:
            if(open)
                nmc_write_outbit(cardNo, 1, 9, 1);
            else
                nmc_write_outbit(cardNo, 1, 9, 0);
            break;
        case 4:
            if(open)
                nmc_write_outbit(cardNo, 1, 4, 1);
            else
                nmc_write_outbit(cardNo, 1, 4, 0);
            break;
        case 5:
            if(open)
                nmc_write_outbit(cardNo, 1, 3, 1);
            else
                nmc_write_outbit(cardNo, 1, 3, 0);
            break;
        case 6:
            if(open)
                nmc_write_outbit(cardNo, 1, 14, 1);
            else
                nmc_write_outbit(cardNo, 1, 14, 0);
            break;
        case 7:
            if(open)
                nmc_write_outbit(cardNo, 1, 11, 1);
            else
                nmc_write_outbit(cardNo, 1, 11, 0);
            break;
        case 8:
            if(open)
                nmc_write_outbit(cardNo, 1, 8, 1);
            else
                nmc_write_outbit(cardNo, 1, 8, 0);
            break;
        case 9:
            if(open)
                nmc_write_outbit(cardNo, 1, 5, 1);
            else
                nmc_write_outbit(cardNo, 1, 5, 0);
            break;
        case 10:
            if(open)
                nmc_write_outbit(cardNo, 1, 2, 1);
            else
                nmc_write_outbit(cardNo, 1, 2, 0);
            break;
        case 11:
            if(open)
                nmc_write_outbit(cardNo, 1, 15, 1);
            else
                nmc_write_outbit(cardNo, 1, 15, 0);
            break;
        case 12:
            if(open)
                nmc_write_outbit(cardNo, 1, 12, 1);
            else
                nmc_write_outbit(cardNo, 1, 12, 0);
            break;
        case 13:
            if(open)
                nmc_write_outbit(cardNo, 1, 7, 1);
            else
                nmc_write_outbit(cardNo, 1, 7, 0);
            break;
        case 14:
            if(open)
                nmc_write_outbit(cardNo, 1, 6, 1);
            else
                nmc_write_outbit(cardNo, 1, 6, 0);
            break;
        case 15:
            if(open)
                nmc_write_outbit(cardNo, 1, 1, 1);
            else
                nmc_write_outbit(cardNo, 1, 1, 0);
            break;
    }
    gripperStatus[node - 1] = open;
    setting.setValue("DMC/gripper" + QString::number(node - 1), open);
    if(!skipCheck)
        QTimer::singleShot(checkArmIOMs, this, &DMC::checkArmIO);
    emit gripperStatusChanged(int(node), open);
}

void DMC::moveXY(long x, long y, bool offset)
{
    #ifdef DEBUG_MODE
    qDebug() << DMC_SYSTEM_NAME << "moving X to: " << x << " mm = " << x * mmToPulseX << " mm";
    qDebug() << DMC_SYSTEM_NAME << "moving Y to: " << y << " mm = " << y * mmToPulseY << " mm";
    #endif
    #ifdef DISABLE_MOTOR
    // Wait for few seconds
    QThread::msleep(SLEEP_MS);
    return;
    #endif

    if(offset)
    {
        // offset
        x += panelOffsetX;
        y += panelOffsetY;
    }

    // X
    dmc_set_profile(cardNo, 0, motorsetting[0].minVelcoty, motorsetting[0].maxVelcoty, motorsetting[0].accTime, motorsetting[0].decTime, motorsetting[0].stopVelcoty);
    dmc_set_s_profile(cardNo, 0, 0, 0);
    // dmc_pmove(WORD CardNo,WORD axis,long Dist,WORD posi_mode)
    dmc_pmove(cardNo, 0, long(x * mmToPulseX), 1);

    // Y
    dmc_set_profile(cardNo, 1, motorsetting[1].minVelcoty, motorsetting[1].maxVelcoty, motorsetting[1].accTime, motorsetting[1].decTime, motorsetting[1].stopVelcoty);
    dmc_set_s_profile(cardNo, 1, 0, 0);
    dmc_pmove(cardNo, 1, long(y * mmToPulseY), 1);

    while (dmc_check_done(cardNo, 0) == 0 || dmc_check_done(cardNo, 1) == 0)
    {
        long xCurrent = dmc_get_encoder(cardNo, 0);
        long yCurrent = dmc_get_encoder(cardNo, 1);
        QCoreApplication::processEvents();
        QThread::msleep(MOTOR_WAIT);
        // Check motor moved or not
        long xNew = dmc_get_encoder(cardNo, 0);
        long yNew = dmc_get_encoder(cardNo, 1);
        if(xCurrent == xNew && dmc_read_current_speed(cardNo, 0) > 0)
        {
            #ifdef DEBUG_MODE
            qDebug() << DMC_SYSTEM_NAME << "Motor 0 failed";
            #endif
            emit motorFailed(0);
            break;
        }
        if(yCurrent == yNew && dmc_read_current_speed(cardNo, 1) > 0)
        {
            #ifdef DEBUG_MODE
            qDebug() << DMC_SYSTEM_NAME << "Motor 1 failed";
            #endif
            emit motorFailed(1);
            break;
        }
    }
    #ifdef DEBUG_MODE
    qDebug() << DMC_SYSTEM_NAME << " motor stopped.";
    #endif
}

void DMC::moveZ(long z, long speed)
{
    #ifdef DEBUG_MODE
    qDebug() << DMC_SYSTEM_NAME << "moving Z to: " << z << " mm " << z * mmToPulseZ << " pulse";
    #endif

    #ifdef DISABLE_MOTOR
    // Wait for few seconds
    QThread::msleep(SLEEP_MS);
    return;
    #endif

    // Z
    dmc_set_profile(cardNo, 2, speed, speed, motorsetting[2].accTime, motorsetting[2].decTime, motorsetting[2].stopVelcoty);
    dmc_set_s_profile(cardNo, 2, 0, 0);
    dmc_pmove(cardNo, 2, long(z * mmToPulseZ), 1);

    while (dmc_check_done(cardNo, 2) == 0)
    {
        long zCurrent = dmc_get_encoder(cardNo, 2);
        QCoreApplication::processEvents();
        QThread::msleep(MOTOR_WAIT);
        // Check motor moved or not
        long zNew = dmc_get_encoder(cardNo, 2);
        if(zCurrent == zNew && dmc_read_current_speed(cardNo, 2) > 0)
        {
            #ifdef DEBUG_MODE
            qDebug() << DMC_SYSTEM_NAME << "Motor 2 failed";
            #endif
            emit motorFailed(2);
            break;
        }
    }

    // Up check
    if(z >= 0 && z <= 10)
    {
        DWORD in1 = dmc_read_inport(cardNo, 1);
        QByteArray data2;
        QDataStream stream2(&data2, QIODevice::ReadWrite);
        stream2 << 32 << quint32(in1);
        QString in1Str = QString::number(data2.toHex().toLongLong(nullptr, 16),2);
        // not in org2
        if(in1Str[35] == 1)
        {
            emit motorFailed(2);
        }
    }
}

void DMC::autoVibration(bool on)
{    
    if(on)
    {
        #ifdef DEBUG_MODE
        qDebug() << DMC_SYSTEM_NAME << " Vibrating";
        #endif
        // Start vibrate
        for(WORD i = 1; i < 16; i++)
        {
            vibrationStatus[i - 1] = true;
            setting.setValue("DMC/vibration" + QString::number(i - 1), true);
            nmc_write_outbit(cardNo, 2, i, 0);
            emit vibrationStatusChanged(int(i), true);
        }
    }
    else
    {
        #ifdef DEBUG_MODE
        qDebug() << DMC_SYSTEM_NAME << " Stop vibrate";
        #endif
        // Stop vibrate
        for(WORD i = 1; i < 16; i++)
        {
            vibrationStatus[i - 1] = false;
            setting.setValue("DMC/vibration" + QString::number(i - 1), false);
            nmc_write_outbit(cardNo, 2, i, 1);
            emit vibrationStatusChanged(int(i), false);
        }
    }
}

void DMC::manualMove(WORD axis, long position)
{
    #ifdef DEBUG_MODE
    qDebug() << DMC_SYSTEM_NAME << "moving to: " << position << " mm ";
    #endif
    #ifdef DISABLE_MOTOR
    // Wait for few seconds
    QThread::msleep(SLEEP_MS);
    return;
    #endif

    if(axis == 0)
    {
        dmc_set_profile(cardNo, axis, motorsetting[0].minVelcoty, motorsetting[0].maxVelcoty, motorsetting[0].accTime, motorsetting[0].decTime, motorsetting[0].stopVelcoty);
        dmc_set_s_profile(cardNo, axis, 0, 0);
        dmc_pmove(cardNo, axis, position * mmToPulseX, 1);
    }
    else if(axis == 1)
    {
        dmc_set_profile(cardNo, axis, motorsetting[1].minVelcoty, motorsetting[1].maxVelcoty, motorsetting[1].accTime, motorsetting[1].decTime, motorsetting[1].stopVelcoty);
        dmc_set_s_profile(cardNo, axis, 0, 0);
        dmc_pmove(cardNo, axis, position * mmToPulseY, 1);
    }
    else
    {
        dmc_set_profile(cardNo, axis, motorsetting[2].minVelcoty, motorsetting[2].maxVelcoty, motorsetting[2].accTime, motorsetting[2].decTime, motorsetting[2].stopVelcoty);
        dmc_set_s_profile(cardNo, axis, 0, 0);
        dmc_pmove(cardNo, axis, position * mmToPulseZ, 1);
    }

    while (dmc_check_done(cardNo, axis) == 0)
    {
    }

    #ifdef DEBUG_MODE
    qDebug() << DMC_SYSTEM_NAME << " motor stopped.";
    #endif
}

void DMC::manualVibration(WORD node, bool on)
{
    #ifdef DEBUG_MODE
    if(on)
    {
        qDebug() << DMC_SYSTEM_NAME << int(node) << " vibrating";
    }
    else
    {
        qDebug() << DMC_SYSTEM_NAME << int(node) << " stop vibrating";
    }
    #endif

    switch (node)
    {
        case 1:
            if(on)
                nmc_write_outbit(cardNo, 2, 13, 0);
            else
                nmc_write_outbit(cardNo, 2, 13, 1);
            break;
        case 2:
            if(on)
                nmc_write_outbit(cardNo, 2, 10, 0);
            else
                nmc_write_outbit(cardNo, 2, 10, 1);
            break;
        case 3:
            if(on)
                nmc_write_outbit(cardNo, 2, 9, 0);
            else
                nmc_write_outbit(cardNo, 2, 9, 1);
            break;
        case 4:
            if(on)
                nmc_write_outbit(cardNo, 2, 4, 0);
            else
                nmc_write_outbit(cardNo, 2, 4, 1);
            break;
        case 5:
            if(on)
                nmc_write_outbit(cardNo, 2, 3, 0);
            else
                nmc_write_outbit(cardNo, 2, 3, 1);
            break;
        case 6:
            if(on)
                nmc_write_outbit(cardNo, 2, 14, 0);
            else
                nmc_write_outbit(cardNo, 2, 14, 1);
            break;
        case 7:
            if(on)
                nmc_write_outbit(cardNo, 2, 11, 0);
            else
                nmc_write_outbit(cardNo, 2, 11, 1);
            break;
        case 8:
            if(on)
                nmc_write_outbit(cardNo, 2, 8, 0);
            else
                nmc_write_outbit(cardNo, 2, 8, 1);
            break;
        case 9:
            if(on)
                nmc_write_outbit(cardNo, 2, 5, 0);
            else
                nmc_write_outbit(cardNo, 2, 5, 1);
            break;
        case 10:
            if(on)
                nmc_write_outbit(cardNo, 2, 2, 0);
            else
                nmc_write_outbit(cardNo, 2, 2, 1);
            break;
        case 11:
            if(on)
                nmc_write_outbit(cardNo, 2, 15, 0);
            else
                nmc_write_outbit(cardNo, 2, 15, 1);
            break;
        case 12:
            if(on)
                nmc_write_outbit(cardNo, 2, 12, 0);
            else
                nmc_write_outbit(cardNo, 2, 12, 1);
            break;
        case 13:
            if(on)
                nmc_write_outbit(cardNo, 2, 7, 0);
            else
                nmc_write_outbit(cardNo, 2, 7, 1);
            break;
        case 14:
            if(on)
                nmc_write_outbit(cardNo, 2, 6, 0);
            else
                nmc_write_outbit(cardNo, 2, 6, 1);
            break;
        case 15:
            if(on)
                nmc_write_outbit(cardNo, 2, 1, 0);
            else
                nmc_write_outbit(cardNo, 2, 1, 1);
            break;
    }
    vibrationStatus[node - 1] = on;
    setting.setValue("DMC/vibration" + QString::number(node - 1), on);
    emit vibrationStatusChanged(int(node), on);
}

void DMC::light(int color)
{
    switch (color)
    {
        case 0:
        // Red
            #ifdef DEBUG_MODE
            qDebug() << DMC_SYSTEM_NAME << " light switched to red.";
            #endif
            dmc_write_outbit(cardNo, 1 , 0);
            dmc_write_outbit(cardNo, 2 , 1);
            dmc_write_outbit(cardNo, 3 , 1);
            break;
        case 1:
        // Yellow
            #ifdef DEBUG_MODE
            qDebug() << DMC_SYSTEM_NAME << " light switched to yellow.";
            #endif
            dmc_write_outbit(cardNo, 1 , 1);
            dmc_write_outbit(cardNo, 2 , 1);
            dmc_write_outbit(cardNo, 3 , 0);
            break;
        case 2:
        // Green
            #ifdef DEBUG_MODE
            qDebug() << DMC_SYSTEM_NAME << " light switched to green.";
            #endif
            dmc_write_outbit(cardNo, 1 , 1);
            dmc_write_outbit(cardNo, 2 , 0);
            dmc_write_outbit(cardNo, 3 , 1);
            break;
    }
}

void DMC::emergencyStop()
{
    dmc_emg_stop(cardNo);
    dmc_stop(cardNo, 0, 1);
    dmc_stop(cardNo, 1, 1);
    dmc_stop(cardNo, 2, 1);
}

void DMC::resetMotor()
{
    qDebug() << "Reset";
    #ifdef DISABLE_MOTOR
    // Wait for few seconds
    QThread::msleep(SLEEP_MS);
    qDebug() << "Reset end";
    emit motorResetFinished();
    return;
    #endif

    // Z axis
    dmc_set_profile(cardNo, 2, motorresetsetting[2].maxVelcoty, motorresetsetting[2].minVelcoty, motorresetsetting[2].accTime, motorresetsetting[2].decTime, motorresetsetting[2].stopVelcoty);
    dmc_set_s_profile(cardNo, 2, 0, 0);
    dmc_set_homemode(cardNo, 2, 0, 0, 2, 0);
    dmc_home_move(cardNo, 2);
    // Wait until finish
    while(dmc_check_done(cardNo, 2) == 0)
    {
    }
    dmc_set_position(cardNo, 2, 0);

    // X, Y axis
    dmc_set_profile(cardNo, 0, motorresetsetting[0].maxVelcoty, motorresetsetting[0].minVelcoty, motorresetsetting[0].accTime, motorresetsetting[0].decTime, motorresetsetting[0].stopVelcoty);
    dmc_set_s_profile(cardNo, 0, 0, 0);
    dmc_set_homemode(cardNo, 0, 0, 0, 2, 0);
    dmc_home_move(cardNo, 0);
    dmc_set_profile(cardNo, 1, motorresetsetting[1].maxVelcoty, motorresetsetting[1].minVelcoty, motorresetsetting[1].accTime, motorresetsetting[1].decTime, motorresetsetting[1].stopVelcoty);
    dmc_set_s_profile(cardNo, 1, 0, 0);
    dmc_set_homemode(cardNo, 1, 0, 0, 2, 0);
    dmc_home_move(cardNo, 1);
    while (dmc_check_done(cardNo, 0) == 0 || dmc_check_done(cardNo, 1) == 0) {
    }
    dmc_set_position(cardNo, 0, 0);
    dmc_set_position(cardNo, 1, 0);

    // Encoder
    dmc_set_encoder(cardNo, 0, 0);
    dmc_set_encoder(cardNo, 1, 0);
    dmc_set_encoder(cardNo, 2, 0);

    emit motorResetFinished();
}

void DMC::regularJobTimeOut()
{
    // AD input
    double voltage0 = 0;
    double voltage1 = 0;
    double voltage2 = 0;
    dmc_get_ad_input(cardNo, 0, &voltage0);
    dmc_get_ad_input(cardNo, 1, &voltage1);
    dmc_get_ad_input(cardNo, 2, &voltage2);
    emit regularJobFinished(voltage0, voltage1, voltage2);

    // Digital input for motor
    DWORD in0 = dmc_read_inport(cardNo, 0);
    DWORD in1 = dmc_read_inport(cardNo, 1);

    QByteArray data;
    QDataStream stream(&data, QIODevice::ReadWrite);
    stream << 32 << quint32(in0);

    QByteArray data2;
    QDataStream stream2(&data2, QIODevice::ReadWrite);
    stream2 << 32 << quint32(in1);

    QString in0Str = QString::number(data.toHex().toLongLong(nullptr, 16),2);
    QString in1Str = QString::number(data2.toHex().toLongLong(nullptr, 16),2);

    #ifdef DEBUG_MODE
    //qDebug() << "0: " << QString::number(data.toHex().toLongLong(nullptr, 16),2);
    //qDebug() << "1: " << QString::number(data2.toHex().toLongLong(nullptr, 16),2);
    #endif

    /*
    // Send fail
    if(qtBitIn0.at(8) || qtBitIn0.at(0))
        emit structureAlarm(0);
    if(qtBitIn0.at(9) || qtBitIn0.at(1))
        emit structureAlarm(1);
    if(qtBitIn0.at(10) || qtBitIn0.at(2))
        emit structureAlarm(2);
    */

    emit inputStatus(in0Str, in1Str);
}

void DMC::checkArmIO()
{
    qDebug() << DMC_SYSTEM_NAME << " checking IO in arm.";
    #ifdef DISABLE_IO_CHECK
    return;
    #endif

    // Open limit
    openLimit.clear();
    for(WORD i = 1; i < 16; i++)
    {
        WORD sensorValue = 0;
        nmc_read_inbit(cardNo, 1, i, &sensorValue);
        // Closed, High, limit
        if(sensorValue)
            openLimit.append(false);
        // Open, Low, not limit
        else
            openLimit.append(true);
    }
    // Box sensor
    boxSensor.clear();
    for(WORD i = 1; i < 16; i++)
    {
        WORD sensorValue = 0;
        nmc_read_inbit(cardNo,2, i, &sensorValue);
        // Closed, High, have box
        if(sensorValue)
            boxSensor.append(false);
        // Open, Low, no box
        else
            boxSensor.append(true);
    }

    // Check
    for(int i = 0; i < 15; i++)
    {
        // Open
        if(gripperStatus.at(i))
        {
            // Open limit failed
            if(!openLimit.at(i))
            {
                #ifdef DEBUG_MODE
                qDebug() << DMC_SYSTEM_NAME << " Open limit failed : " << i+1;
                #endif
                emit checkArmFailed(1, i+1);
            }
        }
        // Close
        else
        {
            // No box
            if(boxSensor.at(i))
            {
                #ifdef DEBUG_MODE
                qDebug() << DMC_SYSTEM_NAME << " Box sensor failed : " << i+1;
                #endif
                emit checkArmFailed(2, i+1);
            }
        }
    }
    emit checkArmFinished();
}

void DMC::updateSetting()
{
    setting.beginGroup("DMC");
    motorsetting[0].minVelcoty = setting.value("motorXMinVelcoty").toInt();
    motorsetting[0].maxVelcoty = setting.value("motorXMaxVelcoty").toInt();
    motorsetting[1].minVelcoty = setting.value("motorYMinVelcoty").toInt();
    motorsetting[1].maxVelcoty = setting.value("motorYMaxVelcoty").toInt();
    motorsetting[2].minVelcoty = setting.value("motorZMinVelcoty").toInt();
    motorsetting[2].maxVelcoty = setting.value("motorZMaxVelcoty").toInt();

    motorresetsetting[0].minVelcoty = setting.value("motorResetXMinVelcoty").toInt();
    motorresetsetting[0].maxVelcoty = setting.value("motorResetXMaxVelcoty").toInt();
    motorresetsetting[1].minVelcoty = setting.value("motorResetYMinVelcoty").toInt();
    motorresetsetting[1].maxVelcoty = setting.value("motorResetYMaxVelcoty").toInt();
    motorresetsetting[2].minVelcoty = setting.value("motorResetZMinVelcoty").toInt();
    motorresetsetting[2].maxVelcoty = setting.value("motorResetZMaxVelcoty").toInt();

    for(int i = 0; i < BOX_TYPE; i++)
    {
        autosetting[i].boxSupplyX = setting.value("box" + QString::number(i+1) + "SupplyX").toDouble();
        autosetting[i].boxSupplyY = setting.value("box" + QString::number(i+1) + "SupplyY").toDouble();
        autosetting[i].boxSupplyZ = setting.value("box" + QString::number(i+1) + "SupplyZ").toDouble();
        autosetting[i].boxWaitX = setting.value("box" + QString::number(i+1) + "WaitX").toDouble();
        autosetting[i].boxWaitY = setting.value("box" + QString::number(i+1) + "WaitY").toDouble();
        autosetting[i].boxGrabSpeed = setting.value("box" + QString::number(i+1) + "GrabSpeed").toDouble();
        autosetting[i].boxEmbedHeight = setting.value("box" + QString::number(i+1) + "EmbedHeight").toDouble();
        autosetting[i].boxEmbedSpeed = setting.value("box" + QString::number(i+1) + "EmbedSpeed").toDouble();
        //qDebug() << autosetting[i].boxSupplyX << autosetting[i].boxSupplyY << autosetting[i].boxSupplyZ <<autosetting[i].boxWaitX << autosetting[i].boxWaitY << autosetting[i].boxGrabSpeed << autosetting[i].boxEmbedHeight << autosetting[i].boxEmbedSpeed;
    }
    setting.endGroup();
}
