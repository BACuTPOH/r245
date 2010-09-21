#include "monitor.h"

MyFilter::MyFilter(QObject *parent):QSortFilterProxyModel(parent)
{
}

bool MyFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 2, sourceParent);
    QModelIndex index1 = sourceModel()->index(sourceRow, 3, sourceParent);
    QModelIndex index2 = sourceModel()->index(sourceRow, 4, sourceParent);
    QModelIndex index3 = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex index4 = sourceModel()->index(sourceRow, 0, sourceParent);

    return     sourceModel()->data(index0).toString().contains(deviceRegExp)
            && sourceModel()->data(index1).toString().contains(channelRegExp)
            && sourceModel()->data(index2).toString().contains(tagRegExp)
            && dateInRange(sourceModel()->data(index3).toDate())
            && timeInRange(sourceModel()->data(index4).toTime());
}

void MyFilter::setRegExp(QRegExp channel, QRegExp device, QRegExp tag)
{
    channelRegExp = channel;
    deviceRegExp = device;
    tagRegExp = tag;
}

bool MyFilter::dateInRange(const QDate &date) const
{
    return (!minDate.isValid() || date > minDate)
           && (!maxDate.isValid() || date < maxDate);
}

bool MyFilter::timeInRange(const QTime &time) const
{
    return (!minTime.isValid() || time > minTime)
           && (!maxTime.isValid() || time < maxTime);
}



Monitor::Monitor()
{
    QStringList header;

    header << "�����" << "����" << "��� ����������" << "�����" << "��� �����" << "��� �������" << "id ����������" << "id �����";
    monitor_model = new QStandardItemModel();
    monitor_model->setHorizontalHeaderLabels(header);

    monitor_model_proxy = new MyFilter();
    monitor_model_proxy->setSourceModel(monitor_model);
    initMas();
}

QMap <int, QString> * Monitor::getState()
{
    return &state;
}

void Monitor::initMas()
{
    month[0] = "������";
    month[1] = "�������";
    month[2] = "����";
    month[3] = "������";
    month[4] = "���";
    month[5] = "����";
    month[6] = "����";
    month[7] = "������";
    month[8] = "��������";
    month[9] = "�������";
    month[10] = "������";
    month[11] = "�������";

    state[0x01]  = "������� ����������� ��������";
    state[0x02]  = "������� ����������� ���������";
    state[0x10]  = "��������� ����� ���";
    state[0x11]  = "��� �������";
    state[0x20]  = "�������� �����������";
    state[0x21]  = "����������� ������������";
    state[0x22]  = "�������� ������ ������� ����";
    state[0x23]  = "������ ������� ���� ������������";
    state[0x24]  = "��������� ��������� � ����";
    state[0x25]  = "��������� ���� �������������";
    state[0x101] = "�������� ������ ����������� ����������";
    state[0x102] = "������ ����������� ���������� ������������";
    state[0x103] = "�������� ������ ����� (���������)";
    state[0x104] = "������ ����� (���������) ������������";
    state[0x121] = "������� ��� ������� �� ��������� � ������ ������";
    state[0x122] = "���������� ������������";
    state[0x123] = "���������� �������������";
    state[0x124] = "����� ���������� � ���������� �������";
    state[0x125] = "���� ������������ ������� ����������� ����������";
    state[0x126] = "���������� �������� � ���� �������";
    state[0x127] = "���������� �� �������� � ���� �������";
    state[0x128] = "����� �����, ������ �� ���������";
    state[0x129] = "������� ���������� �����";
    state[0x12A] = "���� ������� ��������, �������� ���������";
    state[0x12B] = "�������� ������� �� ����������� �������";
    state[0x12C] = "�������� ������� �������� �����";
    state[0x12D] = "������ ���������";
    state[0x12E] = "������ �� ��������� � ������������� �����";
}

void Monitor::addTransToModel(QString dev_num, R245_TRANSACT * trans, const QString &tag_name, const QString &dev_name)
{
    if((trans->code == 0x10) || (trans->code == 0x11))
    {
        int row = 0/*monitor_model->rowCount()*/;

        monitor_model->insertRow(row);
        monitor_model->setItem(row, TypeEventAttr, new QStandardItem(QString("%1").arg(state[trans->code])));
        monitor_model->setItem(row, ChAttr, new QStandardItem(QString("%1").arg(trans->channel)));

        if(tag_name == "")
        {
            monitor_model->setItem(row, TagNameAttr, new QStandardItem(QString().setNum(trans->tid)));
        }
        else
        {
            monitor_model->setItem(row, TagNameAttr, new QStandardItem(tag_name));
        }
        monitor_model->setItem(row, TagIdAttr, new QStandardItem(QString().setNum(trans->tid)));

        if(dev_name != "")
        {
            monitor_model->setItem(row, DevNameAttr, new QStandardItem(dev_name));
        }
        else
        {
            monitor_model->setItem(row, DevNameAttr, new QStandardItem(dev_num));
        }
        monitor_model->setItem(row, DevNumAttr, new QStandardItem(dev_num));


        monitor_model->setItem(row, DateAttr, new QStandardItem(QString("%1 %2 %3")
                                                                    .arg(trans->day)
                                                                    .arg(month[trans->month-1])
                                                                    .arg(trans->year)));
        monitor_model->setItem(row, TimeAttr, new QStandardItem(QString("%1:%2:%3")
                                                                    .arg(trans->hour)
                                                                    .arg(trans->min)
                                                                    .arg(trans->sec)));
    }
}

void Monitor::setFilter(QString channel, QString device, QString tag, QDate daten, QDate datem,
                        QTime timen, QTime timem)
{
    monitor_model_proxy->setRegExp(QRegExp(channel), QRegExp(device), QRegExp(tag));
    //monitor_model_proxy->setFilterMinimumDate(daten);
    //monitor_model_proxy->setFilterMaximumDate(datem);
    //monitor_model_proxy->setFilterMinimumTime(timen);
    //monitor_model_proxy->setFilterMaximumTime(timem);
}

QAbstractItemModel * Monitor::getModel(bool proxy)
{
    if(proxy)
        return monitor_model_proxy;
    else
        return monitor_model;
}

Monitor::~Monitor()
{
    delete monitor_model;
}

