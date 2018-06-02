#include "StdAfx.h"
#include "OverlappedProcessor.h"
#include "Socket.h"
#include "SockHandle.h"


#ifdef WIN32

typedef struct : public WSAOVERLAPPED
{
	int32			 nOperType;
	int32            bListen;
	int32            Param;      //���ڵ�bListenΪtureʱ��������
	int32            nQuestBytes;//������ܶ����ֽ�����
	WSABUF			 WsaBuf;	
	void*            lpContext;
	char             cRecvBuf[sizeof(sockaddr_in)*2+32];//�˻�����Ҫ���ڵ�bListenΪtureʱ����Զ��������ַ
}OVERLAPPED_DATA;

COverlappedProcessor::COverlappedProcessor( uint32 uMemPoolSize, uint32 uMemPoolUnit ) :
CNetProcessor(uMemPoolSize, uMemPoolUnit),
m_bWorking(false),
m_nTreadCount(0),
m_hIOCPThread(0)
{
	memset( m_hWorkThread, 0, sizeof( m_hWorkThread ) );
	m_hCompletionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);	//������ɶ˿�
}

COverlappedProcessor::~COverlappedProcessor()
{
	StartUpWork(false);
	CloseHandle( m_hCompletionPort );
}

/********************************************************************  
/*��������:	ReadyRecv		
/*����ʱ��: 2011/07/08 16:57 
/*��	��:	sock ׼�����ݽ���
			type ���ݽ�������
/*�ļ�����: lcy  
/*����˵��: ���׽��ּ�����ɶ˿ڶ����У��Ա��յ����ݽ�����Ϣ  
*********************************************************************/ 
void COverlappedProcessor::StartRecv( SOCKET sock )
{	
	StartRecv( sock, false );
}

void COverlappedProcessor::StopRecv( SOCKET sock )
{
//	sock_close( sock );
}

void COverlappedProcessor::StopListen( SOCKET sock )
{
	sock_close( sock );
}

void COverlappedProcessor::StartListen( SOCKET sock )
{
	StartRecv( sock, true );
}

int32 COverlappedProcessor::Send( SOCKET sock, const int8* pbData, int32 nLen, const SOCK_ADDRESS* pAddr )
{	
	CDataRecvProcess* pObj = NULL;
	m_synSOCKMap.Lock();
	SOCKMap::iterator it = m_SOCKMap.find( sock );
	if ( it != m_SOCKMap.end() )
	{
		OVERLAPPED_DATA* pData = (OVERLAPPED_DATA*)(it->second);
		if( pData )
		{
			pObj = (CDataRecvProcess*)pData->lpContext;
			if( pObj) pObj->AddRef();
		}
	}
	m_synSOCKMap.Unlock();
	if( pObj )
	{
		int32 rs = pObj->Send( pbData, nLen, pAddr );
		pObj->Release();
		return rs;
	}
	return 0;
}

void COverlappedProcessor::StartRecv( SOCKET sock, bool bListen )
{
	OVERLAPPED_DATA* pOverLapped = NULL;
	m_synSOCKMap.Lock();
	SOCKMap::iterator it =  m_SOCKMap.find( sock );
	if ( it == m_SOCKMap.end() )
	{
		::CreateIoCompletionPort( (HANDLE)sock, m_hCompletionPort, sock, 0);
		pOverLapped = new OVERLAPPED_DATA;
		memset( pOverLapped, 0x0, sizeof(OVERLAPPED_DATA) );
		pOverLapped->lpContext = new CDataRecvProcess( sock, m_pThreadPool );
		pOverLapped->bListen = bListen;
		pOverLapped->nOperType = FD_READ;
		m_SOCKMap.insert( std::pair<SOCKET, void*>(sock, pOverLapped) );
	}
	m_synSOCKMap.Unlock();
	if ( pOverLapped )
	{
		if ( SOCK_STREAM == sock_type(sock) )
			Recv(sock, pOverLapped );
		else if ( SOCK_DGRAM == sock_type(sock) )
			RecvFrom( sock, pOverLapped );	
	}
}

/********************************************************************  
/*��������: StartUpWork			
/*����ʱ��: 2011/07/08 17:05 
/*��	��:	
/*�ļ�����: lcy  
/*����˵��: ������ֹͣ���ݽ��ܹ������� 
*********************************************************************/ 
bool COverlappedProcessor::StartUpWork( bool flag ) 
{
	if ( flag )
	{
		if ( !m_bWorking )
		{
			m_bWorking = true;
#if 1
			SYSTEM_INFO info = { 0 };
			::GetSystemInfo( &info );
			m_nTreadCount = info.dwNumberOfProcessors+2;
			if (  m_nTreadCount > MAX_TREADS ) m_nTreadCount = MAX_TREADS;
			for ( int i = 0; i < m_nTreadCount; ++i )
			{
				if ( NULL == m_hWorkThread[i] )
				{
					m_hWorkThread[i] = (HANDLE)_beginthread(COverlappedProcessor::process, 0, this);			
				}
			}
#else
			m_hIOCPThread = (HANDLE)_beginthread(COverlappedProcessor::process, 0, this);
#endif
		}
	}
	else
	{
		if ( m_bWorking )
		{
			m_bWorking = false;
#if 1
			for( int i = 0; i < m_nTreadCount; ++i )
			{//Ͷ����ɶ˿���Ϣ�������̳߳�
				::PostQueuedCompletionStatus(m_hCompletionPort, -1, 0, NULL);
			}
			//	::WaitForMultipleObjects( m_nTreadCount, m_hWorkThread, TRUE, 1000*TIME_OUT);
			for ( i = 0; i < m_nTreadCount; ++i )
			{
				if ( WAIT_OBJECT_0 != ::WaitForSingleObject( m_hWorkThread[i], 1000) )
				{
					::TerminateThread( m_hWorkThread[i], 0 );
				}
				m_hWorkThread[i] = NULL;
			}
#else
			::PostQueuedCompletionStatus(m_hCompletionPort, -1, 0, NULL);
			::PostQueuedCompletionStatus(m_hCompletionPort, -1, 0, NULL);
			if ( WAIT_OBJECT_0 != ::WaitForSingleObject( m_hIOCPThread, 1000*TIME_OUT) )
			{
				::TerminateThread( m_hIOCPThread, 0 );
			}
			m_hIOCPThread =  NULL;
#endif
		}
	}
	return true;
}

void COverlappedProcessor::process( void* param )
{
	COverlappedProcessor* pthis = (COverlappedProcessor*)param;
	if ( pthis ) pthis->process();
}

/********************************************************************  
/*��������:	process		
/*����ʱ��: 2011/07/08 17:06 
/*��	��:	
/*�ļ�����: lcy  
/*����˵��: �̴߳�����  
*********************************************************************/ 
void COverlappedProcessor::process()
{
	DWORD dwTrans = 0;
	DWORD dkey    = 0;
	LPOVERLAPPED lpover = NULL;
	BOOL bok = false;
	
	while ( m_bWorking )
	{
		dwTrans = 0;
		dkey    = 0;
		lpover  = NULL;
		bok     = FALSE;	
		SetLastError(0);
		bok = ::GetQueuedCompletionStatus(m_hCompletionPort, &dwTrans, &dkey, &lpover, INFINITE );
		if ( !ErrDeal( dkey, lpover, dwTrans, bok) ) continue;
		OVERLAPPED_DATA* lpOverlapped = (OVERLAPPED_DATA*)lpover;
		if ( !(lpOverlapped->bListen) )
			HandleIO( dkey, (WSAOVERLAPPED*)lpover, dwTrans, 1 );
		else
			HandleListen( dkey, (WSAOVERLAPPED*)lpover, dwTrans, 1 );
	}
}

bool COverlappedProcessor::ErrDeal( SOCKET sock, WSAOVERLAPPED* pIOOper, DWORD dwTrans, BOOL bOK  )
{
	int nError = sock_error();
	if ( sock == INVALID_SOCKET || NULL == pIOOper ) return false;
	if ( dwTrans == -1 ) 
	{
		m_bWorking = false;
		return false;
	}
	
	OVERLAPPED_DATA* lpOverlapped = (OVERLAPPED_DATA*)pIOOper;
	if ( ( !bOK && dwTrans == 0 && !(lpOverlapped->bListen )) || 
		(!bOK && (nError == WSAENETRESET || nError == WSAECONNABORTED || nError == WSAECONNRESET))
		) 		 
	{
		sock_close( sock );
		bool bExit = false;
		m_synSOCKMap.Lock();
		SOCKMap::iterator it =  m_SOCKMap.find( sock );
		if ( it != m_SOCKMap.end() ) 
		{
			m_SOCKMap.erase( it );
			bExit =  true;
		}
		m_synSOCKMap.Unlock();
		if ( bExit )
		{
			CDataRecvProcess* pObj = (CDataRecvProcess*)(lpOverlapped->lpContext);
			lpOverlapped->lpContext = NULL;
			pObj->Release();
			delete lpOverlapped;
			CNetProcessor::StopRecv( sock );
		}
		return false;
	}
	return true;
}

void COverlappedProcessor::HandleListen(SOCKET sock, WSAOVERLAPPED* pIOOper, DWORD dwTrans, int nError)
{
	OVERLAPPED_DATA* lpOverlapped = (OVERLAPPED_DATA*)pIOOper;
	if ( FD_READ == lpOverlapped->nOperType ) //���ܵ�����
	{
		SOCK_ADDRESS Addr = { 0 };
		sock_getacceptexsockaddrs( &(lpOverlapped->WsaBuf), Addr.IP_Address, Addr.Port );
		CNetProcessor::Accept( lpOverlapped->Param, Addr );
		Recv(sock, pIOOper);
	}
}

/********************************************************************  
/*��������:	HandleIO		
/*����ʱ��: 2011/07/08 17:09 
/*��	��:	sock �׽���
			pIOOper ��ɶ˿�WSAOVERLAPPED�ṹ�����Ͳ���
			dwTrans ���ݽ��ܴ�С
			nError  ����ֵ
/*�ļ�����: lcy  
/*����˵��: ���ݽ��ܴ���  
*********************************************************************/ 
void COverlappedProcessor::HandleIO(SOCKET sock, WSAOVERLAPPED* pIOOper , DWORD dwTrans, int nError)
{	
	OVERLAPPED_DATA* lpOverlapped = (OVERLAPPED_DATA*)pIOOper;
	if ( FD_READ == lpOverlapped->nOperType ) //���ܵ�����
	{
		CDataRecvProcess* pCurPrecessor = (CDataRecvProcess*)(lpOverlapped->lpContext);
		if ( SOCK_STREAM == sock_type(sock) )
		{			
			if ( pCurPrecessor->Recv() )
			{
				pCurPrecessor->CheckTimeOut( RECC_TIME_OUT );
				Recv(sock, pIOOper);
			}
			else
			{
				SetLastError( WSAECONNABORTED );
				ErrDeal( sock, pIOOper, 0, FALSE );
			}
		}
		else
		{
			pCurPrecessor->RecvFrom();
			pCurPrecessor->CheckTimeOut( RECC_TIME_OUT );
			RecvFrom(sock, pIOOper);
		}
	}	
}

/********************************************************************  
/*��������:	Recv		
/*����ʱ��: 2011/07/08 17:11 
/*��	��:	sock �׽���
pIOOper ��ɶ˿�WSAOVERLAPPED�ṹ�����Ͳ���
/*�ļ�����: lcy  
/*����˵��: ����һ����ȡ���� 
*********************************************************************/ 
void COverlappedProcessor::Recv( SOCKET sock, WSAOVERLAPPED* pIOOper )
{
	OVERLAPPED_DATA* lpOverlapped = (OVERLAPPED_DATA*)pIOOper;
	uint32 nRecvBytes = sizeof(SOCK_DATA_HEADER);
	lpOverlapped->WsaBuf.buf = lpOverlapped->cRecvBuf;		
	lpOverlapped->WsaBuf.len = nRecvBytes;
	bool bok = true;
	if ( lpOverlapped->bListen )
	{
		memset( lpOverlapped->cRecvBuf, 0x0, sizeof(lpOverlapped->cRecvBuf) );
		lpOverlapped->Param = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,0,0,WSA_FLAG_OVERLAPPED);
		bok = sock_acceptex( sock, lpOverlapped->Param, &(lpOverlapped->WsaBuf), pIOOper );
	}
	else
	{
		uint32 uFlag = MSG_PEEK;//����ʹ�ô˱����Ϊ�˻�ȡ�׽��������ݽ���֪ͨ
		//��������������,����������������CDataRecvProcess����
		bok = sock_wsarecv( sock, &(lpOverlapped->WsaBuf), &nRecvBytes, lpOverlapped, uFlag );
	}
	ErrDeal( sock, pIOOper, nRecvBytes, bok );
}

void COverlappedProcessor::RecvFrom( SOCKET sock, WSAOVERLAPPED* pIOOper )
{
	OVERLAPPED_DATA* lpOverlapped = (OVERLAPPED_DATA*)pIOOper;
	lpOverlapped->WsaBuf.buf = lpOverlapped->cRecvBuf ;	
	uint32 nRecvBytes = sizeof(SOCK_DATA_HEADER);
	lpOverlapped->WsaBuf.len  = nRecvBytes;
	uint32 uFlag = MSG_PEEK;//����ʹ�ô˱����Ϊ�˻�ȡ�׽��������ݽ���֪ͨ
						    //��������������,����������������CDataRecvProcess����
	sock_wsarecvfrom( sock, &(lpOverlapped->WsaBuf), &nRecvBytes, lpOverlapped, uFlag, NULL, NULL );
}

#endif
