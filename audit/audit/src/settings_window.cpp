#include <QFileDialog>
#include <QDebug>
#include <QSortFilterProxyModel>
#include "settings_window.h"

SettingsWindow::SettingsWindow(SettingsObj * set, QWidget *parent):
    QDialog(parent)
{
    setupUi(this);

    set_obj = set;

    tag_view->setModel(set_obj->getModel(SettingsObj::TagModel));
    dev_name_view->setModel(set_obj->getModel(SettingsObj::DevNameModel));
    event_view->setModel(set_obj->getModel(SettingsObj::EventModel));
    dev_view->setModel(set_obj->getModel(SettingsObj::DevModel));

    connect(settings_button, SIGNAL(clicked()), SLOT(slotOpenSettings()));
    connect(log_button, SIGNAL(clicked()), SLOT(slotOpenLog()));
    connect(add_button, SIGNAL(clicked()), SLOT(slotAdd()));
    connect(save_button, SIGNAL(clicked()), SLOT(slotSaveSetings()));
    connect(find_dev_button, SIGNAL(clicked()), SLOT(slotReadDevInfo()));
    connect(cancel_button, SIGNAL(clicked()), SLOT(close()));
    connect(dev_view, SIGNAL(clicked(QModelIndex)), SLOT(slotDevClick(QModelIndex)));
    connect(act_dev_button, SIGNAL(clicked()), SLOT(slotDevActive()));
    connect(dist1_dial, SIGNAL(valueChanged(int)), SLOT(slotDist1(int)));
    connect(time1_dial, SIGNAL(valueChanged(int)), SLOT(slotTime1(int)));
    connect(dist2_dial, SIGNAL(valueChanged(int)), SLOT(slotDist2(int)));
    connect(time2_dial, SIGNAL(valueChanged(int)), SLOT(slotTime2(int)));
    connect(ch1_button, SIGNAL(clicked()), SLOT(slotActChannel()));
    connect(ch2_button, SIGNAL(clicked()), SLOT(slotActChannel()));
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
            QStandardItemModel * model = set_obj->getModel(SettingsObj::DevModel);
            DEV_INFO * dev = set_obj->getDevSettings(model->data(model->index(row, SettingsObj::Id)).toInt());

            if(ch1_button->isChecked())
                channel |= CHANNEL_ACT_1;
            if(ch2_button->isChecked())
                channel |= CHANNEL_ACT_2;

            if(dev->channel != channel)
                set_obj->setChannelDev(row, dev->channel);
            if(dev->time1 != time1_le->text().toInt())
                set_obj->setTimeDev(row, time1_le->text().toInt(), true);
            if(dev->time2 != time2_le->text().toInt())
                set_obj->setTimeDev(row, time2_le->text().toInt(), false);
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

void SettingsWindow::slotTime1(int value)
{
    int row = dev_view->selectionModel()->currentIndex().row();

    if(row > -1)
    {
        time1_le->setText(QString().setNum(value));
    } else
    {
        time1_le->setText(QString().setNum(0));
        time1_dial->setValue(0);
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

void SettingsWindow::slotTime2(int value)
{
    int row = dev_view->selectionModel()->currentIndex().row();

    if(row > -1)
    {
        time2_le->setText(QString().setNum(value));
    } else
    {
        time2_le->setText(QString().setNum(0));
        time2_dial->setValue(0);
    }
}

void SettingsWindow::slotReadDevInfo()
{
    set_obj->readDevInfo();
}

void SettingsWindow::slotDevClick(QModelIndex qmi)
{
    QStandardItemModel * model = set_obj->getModel(SettingsObj::DevModel);
    DEV_INFO * dev = set_obj->getDevSettings(model->data(model->index(qmi.row(), SettingsObj::Id)).toInt());

    if(dev != NULL)
    {
        time1_le->setText(QString().setNum(dev->time1));
        time1_dial->setValue(dev->time1);
        time2_le->setText(QString().setNum(dev->time2));
        time2_dial->setValue(dev->time2);
        dist1_le->setText(QString().setNum(dev->dist1));
        dist1_dial->setValue(dev->dist1);
        dist2_le->setText(QString().setNum(dev->dist2));
        dist2_dial->setValue(dev->dist2);

        if(dev->channel && CHANNEL_ACT_1)
            ch1_button->setChecked(true);
        else
            ch1_button->setChecked(false);

        if(dev->channel && CHANNEL_ACT_2)
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
        set_obj->addTagToModel();
    } else if(dev_name_tab->isVisible())
    {
        set_obj->addDevNameToModel();
    } else if(event_tab->isVisible())
    {
        set_obj->addEventToModel();
    }
}

void SettingsWindow::slotOpenSettings()
{
    openFile(settings_le, "Выберите файл настроек");
    set_obj->openSettingFile(settings_le->text());

}

void SettingsWindow::slotOpenLog()
{
    openFile(log_le, "Выберите файл журнала");
}

void SettingsWindow::openFile(QLineEdit * le, QString caption)
{
    QString file_path = QFileDialog::getOpenFileName(0, caption);

    if(!file_path.isEmpty())
    {
        le->setText(file_path);
    }

}

