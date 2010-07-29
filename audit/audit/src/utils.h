#ifndef __UTILS_H__
#define __UTILS_H__

#include <QString>
#include <QLibrary>
#include "r245_types.h"

class Utils
{

private:
    QLibrary *lib;

public:
    Utils();

    bool loadLibrary(QString file_name);
    bool unloadLibrary();
    qint8 testFunc();

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
};

#endif // __UTILS_H__
