#include "copenssl.h"
#include "OverlappedProcessor.h"
#include "SelectProcessor.h"
#include "Socket.h"
#include <assert.h>
volatile BOOL gInitialized = false;
#ifdef WIN32
#define INITSOCK (InitSocket())
#define UINITSOCK (UnInitSocket())
BOOL InitSocket()
{
  if (!gInitialized) {
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA WSAData = { 0 };

    if (WSAStartup(wVersionRequested, &WSAData)) {
      return false;
    }

    if (LOBYTE(WSAData.wVersion) != 2 || HIBYTE(WSAData.wVersion) != 2) {
      return false;
    }
  }

  return true ;
}
void UnInitSocket()
{
  if (gInitialized) {
    gInitialized = false;
    WSACleanup() ;
  }
}
#else
#define INITSOCK 1
#define UINITSOCK 1
#endif
CNetProcessor* m_pProc = NULL;
ListenCallBack m_lpfnListen = NULL;
void* m_lpListenContext = NULL;
DataRecvCallBack m_lpfnRecv = NULL;
void* m_lpRecvContext = NULL;
DataRecvFromCallBack m_lpfnRecvFrom = NULL;
void* m_lpRecvFromContext = NULL;
BOOL CPMDEV_CALL CPM_SetListenCallBack(ListenCallBack lpfnListen, void* pvContext)
{
  m_lpfnListen = lpfnListen;
  m_lpListenContext = pvContext;
  return 0;
}
BOOL CPMDEV_CALL CPM_SetDataRecvCallBack(DataRecvCallBack lpfnRecv, void* pvContext)
{
  m_lpfnRecv = lpfnRecv;
  m_lpRecvContext = pvContext;
  return 0;
}
BOOL CPMDEV_CALL CPM_SetDataRecvFromCallBack(DataRecvFromCallBack lpfnRecv, void* pvContext)
{
  m_lpfnRecvFrom = lpfnRecv;
  m_lpRecvFromContext = pvContext;
  return 0;
}
//��������: UnInit
//����ʱ��: 2011/07/08 16:46
//�� ��:
//�ļ�����: lcy
//����˵��: �ͷ��׽���ģ��
BOOL CPMDEV_CALL CPM_UnInitSock()
{
  if (m_pProc) {
    delete m_pProc;
    m_pProc = NULL;
  }

  m_lpfnListen = NULL;
  m_lpListenContext = NULL;
  m_lpfnRecv = NULL;
  m_lpRecvContext = NULL;
  gInitialized = false;
  UINITSOCK;
  return 0;
}
//��������: Init
//����ʱ��: 2011/07/08 16:44
//�� ��: Model �׽��ִ���ģ��
// nMemPoolSize �ڴ�ش�С
//�ļ�����: lcy
//����˵��: �����׽���ģ��
BOOL CPMDEV_CALL CPM_InitSock(NET_MODEL Model, uint32 uMemPoolSize, uint32 uMemPoolUnit)
{
  if (gInitialized) {
    return true;
  }

  if (!INITSOCK) {
    return false;
  }

  if (uMemPoolSize <= 0) {
    uMemPoolSize = MEMRESERVER_SIZE;
  }

  if (uMemPoolUnit <= 0) {
    uMemPoolUnit = MEMRESERVER_UNIT;
  }

  DEBUG_INFO1("%s" , "CNetManager_Init");

  if (m_pProc) {
    return true;
  }

  try {
    switch (Model) {
    case NET_OVERLAPPED: {
      DEBUG_INFO1("%s" , "NET_OVERLAPPED");
#ifdef WIN32
      COpenSsl::Obj().Init(true);

      if (NULL == m_pProc) {
        m_pProc = new COverlappedProcessor(uMemPoolSize, uMemPoolUnit);
      }

#else
      assert(false);
#endif
    }
    break;

    case NET_SELECTED: {
      DEBUG_INFO1("%s" , "NET_SELECTED");
      COpenSsl::Obj().Init(true);

      if (NULL == m_pProc) {
        m_pProc = new CSelectProcessor(uMemPoolSize, uMemPoolUnit);
      }
    }
    break;

    default:
      assert(false);
      break;
    }
  }
  catch (std::bad_alloc) {
    return false;
  }

  gInitialized = m_pProc != NULL;

  if (!gInitialized) {
    CPM_UnInitSock();
  }

  return gInitialized;
}
//��������: CreateSocket
//����ʱ��: 2011/07/08 16:46
//�� ��: type �׽�������(�ֽ��������ݱ�)
//�ļ�����: lcy
//����˵��: �����׽���
SOCKET CPMDEV_CALL CPM_CreateSock(int af, int nSOCKType)
{
  return sock_create(af, nSOCKType);
}
//��������: ReleaseSocket
//����ʱ��: 2011/07/08 16:47
//�� ��: sock �׽���
//�ļ�����: lcy
//����˵��: �ͷ��׽���
BOOL CPMDEV_CALL CPM_ReleaseSock(SOCKET sock, void* user)
{
  printf("CPM_ReleaseSock\n");
  sock_close(sock);
  m_pProc->StopRecv(sock, user);
  return 0;
}
//��������: Connect
//����ʱ��: 2011/07/08 16:47
//�� ��: sock �׽���
//PeerAddr ��������ַ
//�ļ�����: lcy
//����˵��: ���ӷ�����
BOOL CPMDEV_CALL CPM_Connect(SOCKET sock, const addr_in* addr)
{
  // sock_ioctl (sock, FIONBIO , true ); //�����õ���SSL����ʧ��
  connect(sock, (const sockaddr*)addr, sizeof(addr_in));
  WRITE_TIME_OUT(sock, (3 * 1000));
  return !bTimeOut;
}
//��������: Bind
//����ʱ��: 2011/07/08 16:48
//�� ��: sock �׽���
// LocalAddr ���ص�ַ
//�ļ�����: lcy
//����˵��: ���׽��ְ󶨵����ص�ַ
BOOL CPMDEV_CALL CPM_Bind(SOCKET sock, const addr_in* addr)
{
  int32 on = 1;
  ::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const int8*)&on, sizeof(on));
  int ret = bind(sock, (const sockaddr*)addr, sizeof(addr_in));
  //printf("%s", strerror(GetLastError()));
  return ret == SOCKET_ERROR ? FALSE : TRUE;
}
//��������: Listen
//����ʱ��: 2011/07/08 16:49
//�� ��: sock �׽���
// bStartUp �����������
//�ļ�����: lcy
//����˵��: ������ֹͣ�׽�������
BOOL CPMDEV_CALL CPM_Listen(SOCKET sock, void* user)
{
  sock_listen(sock, 5);
  m_pProc->StartListen(sock, user);
  return 0;
}
//��������: JoinIPGroup
//����ʱ��: 2011/07/08 16:50
//�� ��: sock �׽���
// GroupIPAddr �ಥ���ַ
//�ļ�����: lcy
//����˵��: ����ಥ��
BOOL CPMDEV_CALL CPM_AddInIPGroup(SOCKET sock, const addr_in* addr)
{
  //assert(0);
  return 0;//CPM_Bind(sock, ANYIP(af), GroupIPAddr.Port) && sock_join_group(sock, GroupIPAddr.IP_Address);
}
//��������: DropIPGroup
//����ʱ��: 2011/07/08 16:50 s
//�� ��: sock �׽���
// GroupIPAddr �ಥ���ַ
//�ļ�����: lcy
//����˵��: �˳��ಥ��
BOOL CPMDEV_CALL CPM_DropOutIPGroup(SOCKET sock, const addr_in* GroupIPAddr)
{
  //sock_drop_group(sock, GroupIPAddr);
  return 0;
}
//��������: SetRecvBuf
//����ʱ��: 2011/07/08 16:52
//�� ��: sock �׽���
// nSize ��������С
//�ļ�����: lcy
//����˵��: ���ý��ܻ�������С
BOOL CPMDEV_CALL CPM_SetRecvBufSize(SOCKET sock, int32 nSize)
{
  sock_set_recvbuf_size(sock, nSize);
  return 0;
}
//��������: SetSendBuf
//����ʱ��: 2011/07/08 16:53
//�� ��: sock �׽���
// nSize ��������С
//�ļ�����: lcy
//����˵��: ���÷��ͻ�������С
BOOL CPMDEV_CALL CPM_SetSendBufSize(SOCKET sock, int32 nSize)
{
  sock_set_sendbuf_size(sock, nSize);
  return 0;
}
//��������: GetPeerAddr
//����ʱ��: 2011/07/08 16:53
//�� ��: sock �׽���
// lpPeerAddr ���ص�ַ
//�ļ�����: lcy
//����˵��: ��ȡ�Է���ַ
BOOL CPMDEV_CALL CPM_GetPeerAddr(SOCKET sock, addr_in* addr)
{
  return sock_addr(sock, addr);
}
int cmp_cnt_bytes(const void* a, const void* b, int n)
{
  const unsigned char* ua = (const unsigned char*)a;
  const unsigned char* ub = (const unsigned char*)b;
  int i, d = 0;

  for (i = 0; i < n; ++i) {
    d += ua[i] != ub[i];
  }

  return d;
}
BOOL CPMDEV_CALL CPM_GetLocalIP(const addr_in* addr, int port, addr_in* addr_out)
{
  char hostname[255] = {0}; //������
  //const char* addr;
  int ilRc;
  int d = 10000000, ret = 0;

  if (AF_INET == addr->family) {
    const sockaddr_in* addr1 = (sockaddr_in*)addr;
    sockaddr_in* addr2 = (sockaddr_in*)addr_out;
#ifdef WIN32
    PHOSTENT hostinfo;
    gethostname(hostname, sizeof(hostname));

    if ((hostinfo = gethostbyname(hostname)) == NULL) { //��ñ���ipv4��ַ
      errno = GetLastError();
      fprintf(stderr, "gethostbyname Error:%d\n", errno);
      return 1;
    }

    while (*(hostinfo->h_addr_list) != NULL) { //���ipv4��ַ
      struct in_addr addr3 = *(struct in_addr*) *hostinfo->h_addr_list;
      int dd = cmp_cnt_bytes(&addr1->sin_addr, &addr3, 4);

      if (dd < d) {
        d = dd;
        addr2->sin_family = AF_INET;
        addr2->sin_port = htons(port);
        addr2->sin_addr = addr3;
        ret = sizeof(sockaddr_in);
      }

      hostinfo->h_addr_list++;
    }

#else
    {
      char hname[128];
      struct hostent* hent;
      int i;
      gethostname(hname, sizeof(hname));
      //hent = gethostent();
      hent = gethostbyname(hname);
      printf("hostname: %s/naddress list: ", hent->h_name);

      for (i = 0; hent->h_addr_list[i]; i++) {
        struct in_addr addr3 = *(struct in_addr*)(hent->h_addr_list[i]);
        int dd = cmp_cnt_bytes(&addr1->sin_addr, &addr3, 4);

        if (dd < d) {
          d = dd;
          addr2->sin_family = AF_INET;
          addr2->sin_port = htons(port);
          addr2->sin_addr = addr3;
          ret = sizeof(sockaddr_in);
        }
      }
    }
#endif
  }
  else if (AF_INET6 == addr->family) {
    struct addrinfo* ailist, *aip;
    struct addrinfo hint;
    struct sockaddr_in6* addr1 = (struct sockaddr_in6*)addr;
    sockaddr_in6* addr2 = (sockaddr_in6*)addr_out;
    char portstr[32]; //�˿ں�
    hint.ai_family = AF_INET6; // hint ���޶�����
    hint.ai_socktype = SOCK_STREAM; // ����������� socket type ���� SOCK����DGRAM
    hint.ai_flags = AI_PASSIVE; // flags �ı�־�ܶ� �����õ���AI_CANONNAME;
    hint.ai_protocol = 0; // ����Э�� һ��Ϊ0��Ĭ��
    hint.ai_addrlen = 0; // ���治�������ã�Ϊ0������ΪNULL
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    _snprintf(portstr, 32, "%d", port);
    ilRc = getaddrinfo(hostname, portstr, &hint, &ailist); //ͨ����������õ�ַ��Ϣ

    if (ilRc < 0) {
      char str_error[100];
      strcpy(str_error, (char*)gai_strerror(errno));
      printf("str_error = %s", str_error);
      return 0;
    }

    if (ailist == NULL) {
      printf("sorry not find the IP address,please try again \n");
    }

    for (aip = ailist; aip != NULL; aip = aip->ai_next) { // ��ʾ��ȡ����Ϣ
      assert(aip->ai_family == AF_INET6);
      struct sockaddr_in6* addr3 = (struct sockaddr_in6*)aip->ai_addr; // Ϊʲô��for ѭ�� �������¿�
      int dd = cmp_cnt_bytes(&addr1->sin6_addr, &addr3->sin6_addr, sizeof(in6_addr));

      if (dd < d) {
        d = dd;
        *addr2 = *addr3;
        ret = sizeof(sockaddr_in6);
      }
    }
  }

  return ret;
}
BOOL CPMDEV_CALL CPM_GetSockName(SOCKET sock, addr_in* lpLocalIP)
{
  return sock_name(sock, lpLocalIP);
}
int CPMDEV_CALL CPM_GetLastError()
{
  return sock_error();
}
//��������: Send
//����ʱ��: 2011/07/08 16:54
//�� ��: sock �׽���
// pbData �����ڴ�
// nLen ���ݴ�С
//�ļ�����: lcy
//����˵��: ��������
BOOL CPMDEV_CALL CPM_Send(SOCKET hSOCK, const char* pbData, int* lpLen)
{
  if (NULL == pbData || NULL == lpLen) {
    return false;
  }

  int32 nLen = m_pProc->Send(hSOCK, pbData, *lpLen, NULL);
  BOOL ret = (nLen == *lpLen);
  *lpLen = nLen;
  return ret;
}
//��������: Send
//����ʱ��: 2011/07/08 16:54
//�� ��: sock �׽���
// pbData �����ڴ�
// nLen ���ݴ�С
//�ļ�����: lcy
//����˵��: ��������
int32 CNetManager_SendTo(SOCKET sock, const int8* pbData, int32 nLen, addr_in* lpAddr)
{
  if (NULL == m_pProc || NULL == pbData) {
    return false;
  }

  return m_pProc->Send(sock, pbData, nLen, lpAddr);
}
BOOL CPMDEV_CALL CPM_SendTo(SOCKET hSOCK, const addr_in* addr, const char* pbData, int* lpLen)
{
  if (NULL == m_pProc || NULL == pbData || NULL == lpLen) {
    return false;
  }

  int32 nLen = m_pProc->Send(hSOCK, pbData, *lpLen, addr);
  BOOL ret = (nLen == *lpLen);
  *lpLen = nLen;
  return ret;
}
//��������: Recv
//����ʱ��: 2011/07/08 16:54
//�� ��: sock �׽���
//�ļ�����: lcy
//����˵��: �������ݽ���
BOOL CPMDEV_CALL CPM_Recv(SOCKET sock , int type, void* user)
{
  if (type >= 0) {
    m_pProc->StartRecv(sock , type, user);
  }

  return 0;
}
//��������: StartWork
//����ʱ��: 2011/07/08 16:55
//�� ��: bStartUp ������ر����ݴ�������
//�ļ�����: lcy
//����˵��: ������ر����ݴ�������
BOOL CPMDEV_CALL CPM_StartWork(BOOL bStartUp)
{
  return m_pProc->StartUpWork(bStartUp);
}
//��������: OnListen
//����ʱ��: 2011/07/08 16:56
//�� ��: sock �׽���
// Addr �ͻ��˵�ַ
//�ļ�����: lcy
//����˵��: �����ص�
void CNetManager_OnListen(SOCKET sock, const addr_in* Addr, void* user)
{
  if (m_lpfnListen) {
    m_lpfnListen(sock, Addr, m_lpListenContext, user);
  }
}
//��������: OnRecv
//����ʱ��: 2011/07/08 16:56
//�� ��: sock �׽���
// pbData �����ڴ��׵�ַ
// nLen �����ڴ��С
//�ļ�����: lcy
//����˵��: ���ݽ��ܻص�
void CNetManager_OnRecv(SOCKET sock, const int8* pbData, int32 nLen, const addr_in* lpAddr, void* user)
{
  if (lpAddr == NULL) {
    if (m_lpfnRecv) {
      m_lpfnRecv(sock, pbData, nLen, m_lpRecvContext, user);
    }
  }
  else {
    if (m_lpfnRecvFrom) {
      m_lpfnRecvFrom(sock, pbData, nLen, lpAddr, m_lpRecvFromContext, user);
    }
  }
}

