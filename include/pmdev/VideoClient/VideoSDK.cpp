// VideoSDK.cpp: implementation of the VideoSDK class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VideoClient.h"
#include "VideoSDK.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int     VideoSDK::g_nIndex      = 0;//��������
bool    VideoSDK::g_Author      = false;//��½�豸Ȩ����֤ͨ����־
bool    VideoSDK::g_VideoClose  = false ;//�ر���Ƶ��־
bool    VideoSDK::g_exitThread  = false;
HWND    VideoSDK::g_hVideoWND   = NULL;
HANDLE  VideoSDK::g_hTCPThread  = NULL;
HANDLE  VideoSDK::g_hUDPThread  = NULL;

static DWORD WINAPI RecvVideoDataThread( void* p );

unsigned long _inet_addr( const char* ip )
{
	if ( NULL == ip ) return 0;
	unsigned long a = 0 ;
	unsigned long b = 0;
	unsigned long c = 0;
	unsigned long d = 0;
	sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
	unsigned long ipInt = 0;
	BYTE* pInt = (BYTE*)(&ipInt);
	pInt[0] = a;
	pInt[1] = b;
	pInt[2] = c;
	pInt[3] = d;
	return ipInt;
}

//��ʼ��
bool Init()
{
	WSADATA wsd; //WSADATA����  
	//��ʼ���׽��ֶ�̬��  
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)  
	{  
		cout << "WSAStartup failed!" << endl;  
		return false;  
	}  
    
	//��ʼ��YVUתRGB�ı���ʾ��Ƶ�����ȳ�ʼ����
	InitialTable();
    return true;
} 

// ���豸��������
SOCKET ConnectServer( char* sSeverIP, int nPort )
{ 
	SOCKET sHost = 0;

    SOCKADDR_IN servAddr; //��������ַ  
	//�����׽���  
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
	if(INVALID_SOCKET == sHost)  
	{  
		cout << "socket failed!" << endl;  
		WSACleanup();//�ͷ��׽�����Դ  
		return  0;  
	}  

	//���÷�������ַ  
	servAddr.sin_family      = AF_INET;  
	servAddr.sin_addr.s_addr = inet_addr(sSeverIP);  
	servAddr.sin_port        = htons((short)nPort);  
	int nServAddlen          = sizeof(servAddr);  

	//���ӷ�����  
    int	retVal=connect(sHost,(LPSOCKADDR)&servAddr, sizeof(servAddr));  
	if(SOCKET_ERROR == retVal)  
	{  
		cout << "connect failed!" << endl;  
		closesocket(sHost); //�ر��׽���  
		WSACleanup(); //�ͷ��׽�����Դ  
		return 0;  
	} 

	return sHost;
}


//�رպ��豸������
void DisConnectServer(SOCKET& sHost)
{ 
	if (sHost)
	{
		closesocket(sHost); //�ر��׽��� 
		sHost = NULL;
	}
}

//У��ͼ���
UINT16 sock_checksum( UINT16 *buffer, UINT32 size )
{
    UINT32 cksum = 0;
    while(size >1)
	{ 
        cksum += *buffer++; 
        size  -= sizeof(UINT16); 
    }     
    if(size )
	{ 
        cksum += *(UINT8*)buffer; 
    } 
    cksum = (cksum >> 16) + (cksum & 0xffff); 
    cksum += (cksum >>16); 
    return (UINT16)(~cksum); 
}

//��ͷ�̶��̶�Ϊ20
#define OFFSET_SIZE 20

//��װ���������
bool NETCMDPacket(const char*  pCMDPacketData, const int nlen, const int nSeq)
{
	//�Ӱ�ͷ
	SOCK_DATA_HEADER* hdr = (SOCK_DATA_HEADER*)(pCMDPacketData);
	hdr->msg_identify = nSeq;		//��ʶ
	hdr->msg_offset	  = 0 ;	//ƫ��
	hdr->msg_len	  = nlen;//���Ѿ����͹������������ݳ��ȣ���������ͷ�ĳ��ȣ������������ͷ����      
	hdr->msg_total	  = nlen;//�ܵ��������ݳ��ȣ���������ͷ���Ⱥ�����Ĳ������� 	    
	hdr->msg_checksum = 0;//�����
    hdr->msg_checksum = sock_checksum( (UINT16*)hdr, sizeof(SOCK_DATA_HEADER) );

	return true;
}

//��½�豸
bool  Login(const SOCKET sHost,  const char* pServerIP, const char* sLoginName, const char* sPassword)
{
	char pBuffer[PACK_SIZE] = { 0 };//���60K��Сһ����
	//������ͷ
	NCMD*  pCmd    = (NCMD*)(pBuffer+OFFSET_SIZE);
	pCmd->m_MCmd   = M_AUTH;
	pCmd->m_SCmd   = S_SET;
	pCmd->m_CFlag  = F_REQ_FLAG;
	pCmd->m_CFlag |= AUTH_VERIFY;
	pCmd->m_Param1 = VideoSDK::g_nIndex++;
	pCmd->m_Param2 = _inet_addr(pServerIP);

	//����������
	DEV_AUTH* pAuth = (DEV_AUTH*)( pCmd + 1) ;
	memcpy( pAuth->m_UserName.m_ID, sLoginName, DEV_ID_LEN); 
	memcpy( pAuth->m_Password.m_ID, sPassword, DEV_ID_LEN); 

	//���ͷ
	NETCMDPacket( pBuffer,  sizeof(NCMD)+sizeof(DEV_AUTH), pCmd->m_Param1);

	//����
	int retVal = send(sHost, pBuffer, sizeof(NCMD) + sizeof(DEV_AUTH) + OFFSET_SIZE, 0);  

	if (SOCKET_ERROR == retVal)  
	{  
		cout << "send Author CMD failed!" << endl;  
		closesocket(sHost); //�ر��׽���  
		WSACleanup(); //�ͷ��׽�����Դ  
		return false;  
	}  

	return true;
}

//����
bool  Heart(const SOCKET sHost,  const char* pServerIP )
{
	char pBuffer[PACK_SIZE] = { 0 };//���60K��Сһ����
	//������ͷ
	NCMD*  pCmd    = ( NCMD* ) ( pBuffer + OFFSET_SIZE );
	pCmd->m_MCmd   = M_HEART;
	pCmd->m_SCmd   = S_SET;
	pCmd->m_Param1 = VideoSDK::g_nIndex++;
	pCmd->m_Param2 = _inet_addr(pServerIP);
	pCmd->m_CFlag  = 0;

	//���ͷ
	NETCMDPacket( pBuffer, sizeof(NCMD) , pCmd->m_Param1);

	//����
	int retVal = send(sHost, pBuffer, sizeof(NCMD) + OFFSET_SIZE, 0);  
	if (SOCKET_ERROR == retVal)  
	{  
		cout << "send Heart CMD failed!" << endl;  
		closesocket(sHost); //�ر��׽���  
		WSACleanup(); //�ͷ��׽�����Դ  
		return false;  
	}  

	return true;
}

SOCKET RunRecvVideoData(const char* pServerIP, const char* pClientIP,unsigned short& nUPDPort)
{
	//����������Ƶ���ݵ�SOKECT
	SOCKET sockSrv               = socket(AF_INET,SOCK_DGRAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr =  inet_addr(pClientIP);
	addrSrv.sin_family           =  AF_INET;
	addrSrv.sin_port             =  htons(0);//�����ȡһ�����õ�UDP�˿�
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	
	int nLen         = sizeof( sockaddr_in );
	sockaddr_in addr = {0};
	if ( SOCKET_ERROR == getsockname( sockSrv, (sockaddr*)&addr, &nLen) ) return 0;
	nUPDPort         =  ntohs( addr.sin_port );//����Ѿ��󶨵Ķ˿ں�
	
	//����������Ƶ�����߳�
	ENDPNT*  pEnPnt = new ENDPNT;
	pEnPnt->m_hSck = sockSrv;
	memcpy( pEnPnt->m_IP.m_ID, pServerIP , DEV_ID_LEN);
	VideoSDK::g_hUDPThread = CreateThread( NULL, 0, RecvVideoDataThread, (void*) pEnPnt,  NULL, NULL );
	return sockSrv;
}

//��Ƶ
SOCKET StartVideo(const SOCKET sHost,  const char* pServerIP, const char* pClientIP,unsigned short & nUPDPort )
{
	//����������Ƶ���ݵ�SOKECT,�Ѿ�����������Ƶ���ݰ����̡߳�
	nUPDPort = 0;
	SOCKET sockSrv = RunRecvVideoData( pServerIP, pClientIP, nUPDPort);

	//���豸�������Ƶ������
	char pBuffer[PACK_SIZE] = { 0 };//���60K��Сһ����
	//������ͷ
	NCMD*  pCmd    = (NCMD*)(pBuffer+OFFSET_SIZE);
    pCmd->m_MCmd   = M_VIDEO_OUT;
	pCmd->m_SCmd   = S_SET;
	pCmd->m_Param1 = VideoSDK::g_nIndex++;
	pCmd->m_Param2 = _inet_addr(pServerIP);
	pCmd->m_CFlag  =  F_REQ_FLAG;//����

	//����������
	SOCK_ADDRESS* Addr = (SOCK_ADDRESS*)(pCmd+1);
	strcpy( Addr->IP_Address, pClientIP );
	Addr->Port = nUPDPort;

	UINT32 len = ALIGN( sizeof(NCMD) + sizeof(SOCK_ADDRESS) , 4 ); //����4�ֽڶ���

	//���ͷ
	NETCMDPacket( pBuffer, len  , pCmd->m_Param1);
	
	//����
	int retVal = send(sHost, pBuffer, len + OFFSET_SIZE, 0);  
	if (SOCKET_ERROR == retVal)  
	{  
		cout << "send StartVideo failed!" << endl;  
		closesocket(sHost); //�ر��׽���  
		WSACleanup(); //�ͷ��׽�����Դ  
		return 0;  
	}  

	return sockSrv;
}

//�ر���Ƶ
bool StopVideo(const SOCKET sHost, const char* pServerIP, const char* pClientIP,unsigned short nUPDPort)
{
	char pBuffer[PACK_SIZE] = { 0 };//���60K��Сһ����

	//������ͷ
	NCMD*  pCmd    = (NCMD*)(pBuffer+OFFSET_SIZE);
    pCmd->m_MCmd   = M_VIDEO_OUT;
	pCmd->m_SCmd   = S_GET;
	pCmd->m_Param1 = VideoSDK::g_nIndex++;
	pCmd->m_Param2 = _inet_addr(pServerIP);
	pCmd->m_CFlag  = F_REQ_FLAG;//����
	
	//����������
	SOCK_ADDRESS* Addr = (SOCK_ADDRESS*)(pCmd+1);
	strcpy( Addr->IP_Address, pClientIP );
	Addr->Port = nUPDPort;
	UINT32 len = ALIGN( sizeof(NCMD) + sizeof(SOCK_ADDRESS) , 4 );//����4�ֽڶ���

	//���ͷ
	NETCMDPacket( pBuffer, len , pCmd->m_Param1);
	
	//����
	int retVal = send(sHost, pBuffer, len+OFFSET_SIZE, 0);  
	if (SOCKET_ERROR == retVal)  
	{  
		cout << "send StopVideo failed!" << endl;  
		closesocket(sHost); //�ر��׽���  
		WSACleanup(); //�ͷ��׽�����Դ  
		return false;  
	}  

	return true;
}

//�����豸������������ͷ
int    ResolveNCMD(const char* pBuffer,const int nLen , int& nRlt )
{
	NCMD* pCMD = (NCMD*)(pBuffer+OFFSET_SIZE);
	nRlt       = GET_ERR(pCMD->m_CFlag);
	return pCMD->m_MCmd;
}



//����ʵʱ���������߳�
static DWORD WINAPI RecvRealControlDataThread( void* p )
{
	char pBuffer[PACK_SIZE] = {0};//���60K��Сһ����

	ENDPNT* pEndPnt =  (ENDPNT*) p;
	SOCKET sHost    =  pEndPnt->m_hSck;
	int nLen = 0;
	int nCmd = 0;
	int nRlt = 0;
	while( !VideoSDK::g_exitThread )
	{
		//��ʼ������
		ZeroMemory(pBuffer, PACK_SIZE);  
		nLen = recv( sHost, pBuffer, PACK_SIZE , 0);
		
		if ( nLen > 0 )//���յ�����
		{
			//������������ݰ�
			nCmd = ResolveNCMD( pBuffer, nLen, nRlt );
			
			if (nCmd == M_AUTH)//��½Ȩ����֤
			{
				if ( nRlt == SUC_EXECUT )//Ȩ����֤�ɹ�
					VideoSDK::g_Author = true;
			}

			//����
			if( nCmd == M_HEART)
			{
				if ( nRlt == SUC_EXECUT )
				{
					 if(!Heart(sHost,pEndPnt->m_IP.m_ID))//�ͻ��˻���������Ӧ��
						 break;
				}
			}
			
			//��Ƶ������Ӧ
			if( nCmd == M_VIDEO_OUT)
			{
				if ( nRlt == SUC_EXECUT )
				{
					bool bVideo = false;

				}

			}
		}

		Sleep(50);
	}
	delete pEndPnt;
	VideoSDK::g_exitThread = false;

	return 0;
}


//��ʾ��Ƶ
void  ShowVideo( const DEVIMAGE* pData, VideoPro* pPro)
{
	if ( NULL == pPro->m_pRGBTMP || pData->m_W  != pPro->m_pBitmapInfo->bmiHeader.biWidth || pData->m_H != pPro->m_pBitmapInfo->bmiHeader.biHeight)
	{
		delete [] pPro->m_pRGBTMP;
		delete [] pPro->m_pRGBTRANS;
		free( pPro->m_pBitmapInfo );

		pPro->m_pRGBTMP     = new BYTE[pData->m_H*pData->m_W*3];
		pPro->m_pRGBTRANS   = new BYTE[pData->m_H*pData->m_W*3];
		pPro->m_pBitmapInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER));	

		pPro->m_pBitmapInfo->bmiHeader.biSize          = sizeof(BITMAPINFO);
		pPro->m_pBitmapInfo->bmiHeader.biPlanes        = 1;
		pPro->m_pBitmapInfo->bmiHeader.biBitCount      = 24;
		pPro->m_pBitmapInfo->bmiHeader.biCompression   = BI_RGB;
		pPro->m_pBitmapInfo->bmiHeader.biSizeImage     = 0;
		pPro->m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 11811;
		pPro->m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 11811;
		pPro->m_pBitmapInfo->bmiHeader.biClrUsed       = 0;
		pPro->m_pBitmapInfo->bmiHeader.biClrImportant  = 0;
	}

	if ( IMAGE_UYVY == pData->m_Type )
	{
		pPro->m_pBitmapInfo->bmiHeader.biWidth  =  pData->m_H;
		pPro->m_pBitmapInfo->bmiHeader.biHeight = pData->m_W;
		YUV422_C_RGB( (BYTE*)pData->m_Block, pPro->m_pRGBTMP, pData->m_H, pData->m_W );
		RGB_EDDY( pPro->m_pRGBTMP, pData->m_H, pData->m_W, pPro->m_pRGBTRANS );
	}
	else if ( IMAGE_YUYV ==  pData->m_Type )
	{
		pPro->m_pBitmapInfo->bmiHeader.biWidth  = pData->m_W;
		pPro->m_pBitmapInfo->bmiHeader.biHeight = pData->m_H;
		YUYV_C_RGB( (BYTE*)pData->m_Block, pPro->m_pRGBTRANS, pData->m_H, pData->m_W);
	}
	
	long lStillWidth  = pPro->m_Rect.right - pPro->m_Rect.left;
	long lStillHeight = pPro->m_Rect.bottom - pPro->m_Rect.top;
	
	HDC hdc = ::GetDC( (HWND)(pPro->m_hWnd) );
	::SetStretchBltMode( hdc, COLORONCOLOR);
	::StretchDIBits(  
		hdc, 0, 0,  
		lStillWidth, lStillHeight,  
		0, 0,  
		pPro->m_pBitmapInfo->bmiHeader.biWidth, 
		pPro->m_pBitmapInfo->bmiHeader.biHeight, 
		pPro->m_pRGBTRANS,  
		pPro->m_pBitmapInfo,  
		DIB_RGB_COLORS,  
		SRCCOPY );
	::ReleaseDC( pPro->m_hWnd, hdc );
}


//������Ƶ�����߳�
static DWORD WINAPI RecvVideoDataThread( void* p )
{
	char pTemp[ sizeof(SOCK_DATA_HEADER) + PACK_SIZE  ] = { 0 }; //�����С���벻С��sizeof(SOCK_DATA_HEADER) + PACK_SIZE 
	
	ENDPNT* pEndPnt = (ENDPNT*) p;
	SOCKET sVUDPSck = pEndPnt->m_hSck;
	int nLen = 0;
	int nCmd = 0;
	int nRlt = 0;

	SOCKADDR_IN addrClient = {0};
	int len                = sizeof(SOCKADDR);
	char* pData            = NULL;
	DEVIMAGE* pDevImage    = NULL;
	VideoPro stPro         = {0};
	stPro.m_hWnd           = VideoSDK::g_hVideoWND;
	::GetWindowRect( (HWND)VideoSDK::g_hVideoWND, &(stPro.m_Rect) );//��ȡ��ʾ��Ƶ���ݵĴ���RECT

	typedef std::map<UINT32, char* > DATA_MAP_EX;
    DATA_MAP_EX mapDataEx;
	char* pBuffer = NULL;
	while( !VideoSDK::g_VideoClose )
	{

    //������Ƶ����
		nLen = recvfrom( sVUDPSck,  pTemp, sizeof(SOCK_DATA_HEADER) + PACK_SIZE, 0, (sockaddr*)&addrClient, &len );

		if ( nLen > 0 )
		{
			
			//��ȡ��ͷ
			SOCK_DATA_HEADER Head = *((SOCK_DATA_HEADER*)(pTemp));

			//У��
			if ( 0 != sock_checksum( (UINT16*) (&Head), sizeof(SOCK_DATA_HEADER) ) )
				continue;//У�����,����

			if (Head.msg_offset == 0)//��һ���ְ�
			{
				//������������ݰ�
				nCmd = ResolveNCMD( pTemp, nLen, nRlt );
				if ( nCmd == M_VIDEO_OUT )
				{
					;//��һ���ְ�,����ͷ�ڵ�һ���ְ����档
				}
				else
					continue;//�������Ƿ���
			}

			 //���ұ��α�ʶ�����ݽ��ջ���
			 DATA_MAP_EX::iterator it  = mapDataEx.find( Head.msg_identify );
			 if ( it == mapDataEx.end() ) //û�ҵ�����Ϊ����Ƶ���ݰ���
			 {
				if ( Head.msg_offset != 0 )//�����Ƿ񶪰�ͷ������Ϊ������ͷ�ķǷ���Ƶ���ݡ�
					continue;

				//Ϊ���ձ�����Ƶ���ݿ���һ���컺�棬�����뵽MAPӳ�����
				int nSize = Head.msg_total+sizeof(SOCK_DATA_HEADER)+sizeof(UINT32)+4 ;
				pBuffer = new char[ nSize ];
				ZeroMemory( pBuffer, nSize );
				std::pair<DATA_MAP_EX::iterator, bool> IT =  mapDataEx.insert( std::pair< UINT32, char* >( Head.msg_identify, pBuffer ) );
				it = IT.first;
			 }
			 else
				  pBuffer =  it -> second;//�ҵ�������Ƶ���ݰ������ݽ��ջ���
				

			 assert( (long)pBuffer%4  == 0 );//��֤�����Ƿ�4�ֽڶ���

			 //���
			 char* pMem = pBuffer + sizeof(SOCK_DATA_HEADER) + sizeof(UINT32);
			 memcpy( pMem + Head.msg_offset, pTemp + sizeof(SOCK_DATA_HEADER), Head.msg_len );

			 //�ۻ���ǰ�����ѽ��յķְ��ܳ���
			 SOCK_DATA_HEADER* pHdr = (SOCK_DATA_HEADER*) pBuffer;
			 pHdr->msg_total += Head.msg_len;

			 //�ж�����Ƿ���ɣ�����򲥷�
			 if ( pHdr->msg_total ==  Head.msg_total )
			 {
				NCMD* pNCM = (NCMD*) ( pBuffer + sizeof(SOCK_DATA_HEADER) + sizeof(UINT32) );
				pDevImage  = (DEVIMAGE*) ( pNCM + 1 );

				//��ʾ��Ƶ
				ShowVideo( pDevImage, &stPro );

				//�������ǰ�Ѿ����ŵ���Ƶ���ݻ���
				delete [] pBuffer;
				mapDataEx.erase(it);

			 }

			 ZeroMemory( pTemp, sizeof(SOCK_DATA_HEADER) + PACK_SIZE );
		}
		Sleep(1);
	}

	if(stPro.m_pRGBTMP)
		delete [] stPro.m_pRGBTMP;

	if (stPro.m_pRGBTRANS)
		delete [] stPro.m_pRGBTRANS;

	delete pEndPnt;

	VideoSDK::g_VideoClose = false;
	return 0;
}

VideoSDK::VideoSDK()
{
	m_sHost       = NULL;
	m_sVVIDEOSck  = NULL;
	m_strDevIP    = "";
	m_strClientIP = "";
	m_nTCPPort    = 0;
	m_nUDPPort    = 0;

}

VideoSDK::~VideoSDK()
{

}

//��ʼ��
bool  VideoSDK::SDKInit(HWND hWnd, CString strDevIP, CString strClientIP, int nTCPPort)
{
	VideoSDK::g_hVideoWND  = hWnd;
	m_sHost				   = 0;
	m_sVVIDEOSck		   = 0;
	m_strDevIP			   = strDevIP;
	m_strClientIP		   = strClientIP;
	m_nTCPPort			   = nTCPPort;
		 
	return Init();
}

//���豸��������
bool   VideoSDK::SDKConnect()
{
  //���豸��������
  m_sHost = ConnectServer( m_strDevIP.GetBuffer(0), m_nTCPPort );

  if (!m_sHost) return false;

  //��������ʵʱ���������߳�
  ENDPNT*  pEnPnt = new ENDPNT ;
  pEnPnt->m_hSck  = m_sHost;
  memcpy( pEnPnt->m_IP.m_ID, m_strDevIP.GetBuffer(0) , DEV_ID_LEN);
  g_hTCPThread = CreateThread( NULL, 0, RecvRealControlDataThread, (void*) pEnPnt,  NULL, NULL );


  //��½�豸���������Ȩ����֤���豸���Ե�½���Ϳ���Ϊ��admin��201031
  if( !Login( m_sHost, m_strDevIP.GetBuffer(0), "admin", "201031" ) )
		return false;

  while(!g_Author)//�ȴ�Ȩ����֤�ɹ����豸�˵���֤��Ӧ��Ϣ��
	 Sleep(50);

  return VideoSDK::g_Author;
}

//��½�豸
bool   VideoSDK::SDKLogin(CString sUserName, CString strPassword)
{
	//��½�豸������Ȩ����֤
	if( !Login( m_sHost, m_strDevIP.GetBuffer(0), sUserName.GetBuffer(0), strPassword.GetBuffer(0) ) )
		return false;

	while(!g_Author)//�ȴ�Ȩ����֤�ɹ����豸�˵���֤��Ӧ��Ϣ��
	 Sleep(50);

    return VideoSDK::g_Author;
}

//������Ƶ
bool   VideoSDK::SDKStartVideo()
{
	if ( g_Author )//ͨ��Ȩ����֤
	{
		//����Ƶ������������Ƶ���ݽ����߳�
		m_sVVIDEOSck = StartVideo( m_sHost, m_strDevIP.GetBuffer(0), m_strClientIP.GetBuffer(0), m_nUDPPort);
		if (!m_sVVIDEOSck)
			return false;

		return true;
	}
	else
		return false;
}

//ֹͣ��Ƶ
bool   VideoSDK::SDKStopVideo()
{
	//�ر���Ƶ
	bool bRet = StopVideo( m_sHost, m_strDevIP.GetBuffer(0), m_strClientIP.GetBuffer(0), m_nUDPPort);
	if (!bRet)
		return false;

	//�ر���ƵUDP�׽���
	if (m_sVVIDEOSck)
	{
		closesocket(m_sVVIDEOSck);
		m_sVVIDEOSck = NULL;
	}

	//�ر���Ƶ���ݴ����߳�
	if(g_hUDPThread)
	{
		VideoSDK::g_VideoClose = true;
   		if( WaitForSingleObject( g_hUDPThread, 5000 ) != WAIT_OBJECT_0 )
			TerminateThread( g_hUDPThread, 0 );
		CloseHandle( g_hUDPThread );
		g_hUDPThread = NULL;
	}
	return true;
}

//�Ͽ�����
void   VideoSDK::SDKDisConnect()
{	
	//�ر�����
	DisConnectServer(m_sHost);

	//�ر�ʵʱ���������߳�
	if(g_hTCPThread)
	{
		VideoSDK::g_exitThread = true;
   		if( WaitForSingleObject( g_hTCPThread, 5000 ) != WAIT_OBJECT_0 )
			TerminateThread( g_hTCPThread, 0 );
		CloseHandle( g_hTCPThread );
		g_hTCPThread = NULL;
	}
	g_Author = false;
}