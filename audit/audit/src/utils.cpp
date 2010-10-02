#include "utils.h"
#include "settings_obj.h"
#include <QDebug>

Utils::Utils()
{
    lib = NULL;
}

bool Utils::loadLibrary(QString file_name)
{
    
    lib = new QLibrary(file_name);
    
    if(lib->load())
    {
        qDebug("LOAD is OK");
    } else
    {
        qDebug("LOAD isn't ok");
        lib = NULL;
        return false;
    }

    R245_Init = (Init) lib->resolve("R245_Init");
    R245_CloseAllDev = (CloseAllDev) lib->resolve("R245_CloseAllDev");
    R245_GetNumDevs = (GetNumDevs) lib->resolve("R245_GetNumDevs");
    R245_GetDevInfo = (GetDevInfo) lib->resolve("R245_GetDevInfo");
    R245_CloseDev = (CloseDev) lib->resolve("R245_CloseDev");
    R245_InitDev = (InitDev) lib->resolve("R245_InitDev");
    R245_AuditEn = (AuditEn) lib->resolve("R245_AuditEn");
    R245_GetVersion = (GetVersion) lib->resolve("R245_GetVersion");
    R245_GetNumTrans = (GetNumTrans) lib->resolve("R245_GetNumTrans");
    R245_GetTransact = (GetTransact) lib->resolve("R245_GetTransact");
    R245_GetDamp = (GetDamp) lib->resolve("R245_GetDamp");
    R245_SetDamp = (SetDamp) lib->resolve("R245_SetDamp");
    R245_GetTime = (GetTime) lib->resolve("R245_GetTime");
    R245_SetTime = (SetTime) lib->resolve("R245_SetTime");
    R245_SetChan = (SetChan) lib->resolve("R245_SetChan");
    R245_GetChan = (GetChan) lib->resolve("R245_GetChan");
    R245_SetTimeRTC = (SetTimeRTC) lib->resolve("R245_SetTimeRTC");
    R245_SetDateRTC = (SetDateRTC) lib->resolve("R245_SetDateRTC");
    
    return true;
}

void Utils::showMessage(QMessageBox::Icon icon, QString header, QString msg)
{
    QMessageBox* pmbx = new QMessageBox(
            icon,
            header,
            msg);
    pmbx->exec();
    delete pmbx;
}

bool Utils::unloadLibrary()
{
    if(lib != NULL)
    {
        if(lib->unload())
        {
            qDebug("UnLOAD is OK");
            delete lib;
            lib = NULL;
            
            return true;
        } else
        {
            qDebug("UnLOAD isn't OK");
        }
    }
    
    return false;
}

void Utils::findAlias(QAbstractItemModel * model, QString find_val, QString * alias)
{
    *alias = "";

    for(int i = 0; i < model->rowCount(); ++i)
    {
        if(model->index(i, SettingsObj::AliasId).data().toString() == find_val)
        {
            *alias = model->index(i, SettingsObj::AliasName).data().toString();
            break;
        }
    }
}

void Utils::changeAlias(QStandardItem * alias_item, QStandardItemModel * model, bool clear)
{
    QStandardItemModel * alias_model = alias_item->model();
    QStandardItem * item_model;

    int row_count = model->rowCount();
    int id_attr, name_attr;

    if(alias_model->objectName() == "tag_model")
    {
        if(model->objectName() == "monitor_model")
        {
            id_attr = Monitor::TagIdAttr;
            name_attr = Monitor::TagNameAttr;
        } else // event_model
        {
            id_attr = SettingsObj::EvIdTag;
            name_attr = SettingsObj::EvNameTag;
        }

    } else // dev_name_model
    {
        if(model->objectName() == "monitor_model")
        {
            id_attr = Monitor::DevNumAttr;
            name_attr = Monitor::DevNameAttr;
        }
        else // event_model
        {
            id_attr = SettingsObj::EvIdDev;
            name_attr = SettingsObj::EvNameDev;
        }
    }

    for(int row = 0; row < row_count; row++)
    {
        item_model = model->item(row, id_attr);

        if(item_model->text() == alias_model->item(alias_item->row(), SettingsObj::AliasId)->text())
        {
            if(clear)
            {

                model->blockSignals(true); // чтобы не работали сигналы на изменение моделей (актуально для event_model)
                model->item(row, name_attr)->setText(
                        alias_model->item(alias_item->row(), SettingsObj::AliasId)->text());
                model->blockSignals(false);
            } else
            {
                model->item(row, name_attr)->setText(
                        alias_model->item(alias_item->row(), SettingsObj::AliasName)->text());
            }
        }
    }
}

int Utils::timeToSec(QTime time)
{
    return (time.hour()*3600 +
           time.minute()*60 +
           time.second())*10;
}


QTime Utils::secToTime(int time_sec)
{
    int hour = time_sec / 36000;
    int min  = (time_sec/10%36000) / 60;
    int sec  = (time_sec/10%3600) % 60;

    return QTime(hour, min, sec);
}
