#define R245_EXPORTS

#include "r245.h"

const static unsigned short int crc_table[256] = {
    0x0000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241,
    0xc601, 0x06c0, 0x0780, 0xc741, 0x0500, 0xc5c1, 0xc481, 0x0440,
    0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40,
    0x0a00, 0xcac1, 0xcb81, 0x0b40, 0xc901, 0x09c0, 0x0880, 0xc841,
    0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
    0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
    0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
    0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
    0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
    0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
    0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
    0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
    0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
    0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
    0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
    0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
    0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
    0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
    0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
    0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
    0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
    0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
    0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
    0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
    0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
    0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
    0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
    0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
    0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
    0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
    0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
    0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040
};

// количество устройств
static DWORD num_devs = 0;
static unsigned char packet_ctr = 0; // packet counter

// Постройка списка подключенных устройств
// Необходимо вызывать, когда меняется состояние устройств (подключили, откл и т.п.),
// чтобы обновить данные об устройствах во внутренних структурах драйвера ftdi.
R245_API FT_STATUS R245_Init()
{
    FT_STATUS ft_status;
    
    ft_status = FT_CreateDeviceInfoList(&num_devs);
    /*if (ft_status == FT_OK) {
        printf("Number of devices is %d\n",num_devs);
    }*/

    return ft_status;
}

R245_API DWORD R245_GetNumDevs()
{
    return num_devs;
}

R245_API FT_STATUS R245_CloseAllDev()
{
    DWORD i = 0;
    FT_STATUS ft_status;

    ft_status = R245_Init();
    
    if(num_devs > 0 && !ft_status)
    {
        for(i = 0; i < num_devs; i++)
        {
            ft_status = R245_CloseDev(i);
        }
    }

    return R245_OK;
}

R245_API FT_STATUS R245_GetDevInfo(short int num_dev, R245_DEV_INFO *info)
{
    FT_STATUS ft_status;

    ft_status = R245_Init();

    if (num_devs > 0 && num_dev < num_devs && !ft_status)
    {
        ft_status = FT_GetDeviceInfoDetail(num_dev, &(info->flags), &(info->type),
                &(info->id), &(info->loc_id), info->serial_number, info->desc, &(info->ft_handle));
        
        if (ft_status == FT_OK) {
          /*printf("Dev 0:\n");
          printf("  Flags=0x%x\n",info->flags);
          printf("  Type=0x%x\n",info->type);
          printf("  ID=0x%x\n",info->id);
          printf("  LocId=0x%x\n",info->loc_id);
          printf("  SerialNumber=%s\n",info->serial_number);
          printf("  Description=%s\n",info->desc);*/
          //printf("  ftHandle=0x%x\n",info->ft_handle);
        }
    } else
    {
        return R245_ERROR;
    }

    return ft_status;
}

R245_API FT_STATUS R245_InitDev(short int num_dev)
{
    FT_STATUS ft_status;
    FT_HANDLE ft_handle;
    
    if (ft_status = FT_Open(num_dev, &ft_handle) != FT_OK) {
        return ft_status;
    }

    printf("  INIT ftHandle=0x%x\n", ft_handle);

    ft_status = FT_SetBaudRate(ft_handle, R245_BAUD_RATE);
    if (ft_status != FT_OK) {
        return ft_status;
    }

    ft_status = FT_SetDataCharacteristics(ft_handle, FT_BITS_8, FT_STOP_BITS_1,
    FT_PARITY_NONE);
    if (ft_status != FT_OK) {
        return ft_status;
    }

    ft_status = FT_SetTimeouts(ft_handle, 5000, 1000);
    if (ft_status != FT_OK) {
        return ft_status;
    }

    return R245_OK;
}

void R245_UpdateCRC(short int *crc, char byte)
{
    *crc = (short int) (((*crc >> 8) & 0xff) ^ crc_table[(*crc ^ byte) & 0xff]);
}

short int R245_PacketForm(unsigned char dev_addr, unsigned char cmd,
    unsigned char * data, unsigned char data_len, unsigned char *packet_out)
{
    
    unsigned char packet[MAX_PACKET_LEN];
    unsigned char packet_len = 0;
    unsigned short int crc = 0;

    packet[0] = 0xFB; //start
    packet[1] = packet_ctr; // number of packet
    packet[2] = dev_addr; // address device
    packet[3] = 0x00;
    packet[4] = cmd;
    packet[5] = data_len; // len

    for(packet_len = PACKET_HEAD_LEN; packet_len < data_len+PACKET_HEAD_LEN; packet_len++)
    {
        packet[packet_len] = data[packet_len - PACKET_HEAD_LEN];
    }

    crc = R245_CRCCount(INITIAL_CRC, &packet[1], packet_len-1);// crc without start bit (i-1)

    packet[packet_len++] = crc;
    packet[packet_len++] = crc >> 8;
    packet[packet_len++] = 0xFD; // stop bit
    
    if(R245_CorrectFA(packet, packet_len, packet_out) == R245_OK)
    {  
        return R245_OK;
    } else {
        return R245_ERROR;
    }

}

short int R245_CorrectFA(unsigned char * packet, unsigned char packet_len,
        unsigned char *packet_out)
{
    unsigned char i;
    unsigned short int j = 0;

    for(i = 0; i < packet_len; i++)
    {
        if((packet[i] >= 0xFA) && (i != 0) && (i != packet_len-1))
        {
            packet_out[j++] = 0xFA;
            packet_out[j++] = packet[i] - 0xF0;
        }
        else
        {
            packet_out[j++] = packet[i];
        }
        if(j >= BUFFER_LEN)
            return R245_ERROR;
    }

    return R245_OK;
}

short int R245_DeleteFA(unsigned char * packet, DWORD * packet_len)
{
    short int i = 0, j = 0;
    short int len = *packet_len;

    while(i < len)
    {
        if(packet[i] == 0xFA)
        {
            packet[i] = packet[i+1] + 0xF0;
            i++; len--;
            
            for(j = i; j < len; ++j)
            {
                packet[j] = packet[j+1];
            }
        }
        i++;
    }
    
    *packet_len = len;

    return R245_OK;
}

R245_API FT_STATUS R245_CloseDev(DWORD num_dev)
{
    FT_STATUS ft_status;
    R245_DEV_INFO info;

    R245_GetDevInfo(num_dev, &info);
    // If device was opened
    if(info.flags & 1)
    {
        ft_status = FT_Close(info.ft_handle);
    }

    return ft_status;  
}

short int R245_CRCCount(short int init_crc, char * data, unsigned int len)
{
    short int crc;
    char * u8_data;

    u8_data = data;
    crc    = init_crc;
    
    while (len--) {
        R245_UpdateCRC(&crc, *u8_data++);
    }
    return crc;
}

short int R245_PacketSend(FT_HANDLE ft_handle, unsigned char dev_addr, 
        unsigned char cmd, unsigned char * data, unsigned char data_len,
        unsigned char *rx_data, unsigned char *rx_data_len)
{
    FT_STATUS ft_status;
    DWORD bytes_written;
    short int tx_packet_len = data_len + PACKET_HEAD_LEN + PACKET_END_LEN;
    static unsigned char tx_buffer[BUFFER_LEN];
    static unsigned char rx_buffer[BUFFER_LEN];
    
    R245_PacketForm(dev_addr, cmd, data, data_len, tx_buffer);

    ft_status = FT_Write(ft_handle, tx_buffer, tx_packet_len, &bytes_written);
    if (ft_status == FT_OK)
    {
        printf("bytes = %d\n", bytes_written);
    }
    else
    {
        printf("Error: write\n");
        
        return R245_ERROR;
    }

    return R245_PacketRecieve(ft_handle, rx_buffer, rx_data, rx_data_len);
}

short int R245_PacketRecieve(FT_HANDLE ft_handle, unsigned char * rx_buffer,
        unsigned char * rx_data, unsigned char *rx_data_len)
{
    HANDLE h_event;
    DWORD event_mask, event_dword;
    FT_STATUS ft_status;
    short int rx_packet_len = 0;
    boolean recieve = TRUE;
    DWORD bytes_received, rx_bytes, tx_bytes;
    short int i = 0;
    
    h_event = CreateEvent(
          NULL,// NULL
          0, // auto-reset event 0
          0, // non-signalled state 0
          ""
    );

    event_mask = FT_EVENT_RXCHAR;
    ft_status = FT_SetEventNotification(ft_handle, event_mask, h_event);
    if(ft_status)
        printf("Error set event notification");

    rx_buffer[N_DATA_LEN] = 0;

    /*ft_status = FT_ResetDevice(ft_handle);
    if (ft_status == FT_OK) {
        printf("OK\n");
    }
    else {
        printf("FAILED\n");
    }*/

    while(recieve 
            /*rx_packet_len <= PACKET_HEAD_LEN ||*/ /*rx_packet_len !=
                        rx_buffer[N_DATA_LEN]+PACKET_HEAD_LEN + PACKET_END_LEN*/)
    {
        if(WaitForSingleObject(h_event, 0xFF/*INFINITE*/))
        {
            printf("Error: wait recieve data\n");
            FT_GetStatus(ft_handle, &rx_bytes,&tx_bytes,&event_dword);
            printf("READ bytes %d\n", rx_bytes);
            printf("Write bytes %d\n", tx_bytes);

            /*ft_status = FT_CyclePort(ft_handle);
            if (ft_status == FT_OK) {
                printf("Reset OK\n");
            }
            else
                printf("Reset FAILED\n");*/

            /*printf("PACKET\n");
            for(i = 0; i < rx_packet_len; i ++)
                printf("0x%x ", rx_buffer[i]);

            printf("\n");*/

            return R245_ERROR;
        }

        FT_GetStatus(ft_handle, &rx_bytes,&tx_bytes,&event_dword);
        if (rx_bytes > 0)
        {
            printf("READ bytes %d\n", rx_bytes);

            ft_status = FT_Read(ft_handle, &rx_buffer[rx_packet_len],
                rx_bytes, &bytes_received);

            R245_DeleteFA(&rx_buffer[rx_packet_len], &bytes_received);

            rx_packet_len += bytes_received;
            
            if(ft_status != FT_OK)
                break;

            if(rx_buffer[rx_packet_len - 1] == 0xFE)
                recieve = FALSE;
        }
        /*if(rx_buffer[rx_packet_len - 1] == 0xFE)
            recieve = FALSE;*/
    }

    CloseHandle(h_event);

    if (ft_status == FT_OK && rx_buffer[N_CMD_RES] == R245_RES_OK &&
            rx_buffer[N_PACKET_NUMBER] == packet_ctr)
    {
       packet_ctr++;

       if(rx_data != NULL)
            memcpy(rx_data, &rx_buffer[PACKET_HEAD_LEN], rx_buffer[N_DATA_LEN]);

       //TEST
       /*printf("PACKET\n");
       for(i = 0; i < rx_packet_len; i ++)
           printf("0x%x ", rx_buffer[i]);
       //TEST*/

       *rx_data_len = rx_buffer[N_DATA_LEN];

       printf("Send ok: data len = %d\n", rx_buffer[N_DATA_LEN]);

       return R245_OK;
    }
    else
    {
        printf("Error: read");
        return R245_ERROR;
    }
}

R245_API FT_STATUS R245_AuditEn(unsigned char num_dev,
        unsigned char enable)
{
    unsigned char rx_data_len = 0;
    R245_DEV_INFO info;

    R245_GetDevInfo(num_dev, &info);

    return R245_PacketSend(info.ft_handle, DEV_ADDR, AUDIT_EN, &enable, 1,
            NULL, &rx_data_len);
}

R245_API FT_STATUS R245_GetVersion(FT_HANDLE ft_handle, unsigned char dev_addr,
        unsigned char *version)
{
    FT_STATUS ft_status;
    unsigned char rx_data_len = 0;

    ft_status = R245_PacketSend(ft_handle, dev_addr, GET_VERSION, 
            NULL, 0, version, &rx_data_len);

    return ft_status;
}

R245_API FT_STATUS R245_GetNumTrans(unsigned char num_dev,
        short unsigned int * num_trans)
{
    FT_STATUS ft_status;
    R245_DEV_INFO info;

    R245_GetDevInfo(num_dev, &info);

    unsigned char data[2];
    unsigned char rx_data_len = 0;

    ft_status = R245_PacketSend(info.ft_handle, DEV_ADDR, GET_NUM_TRANS, NULL,
            0, data, &rx_data_len);

    if(!ft_status && rx_data_len == 2)
    {
        *num_trans = ((short unsigned int)data[0] << 8) | data[1];
    }

    return ft_status;
}

R245_API FT_STATUS R245_GetDamp(unsigned char num_dev,
        unsigned char num_ch, unsigned char * damp)
{
    FT_STATUS ft_status;
    R245_DEV_INFO info;

    R245_GetDevInfo(num_dev, &info);

    unsigned char rx_data_len = 0;
    unsigned char cmd = (num_ch == 1)? GET_DAMP_1: GET_DAMP_2;

    ft_status = R245_PacketSend(info.ft_handle, DEV_ADDR, cmd, NULL,
            0, damp, &rx_data_len);
    if(!ft_status)
    {
        printf("GET DAMP OK\n");
    }

    return ft_status;
}

R245_API FT_STATUS R245_SetDamp(unsigned char num_dev,
        unsigned char num_ch, unsigned char damp)
{
    FT_STATUS ft_status;
    R245_DEV_INFO info;

    R245_GetDevInfo(num_dev, &info);

    unsigned char rx_data_len = 0;
    unsigned char cmd = (num_ch == 1)? SET_DAMP_1: SET_DAMP_2;

    ft_status = R245_PacketSend(info.ft_handle, DEV_ADDR, cmd, &damp,
            1, NULL, &rx_data_len);

    if(!ft_status)
    {
        printf("SET DAMP OK\n");
    }

    return ft_status;
}

R245_API FT_STATUS R245_GetTime(unsigned char num_dev,
        unsigned char num_ch, short int *time)
{
    FT_STATUS ft_status;
    R245_DEV_INFO info;

    R245_GetDevInfo(num_dev, &info);

    unsigned char data[2];
    unsigned char rx_data_len = 0;
    unsigned char cmd = (num_ch == 1)? GET_TIME_1: GET_TIME_2;

    ft_status = R245_PacketSend(info.ft_handle, DEV_ADDR, cmd, NULL,
            0, data, &rx_data_len);

    if(!ft_status && rx_data_len == 2)
    {
        *time = ((short unsigned int)data[1] << 8) | data[0];
        printf("GET Time OK\n");
    }

    return ft_status;
}

R245_API FT_STATUS R245_SetTime(unsigned char num_dev,
        unsigned char num_ch, short int time)
{
    FT_STATUS ft_status;
    R245_DEV_INFO info;

    R245_GetDevInfo(num_dev, &info);

    unsigned char rx_data_len = 0;
    unsigned char cmd = (num_ch == 1)? SET_TIME_1: SET_TIME_2;
    unsigned char data[2];

    data[0] = (unsigned char) time;
    data[1] = (unsigned char) (time >> 8);

    ft_status = R245_PacketSend(info.ft_handle, DEV_ADDR, cmd, data,
            2, NULL, &rx_data_len);

    if(!ft_status)
    {
        printf("Set Time OK\n");
    }

    return ft_status;
}

// Возвращаемое значениен:
//    ch = 0 - каналы не активны
//    ch = 1 - канал 1 активен
//    ch = 2 - канал 2 активен
//    ch = 3 - каналы 1, 2 активны
R245_API FT_STATUS R245_GetChan(unsigned char num_dev, unsigned char * ch)
{
    FT_STATUS ft_status;
    R245_DEV_INFO info;

    R245_GetDevInfo(num_dev, &info);

    unsigned char data[16];
    unsigned char rx_data_len = 0;
    unsigned char channel = 0;

    ft_status = R245_PacketSend(info.ft_handle, DEV_ADDR, READ_CFG, NULL,
            0, data, &rx_data_len);

    if(!ft_status && rx_data_len == 16)
    {
        if(data[ACCESS_OPTION_1] && (1 << CHANELL_IS_ACTIVE))
        {
            channel |= 0x01;
        } else
        {
            channel &= 0xFE;
        }

        if(data[ACCESS_OPTION_2] && (1 << CHANELL_IS_ACTIVE))
        {
            channel |= 0x02;
        } else
        {
            channel &= 0xFD;
        }
        printf("GET Chan OK\n");
    }

    *ch = channel;



    return ft_status;
}

R245_API FT_STATUS R245_SetChan(unsigned char num_dev,
        unsigned char num_ch, unsigned char enable)
{
    FT_STATUS ft_status;
    R245_DEV_INFO info;

    R245_GetDevInfo(num_dev, &info);

    unsigned char rx_data_len = 0;
    unsigned char cmd = (num_ch == 1)? SET_CHAN_1: SET_CHAN_2;

    ft_status = R245_PacketSend(info.ft_handle, DEV_ADDR, cmd, &enable,
            1, NULL, &rx_data_len);

    if(!ft_status)
    {
        printf("Set Chan OK\n");
    }

    return ft_status;
}

R245_API FT_STATUS R245_GetTransact(unsigned char num_dev,
        R245_TRANSACT * trans)
{
    FT_STATUS ft_status;
    R245_DEV_INFO info;

    R245_GetDevInfo(num_dev, &info);

    unsigned char data[17];
    unsigned char rx_data_len = 0;

    ft_status = R245_PacketSend(info.ft_handle, DEV_ADDR, GET_TRANS, NULL,
            0, data, &rx_data_len);

    if(!ft_status)
    {

        if(rx_data_len > 0 && !ft_status)
        {
            trans->code = ((short unsigned int)data[1] << 8) | data[0];
            trans->channel = data[2];
            trans->tid = (data[7] << 24) | (data[6]<<16) | (data[5]<<8) | data[4];
            trans->day = data[8];
            trans->month = data[9];
            trans->year = (data[11] << 8) | data[10];
            trans->sec = data[12];
            trans->min = data[13];
            trans->hour = data[14];
            trans->dow = data[15];

            return ft_status;
        }
    }

    return R245_ERROR;
}
