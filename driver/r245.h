#define GET_VERSION 	0x28
#define SET_TIME		0xC9
#define SET_DATE		0xCA
#define GET_TIME		0xCB
#define GET_DATE		0xCC
#define SET_ADRESS		0x1E
#define GET_SENSORS		0xB6
#define GET_NUM_TAGS_1	0x50
#define GET_FIRST_TAG_1	0x51
#define GET_NEXT_TAG_1	0x52
#define GET_NUM_TAGS_2	0x53
#define GET_FIRST_TAG_2	0x54
#define GET_NEXT_TAG_2	0x55
#define SET_DAMP_1		0x0C
#define SET_DAMP_2		0x0D
#define GET_DAMP_1		0x12
#define GET_DAMP_2		0x13
#define SET_TIME_1		0x19
#define SET_TIME_2		0x1A
#define GET_TIME_1		0x1B
#define GET_TIME_2		0x1C
#define SET_CHAN_1		0xCD
#define SET_CHAN_2		0xCE
#define AUDIT_EN		0xB5
#define CLEAR_TRANS		0x78
#define GET_NUM_TRANS	0x79
#define GET_TRANS		0x7A

void PacketForm(unsigned short int cmd, unsigned short int datalen, unsigned short int data, void * packet);
void CRCCount(int init_crc, void * data, int len);
void PacketSend(void * data, unsigned short int adress);
