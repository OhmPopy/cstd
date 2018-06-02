#ifndef DEVMAN_H__
#define DEVMAN_H__


#define DEVMANSOFTVER "V1.0.1"
#define DEVMANHARDVER "V1.0.1"

/*
 * �����ע���ģ���
 */

#define MOD_MAIN            1
#define MOD_CHARGER         10
#define MOD_PRINTERID       11
#define MOD_NFCID           12
#define MOD_PAYID           13
#define MOD_SCANQRCODEID    14
#define MOD_READQRCODEID    15

#define MOD_NMSID           100
#define MOD_TIMEDID         101
#define MOD_LOCALID         102
#define MOD_WIFIPROBE       103
#define MOD_NMSCLIENTID     104
#define MOD_SYSMONITOR      105
#define MOD_REMOTECTRL      106
#define MOD_SCRRENSHOT      107

#define SETDEVID(HOST,MOD,DEV)  ((HOST<<24)|((MOD)<<16)|(DEV))      // ����DEVID����
#define GETHOSTID(devid)        (((devid) >> 24) & 0x000000FF)      // ��DEVID��������ַ
#define GETMODID(devid)         (((devid) >> 16) & 0x000000FF)      // ��DEVID��ģ���ַ
#define GETDEVID(devid)         ((devid) & 0x0000FFFF)              // ��DEVID�����豸��ַ

/*
 * ����Ӧ�ò���Ϣ�ṹ��
 * ----��ֵַ˵��:[0]��Чֵ
 */
typedef enum __MSGTYPE {
  /*
   *  ϵͳ����Ϣ
   */
  DEVDPCTRL  = 1,         // �豸������Ϣ
  SYSEXIT,                // �˳�Ӧ�ó���
  MSG_ACK    = 0x41,
  MSG_ERR    = 0x45,
  MSG_REQACK = 0x51,
  MSG_REQ    = 0x71,
  /*
   *  ϵͳ�澯��Ϣ
   */
  DEVWARNING = 0x100,     // �豸�澯��Ϣ
  /*
   *  ��ʱģ����Ϣ
   */
  MSG_TIMEDADD = 0x200,
  MSG_TIMEDDEL,
} MSGTYPE;

typedef enum __DATATYPE {
  TYPE_NULL = 0,
  TYPE_S8,
  TYPE_U8,
  TYPE_S16,
  TYPE_U16,
  TYPE_S32,
  TYPE_U32,
  TYPE_S64,
  TYPE_U64,
  TYPE_F32,
  TYPE_F64,
  TYPE_DATE,
  TYPE_HEX,
  TYPE_STRING,
  TYPE_JSON,
  TYPE_UDF = 0xFF
} DATATYPE;

#pragma pack(1)
typedef struct __SMSGPACK {
  unsigned int    SrcDevID;       // Դ�豸��ַ
  unsigned int    DesDevID;       // Ŀ���豸��ַ
  MSGTYPE         MsgType;        // ��Ϣ����
  int             Datalen;        // ����������
  DATATYPE        DataType;       // ��������
  char            Data[0];        // ������
} SMSGPACK;

#pragma pack()

#define MsgPackDebug(pInMsg) \
  do{    \
    MDEBUG(DDEBUG,"[%x]:[%x-%x]:%s\n", \
        pInMsg->MsgType,\
        pInMsg->SrcDevID,\
        pInMsg->DesDevID,\
        (pInMsg->DataType)?pInMsg->Data:"NULL");\
  }while(0)

#define GETMSGPACKLEN(datalen) (datalen+sizeof(SMSGPACK))                       // ��ȡ��Ϣ���ܳ���
//#define MallocMsgPack(datalen) ((SMSGPACK *)malloc(GETMSGPACKLEN(datalen) + 1)) // ����ռ�

static SMSGPACK* MallocMsgPack(int datalen)
{
  int len = GETMSGPACKLEN(datalen) + 1;
  SMSGPACK* p = (SMSGPACK*)malloc(len);
  memset(p, 0, len);
  return p;
}

typedef struct __ModuleReg {
  const unsigned int ModeId;      // ע��ģ��ID
  const char* pName;              // ģ����
  int(*moduleInit)(void*);        // ģ���ʼ��
  int(*msgProc)(void*, int);      // ���ݴ����ýӿ����յ���ģ����ص�������Ϣ
  int(*moduleExit)(void);         // ģ���˳�
} ModuleReg;

/*
 * ģ����Ϣ���͵��ϲ㴦��
 */
//int SendMsg2Main(char* pData,int len);

/*
 * ģ����Ϣֱ�ӷ��͵�����
 */
//extern int32_t NMSClientSendMsg(SMSGPACK *pMsgPack);

#endif

