#include "monitor_window.h"
#include "global.h"

MonitorWindow::MonitorWindow(QWidget *parent):
    QDialog(parent)
{
    setupUi(this);
    connect(clear_button, SIGNAL(clicked()), SLOT(updateTrans()));
}

void MonitorWindow::updateTrans()
{
    qDebug("Clear button");
    qint8 ft_status = 0;
    void * ft_handle;
    qint8 num_devs = 0;
    quint16 num_trans = 0;
    unsigned char dev_number = 0, dev_addr = 1;
    R245_TRANSACT trans;
    static int row = 0;

    QString month[12] = {"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август",
                     "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};

    QMap <int, QString> state;

    state[0x01]  = "Питание считывателя включено";
    state[0x02]  = "Питание считывателя выключено";
    state[0x10]  = "Обнаружен новый таг в поле чтения";
    state[0x11]  = "Таг считывателем потерян";
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


    ft_status = utils.R245_Init();

    if(!ft_status)
    {
        num_devs = utils.R245_GetNumDevs();

        if(num_devs > 0)
        {
            ft_status = utils.R245_InitDev(dev_number, &ft_handle);
            ft_status = utils.R245_AuditEn(ft_handle, dev_addr, 1);

            ft_status = utils.R245_GetNumTrans(ft_handle, dev_addr, &num_trans);
            //monitor_table->setRowCount(num_trans);
            if(!ft_status)
            {
                qDebug("Num trans = %d", num_trans);
            }

            while(!utils.R245_GetTransact(ft_handle, dev_addr, &trans))
            {

                monitor_table->insertRow(row);
                monitor_table->setItem(row, 5, new QTableWidgetItem(QString("%1").arg(state[trans.code])));
                monitor_table->setItem(row, 3, new QTableWidgetItem(QString("%1").arg(trans.channel)));
                monitor_table->setItem(row, 4, new QTableWidgetItem(QString("0x%1").arg(trans.tid, 0, 16)));
                monitor_table->setItem(row, 2, new QTableWidgetItem(QString("%1").arg(dev_number)));
                monitor_table->setItem(row, 0, new QTableWidgetItem(QString("%1:%2:%3")
                                                                    .arg(trans.hour)
                                                                    .arg(trans.min)
                                                                    .arg(trans.sec)));
                monitor_table->setItem(row, 1, new QTableWidgetItem(QString("%1 %2 %3")
                                                                    .arg(trans.day)
                                                                    .arg(month[trans.month-1])
                                                                    .arg(trans.year)));
                row++;
            }

        }
    }

    ft_status = utils.R245_CloseAllDev();

    if(!ft_status)
    {
        qDebug("Trans is OK\n");
    }
}
