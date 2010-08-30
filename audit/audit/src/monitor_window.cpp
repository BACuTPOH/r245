#include "monitor_window.h"
#include "global.h"

MonitorWindow::MonitorWindow(SettingsObj * set, Monitor * mon, QWidget *parent):
    QDialog(parent)
{
    setupUi(this);

    set_obj = set;
    monitor = mon;

    monitor_view->setModel(monitor->getModel());

    connect(&timer, SIGNAL(timeout()), SLOT(slotUpdateTrans()));
    //timer.start(1000);
    connect(clear_button, SIGNAL(clicked()), SLOT(slotClear()));
}

void MonitorWindow::slotClear()
{
    timer.start(2000);
}

void MonitorWindow::slotUpdateTrans()
{
    if(this->isActiveWindow())
    {
        R245_TRANSACT trans;
        short int status = 0;
        int dev_count = set_obj->getModel(SettingsObj::DevModel)->rowCount();

        for(int dev_num = 0; dev_num < dev_count; dev_num++)
        {
            status = utils.R245_GetTransact(dev_num, &trans);
            /*if(status == 55)
            {
                utils.R245_CloseDev(dev_num);
                utils.R245_InitDev(dev_num);

                qDebug("RESET OK");
                status = utils.R245_GetTransact(dev_num, &trans);
            }*/
            while(!status)
            {
                QString tag_name = "", dev_name = "";
                QStandardItemModel * tag_model = set_obj->getModel(SettingsObj::TagModel);
                QStandardItemModel * dev_name_model = set_obj->getModel(SettingsObj::DevNameModel);

                for(int i = 0; i < tag_model->rowCount(); ++i)
                {
                    if((unsigned long)tag_model->index(i, 0).data().toInt() == trans.tid)
                    {
                        tag_name = tag_model->index(i, 1).data().toString();
                        break;
                    }
                }

                for(int i = 0; i < dev_name_model->rowCount(); ++i)
                {
                    if(dev_name_model->index(i, 0).data().toInt() == dev_num)
                    {
                        dev_name = dev_name_model->index(i, 1).data().toString();
                        break;
                    }
                }

                monitor->addTransToModel(QString().setNum(dev_num), &trans, tag_name, dev_name);
                /*status = utils.R245_GetTransact(dev_num, &trans);
                if(status == 55)
                {
                    utils.R245_CloseDev(dev_num);
                    utils.R245_InitDev(dev_num);

                    qDebug("RESET OK");
                    status = utils.R245_GetTransact(dev_num, &trans);
                }*/
            }
        }
    }
}

MonitorWindow::~MonitorWindow()
{
    utils.R245_CloseAllDev();
}
