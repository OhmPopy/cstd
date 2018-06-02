#include "winsock2.h"  
#include <iostream>  
#pragma comment(lib, "ws2_32.lib")  

using namespace std;  

#define M_AUTH        0x07                //�ͻ��������֤
#define M_HEART       0x09               //������
#define M_VIDEO_OUT   0x23               //UDP������Ƶ���� NET_ADDRESS

//����������
#define S_SET             0x81                //������Ƶ
#define S_DEL             0x82                //�ر���Ƶ

#define F_REQ_FLAG		0x40000000  //����ռ�ô�λ
#define F_REPLY_FLAG	    0x20000000  //����ռ�ô�λ
//�û���֤
#define AUTH_VERIFY       0x00000100   //�û������֤

#define PACK_SIZE (60*1024) //һ����Ϊ60��С

//����ṹ�壬�����ϴ����ص���������,ͨ�������ݽṹ�������������
typedef struct
{
	unsigned int m_MCmd;           //������
	unsigned int m_SCmd;           //������
	unsigned int m_CFlag;            //�ض��������ò��������һ���ֽ�Ϊ����ִ��״̬��
	unsigned int m_Param1;         //�û��Զ������1
	unsigned int m_Param2;         //�û��Զ������2
}NCMD;


//Sock���ݰ�ͷ
typedef struct 
{
	UINT32 msg_identify; //��ʶ
	UINT32 msg_offset ;  //ƫ��
	UINT32 msg_len;      //���ݳ���
	UINT32 msg_total;    //�����ܳ���
	UINT32 msg_checksum; //�����
	//	int8   msg_data[4];  //����
}SOCK_DATA_HEADER;	


typedef struct
{
	unsigned short   Port;                //�˿�
	char           IP_Address[16];      //���ʮ����IP��ַ
}SOCK_ADDRESS, *PSOCK_ADDRESS;


#define DEV_ID_LEN 24
typedef struct
{
	char m_ID[ DEV_ID_LEN ];
}DEV_CID; //ID��ʶ�ṹ��
//�ͻ��������֤
typedef struct
{
	DEV_CID m_UserName;        //�û���
	DEV_CID m_Password;        //����
}DEV_AUTH;

#define IMAGE_UYVY   0x01
#define IMAGE_YUYV   0x02
#define IMAGE_RGB24  0x03
typedef struct
{
	//ͼ�������ܴ�СBYTES
	int     m_Size;
	//ͼ���
	int     m_W;
	//ͼ���
	int     m_H;
	//�����ݲ���
	int     m_Step;
	//ͼ������
	char    m_Type;
	//ͼ������
	char    m_Block[3];
}DEVIMAGE;

typedef struct  
{
	DEV_CID  m_IP;
	SOCKET   m_hSck;
}ENDPNT;

static int g_nIndex = 0;//��������
static bool g_Author = false;//��½�豸Ȩ����֤ͨ����־
static bool g_VideoClose  = false ;//�ر���Ƶ��־

 
