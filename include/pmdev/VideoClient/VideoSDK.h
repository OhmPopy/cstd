// VideoSDK.h: interface for the VideoSDK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIDEOSDK_H__375BCE24_70DD_4474_93EE_2D2F8C629C3B__INCLUDED_)
#define AFX_VIDEOSDK_H__375BCE24_70DD_4474_93EE_2D2F8C629C3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winsock2.h"  
#include <iostream>  
#include <ASSERT.H>
#include "yuv2bmp.h"
#pragma comment(lib, "ws2_32.lib")  
#include   <map> 
using namespace std;  



#define M_AUTH        0x07                //�ͻ��������֤
#define M_HEART       0x09               //������
#define M_VIDEO_OUT   0x23               //UDP������Ƶ���� NET_ADDRESS

//����������
#define S_SET           0x81                //������Ƶ
#define S_GET           0x83                      //�ر���Ƶ

#define F_REQ_FLAG		0x40000000  //����ռ�ô�λ
#define F_REPLY_FLAG	0x20000000  //����ռ�ô�λ

//�û���֤
#define AUTH_VERIFY     0x00000100   //�û������֤

#define PACK_SIZE (60*1024) //һ����Ϊ60��С

#define ALIGN(bytes, alignments) (((bytes)+((alignments)-1))&~((alignments)-1)) 


//����ṹ�壬�����ϴ����ص���������,ͨ�������ݽṹ�������������
typedef struct
{
	UINT32 m_MCmd;           //������
	UINT32 m_SCmd;           //������
	UINT32 m_CFlag;            //�ض��������ò��������һ���ֽ�Ϊ����ִ��״̬��
	UINT32 m_Param1;         //�û��Զ������1
	UINT32 m_Param2;         //�û��Զ������2
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
	INT32     m_Size;
	//ͼ���
	INT32     m_W;
	//ͼ���
	INT32     m_H;
	//�����ݲ���
	INT32     m_Step;
	//ͼ������
	char      m_Type;
	//ͼ������
	char      m_Block[3];
}DEVIMAGE;

typedef struct  
{
	DEV_CID  m_IP;
	SOCKET   m_hSck;
}ENDPNT;

struct VideoPro
{
	BYTE* m_pRGBTMP;
	BYTE* m_pRGBTRANS;
	BITMAPINFO* m_pBitmapInfo;
	HWND  m_hWnd;
	RECT  m_Rect;
};


#define ERR_VERIFY_FAIL  0x06        //��֤ʧ��
#define SUC_EXECUT       0x00        //�ɹ�
#define GET_ERR( state ) ( (state) & 0xff )

class VideoSDK  
{
public:
	VideoSDK();
	virtual ~VideoSDK();

public:
	//��ʼ��
	bool  SDKInit(HWND hWnd, CString strDevIP, CString strClientIP, int nTCPPort);


	//���豸��������
	bool   SDKConnect();

	//��½�豸
	bool   SDKLogin(CString sUserName, CString strPassword);

	//������Ƶ
	bool   SDKStartVideo();

	//ֹͣ��Ƶ
	bool   SDKStopVideo();

	//�Ͽ�����
	void   SDKDisConnect();

	//����ʼ��
    void   SDKUInit()
	{
		SDKStopVideo();//�ر���Ƶ
		SDKDisConnect();//�ر�����
		WSACleanup();//�ͷ��׽�����Դ  
	}
public:
	static int    g_nIndex;//��������
	static bool   g_Author;//��½�豸Ȩ����֤ͨ����־
	static bool   g_VideoClose;//�ر���Ƶ��־
    static bool   g_exitThread;
	static HWND   g_hVideoWND;//������Ƶ�Ĵ��ھ��
	static HANDLE g_hTCPThread;//����ʵʱ������Ӧ���ݵ��߳�
	static HANDLE g_hUDPThread;//����ʵʱ��Ƶ���ݵ��߳�
private:
	SOCKET        m_sHost;//TCP�׽���,���豸�������ӣ����豸�˵�ʵʱ����
    SOCKET        m_sVVIDEOSck;//UDP�׽��֣������豸���͹�����ʵʱ��Ƶ����

private:
	CString m_strDevIP;//�豸IP
	CString m_strClientIP;//����IP
	int m_nTCPPort;//TCP�˿�
	unsigned short m_nUDPPort;//UDP�˿�,���ѡ��Ŀ��ö˿�
};

#endif // !defined(AFX_VIDEOSDK_H__375BCE24_70DD_4474_93EE_2D2F8C629C3B__INCLUDED_)
