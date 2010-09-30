#include "utils.h"
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
        if(model->index(i, 0).data().toString() == find_val)
        {
            *alias = model->index(i, 1).data().toString();
            break;
        }
    }
}

int Utils::timeToSec(QTime time)
{
    return time.hour()*3600 +
           time.minute()*60 +
           time.second();
}


QTime Utils::secToTime(int time_sec)
{
    int hour = time_sec / 3600;
    int min  = (time_sec%3600) / 60;
    int sec  = (time_sec%3600) % 60;

    return QTime(hour, min, sec);
}
