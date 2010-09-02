#include "monitor_window.h"
#include "global.h"

MonitorWindow::MonitorWindow(SettingsObj * set, Monitor * mon, QWidget *parent):
    QDialog(parent)
{
    setupUi(this);

    set_obj = set;
    monitor = mon;

    monitor_view->setModel(monitor->getModel(true));
    monitor_view->hideColumn(6);
    monitor_view->hideColumn(7);

    connect(&timer, SIGNAL(timeout()), SLOT(slotUpdateTrans()));
    timer.start(1000);
}

void MonitorWindow::slotClear()
{
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
            while(!(status = utils.R245_GetTransact(dev_num, &trans)))
            {
                QString tag_name = "", dev_name = "";
                QAbstractItemModel * tag_model = set_obj->getModel(SettingsObj::TagModel);
                QAbstractItemModel * dev_name_model = set_obj->getModel(SettingsObj::DevNameModel);

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
                set_obj->addLogNode(QString().setNum(dev_num), &trans); // add node to log file
            }
        }
    }
}

MonitorWindow::~MonitorWindow()
{
    utils.R245_CloseAllDev();
}
