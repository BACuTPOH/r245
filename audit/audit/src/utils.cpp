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
    
    return true;
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

qint8 Utils::testFunc()
{
    qint8 ft_status = 0;
    void * ft_handle;
    qint8 num_devs = 0;
    quint16 num_trans = 0;
    unsigned char dev_number = 0, dev_addr = 1;
    R245_TRANSACT trans;
    
    ft_status = R245_Init();

    if(!ft_status)
    {
        num_devs = R245_GetNumDevs();

        if(num_devs > 0)
        {
            ft_status = R245_InitDev(dev_number, &ft_handle);
            ft_status = R245_AuditEn(ft_handle, dev_addr, 1);

            ft_status = R245_GetNumTrans(ft_handle, dev_addr, &num_trans);
            if(!ft_status)
            {
                qDebug("Num trans = %d", num_trans);
            }

            qDebug("Code | Channel | TID | Day | Month | Year | Sec | Min | Hour | Dow");
            while(!R245_GetTransact(ft_handle, dev_addr, &trans))
            {
                qDebug("%x | %x | %x | %x | %x | %x | %x | %x | %x | %x",
                       trans.code, trans.channel, (unsigned int)trans.tid, trans.day, trans.month,
                       trans.year, trans.sec, trans.min, trans.hour, trans.dow);
            }

        }
    }
    
    ft_status = R245_CloseAllDev();

    if(!ft_status)
    {
        qDebug("Trans is OK\n");
    }
    
    return ft_status;
}
