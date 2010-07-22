#ifndef _R245_H
#define	_R245_H

#ifdef R245_EXPORTS
#define R245_API __declspec(dllexport)
#else
#define R245_API __declspec(dllimport)
#endif

// CMD CODE=====================================================================
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
#define INITIAL_CRC     0x6363
//==============================================================================

#define MAX_PACKET_LEN          256
#define BUFFER_LEN              256
#define PACKET_HEAD_LEN         6
#define PACKET_END_LEN          3
#define R245_OK                 0
#define R245_ERROR              1

//Error code====================================================================
#define R245_RES_OK             0x00  //Операция выполнена успешно
#define R245_ERR_WRONG_PARAM    0x20  //Неправильное число параметров
#define R245_ERR_INVALID_DATA   0x21  //Недопустимое значение данных
#define R245_ERR_NO_TAG         0x80  //Нет тага в поле считывателя
#define R245_ERR_UNKNOWN        0x200 //Неизвестная ошибка
#define R245_ERR_NO_IMLEMENT    0x201 //Команда не поддерживается
//==============================================================================

#define N_START_BIT         0
#define N_PACKET_NUMBER     1
#define N_DEV_ADDR          2
#define N_PROTOCOL_OPTION   3
#define N_CMD_RES           4
#define N_DATA_LEN          5
#define N_CRC               6
#define N_END_BIT           7

// Export functions
R245_API unsigned long R245_InitDev(short int dev_number,
        void ** ft_handle);
R245_API short int R245_AuditEn(void * ft_handle,
        unsigned char dev_addr, unsigned char enable);
R245_API short int R245_GetVersion(void * ft_handle,
        unsigned char dev_addr, unsigned char *version);
//==============================================================================

short int R245_PacketForm(unsigned char dev_addr, unsigned short int cmd,
    unsigned char * data, unsigned char data_len, unsigned char * packet_out);
void R245_UpdateCRC(short int *crc, char byte);
short int R245_CRCCount(short int init_crc, char * data, int len);
short int R245_PacketSend(void * ft_handle, unsigned char dev_addr,
        unsigned short int cmd, unsigned char * data, unsigned char data_len);
short int R245_CorrectFA(unsigned char * packet, unsigned char packet_len,
        unsigned char *packet_out);


#endif	/* _R245_H */

