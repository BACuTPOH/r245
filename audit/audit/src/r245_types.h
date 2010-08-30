#ifndef R245_TYPES_H
#define R245_TYPES_H

typedef struct struct_r245_dev_info
{
    unsigned long flags;
    unsigned long id;
    unsigned long loc_id;
    unsigned long type;
    char serial_number[16];
    char desc[64];
    void * ft_handle;

} R245_DEV_INFO;

typedef struct struct_dev_info
{
    unsigned int id;
    bool active;
    unsigned char channel;
    short int time1;
    unsigned char dist1;
    short int time2;
    unsigned char dist2;
} DEV_INFO;

typedef struct struct_transact
{
    short unsigned int code;
    unsigned char channel;
    unsigned long tid;
    unsigned char day;
    unsigned char month;
    short unsigned int year;
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
    unsigned char dow;
} R245_TRANSACT;

typedef short int (*InitDev) (short int);
typedef short int (*AuditEn) (unsigned char, unsigned char);
typedef short int (*GetVersion) (void *, unsigned char, unsigned char *);
typedef short int (*Init)();
typedef short int (*GetDevInfo)(short int num_dev, R245_DEV_INFO *);
typedef short int (*CloseDev) (short int);
typedef short int (*CloseAllDev) ();
typedef short int (*GetNumDevs) ();
typedef short int (*GetNumTrans) (unsigned char, short unsigned int *);
typedef short int (*GetTransact) (unsigned char, R245_TRANSACT *);
typedef short int (*GetDamp) (unsigned char, unsigned char, unsigned char *);
typedef short int (*SetDamp) (unsigned char, unsigned char, unsigned char);
typedef short int (*GetTime) (unsigned char, unsigned char, short int *);
typedef short int (*SetTime) (unsigned char, unsigned char, short int);
typedef short int (*SetChan) (unsigned char, unsigned char, unsigned char);
typedef short int (*GetChan) (unsigned char, unsigned char *);

#endif // R245_TYPES_H
