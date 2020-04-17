#ifndef DMCIOMODEL_H
#define DMCIOMODEL_H


#include <QObject>
#include <QAbstractTableModel>

#include "DMC.h"
#include "Sql.h"

class DMCIOModel : public QAbstractTableModel
{
    Q_OBJECT
    enum TableRoles{
        TableDataRole = Qt::UserRole + 1,
        HeadingRole,
        TrueRole,
        NARole
    };

private:
    QVector<QVector<QString>> tableData;
    QVector<QVector<QString>> heading = {{"", "ORG", "EL+", "EL-", tr("Safe Area")},
                                         {"X", "", "", "", ""},
                                         {"Y", "", "", "", ""},
                                         {"Z", "", "", "", ""}};

    DMC *dmc;
    QString bitToStr(bool bit);

public:
    explicit DMCIOModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void updateModel(QString in0, QString in1);
};


#endif // DMCIOMODEL_H
