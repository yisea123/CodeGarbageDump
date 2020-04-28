QT += quick
QT += sql network
QT += quickcontrols2

CONFIG += c++11


# AIC Support
DEFINES += PROTOBUF_USE_DLLS
LIBS += -lprotobuf
LIBS += -L/usr/lib/ -lprotobuf
LIBS += -laic_wms_sdk
LIBS += -laic_commu

unix:!macx:!android{
    LIBS += -L/usr/lib/ -lprotobuf
    LIBS += -laic_wms_sdk
    LIBS += -laic_commu
    LIBS += -lpthread
}

#For Windows
win32 {
    CONFIG(debug, release|debug) {
        win32:LIBS += -llibprotobuf-debug
    } else {
        win32:LIBS += -llibprotobuf
    }
    LIBS += -L/usr/lib/ -lprotobuf
    LIBS += -laic_wms_sdk
    LIBS += -laic_commu
}

INCLUDEPATH += $$PWD/include
INCLUDEPATH     +=  .\include_protobuf \
                    .\include_protobuf\google\protobuf\io \
                    .\include_protobuf\google\protobuf\util \
                    .\include_protobuf\google\protobuf\util\internal \

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        QMLModels/LogModel.cpp \
        QMLModels/OrderModel.cpp \
        controlunit.cpp \
        main.cpp \
        mytcpsocket.cpp \
        orderitem.cpp \
        sql.cpp \
        wms_sdk_demo/amrsystem.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    QMLModels/LogModel.h \
    QMLModels/OrderModel.h \
    controlunit.h \
    mytcpsocket.h \
    orderitem.h \
    sql.h \
    wms_sdk_demo/amrsystem.h \
    wms_sdk_demo/wms_sdk/include/aic_commu.h \
    wms_sdk_demo/wms_sdk/include/aicwmssdk.h \
    wms_sdk_demo/wms_sdk/include/commu.pb.h \
    wms_sdk_demo/wms_sdk/include/wms_commu.pb.h \
    wms_sdk_demo/wms_sdk/include/wms_commu_def.h

DISTFILES += \
    wms_sdk_demo/libaic_commu.so \
    wms_sdk_demo/libaic_wms_sdk.so \
    wms_sdk_demo/libprotobuf.so \
    wms_sdk_demo/wms_sdk/commu.proto \
    wms_sdk_demo/wms_sdk/lib/libaic_commu.so \
    wms_sdk_demo/wms_sdk/lib/libaic_wms_sdk.so \
    wms_sdk_demo/wms_sdk/redirect_commu.proto \
    wms_sdk_demo/wms_sdk/wms_commu.proto
