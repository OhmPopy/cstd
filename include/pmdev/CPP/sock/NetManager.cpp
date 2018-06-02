//#include "StdAfx.h"
#include "NetManager.h"
#include "OverlappedProcessor.h"
#include "SelectProcessor.h"
#include "Socket.h"

CNetManager::CNetManager() :
  m_pProc(NULL),
  m_lpfnListen(NULL),
  m_lpListenContext(NULL),
  m_lpfnRecv(NULL),
  m_lpRecvContext(NULL),
  m_lpfnRecvFrom(NULL),
  m_lpRecvFromContext(NULL)
{
}

CNetManager::~CNetManager()
{
  UnInit();
}

/********************************************************************
/*��������: Init
/*����ʱ��: 2011/07/08 16:44
/*��  ��: Model �׽��ִ���ģ��
      nMemPoolSize �ڴ�ش�С
/*�ļ�����: lcy
/*����˵��: �����׽���ģ��
*********************************************************************/
BOOL CNetManager::Init(NET_MODEL Model, uint32 uMemPoolSize, uint32 uMemPoolUnit)
{
  if (m_pProc) {
    return true;
  }

  try {
    if (NET_OVERLAPPED == Model) {
#ifdef WIN32

      //if ( NULL == m_pProc ) m_pProc = new COverlappedProcessor( uMemPoolSize, uMemPoolUnit );
      if (NULL == m_pProc) {
        m_pProc = new CSelectProcessor(uMemPoolSize, uMemPoolUnit);
      }

#else

      if (NULL == m_pProc) {
        m_pProc = new CSelectProcessor(uMemPoolSize, uMemPoolUnit);
      }

#endif
    }
    else if (NET_SELECTED == Model) {
      if (NULL == m_pProc) {
        m_pProc = new CSelectProcessor(uMemPoolSize, uMemPoolUnit);
      }
    }
  }
  catch (std::bad_alloc) {
    return false;
  }

  return m_pProc != NULL;
}

/********************************************************************
/*��������: UnInit
/*����ʱ��: 2011/07/08 16:46
/*��  ��:
/*�ļ�����: lcy
/*����˵��: �ͷ��׽���ģ��
*********************************************************************/
void CNetManager::UnInit()
{
  if (m_pProc) {
    delete m_pProc;
    m_pProc = NULL;
  }

  m_lpfnListen = NULL;
  m_lpListenContext = NULL;
  m_lpfnRecv = NULL;
  m_lpRecvContext = NULL;
}

/********************************************************************
/*��������: CreateSocket
/*����ʱ��: 2011/07/08 16:46
/*��  ��: type �׽�������(�ֽ��������ݱ�)
/*�ļ�����: lcy
/*����˵��: �����׽���
*********************************************************************/
SOCKET CNetManager::CreateSocket(int32 type)
{
  return sock_create(type);
}

/********************************************************************
/*��������: ReleaseSocket
/*����ʱ��: 2011/07/08 16:47
/*��  ��: sock �׽���
/*�ļ�����: lcy
/*����˵��: �ͷ��׽���
*********************************************************************/
void CNetManager::ReleaseSocket(SOCKET sock)
{
  sock_close(sock);
  m_pProc->StopRecv(sock);
}

/********************************************************************
/*��������: Connect
/*����ʱ��: 2011/07/08 16:47
/*��  ��: sock �׽���
PeerAddr ��������ַ
/*�ļ�����: lcy
/*����˵��: ���ӷ�����
*********************************************************************/
BOOL CNetManager::Connect(SOCKET sock, const SOCK_ADDRESS& PeerAddr)
{
  sock_ioctl(sock,  FIONBIO , true);
  sock_connect(sock, PeerAddr.IP_Address, PeerAddr.Port);
  WRITE_TIME_OUT(sock, (3 * 1000));
  return !bTimeOut;
}

/********************************************************************
/*��������: Bind
/*����ʱ��: 2011/07/08 16:48
/*��  ��: sock �׽���
      LocalAddr ���ص�ַ
/*�ļ�����: lcy
/*����˵��: ���׽��ְ󶨵����ص�ַ
*********************************************************************/
BOOL CNetManager::Bind(SOCKET sock, const SOCK_ADDRESS& LocalAddr)
{
  return sock_bind(sock, LocalAddr.IP_Address, LocalAddr.Port) ;
}

/********************************************************************
/*��������: Listen
/*����ʱ��: 2011/07/08 16:49
/*��  ��: sock �׽���
      bStartUp �����������
/*�ļ�����: lcy
/*����˵��: ������ֹͣ�׽�������
*********************************************************************/
void CNetManager::Listen(SOCKET sock)
{
  sock_listen(sock, 5);
  m_pProc->StartListen(sock);
}

/********************************************************************
/*��������: JoinIPGroup
/*����ʱ��: 2011/07/08 16:50
/*��  ��: sock �׽���
      GroupIPAddr �ಥ���ַ
/*�ļ�����: lcy
/*����˵��: ����ಥ��
*********************************************************************/
BOOL CNetManager::JoinIPGroup(SOCKET sock, const SOCK_ADDRESS& GroupIPAddr)
{
  return sock_bind(sock, NULL, GroupIPAddr.Port) && sock_join_group(sock, GroupIPAddr.IP_Address);
}

/********************************************************************
/*��������: DropIPGroup
/*����ʱ��: 2011/07/08 16:50 s
/*��  ��: sock �׽���
      GroupIPAddr �ಥ���ַ
/*�ļ�����: lcy
/*����˵��: �˳��ಥ��
*********************************************************************/
void CNetManager::DropIPGroup(SOCKET sock, const SOCK_ADDRESS& GroupIPAddr)
{
  sock_drop_group(sock, GroupIPAddr.IP_Address);
}

/********************************************************************
/*��������: SetRecvBuf
/*����ʱ��: 2011/07/08 16:52
/*��  ��: sock �׽���
      nSize ��������С
/*�ļ�����: lcy
/*����˵��: ���ý��ܻ�������С
*********************************************************************/
void CNetManager::SetRecvBuf(SOCKET sock, int32 nSize)
{
  sock_set_recvbuf_size(sock, nSize);
}

/********************************************************************
/*��������: SetSendBuf
/*����ʱ��: 2011/07/08 16:53
/*��  ��: sock �׽���
      nSize ��������С
/*�ļ�����: lcy
/*����˵��: ���÷��ͻ�������С
*********************************************************************/
void CNetManager::SetSendBuf(SOCKET sock, int32 nSize)
{
  sock_set_sendbuf_size(sock, nSize);
}

/********************************************************************
/*��������: GetPeerAddr
/*����ʱ��: 2011/07/08 16:53
/*��  ��: sock �׽���
      lpPeerAddr ���ص�ַ
/*�ļ�����: lcy
/*����˵��: ��ȡ�Է���ַ
*********************************************************************/
BOOL CNetManager::GetPeerAddr(SOCKET sock, PSOCK_ADDRESS lpPeerAddr)
{
  return sock_addr(sock, lpPeerAddr->IP_Address, lpPeerAddr->Port);
}

BOOL CNetManager::GetLocalIP(SOCKET sock, PSOCK_ADDRESS lpLocalIP)
{
  return sock_DNS2LocalIP(lpLocalIP->IP_Address);
}

BOOL CNetManager::GetSockName(SOCKET sock, PSOCK_ADDRESS lpLocalIP)
{
  return sock_name(sock, lpLocalIP->IP_Address, lpLocalIP->Port);
}

/********************************************************************
/*��������: Send
/*����ʱ��: 2011/07/08 16:54
/*��  ��: sock �׽���
      pbData �����ڴ�
      nLen   ���ݴ�С
/*�ļ�����: lcy
/*����˵��: ��������
*********************************************************************/
int32 CNetManager::Send(SOCKET sock, const int8* pbData, int32 nLen)
{
  return m_pProc->Send(sock, pbData, nLen);
}

/********************************************************************
/*��������: Send
/*����ʱ��: 2011/07/08 16:54
/*��  ��: sock �׽���
      pbData �����ڴ�
      nLen   ���ݴ�С
/*�ļ�����: lcy
/*����˵��: ��������
*********************************************************************/
int32 CNetManager::SendTo(SOCKET sock, const int8* pbData, int32 nLen, PSOCK_ADDRESS lpAddr)
{
  return m_pProc->Send(sock, pbData, nLen, lpAddr);
}

/********************************************************************
/*��������: Recv
/*����ʱ��: 2011/07/08 16:54
/*��  ��: sock �׽���
/*�ļ�����: lcy
/*����˵��: �������ݽ���
*********************************************************************/
void CNetManager::Recv(SOCKET sock)
{
  m_pProc->StartRecv(sock);
}

/********************************************************************
/*��������: StartWork
/*����ʱ��: 2011/07/08 16:55
/*��  ��: bStartUp ������ر����ݴ�������
/*�ļ�����: lcy
/*����˵��: ������ر����ݴ�������
*********************************************************************/
BOOL CNetManager::StartWork(BOOL bStartUp)
{
  return m_pProc->StartUpWork(bStartUp);
}


/********************************************************************
/*��������: OnListen
/*����ʱ��: 2011/07/08 16:56
/*��  ��: sock �׽���
      Addr �ͻ��˵�ַ
/*�ļ�����: lcy
/*����˵��: �����ص�
*********************************************************************/
void CNetManager::OnListen(SOCKET sock, const SOCK_ADDRESS& Addr)
{
  if (m_lpfnListen) {
    m_lpfnListen((HANDLE)sock, Addr, m_lpListenContext);
  }
}

/********************************************************************
/*��������: OnRecv
/*����ʱ��: 2011/07/08 16:56
/*��  ��: sock �׽���
      pbData �����ڴ��׵�ַ
      nLen �����ڴ��С
/*�ļ�����: lcy
/*����˵��: ���ݽ��ܻص�
*********************************************************************/
void CNetManager::OnRecv(SOCKET sock, const int8* pbData, int32 nLen, PSOCK_ADDRESS lpAddr /*= NULL*/)
{
  if (lpAddr == NULL) {
    if (m_lpfnRecv) {
      m_lpfnRecv((HANDLE)sock, pbData, nLen, m_lpRecvContext);
    }
  }
  else {
    if (m_lpfnRecvFrom) {
      m_lpfnRecvFrom((HANDLE)sock, pbData, nLen, *lpAddr, m_lpRecvFromContext);
    }
  }
}


