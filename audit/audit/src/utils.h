#ifndef __UTILS_H__
#define __UTILS_H__

#include <QString>
#include <QLibrary>
#include <QAbstractItemModel>
#include "r245_types.h"

class Utils
{
private:
    QLibrary *lib;

public:
    Utils();

    bool loadLibrary(QString file_name);
    bool unloadLibrary();
    //qint8 testFunc();

    void findAlias(QAbstractItemModel * model, QString find_val, QString * alias);
    // Import functions from libr245dll.dll
    
    Init R245_Init;
    CloseAllDev R245_CloseAllDev;
    GetNumDevs R245_GetNumDevs;
    GetDevInfo R245_GetDevInfo;
    CloseDev R245_CloseDev;
    InitDev R245_InitDev;
    AuditEn R245_AuditEn;
    GetVersion R245_GetVersion;
    GetNumTrans R245_GetNumTrans;
    GetTransact R245_GetTransact;
    GetDamp R245_GetDamp;
    SetDamp R245_SetDamp;
    GetTime R245_GetTime;
    SetTime R245_SetTime;
    SetChan R245_SetChan;
    GetChan R245_GetChan;
    SetTimeRTC R245_SetTimeRTC;
    SetDateRTC R245_SetDateRTC;
};

#endif // __UTILS_H__
