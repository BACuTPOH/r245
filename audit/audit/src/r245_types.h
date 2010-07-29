#ifndef R245_TYPES_H
#define R245_TYPES_H

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

typedef short int (*InitDev) (short int, void **);
typedef short int (*AuditEn) (void *, unsigned char, unsigned char);
typedef short int (*GetVersion) (void *, unsigned char, unsigned char *);
typedef short int (*Init)();
typedef short int (*GetDevInfo)(short int num_dev, R245_DEV_INFO *);
typedef short int (*CloseDev) (short int);
typedef short int (*CloseAllDev) ();
typedef short int (*GetNumDevs) ();
typedef short int (*GetNumTrans) (void *, unsigned char, short unsigned int *);
typedef short int (*GetTransact) (void *, unsigned char, R245_TRANSACT *);

#endif // R245_TYPES_H
