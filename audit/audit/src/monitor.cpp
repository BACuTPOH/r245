#include "monitor.h"
#include "settings_obj.h"

Monitor::Monitor()
{

    monitor_model = new QStandardItemModel();
    monitor_model->setObjectName("monitor_model");

    initHeader();

    monitor_model_proxy = new MonitorFilter();
    monitor_model_proxy->setSourceModel(monitor_model);
    initMas();
}

void Monitor::initHeader()
{
    QStringList header;

    header << "время" << "дата" << "имя устройства" << "id устройства" << "канал" << "имя метки" << "id метки" << "тип события" << "код события";
    monitor_model->setHorizontalHeaderLabels(header);
}

void Monitor::clear()
{
    monitor_model->clear();
    initHeader();
}

QMap <int, QString> * Monitor::getState()
{
    return &state;
}

void Monitor::initMas()
{
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
    state[0x124] = "Карта направлена к контроллеру доступа";
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
    state[0x12F] = "Оператор среагировал на событие";
}

void Monitor::addTransToModel(QString dev_num, R245_TRANSACT * trans, const QString &tag_name, const QString &dev_name)
{
        int row = 0/*monitor_model->rowCount()*/;

        monitor_model->insertRow(row);
        monitor_model->setItem(row, TypeEventAttr, new QStandardItem(QString("%1").arg(state[trans->code])));
        monitor_model->setItem(row, TransCodeAttr, new QStandardItem(QString().setNum(trans->code)));
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


        monitor_model->setItem(row, DateAttr, new QStandardItem(QDate(trans->year, trans->month, trans->day).toString(Qt::LocalDate)));
        monitor_model->setItem(row, TimeAttr, new QStandardItem(QTime(trans->hour, trans->min, trans->sec).toString()));
}

void Monitor::setFilter(QString channel, QString device, QString tag, QDate daten, QDate datem,
                        QTime timen, QTime timem)
{
    monitor_model_proxy->setRegExp(QRegExp(channel), QRegExp(device), QRegExp(tag));
    monitor_model_proxy->setFilterMinimumDate(daten);
    monitor_model_proxy->setFilterMaximumDate(datem);
    monitor_model_proxy->setFilterMinimumTime(timen);
    monitor_model_proxy->setFilterMaximumTime(timem);

    update();
}

void Monitor::update()
{
    monitor_model_proxy->setFilterRegExp(QRegExp(""));
}

void Monitor::onlyTagInf(bool only)
{
    if(only)
    {
        monitor_model_proxy->setTransCodeRE("16|17");
    } else
    {
        monitor_model_proxy->setTransCodeRE("");
    }
    update();
}

QAbstractItemModel * Monitor::getModel(bool proxy)
{
    if(proxy)
        return monitor_model_proxy;
    else
        return monitor_model;
}

void Monitor::updateAlias(QStandardItemModel * tag_model, QStandardItemModel * dev_name_model)
{
    QStandardItem * dev_item, * tag_item;

    int row_count = monitor_model->rowCount();
    int tag_row_count = tag_model->rowCount();
    int dev_row_count = dev_name_model->rowCount();

    int row_alias = 0;
    bool find_tag = false, find_dev = false;

    for(int row = 0; row < row_count; row++)
    {
        dev_item = monitor_model->item(row, DevNumAttr);
        tag_item = monitor_model->item(row, TagIdAttr);

        row_alias = 0;
        find_tag = false;
        find_dev = false;

        while(row_alias < tag_row_count || row_alias < dev_row_count)
        {
            if(row_alias < tag_row_count)
            {
                if(tag_item->text() == tag_model->item(row_alias, SettingsObj::AliasId)->text())
                {
                    monitor_model->item(row, TagNameAttr)->setText(
                            tag_model->item(row_alias, SettingsObj::AliasName)->text());
                    find_tag = true;
                }
            }

            if(row_alias < dev_row_count)
            {
                if(dev_item->text() == dev_name_model->item(row_alias, SettingsObj::AliasId)->text())
                {
                    monitor_model->item(row, DevNameAttr)->setText(
                            dev_name_model->item(row_alias, SettingsObj::AliasName)->text());
                    find_dev = true;
                }
            }
            row_alias++;
        }

        if(!find_tag)
        {
            monitor_model->item(row, TagNameAttr)->setText(tag_item->text());
        }

        if(!find_dev)
        {
            monitor_model->item(row, DevNameAttr)->setText(dev_item->text());
        }
    }
}

Monitor::~Monitor()
{
    delete monitor_model;
}
