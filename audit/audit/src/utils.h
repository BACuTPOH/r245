#ifndef __UTILS_H__
#define __UTILS_H__

#include <QString>
#include <QLibrary>


typedef struct struct_dev_info
{
    unsigned long flags;
    unsigned long id;
    unsigned long loc_id;
    unsigned long type;
    char serial_number[16];
    char desc[64];
    void * ft_handle;

} R245_DEV_INFO;

typedef short int (*InitDev) (short int, void **);//
typedef short int (*AuditEn) (void *, unsigned char, unsigned char); //
typedef short int (*GetVersion) (void *, unsigned char, unsigned char *); //
typedef short int (*Init)();//
typedef short int (*GetDevInfo)(short int num_dev, R245_DEV_INFO *); //
typedef short int (*CloseDev) (short int); //
typedef short int (*CloseAllDev) ();//
typedef short int (*GetNumDevs) (); //

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
    
    Init R245_Init;//
    CloseAllDev R245_CloseAllDev; //
    GetNumDevs R245_GetNumDevs; //
    GetDevInfo R245_GetDevInfo;//
    CloseDev R245_CloseDev;//
    InitDev R245_InitDev; //
    AuditEn R245_AuditEn;//
    GetVersion R245_GetVersion;//
    

};

#endif // __UTILS_H__
