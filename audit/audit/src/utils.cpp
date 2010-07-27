#include "utils.h"

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

    R245_Init = (Init) lib->resolve("R245_Init"); //
    R245_CloseAllDev = (CloseAllDev) lib->resolve("R245_CloseAllDev");//
    R245_GetNumDevs = (GetNumDevs) lib->resolve("R245_GetNumDevs");//
    R245_GetDevInfo = (GetDevInfo) lib->resolve("R245_GetDevInfo");//
    R245_CloseDev = (CloseDev) lib->resolve("R245_CloseDev"); //
    R245_InitDev = (InitDev) lib->resolve("R245_InitDev");//
    R245_AuditEn = (AuditEn) lib->resolve("R245_AuditEn");//
    R245_GetVersion = (GetVersion) lib->resolve("R245_GetVersion");//
    
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
    qint8 ft_status;
    void * ft_handle;
    //unsigned char dev_number = 0, dev_addr = 1;
    //unsigned char version[50];
    R245_DEV_INFO dev_info;
    
    if(!ft_status)
    {

        qDebug("Get info");
        ft_status = R245_GetDevInfo(0, &dev_info);
        if(ft_status)
        {
            qDebug("Error: Get info");
            return 0;
        }
           
        
        ft_status = R245_InitDev(0, &ft_handle);
        if(!ft_status)
            qDebug("Init ok");
      
          //ft_status = R245_Init();
      
        ft_status = R245_GetDevInfo(0, &dev_info);
        if(ft_status)
            return 0;
        
        //R245_CloseAllDev();
    }
    
    /*ft_status = R245_InitDev(dev_number, &ft_handle);
    if(!R245_GetVersion(ft_handle, dev_addr, version))
    {
        qDebug("Version = %s\n", version);
    } else
    {
        qDebug("Error: get version\n");
    }*/

    //ft_status = R245_AuditEn(ft_handle, dev_addr, 1);

    /*if(!ft_status)
    {
        qDebug("Trans is OK\n");
    }*/
    
    return ft_status;
}
