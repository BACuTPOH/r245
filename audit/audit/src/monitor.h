#ifndef MONITOR_H
#define MONITOR_H

#include <QStandardItemModel>
#include "global.h"

class Monitor
{
public:
    enum MonitorAttr
    {
        TimeAttr,
        DateAttr,
        IdDevAttr,
        ChAttr,
        IdTagAttr,
        TypeEventAttr
    };

    Monitor();
    ~Monitor();

    void addTransToModel(QString dev_num, R245_TRANSACT * trans, const QString &tag_name, const QString &dev_name);
    QStandardItemModel * getModel();
private:
    QString month[12];
    QMap <int, QString> state;
    QStandardItemModel * monitor_model;

    void initMas();
};

#endif // MONITOR_H
