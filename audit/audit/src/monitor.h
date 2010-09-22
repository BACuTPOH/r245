#ifndef MONITOR_H
#define MONITOR_H

#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include "global.h"
#include <QDateTime>

class MyFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    MyFilter(QObject *parent = 0);

    QDate filterMinimumDate() const { return minDate; }
    void setFilterMinimumDate(const QDate date);

    QDate filterMaximumDate() const { return maxDate; }
    void setFilterMaximumDate(const QDate date);

    QTime filterMinimumTime() const { return minTime; }
    void setFilterMinimumTime(const QTime time);

    QTime filterMaximumTime() const { return maxTime; }
    void setFilterMaximumTime(const QTime time);

    void setRegExp(QRegExp channel, QRegExp device, QRegExp tag);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    //bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    bool dateInRange(const QDate &date) const;
    bool timeInRange(const QTime &time) const;

    QDate minDate;
    QDate maxDate;
    QTime minTime;
    QTime maxTime;
    QRegExp channelRegExp;
    QRegExp tagRegExp;
    QRegExp deviceRegExp;
};

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
    void setFilter(QString channel, QString device, QString tag, QDate daten, QDate datem, QTime timen, QTime timem);
    QAbstractItemModel * getModel(bool proxy);
    QMap <int, QString> * getState();
private:
    QString month[12];
    QMap <int, QString> state;
    QStandardItemModel * monitor_model;
    MyFilter * monitor_model_proxy;
    void initMas();
};

#endif // MONITOR_H
