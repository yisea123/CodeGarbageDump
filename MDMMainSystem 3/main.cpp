#include <QApplication>
#include <QSplashScreen>
#include <QPropertyAnimation>
#include <QScreen>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include <QTranslator>


#include "System.h"
#include "DxfImport.h"
#include "QMLModels/DxfImportModel.h"
#include "QMLModels/CurrentJobModel.h"
#include "QMLModels/ArmIOModel.h"
#include "QMLModels/LogModel.h"
#include "QMLModels/DMCIOModel.h"

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    /*
     * Setting
     */
    QCoreApplication::setOrganizationName("Karmo Apollo");
    QCoreApplication::setOrganizationDomain("karmo.com.hk");
    QCoreApplication::setApplicationName("MDMMain");

    QSettings settings;

    settings.beginGroup("System");
    if(settings.allKeys().size() == 0)
    {
        settings.setValue("currentJobProcedure", 0);
        settings.setValue("currentOpeartion", 0);
    }
    settings.endGroup();
    settings.beginGroup("DMC");
    if(settings.allKeys().size() == 0)
    {
        // Box Supplier Location
        settings.setValue("box1SupplyX", 2759);
        settings.setValue("box1SupplyY", 1500);
        settings.setValue("box1SupplyZ", 310);
        settings.setValue("box2SupplyX", 1);
        settings.setValue("box2SupplyY", 2);
        settings.setValue("box2SupplyZ", 3);
        settings.setValue("box3SupplyX", 4);
        settings.setValue("box3SupplyY", 5);
        settings.setValue("box3SupplyZ", 6);
        settings.setValue("box4SupplyX", 7);
        settings.setValue("box4SupplyY", 8);
        settings.setValue("box4SupplyZ", 9);
        // Waiting Location
        settings.setValue("box1WaitX", 2759);
        settings.setValue("box1WaitY", 1000);
        settings.setValue("box2WaitX", 10);
        settings.setValue("box2WaitY", 11);
        settings.setValue("box3WaitX", 12);
        settings.setValue("box3WaitY", 13);
        settings.setValue("box4WaitX", 14);
        settings.setValue("box4WaitY", 15);
        // Grab box
        settings.setValue("box1GrabSpeed", 5000);
        settings.setValue("box2GrabSpeed", 16);
        settings.setValue("box3GrabSpeed", 17);
        settings.setValue("box4GrabSpeed", 18);
        // Embed
        settings.setValue("box1EmbedHeight", 400);
        settings.setValue("box1EmbedSpeed", 5000);
        settings.setValue("box2EmbedHeight", 19);
        settings.setValue("box2EmbedSpeed", 20);
        settings.setValue("box3EmbedHeight", 21);
        settings.setValue("box3EmbedSpeed", 22);
        settings.setValue("box4EmbedHeight", 23);
        settings.setValue("box4EmbedSpeed", 24);

        //Motor
        settings.setValue("motorXMinVelcoty", 10000);
        settings.setValue("motorXMaxVelcoty", 10000);
        settings.setValue("motorYMinVelcoty", 10000);
        settings.setValue("motorYMaxVelcoty", 10000);
        settings.setValue("motorZMinVelcoty", 10000);
        settings.setValue("motorZMaxVelcoty", 10000);
        // Motor reset
        settings.setValue("motorResetXMinVelcoty", 10000);
        settings.setValue("motorResetXMaxVelcoty", 10000);
        settings.setValue("motorResetYMinVelcoty", 10000);
        settings.setValue("motorResetYMaxVelcoty", 10000);
        settings.setValue("motorResetZMinVelcoty", 10000);
        settings.setValue("motorResetZMaxVelcoty", 10000);

        // Arm
        for (int i = 0; i < GRIPPER_COUNT; ++i)
        {
            settings.setValue("gripper" + QString::number(i), false);
            settings.setValue("vibration" + QString::number(i), false);
        }
        settings.endGroup();

    }
    /*
     * QML
     */
    QQuickStyle::setStyle("Material");
    qmlRegisterType<DxfImportModel>("DxfImportModel", 0, 1, "DxfImportModel");
    qmlRegisterType<CurrentJobModel>("CurrentJobModel", 0, 1, "CurrentJobModel");
    qmlRegisterType<ArmIOModel>("ArmIOModel", 0, 1, "ArmIOModel");
    qmlRegisterType<DMCIOModel>("DMCIOModel", 0, 1, "DMCIOModel");
    qmlRegisterType<LogModel>("LogModel", 0, 1, "LogModel");

    QQmlApplicationEngine engine;
    DxfImport *dxfImport = DxfImport::getInstance();
    System *system = System::getInstance();
    QQmlContext* context = engine.rootContext();
    context->setContextProperty("dxfImport", dxfImport);
    context->setContextProperty("system", system);

    const QUrl url(QStringLiteral("qrc:/QML/startApp.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QTranslator translator;
    translator.load(":/main_zh_hk.qm");
    app.installTranslator(&translator);
    engine.retranslate();

    return app.exec();
}
