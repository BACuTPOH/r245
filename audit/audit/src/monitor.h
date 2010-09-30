#ifndef MONITOR_H
#define MONITOR_H

#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include "global.h"
#include <QDateTime>
#include "monitor_filter.h"

class Monitor
{
public:
    enum MonitorAttr
    {
        TimeAttr,
        DateAttr,
        DevNameAttr,
        DevNumAttr,
        ChAttr,
        TagNameAttr,
        TagIdAttr,
        TypeEventAttr,
        TransCodeAttr
    };

    Monitor();
    ~Monitor();

    void addTransToModel(QString dev_num, R245_TRANSACT * trans, const QString &tag_name, const QString &dev_name);
    void setFilter(QString channel, QString device, QString tag, QDate daten, QDate datem, QTime timen, QTime timem);
    void onlyTagInf(bool only = true);

    QAbstractItemModel * getModel(bool proxy);
    QMap <int, QString> * getState();
    void clear();
    void update();

private:
    QMap <int, QString> state;
    QStandardItemModel * monitor_model;
    MonitorFilter * monitor_model_proxy;
    void initMas();
    void initHeader();
};

#endif // MONITOR_H
