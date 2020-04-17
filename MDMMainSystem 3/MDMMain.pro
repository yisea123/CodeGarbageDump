QT += quick quickcontrols2 widgets sql charts
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000 # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
DMC.cpp \
DxfImport.cpp \
DxfLib.cpp \
    QMLModels/ArmIOModel.cpp \
    QMLModels/CurrentJobModel.cpp \
    QMLModels/DMCIOModel.cpp \
    QMLModels/DxfImportModel.cpp \
    QMLModels/LogModel.cpp \
    Sql.cpp \
System.cpp \
lib/dxf/dl_dxf.cpp \
lib/dxf/dl_writer_ascii.cpp \
    main.cpp

TRANSLATIONS    = main_zh_hk.ts

RESOURCES += qml.qrc \
images.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
.gitignore \
    main_zh_hk.ts

HEADERS += \
DMC.h \
DxfImport.h \
DxfLib.h \
JobItem.h \
    QMLModels/ArmIOModel.h \
    QMLModels/CurrentJobModel.h \
    QMLModels/DMCIOModel.h \
    QMLModels/DxfImportModel.h \
    QMLModels/LogModel.h \
    Sql.h \
System.h \
lib/dxf/dl_attributes.h \
lib/dxf/dl_codes.h \
lib/dxf/dl_creationadapter.h \
lib/dxf/dl_creationinterface.h \
lib/dxf/dl_dxf.h \
lib/dxf/dl_entities.h \
lib/dxf/dl_exception.h \
lib/dxf/dl_extrusion.h \
lib/dxf/dl_global.h \
lib/dxf/dl_writer.h \
lib/dxf/dl_writer_ascii.h

win32: LIBS += -L$$PWD/lib/dmc/ -lLTDMC

INCLUDEPATH += $$PWD/lib/dmc
DEPENDPATH += $$PWD/lib/dmc
