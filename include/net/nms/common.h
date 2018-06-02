#ifndef _RYCOMMON_H__
#define _RYCOMMON_H__

#include <stdio.h>
#include <stdlib.h>
//#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <unistd.h>
#include <string.h>
#include <errno.h>
//#include <sys/ioctl.h>
//#include <sys/wait.h>
//#include <sys/ipc.h>
//#include <sys/msg.h>
//#include <arpa/inet.h>
#include <pthread.h>
//#include <syslog.h>
//#include "dataqueue.h"
//#include "jsDevParamConfig.h"

#ifndef _WIN32
#define RYW_ANDROID
#endif // 


#ifndef BOOL
  //#define BOOL      BOOLEAN
#endif

#ifndef FALSE
#define FALSE 0
#define TRUE  1
#endif

static const char* gsDebugType[] = {
  "EMERG",        // ����
  "ALERT",        // ����
  "CRIT",
  "ERROR",        // ����
  "WARNING",      // ����
  "DNOTICE",
  "INFO",         // ��Ϣ
  "DEBUG",        // ����
};

typedef enum __DEBUGLEVEL {
  DEMERG = 0,      // ����
  DALERT,          // ����
  DCRIT,
  DERR,            // ����
  DWARNING,        // ����
  DNOTICE,
  DINFO,           // ��Ϣ
  DDEBUG,          // ����
} DEBUGLEVEL;

#ifdef RYW_ANDROID
#include <android/log.h>

static int gsAndroidDebugType[] = {
  ANDROID_LOG_SILENT,
  ANDROID_LOG_FATAL,
  ANDROID_LOG_VERBOSE,
  ANDROID_LOG_ERROR,
  ANDROID_LOG_WARN,
  ANDROID_LOG_DEFAULT,
  ANDROID_LOG_INFO,
  ANDROID_LOG_DEBUG,
  ANDROID_LOG_UNKNOWN
};
static char gcDbugLevel = 0;
#define MDEBUG(debug,format,...) \
do{ \
  if((debug)<=gcDbugLevel) \
  { \
    __android_log_print(gsAndroidDebugType[debug],"RYWNMS","%s(%d): "format,__FILE__,__LINE__,##__VA_ARGS__); \
  } \
}while(0)
#else
#if 1
#define MDEBUG     ___FILE = __FILE__, ___LINE = __LINE__; MDEBUG_
static const char* ___FILE = 0;
static int ___LINE = 0;
#ifdef _DEBUG
static int gcDbugLevel = DDEBUG;
#else
static int gcDbugLevel = 0;
#endif
int MDEBUG_(int debug, const char* fmt, ...)
{
  if ((debug) <= gcDbugLevel) {
    char dbg_buf[1024];
    va_list arglist;
    printf("<%s>%s(%d):", gsDebugType[debug], ___FILE, ___LINE);
    va_start(arglist, fmt);
    vsnprintf(dbg_buf, countof(dbg_buf), fmt, arglist);
    va_end(arglist);
    printf("%s", dbg_buf);
  }

  return 0;
}
#endif
#if 0
#define MDEBUG(debug,...) \
do{ \
  if((debug)<=gcDbugLevel) \
  { \
    printf("<%s>%s(%d):",gsDebugType[debug],__FILE__,__LINE__); \
    printf(__VA_ARGS__); \
  } \
}while(0)
#endif
#if 0
#define MDEBUG(debug,format,...) \
if((debug)<=gcDbugLevel) \
{ \
  syslog(debug,"<%s> %s(%d): "format,gsDebugType[debug],__func__,__LINE__,##__VA_ARGS__); \
}
#endif
#endif

typedef enum _PACKETTYPE {
  TYPE_REQ    = 0x71,
  TYPE_REQACK = 0x51,
  TYPE_ACK    = 0x41,
  TYPE_ERROR  = 0x45,
} PACKETTYPE;

typedef enum _PACKETERR {
  ERR_PACKFORMAT  = 0x01,
  ERR_DEVOFFLINE,
  ERR_MODOFFLINE,
  ERR_DATAFORMAT,
  ERR_LOGIN
} PACKETERR;


#define AccessNull(p,r) if(!(p)) { \
  MDEBUG(DDEBUG,"Check false\n"); \
  return (r);}

#define FreePointer(pMsg)   if(pMsg)free(pMsg);pMsg=NULL

int32_t  SprintfHex(char* pInHex, int32_t HexLen, char* pOutStr, int32_t buflen);
uint8_t  Crc8Calc(char* buf, uint32_t len);
uint16_t Crc16Calc(char* buf, uint32_t len);
uint32_t Crc32Calc(char* buf, uint32_t len);
void ThreadExitHandle(int32_t sig);

#define TSPORT               60000                  // ֱ���������˿�
#define NETCONNECT_TIMEOUT  (3*60*1000)             // �������ӳ�ʱʱ��
#define SOCKETBUFFSIZE      (800)
#define MSGHEADLEN          sizeof(SMSGBUFF)
//int SrcModID = 0;
//#define DEVMANMODID         SrcModID
#pragma pack(1)
typedef struct __SMSGBUFF {
  uint8_t         head[2];
  uint8_t         type;
  uint8_t         reserved[5];
  uint8_t         SrcDevID[16];   // Դ�豸��ַ
  uint8_t         DesDevID[16];   // Ŀ���豸��ַ
  uint8_t         SrcModID[2];    // Դģ��
  uint8_t         DesModID[2];    // Ŀ��ģ��
  uint8_t         Index[4];       // ��ϢID
  uint8_t         DataLen[4];     // ����������
  char            Data[0];        // ������
} SMSGBUFF;
#pragma pack()

#define SETMSGBUFFVALUE(pmsg,msgtype,pSguid,pDguid,smod,dmod,index,datalen,pData) {\
  char *p = (char *)pData; \
  memset((char *)(pmsg),0,sizeof(SMSGBUFF)); \
  (pmsg)->head[0] = 0x52; (pmsg)->head[1] = 0x31; \
  (pmsg)->type    = (msgtype);   \
  if((pSguid) != NULL)memcpy((pmsg)->SrcDevID,(pSguid),16); \
  if((pDguid) != NULL)memcpy((pmsg)->DesDevID,(pDguid),16); \
  (pmsg)->SrcModID[0]  = ((smod) >> 8) & 0xff; \
  (pmsg)->SrcModID[1]  = (smod) & 0xff; \
  (pmsg)->DesModID[0]  = ((dmod) >> 8) & 0xff; \
  (pmsg)->DesModID[1]  = (dmod) & 0xff; \
  (pmsg)->Index[0] = ((index) >> 24) & 0xff;  \
  (pmsg)->Index[1] = ((index) >> 16) & 0xff;  \
  (pmsg)->Index[2] = ((index) >> 8) & 0xff;  \
  (pmsg)->Index[3] = (index) & 0xff;  \
  (pmsg)->DataLen[0] = ((datalen) >> 24) & 0xff;  \
  (pmsg)->DataLen[1] = ((datalen) >> 16) & 0xff;  \
  (pmsg)->DataLen[2] = ((datalen) >> 8) & 0xff;  \
  (pmsg)->DataLen[3] = (datalen) & 0xff;  \
  if(p && datalen) memcpy((pmsg)->Data,p,datalen); \
}

#define MsgBuffDebug(dir,pInMsg) \
do{    \
  int index = ((pInMsg)->Index[0]<<24L) + ((pInMsg)->Index[1]<<16L) + ((pInMsg)->Index[2]<<8L) + (pInMsg)->Index[3]; \
  int datalen = ((pInMsg)->DataLen[0]<<24L) + ((pInMsg)->DataLen[1]<<16L) + ((pInMsg)->DataLen[2]<<8L) + (pInMsg)->DataLen[3]; \
  MDEBUG(DDEBUG,"%s:[%X]%c:[%x...%x]-[%x...%x] [%d]-[%d]:datalen=%d\n", \
      dir,index, \
      (pInMsg)->type,\
      (pInMsg)->SrcDevID[0],(pInMsg)->SrcDevID[15],\
      (pInMsg)->DesDevID[0],(pInMsg)->DesDevID[15],\
      ((pInMsg)->SrcModID[0]<<8L) + (pInMsg)->SrcModID[1],\
      ((pInMsg)->DesModID[0]<<8L) + (pInMsg)->DesModID[1],\
      datalen);\
}while(0)

#ifdef _WIN32
#define usleep delay_us
#define sleep(s) Sleep(1000*s)
static void delay_us(int us)
{
  LARGE_INTEGER litmp;
  LONGLONG QPart1, QPart2;
  double dfMinus, dfFreq, dfTim, dus = us / (1000000.);
  QueryPerformanceFrequency(&litmp);
  dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
  QueryPerformanceCounter(&litmp);
  QPart1 = litmp.QuadPart;// ��ó�ʼֵ

  do {
    QueryPerformanceCounter(&litmp);
    QPart2 = litmp.QuadPart;//�����ֵֹ
    dfMinus = (double)(QPart2 - QPart1);
    dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
  }
  while (dfTim < dus);
}
#endif // _WIN32

#endif

