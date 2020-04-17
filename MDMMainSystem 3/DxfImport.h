#ifndef DXFIMPORT_H
#define DXFIMPORT_H

#include <QObject>
#include <QUrl>
#include <QDebug>
#include <QGraphicsScene>
#include <QPainter>
#include <QImage>

#include "DxfLib.h"
#include "System.h"

class DxfImport : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF dxfOrigin MEMBER dxfOrigin)
    Q_PROPERTY(QVariantList dxfBox MEMBER dxfBox)

private:
    static DxfImport *instance;
    DxfImport(QObject *parent = nullptr);
    System *system;

    // Variables for QML
    QPointF dxfOrigin;
    QVariantList dxfBox;

    // Variables for table
    QList<int> validCycles;
    QList<QPointF> ArmCoordinate;
    QList<QList<int>> mBoxArrayList; // Number follow DXF lib
    QList<QList<int>> standardBoxArrayList; // Number follow standard

    // Variables for new Job
    QString boxType;
    double panelHeight;
    QList<QPointF> finalArmCoordinate;
    QString dxfFile;
    //QList<QList<int>> finalBoxArrayList;

    // TableView
    QVector<QVector<QString>> table;

public:
    static DxfImport *getInstance();

public slots:
    int nameCheck(QUrl file);
    void import(QUrl file);
    void newJob();
    void clear();
    QVector<QVector<QString>> getTable(){return table;}

signals:
    void importFinished();
};

#endif // DXFIMPORT_H
