#ifndef MONITOR_FILTER_H
#define MONITOR_FILTER_H

#include <QSortFilterProxyModel>
#include <QModelIndex>
#include <QDateTime>
#include "global.h"

class MonitorFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    MonitorFilter(QObject *parent = 0);

    QDate filterMinimumDate() const { return minDate; }
    void setFilterMinimumDate(const QDate date);

    QDate filterMaximumDate() const { return maxDate; }
    void setFilterMaximumDate(const QDate date);

    QTime filterMinimumTime() const { return minTime; }
    void setFilterMinimumTime( QTime time);

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

#endif // MONITOR_FILTER_H
