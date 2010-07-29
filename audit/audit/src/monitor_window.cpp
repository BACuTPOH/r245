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

    QString month[12] = {"������", "�������", "����", "������", "���", "����", "����", "������",
                     "��������", "�������", "������", "�������"};

    QMap <int, QString> state;

    state[0x01]  = "������� ����������� ��������";
    state[0x02]  = "������� ����������� ���������";
    state[0x10]  = "��������� ����� ��� � ���� ������";
    state[0x11]  = "��� ������������ �������";
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
