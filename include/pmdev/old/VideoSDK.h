
//////////////////////////////////////////////////////////////////////

#ifndef _VIDEOSDK_H_
#define _VIDEOSDK_H_

#include <assert.h>
#include "net.h"
#include "thread.h"
#include "thread.h"


#define M_AUTH      0x07 //�ͻ��������֤
#define M_HEART     0x09 //������
#define M_VIDEO_OUT 0x23 //UDP������Ƶ���� NET_ADDRESS

//����������
#define S_SET 0x81 //������Ƶ
#define S_GET 0x83 //�ر���Ƶ

#define F_REQ_FLAG    0x40000000 //����ռ�ô�λ
#define F_REPLY_FLAG  0x20000000 //����ռ�ô�λ

//�û���֤
#define AUTH_VERIFY 0x00000100 //�û������֤

#define PACK_SIZE (60*1024) //һ����Ϊ60��С

#define ALIGN(bytes, alignments) (((bytes)+((alignments)-1))&~((alignments)-1))


//����ṹ�壬�����ϴ����ص���������,ͨ�������ݽṹ�������������
typedef struct {
  UINT32 m_MCmd; //������
  UINT32 m_SCmd; //������
  UINT32 m_CFlag; //�ض��������ò��������һ���ֽ�Ϊ����ִ��״̬��
  UINT32 m_Param1; //�û��Զ������1
  UINT32 m_Param2; //�û��Զ������2
} NCMD;


//Sock���ݰ�ͷ
typedef struct {
  UINT32 msg_identify; //��ʶ
  UINT32 msg_offset ; //ƫ��
  UINT32 msg_len; //���ݳ���
  UINT32 msg_total; //�����ܳ���
  UINT32 msg_checksum; //�����
  //  int8 msg_data[4]; //����
} SOCK_DATA_HEADER;


typedef struct {
  unsigned short Port; //�˿�
  char IP_Address[16]; //���ʮ����IP��ַ
} SOCK_ADDRESS, *PSOCK_ADDRESS;


#define DEV_ID_LEN 24
typedef struct {
  char m_ID[ DEV_ID_LEN ];
} DEV_CID; //ID��ʶ�ṹ��

//�ͻ��������֤
typedef struct {
  DEV_CID m_UserName; //�û���
  DEV_CID m_Password; //����
} DEV_AUTH;

#define IMAGE_UYVY 0x01
#define IMAGE_YUYV 0x02
#define IMAGE_RGB24 0x03
typedef struct {
  //ͼ�������ܴ�СBYTES
  INT32 m_Size;
  //ͼ���
  INT32 m_W;
  //ͼ���
  INT32 m_H;
  //�����ݲ���
  INT32 m_Step;
  //ͼ������
  char m_Type;
  //ͼ������
  char m_Block[3];
} DEVIMAGE;

typedef int (*frame_fun_t)(int h, int w, const unsigned char* img, int step, int cn);

typedef struct VideoPro {
  DEV_CID m_IP;
  SOCKET m_sHost;//TCP�׽���,���豸�������ӣ����豸�˵�ʵʱ����
  SOCKET m_sVVIDEOSck;//UDP�׽��֣������豸���͹�����ʵʱ��Ƶ����

  int h, w;
  int m_nIndex;//��������
  int m_Author;//��½�豸Ȩ����֤ͨ����־
  int m_VideoClose;//�ر���Ƶ��־
  int m_exitThread;
  thread_t m_hTCPThread;//����ʵʱ������Ӧ���ݵ��߳�
  thread_t m_hUDPThread;//����ʵʱ��Ƶ���ݵ��߳�
  frame_fun_t m_framefun;
  void* userdata;
  //int m_nTCPPort;//TCP�˿� 13333
  int m_nUDPPort;//UDP�˿�,���ѡ��Ŀ��ö˿�
  char m_strDevIP[32];//�豸IP
  char m_strClientIP[32];//����IP
  int cn;
  int flipud;
  unsigned char* pRGBTMP;
  unsigned char* pRGBTRANS;
  unsigned char* buf;
  int run;
} VideoPro;

#define TCPPORT   13333
#define ERR_VERIFY_FAIL 0x06 //��֤ʧ��
#define SUC_EXECUT      0x00 //�ɹ�
#define GET_ERR( state )   ( (state) & 0xff )

//��ͷ�̶��̶�Ϊ20
#define OFFSET_SIZE 20

#endif // _VIDEOSDK_H_
