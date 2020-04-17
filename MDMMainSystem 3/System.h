#ifndef SYSTEM_H
#define SYSTEM_H

#include <QDir>
#include <QDebug>
#include <QObject>
#include <QPointF>
#include <QList>
#include <QThread>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTranslator>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSettings>
#include "JobItem.h"
#include "DMC.h"
#include "Sql.h"

#define SYSTEM_NAME "Robot Arm Main Loop: "

class System : public QObject
{
    Q_OBJECT
    // Auto mode
    Q_PROPERTY(QList<QObject*> jobList MEMBER jobList NOTIFY jobListChanged)
    Q_PROPERTY(int jobCount MEMBER jobCount NOTIFY fakeSignal)
    Q_PROPERTY(QString currentPath MEMBER currentPath NOTIFY fakeSignal)
    Q_PROPERTY(bool autoMode MEMBER autoMode NOTIFY autoModeChanged)
    // Current Job
    Q_PROPERTY(QString currentJobID MEMBER currentJobID NOTIFY currentJobChanged)
    Q_PROPERTY(QString currentJobFilePath MEMBER currentJobFilePath NOTIFY currentJobChanged)
    Q_PROPERTY(int currentJobProcedureSize MEMBER currentJobProcedureSize NOTIFY currentJobChanged)
    Q_PROPERTY(int currentJobProcedure MEMBER currentJobProcedure NOTIFY dmcProcedureFinished)
    Q_PROPERTY(QString systemStatus MEMBER systemStatus NOTIFY systemStatusChanged)
    Q_PROPERTY(QString CurrentBoxType MEMBER CurrentBoxType NOTIFY currentJobChanged)
    Q_PROPERTY(double CurrentPanelHeight MEMBER CurrentPanelHeight NOTIFY currentJobChanged)

    // Status
    Q_PROPERTY(QList<bool> gripperStatus READ getGripperStatus NOTIFY gripperStatusChanged)
    Q_PROPERTY(QList<bool> vibrationStatus READ getVibrationStatus NOTIFY vibrationStatusChanged)
    // Laser Finder
    Q_PROPERTY(QList<double> laserFinder MEMBER laserFinder NOTIFY regularJobFinished)
    // emergencyMode
    Q_PROPERTY(bool emergencyMode MEMBER emergencyMode NOTIFY emergencyModeChanged)

    enum UdpSend{
        BoxRequest,
        PauseSupplier,
        ResumeSupplier,
        EmergencyStop
    };

private:
    static System *instance;
    explicit System(QObject *parent = nullptr);

    /*
     *  Job
     */
    long jobCount = 0;
    QList<QObject*> jobList;
    // Current job
    QString currentJobID = "";
    QString currentJobFilePath = "";
    int currentJobProcedureSize = 0;
    int currentJobProcedure = 0;
    QVector<QVector<QString>> currentJobTable;
    int currentOpeartion = 0;
    QString CurrentBoxType = "";
    double CurrentPanelHeight = 0;
    QString systemStatus = "";
    QList<QPointF> currentArmCoordinate;
    QList<QList<int>> currentBoxArrayList;
    // Image
    QString currentPath;

    // Status
    bool autoMode = false;
    bool autoRunning = false;
    bool autoBlocking = false;
    bool supplierAuto = false;
    bool supplierOperating = false;
    bool emergencyMode = false;

    // SQL
    Sql *sql;

    // DMC
    QThread dmcThread;
    DMC *dmc;

    // Boxes supplier
    QUdpSocket *boxesSupplier;

    // Laser finder
    QList<double> laserFinder;
    // even = voltage
    // odd = distance

    // Setting
    QSettings settings;

    // i18n
    QGuiApplication *m_app;
    QQmlApplicationEngine *m_engine;

public:
    static System *getInstance();

public slots:
    // System start
    void start();

    // Auto Mode
    void runAuto();
    void pauseAuto();
    void newJob(QString file, QList<QPointF> armCoordinate, QList<QList<int>> boxArrayList, double panelHeight, QString boxType);
    void autoJobStart();
    void autoJobsFinished();
    void autoOperationFinished(int operation);
    void dmcOperationStart();
    void emergencyStop();
    void emergencyStopOff();

    // Motor
    void resetMotor();
    void receiveMotorResetFinished();

    // Manual Mode
    void manualGripper(int node, bool open, bool skip = false);
    void manualVibration(int node, bool open);
    void manualMove(int axis, QString distance);

    // Status
    QList<bool> getGripperStatus(){return dmc->getGripperStatus();}
    QList<bool> getVibrationStatus(){return dmc->getVibrationStatus();}

    // Status changed signal
    void receiveGripperStatusChanged(int node, bool open);
    void receiveVibrationStatusChanged(int node, bool on);

    // Regular job signal
    void receiveRegularJobFinished(double laser0, double laser1, double laser2);
    void receiveCheckArmFailed(int event, int node);
    void receiveStructureAlarm(int node);

    // Receive motor fail
    void receiveMotorFail(int node);

    // Boxes supplier
    void boxesSupplierRequest(QString requestBoxes);
    void boxesSupplierSend(UdpSend command, QString requestBoxes = "");
    void boxSuplierReceive();

    // Log
    void newLog(int event);

    // Setting
    void updateSetting();
    void resetApplication();

    // Model
    QVector<QVector<QString>> getCurrentJobTable(){return currentJobTable;}

signals:
    /*
     * QML
     */
    // Auto mode
    void jobListChanged();
    void currentJobChanged();
    void fakeSignal();

    // Status
    void gripperStatusChanged();
    void vibrationStatusChanged();
    void autoModeChanged();

    // Regular job
    void regularJobFinished();
    void dmcProcedureFinished();

    // DMC
    void systemStatusChanged();
    void dmcMotorResetFinished();

    // Setting
    void settingChanged();
    void emergencyModeChanged();

    /*
     * DMC Thread
     */
    void dmcPerformOperation(int operation, QVariantList *parameter = nullptr);
    void dmcManualGripper(WORD node, bool open, bool skip);
    void dmcManualVibration(WORD node, bool on);
    void dmcManualMove(WORD axis, long distance);
    void dmcSwitchLight(int color);
    void dmcUpdateAutoSetting();
    void dmcResetMotor();
    void dmcEmergencyStop();
};

#endif // SYSTEM_H
