#include <QFileDialog>
#include <QDebug>
#include <QSortFilterProxyModel>
#include "settings_window.h"

SettingsWindow::SettingsWindow(SettingsObj * set, Monitor * monitor, QWidget *parent):
    QDialog(parent)
{
    setupUi(this);

    set_menu_tab->setTabEnabled(1, false);

    set_obj = set;
    monitor_obj = monitor;

    block_alias_change = false;

    settings_le->setText(settings.value("/settings/settings_file", "").toString());
    log_le->setText(settings.value("/settings/log_file", "").toString());

    slotOpenSettings(false);
    slotOpenLog(false);

    react_list << "ничего не делать" << "выделить цветом" << "показать сообщение";
    event_list << "обнаружен новый таг" << "таг потерян";
    chanell_list << "1" << "2";

    tag_view->setModel(set_obj->getModel(SettingsObj::TagModelProxy));
    dev_name_view->setModel(set_obj->getModel(SettingsObj::DevNameModelProxy));
    event_view->setModel(set_obj->getModel(SettingsObj::EventModelProxy));
    dev_view->setModel(set_obj->getModel(SettingsObj::DevModel));

    event_view->hideColumn(SettingsObj::EvIdDev);
    event_view->hideColumn(SettingsObj::EvIdTag);

    event_view->setItemDelegate(new EventDelegate(&event_list, &react_list, &chanell_list, event_view));

    connect(settings_button, SIGNAL(clicked()), SLOT(slotOpenSettings()));
    connect(log_button, SIGNAL(clicked()), SLOT(slotOpenLog()));
    connect(add_button, SIGNAL(clicked()), SLOT(slotAdd()));
    connect(del_button, SIGNAL(clicked()), SLOT(slotDelete()));
    connect(save_button, SIGNAL(clicked()), SLOT(slotSaveSetings()));
    connect(find_dev_button, SIGNAL(clicked()), SLOT(slotReadDevInfo()));
    connect(cancel_button, SIGNAL(clicked()), SLOT(close()));
    connect(dev_view, SIGNAL(clicked(QModelIndex)), SLOT(slotDevClick(QModelIndex)));
    connect(act_dev_button, SIGNAL(clicked()), SLOT(slotDevActive()));
    connect(dist1_dial, SIGNAL(valueChanged(int)), SLOT(slotDist1(int)));
    connect(time1_edt, SIGNAL(timeChanged(QTime)), SLOT(slotTime1()));
    connect(dist2_dial, SIGNAL(valueChanged(int)), SLOT(slotDist2(int)));
    connect(time2_edt, SIGNAL(timeChanged(QTime)), SLOT(slotTime2()));
    connect(ch1_button, SIGNAL(clicked()), SLOT(slotActChannel()));
    connect(ch2_button, SIGNAL(clicked()), SLOT(slotActChannel()));
    connect(find_tag_le, SIGNAL(textChanged(QString)), SLOT(slotFindTag()));
    connect(find_dev_le, SIGNAL(textChanged(QString)), SLOT(slotFindDevName()));
    connect(find_event_le, SIGNAL(textChanged(QString)), SLOT(slotFindEvent()));
    connect(synch_time_button, SIGNAL(clicked()), SLOT(slotSynchTime()));

    QStandardItemModel * event_model = (QStandardItemModel*)set_obj->getModel(SettingsObj::EventModel);
    connect(event_model, SIGNAL(itemChanged(QStandardItem*)), SLOT(slotEventDataChanged(QStandardItem*)));

    QStandardItemModel * tag_model = (QStandardItemModel*)set_obj->getModel(SettingsObj::TagModel);
    QStandardItemModel * dev_name_model = (QStandardItemModel*)set_obj->getModel(SettingsObj::DevNameModel);
    connect(tag_model, SIGNAL(itemChanged(QStandardItem*)), SLOT(slotAliasChanged(QStandardItem*)));
    connect(dev_name_model, SIGNAL(itemChanged(QStandardItem*)), SLOT(slotAliasChanged(QStandardItem*)));
}

void SettingsWindow::slotAliasChanged(QStandardItem *item)
{
    if(!block_alias_change)
    {
        utils.changeAlias(item, (QStandardItemModel *) monitor_obj->getModel(false), false);
        utils.changeAlias(item, (QStandardItemModel *) set_obj->getModel(SettingsObj::EventModel), false);
    }
}

void SettingsWindow::slotEventDataChanged(QStandardItem *item)
{
    if(item->column() == SettingsObj::EvNameDev || item->column() == SettingsObj::EvNameTag)
    {
        int id_attr;
        SettingsObj::TypeModel model_type;
        bool is_num = false;

        if(item->column() == SettingsObj::EvNameDev)
        {
            id_attr = SettingsObj::EvIdDev;
            model_type = SettingsObj::DevNameModel;
        } else
        {
            id_attr = SettingsObj::EvIdTag;
            model_type = SettingsObj::TagModel;
        }

        item->text().toInt(&is_num);

        if(is_num)
        {
            QString name = "";

            item->model()->blockSignals(true);

            utils.findAlias(set_obj->getModel(model_type), item->text(), &name);
            item->model()->item(item->row(), id_attr)->setText(item->text());

            if(name != "")
            {
                item->setText(name);
            }
            item->model()->blockSignals(false);

        } else {
            QString id_val = item->model()->item(item->row(), id_attr)->text();
            item->setText(id_val);
        }
    }

}

void SettingsWindow::slotFindEvent()
{
    set_obj->setFilterWildCard(find_event_le->text() + "*", SettingsObj::EventModelProxy);
}

void SettingsWindow::slotFindTag()
{
    set_obj->setFilterWildCard(find_tag_le->text() + "*", SettingsObj::TagModelProxy);
}

void SettingsWindow::slotSynchTime()
{
    R245_RTC rtc_data;
    QAbstractItemModel * model = set_obj->getModel(SettingsObj::DevModel);
    short int dev_count = model->rowCount();

    for(int dev_num = 0; dev_num < dev_count; dev_num++)
    {
        DEV_INFO * dev = set_obj->getDevSettings(model->data(model->index(dev_num, SettingsObj::Id)).toInt());
        QDateTime clock = QDateTime::currentDateTime();

        rtc_data.hour = clock.time().hour();
        rtc_data.min = clock.time().minute();
        rtc_data.sec = clock.time().second();
        rtc_data.dow = clock.date().dayOfWeek();
        rtc_data.year = clock.date().year();
        rtc_data.month = clock.date().month();
        rtc_data.day = clock.date().day();

        if(!dev->active)
            utils.R245_InitDev(dev_num);

        utils.R245_SetTimeRTC(dev_num, &rtc_data);
        utils.R245_SetDateRTC(dev_num, &rtc_data);

        if(!dev->active)
            utils.R245_CloseDev(dev_num);
    }
}

void SettingsWindow::slotFindDevName()
{
    set_obj->setFilterWildCard(find_dev_le->text() + "*", SettingsObj::DevNameModelProxy);
}

void SettingsWindow::slotActChannel()
{
    int row = dev_view->selectionModel()->currentIndex().row();

    if(row == -1)
    {
        ch1_button->setChecked(false);
        ch2_button->setChecked(false);
    }
}

void SettingsWindow::slotSaveSetings()
{
    if(dev_tab->isVisible())
    {
        int row = dev_view->selectionModel()->currentIndex().row();
        if(row > -1)
        {
            unsigned char channel = 0;
            unsigned short time1 = utils.timeToSec(time1_edt->time());
            unsigned short time2 = utils.timeToSec(time2_edt->time());


            QAbstractItemModel * model = set_obj->getModel(SettingsObj::DevModel);
            DEV_INFO * dev = set_obj->getDevSettings(model->data(model->index(row, SettingsObj::Id)).toInt());

            if(ch1_button->isChecked())
                channel |= CHANNEL_ACT_1;
            if(ch2_button->isChecked())
                channel |= CHANNEL_ACT_2;
            qDebug() << "Channel" << channel;

            if(dev->channel != channel)
                set_obj->setChannelDev(row, channel);
            if(dev->time1 != time1)
                set_obj->setTimeDev(row, time1, true);
            if(dev->time2 != time2)
                set_obj->setTimeDev(row, time2, false);
            if(dev->dist1 != dist1_le->text().toInt())
                set_obj->setDistDev(row, dist1_le->text().toInt(), true);
            if(dev->dist2 != dist2_le->text().toInt())
                set_obj->setDistDev(row, dist2_le->text().toInt(), false);
        }
    } else
    {
        set_obj->saveSetings();
    }

}

void SettingsWindow::slotDist1(int value)
{
    int row = dev_view->selectionModel()->currentIndex().row();

    if(row > -1)
    {
        dist1_le->setText(QString().setNum(value));
    } else
    {
        dist1_le->setText(QString().setNum(0));
        dist1_dial->setValue(0);
    }
}

void SettingsWindow::slotTime1()
{
    int row = dev_view->selectionModel()->currentIndex().row();

    if(row == -1)
    {
        time1_edt->setTime(QTime().fromString("00:00:00"));
    }

}

void SettingsWindow::slotDist2(int value)
{
    int row = dev_view->selectionModel()->currentIndex().row();

    if(row > -1)
    {
        dist2_le->setText(QString().setNum(value));
    } else
    {
        dist2_le->setText(QString().setNum(0));
        dist2_dial->setValue(0);
    }
}

void SettingsWindow::slotTime2()
{
    int row = dev_view->selectionModel()->currentIndex().row();

    if(row == -1)
    {
        time2_edt->setTime(QTime().fromString("00:00:00"));
    }
}

void SettingsWindow::slotReadDevInfo()
{
    set_obj->readDevInfo();
}

void SettingsWindow::slotDevClick(QModelIndex qmi)
{
    QAbstractItemModel * model = set_obj->getModel(SettingsObj::DevModel);
    DEV_INFO * dev = set_obj->getDevSettings(model->data(model->index(qmi.row(), SettingsObj::Id)).toInt());

    if(dev != NULL)
    {

        time1_edt->setTime(utils.secToTime(dev->time1));

        time2_edt->setTime(utils.secToTime(dev->time2));

        dist1_le->setText(QString().setNum(dev->dist1));
        dist1_dial->setValue(dev->dist1);

        dist2_le->setText(QString().setNum(dev->dist2));
        dist2_dial->setValue(dev->dist2);

        if(dev->channel & CHANNEL_ACT_1)
            ch1_button->setChecked(true);
        else
            ch1_button->setChecked(false);

        if(dev->channel & CHANNEL_ACT_2)
            ch2_button->setChecked(true);
        else
            ch2_button->setChecked(false);

        if(dev->active)
        {
            act_dev_button->setChecked(true);
        } else
        {
            act_dev_button->setChecked(false);
        }
    }
}

void SettingsWindow::slotDevActive()
{
    int row = dev_view->selectionModel()->currentIndex().row();

    short int status = 1;

    if(row > -1)
    {
        if(act_dev_button->isChecked())
        {
            status = set_obj->setActiveDev(row, true);
        } else
        {
            status = set_obj->setActiveDev(row, false);
        }
    } else
    {
        act_dev_button->setChecked(false);
    }

    if(status)
    {
        act_dev_button->setChecked(false);
    }
}

void SettingsWindow::slotAdd()
{
    if(tag_tab->isVisible())
    {
        find_tag_le->setText("");
        set_obj->addTagToModel();
    } else if(dev_name_tab->isVisible())
    {
        find_dev_le->setText("");
        set_obj->addDevNameToModel();
    } else if(event_tab->isVisible())
    {
        find_event_le->setText("");
        set_obj->addEventToModel();
    }
}

void SettingsWindow::slotDelete()
{
    QTableView * table_view;
    SettingsObj::TypeModel type_model;

    if(tag_tab->isVisible())
    {
        table_view = tag_view;
        type_model = SettingsObj::TagModel;
    } else if(dev_name_tab->isVisible())
    {
        table_view = dev_name_view;
        type_model = SettingsObj::DevNameModel;
    } else if(event_tab->isVisible())
    {
        table_view = event_view;
        type_model = SettingsObj::EventModel;
    }

    int row = table_view->selectionModel()->currentIndex().row();

    if(row > -1)
    {
        // column = 0 - без разницы чему оно равно. В utils.changeAlias по нему определяется тип модели данных.
        QStandardItem * item = ((QStandardItemModel *)set_obj->getModel(type_model))->item(row, 0);

        utils.changeAlias(item, (QStandardItemModel *) monitor_obj->getModel(false), true);
        utils.changeAlias(item, (QStandardItemModel *) set_obj->getModel(SettingsObj::EventModel), true);

        set_obj->getModel(type_model)->removeRow(row);
    }
}

void SettingsWindow::slotOpenSettings(bool dialog)
{
    if(dialog)
        openFile(settings_le, "Выберите файл настроек");

    block_alias_change = true;
    if(settings_le->text() != "")
    {
        if(set_obj->openSettingFile(settings_le->text()))
        {
            event_view->hideColumn(SettingsObj::EvIdDev);
            event_view->hideColumn(SettingsObj::EvIdTag);
            set_menu_tab->setTabEnabled(1, true);

            monitor_obj->updateAlias((QStandardItemModel *)set_obj->getModel(SettingsObj::TagModel),
                                     (QStandardItemModel *)set_obj->getModel(SettingsObj::DevNameModel));
            block_alias_change = false;
            return;
        }
    }
    set_menu_tab->setTabEnabled(1, false);
    block_alias_change = false;

}

void SettingsWindow::slotOpenLog(bool dialog)
{
    if(dialog)
        openFile(log_le, "Выберите файл журнала");
    if(log_le->text() != "")
        set_obj->openLogFile(log_le->text(), monitor_obj);

    monitor_obj->updateAlias((QStandardItemModel *)set_obj->getModel(SettingsObj::TagModel),
                             (QStandardItemModel *)set_obj->getModel(SettingsObj::DevNameModel));
}

void SettingsWindow::openFile(QLineEdit * le, QString caption)
{
    QString file_path = QFileDialog::getOpenFileName(0, caption);

    if(!file_path.isEmpty())
    {
        le->setText(file_path);
    } else
    {
        le->setText("");
    }

}

SettingsWindow::~SettingsWindow()
{
    settings.setValue("/settings/settings_file", settings_le->text());
    settings.setValue("/settings/log_file", log_le->text());
}
