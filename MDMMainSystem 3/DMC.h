#ifndef DMC_H
#define DMC_H

// Debugging purpose, comment code below to disable
//#define DISABLE_MOTOR
#define DISABLE_IO_CHECK
#define DEBUG_MODE

// Pre-define
#define DMC_SYSTEM_NAME "Robot Arm DMC: "
#define SLEEP_MS 500
#define MOTOR_WAIT 200
#define DMC_MAX_OPERATION 14
#define BOX_TYPE 4
#define DOF_COUNT 3
#define GRIPPER_COUNT 15

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QPointF>
#include <QList>
#include <QTimer>
#include <QBitArray>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QCoreApplication>
#include <QSettings>
#include "lib/dmc/LTDMC.h"

struct AutoSetting {
    long boxSupplyX = 0;
    long boxSupplyY = 0;
    long boxSupplyZ = 0;
    long boxWaitX = 0;
    long boxWaitY = 0;
    long boxGrabSpeed = 0;
    long boxEmbedHeight = 0;
    long boxEmbedSpeed = 0;
};

struct MotorSetting {
    WORD minVelcoty = 10000;
    WORD maxVelcoty = 10000;
    double stopVelcoty = 500;
    double accTime = 0.5;
    double decTime = 0.5;
};

class DMC : public QObject
{
    Q_OBJECT

private:
    static DMC *instance;

    // DMC status
    QList<bool> boxSensor;
    QList<bool> openLimit;
    QList<bool> gripperStatus;
    QList<bool> vibrationStatus;
    QList<struct MotorSetting> motorsetting;
    QList<struct MotorSetting> motorresetsetting;
    QList<struct AutoSetting> autosetting;
    int currentBox = 0;

    // Current card
    WORD cardNo = 0;
    // Moving
    double mmToPulseX = 107.238605898;
    double mmToPulseY = 222.222222222;
    double mmToPulseZ = 93.4579439252;

    // Panel offset
    long panelOffsetX = 0;
    long panelOffsetY = 0;

    // Debug
    unsigned long ioWaitMs = 1000;

    // Regular job
    QTimer *regularJob;
    int regularJobMs = 500;

    // IO
    int checkArmIOMs = 1500;

    QSettings setting;
    explicit DMC(QObject *parent = nullptr);

public:
    static DMC *getInstance();

public slots:
    // job
    void autoOperation(int operation, QVariantList *parameter = nullptr);
    // DMC opeartion
    // Auto
    void gripper(WORD node, bool open, bool skipCheck = false);
    void moveXY(long x, long y, bool offset = false);
    void moveZ(long z, long speed);
    void autoVibration(bool on);
    // Manual
    void manualMove(WORD axis, long position);
    void manualVibration(WORD node, bool on);
    // Light Control
    void light(int color);
    // emergency Stop
    void emergencyStop();

    // Status
    QList<bool> getGripperStatus(){return gripperStatus;}
    QList<bool> getVibrationStatus(){return vibrationStatus;}
    QList<bool> getBoxSensor(){return boxSensor;}
    QList<bool> getOpenLimit(){return openLimit;}

    // Reset
    void resetMotor();

    // Regular job
    void regularJobTimeOut();

    // IO
    void checkArmIO();

    void updateSetting();

signals:
    // Auto mode
    void opeartionFinished(int step);

    // DMC Status
    void gripperStatusChanged(int node, bool open);
    void vibrationStatusChanged(int node, bool on);
    void checkArmFinished();
    void checkArmFailed(int event, int node);
    void autoModeChanged(bool autoMode);

    // Regular job
    void regularJobFinished(double laser0, double laser1, double laser2);
    void inputStatus(QString in0, QString in1);
    void structureAlarm(int node);

    // Motor reset
    void motorResetFinished();

    // Fail
    void motorFailed(int axis);
};

#endif // DMC_H
