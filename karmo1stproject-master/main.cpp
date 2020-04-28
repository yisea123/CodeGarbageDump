#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QDebug>
#include <QQmlContext>

#include "controlunit.h"
#include "QMLModels/LogModel.h"
#include "QMLModels/OrderModel.h"
//#include "orderitem.h"

int main(int argc, char *argv[])
{
    ControlUnit* CU = new ControlUnit();

    //qmlRegisterType<classitem>("namespace_name", 1, 0, "qmlName");
    //type names must begin with an uppercase letter
    qmlRegisterType<ControlUnit>("Karmo.Max.ControlUnit", 1, 0, "Controlunit");
    qmlRegisterUncreatableType<OrderItem>("Karmo.Max.OrderItem", 1, 0, "OrderItem","code");
    qmlRegisterType<LogModel>("Karmo.Max.LogModel", 0, 1, "LogModel");
    qmlRegisterType<OrderModel>("Karmo.Max.OrderModel", 0, 1, "OrderModel");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QQuickStyle::setStyle("Material");

    engine.rootContext()->setContextProperty("ControlUnit", CU);

    const QUrl url(QStringLiteral("qrc:/QML/startApp.qml"));
    //const QUrl url(QStringLiteral("qrc:/QML/MainWindow.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
