#ifndef _R245_H
#define	_R245_H

#ifdef R245_EXPORTS
#define R245_API __declspec(dllexport)
#include "ftd2xx.h"
#else
#define R245_API __declspec(dllimport)
#endif

#define DEV_ADDR 1 // верно для устройств с адресами 1

// Номера полей конфигурации считывателя, возвращаемой
// при вызове команды R245_CMD_CONFIG_ACCESS
#define ACCESS_OPTION_1 7
#define ACCESS_OPTION_2 13

// Битовые опции канала
#define CHANELL_IS_ACTIVE 0x08

// CMD CODE=================
#define GET_VERSION     0x28
#define SET_TIME        0xC9
#define SET_DATE        0xCA
#define GET_TIME        0xCB
#define GET_DATE        0xCC
#define SET_ADRESS      0x1E
#define GET_SENSORS     0xB6
#define GET_NUM_TAGS_1	0x50
#define GET_FIRST_TAG_1	0x51
#define GET_NEXT_TAG_1	0x52
#define GET_NUM_TAGS_2	0x53
#define GET_FIRST_TAG_2	0x54
#define GET_NEXT_TAG_2	0x55
#define SET_DAMP_1      0x0C
#define SET_DAMP_2      0x0D
#define GET_DAMP_1      0x12
#define GET_DAMP_2      0x13
#define SET_TIME_1      0x19
#define SET_TIME_2      0x1A
#define GET_TIME_1      0x1B
#define GET_TIME_2      0x1C
#define SET_CHAN_1      0xCD
#define SET_CHAN_2      0xCE
#define AUDIT_EN        0xB5
#define CLEAR_TRANS     0x78
#define GET_NUM_TRANS   0x79
#define GET_TRANS       0x7A
#define READ_CFG        0x97
//==========================
#define MAX_PACKET_LEN  256
#define BUFFER_LEN      256
#define PACKET_HEAD_LEN 6
#define PACKET_END_LEN  3
#define R245_OK         0
#define R245_ERROR      1
#define R245_ERROR_WAIT  55
#define R245_BAUD_RATE  9600
#define INITIAL_CRC     0x6363
//Packet error code=================================================
#define R245_RES_OK             0x00  //Операция выполнена успешно
#define R245_ERR_WRONG_PARAM    0x20  //Неправильное число параметров
#define R245_ERR_INVALID_DATA   0x21  //Недопустимое значение данных
#define R245_ERR_NO_TAG         0x80  //Нет тага в поле считывателя
#define R245_ERR_UNKNOWN        0x200 //Неизвестная ошибка
#define R245_ERR_NO_IMLEMENT    0x201 //Команда не поддерживается
//===================================================================
#define N_START_BIT             0
#define N_PACKET_NUMBER         1
#define N_DEV_ADDR              2
#define N_PROTOCOL_OPTION       3
#define N_CMD_RES               4
#define N_DATA_LEN              5
#define N_CRC                   6
#define N_END_BIT               7
//================================

typedef struct struct_dev_info
{
    DWORD flags;
    DWORD id;
    DWORD loc_id;
    DWORD type;
    char serial_number[16];
    char desc[64];
    FT_HANDLE ft_handle;

} R245_DEV_INFO;

typedef struct struct_rtc
{
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
    unsigned char dow;
    unsigned short int year;
    unsigned char month;
    unsigned char day;
} R245_RTC;

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

// Export functions
R245_API FT_STATUS R245_Init();
R245_API FT_STATUS R245_CloseAllDev();
R245_API DWORD R245_GetNumDevs();
R245_API FT_STATUS R245_GetDevInfo(short int num_dev, R245_DEV_INFO *info);
R245_API FT_STATUS R245_CloseDev(DWORD num_dev);
R245_API FT_STATUS R245_InitDev(short int num_dev);
R245_API FT_STATUS R245_AuditEn(
    unsigned char num_dev,
    unsigned char enable
);
R245_API FT_STATUS R245_GetVersion(
    FT_HANDLE ft_handle,
    unsigned char dev_addr, 
    unsigned char *version
);
R245_API FT_STATUS R245_GetNumTrans(
    unsigned char num_dev,
    short unsigned int * num_trans
);
R245_API FT_STATUS R245_GetTransact(
    unsigned char num_dev,
    R245_TRANSACT * trans
);
R245_API FT_STATUS R245_GetDamp(
    unsigned char num_dev,
    unsigned char num_ch,
    unsigned char * damp
);
R245_API FT_STATUS R245_SetDamp(
    unsigned char num_dev,
    unsigned char num_ch,
    unsigned char damp
);
R245_API FT_STATUS R245_GetTime(
    unsigned char num_dev,
    unsigned char num_ch,
    short int *time
);
R245_API FT_STATUS R245_SetTime(
    unsigned char num_dev,
    unsigned char num_ch,
    short int time
);
R245_API FT_STATUS R245_SetChan(
    unsigned char num_dev,
    unsigned char num_ch,
    unsigned char enable
);
R245_API FT_STATUS R245_GetChan(
    unsigned char num_dev,
    unsigned char * ch
);
R245_API FT_STATUS R245_SetTimeRTC(unsigned char num_dev, R245_RTC * rtc_data);
R245_API FT_STATUS R245_SetDateRTC(unsigned char num_dev, R245_RTC * rtc_data);
//=================================

short int R245_PacketForm(
    unsigned char dev_addr, 
    unsigned char cmd,
    unsigned char * data, 
    unsigned char data_len, 
    unsigned char * packet_out
);
void R245_UpdateCRC(short int *crc, char byte);
short int R245_CRCCount(short int init_crc, char * data, unsigned int len);
short int R245_PacketSend(
    FT_HANDLE ft_handle, 
    unsigned char dev_addr,
    unsigned char cmd, 
    unsigned char *data, 
    unsigned char data_len,
    unsigned char *rx_data,
    unsigned char *rx_data_len
);
short int R245_PacketRecieve(
    FT_HANDLE ft_handle,
    unsigned char * rx_buffer,
    unsigned char * rx_data,
    unsigned char *rx_data_len
);
short int R245_CorrectFA(
    unsigned char * packet,
    unsigned char packet_len,
    unsigned char *packet_out
);
short int R245_DeleteFA(
    unsigned char * packet,
    DWORD * packet_len
);


#endif	/* _R245_H */

