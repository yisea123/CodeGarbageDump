#ifndef JOBITEM_H
#define JOBITEM_H

#include <QObject>
#include <QList>
#include <QPointF>
#include <QDateTime>

class JobItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id MEMBER mID NOTIFY itemChanged)
    Q_PROPERTY(QString file MEMBER mFile NOTIFY itemChanged)
    Q_PROPERTY(int step MEMBER mStep NOTIFY itemChanged)
    Q_PROPERTY(QDateTime createTime MEMBER mCreateTime NOTIFY itemChanged)
    Q_PROPERTY(QString boxType MEMBER mBoxType NOTIFY itemChanged)
    Q_PROPERTY(double panelHeight MEMBER mPanelHeight NOTIFY itemChanged)

private:
    QString mID;
    QString mFile;
    int mStep;
    QList<QPointF> mArmCoordinate;
    QList<QList<int>> mBoxArrayList;
    double mPanelHeight;
    QString mBoxType;
    QDateTime mCreateTime;

public:
    JobItem(QString id, QString file, QList<QPointF> armCoordinate, QList<QList<int>> boxArrayList, double panelHeight, QString boxType){mID = id; mFile = file ; mStep = armCoordinate.size();mArmCoordinate = armCoordinate;mBoxArrayList = boxArrayList; mPanelHeight = panelHeight; mBoxType = boxType; mCreateTime = QDateTime::currentDateTime();emit itemChanged();}
    JobItem(QString id, QString file, QList<QPointF> armCoordinate, QList<QList<int>> boxArrayList, double panelHeight, QString boxType, qint64 createTime){mID = id; mFile = file ; mStep = armCoordinate.size();mArmCoordinate = armCoordinate;mBoxArrayList = boxArrayList; mPanelHeight = panelHeight; mBoxType = boxType; mCreateTime = QDateTime::fromSecsSinceEpoch(createTime);emit itemChanged();}
    QString id(){return mID;}
    QString file(){return mFile;}
    int step(){return mStep;}
    double panelHeight() {return mPanelHeight;}
    QString boxType(){return mBoxType;}
    QDateTime createTime(){return mCreateTime;}
    QList<QPointF> armCoordinate(){return mArmCoordinate;}
    QList<QList<int>> boxArrayList(){return mBoxArrayList;}

signals:
    void itemChanged();
};

#endif // TASKITEM_H
