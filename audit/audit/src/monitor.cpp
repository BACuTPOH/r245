#include "monitor.h"

Monitor::Monitor()
{
    QStringList header;

    header << "время" << "дата" << "имя устройства" << "канал" << "имя метки" << "тип события" << "id устройства" << "id метки";
    monitor_model = new QStandardItemModel();
    monitor_model->setHorizontalHeaderLabels(header);

    monitor_model_proxy = new QSortFilterProxyModel();
    monitor_model_proxy->setSourceModel(monitor_model);

    initMas();
}

QMap <int, QString> * Monitor::getState()
{
    return &state;
}

void Monitor::initMas()
{
    month[0] = "Январь";
    month[1] = "Февраль";
    month[2] = "Март";
    month[3] = "Апрель";
    month[4] = "Май";
    month[5] = "Июнь";
    month[6] = "Июль";
    month[7] = "Август";
    month[8] = "Сентябрь";
    month[9] = "Октябрь";
    month[10] = "Ноябрь";
    month[11] = "Декабрь";

    state[0x01]  = "Питание считывателя включено";
    state[0x02]  = "Питание считывателя выключено";
    state[0x10]  = "обнаружен новый таг";
    state[0x11]  = "таг потерян";
    state[0x20]  = "Сработал вибродатчик";
    state[0x21]  = "Вибродатчик восстановлен";
    state[0x22]  = "Сработал тампер корпуса тага";
    state[0x23]  = "Тампер корпуса тага восстановлен";
    state[0x24]  = "Разряжена батарейка в таге";
    state[0x25]  = "Батарейка тага восстановлена";
    state[0x101] = "Сработал датчик присутствия автомобиля";
    state[0x102] = "Датчик присутствия автомобиля восстановлен";
    state[0x103] = "Сработал датчик ворот (шлагбаума)";
    state[0x104] = "Датчик ворот (шлагбаума) восстановлен";
    state[0x121] = "Текущий таг имеется на обработке в другом канале";
    state[0x122] = "Радиоканал заблокирован";
    state[0x123] = "Радиоканал разблокирован";
    state[0x124] = "Карта направлена к контроллер доступа";
    state[0x125] = "Ждем срабатывания датчика присутствия автомобиля";
    state[0x126] = "Автомобиль появился в зоне датчика";
    state[0x127] = "Автомобиль не появился в зоне датчика";
    state[0x128] = "Время вышло, ворота не открылись";
    state[0x129] = "Ожидаем закрывания ворот";
    state[0x12A] = "Цикл доступа закончен, исходное состояние";
    state[0x12B] = "Ожидание решения от контроллера доступа";
    state[0x12C] = "Ожидание времени открытых ворот";
    state[0x12D] = "Ворота закрылись";
    state[0x12E] = "Ворота не закрылись в установленное время";
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

void Monitor::setFilter(QString filter, int colnum)
{
    monitor_model_proxy->setFilterRegExp(QRegExp(filter));
    monitor_model_proxy->setFilterKeyColumn(colnum);
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

