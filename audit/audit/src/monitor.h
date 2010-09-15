#ifndef MONITOR_H
#define MONITOR_H

#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include "global.h"

class Monitor
{
public:
    enum MonitorAttr
    {
        TimeAttr,
        DateAttr,
        DevNameAttr,
        ChAttr,
        TagNameAttr,
        TypeEventAttr,
        DevNumAttr,
        TagIdAttr
    };

    Monitor();
    ~Monitor();

    void addTransToModel(QString dev_num, R245_TRANSACT * trans, const QString &tag_name, const QString &dev_name);
    QAbstractItemModel * getModel(bool proxy);
    QMap <int, QString> * getState();
private:
    QString month[12];
    QMap <int, QString> state;
    QStandardItemModel * monitor_model;
    QSortFilterProxyModel * monitor_model_proxy;

    void initMas();
};

#endif // MONITOR_H
