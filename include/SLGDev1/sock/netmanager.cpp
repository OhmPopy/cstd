
#include "../slgdev_private.h"
#include "openssl.h"
#include "overlappedprocessor.h"
#include "selectprocessor.h"
#include "socket.h"
#include <assert.h>
volatile BOOL gInitialized = false;
#ifdef WIN32
#define INITSOCK()  (InitSocket())
#define UINITSOCK() (UnInitSocket())

BOOL InitSocket()
{
    if (!gInitialized)
    {
        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA WSAData = { 0 };

        if (WSAStartup(wVersionRequested, &WSAData))
        {
            return false;
        }

        if (LOBYTE(WSAData.wVersion) != 2 || HIBYTE(WSAData.wVersion) != 2)
        {
            return false;
        }

    }

    return true ;
}

void UnInitSocket()
{
    if (gInitialized)
    {
        gInitialized = false;
        WSACleanup() ;
    }

}

#else
#define INITSOCK()
#define UINITSOCK()
#endif
CNetProcessor* m_pProc = NULL;
ListenCallBack m_lpfnListen = NULL;
void* m_lpListenContext = NULL;
DataRecvCallBack m_lpfnRecv = NULL;
void* m_lpRecvContext = NULL;
DataRecvFromCallBack m_lpfnRecvFrom = NULL;
void* m_lpRecvFromContext = NULL;
BOOL SLGDEV_CALL SLG_SetListenCallBack(ListenCallBack lpfnListen, void* pvContext)
{
    m_lpfnListen = lpfnListen;
    m_lpListenContext = pvContext;
    return 0;
}

BOOL SLGDEV_CALL SLG_SetDataRecvCallBack(DataRecvCallBack lpfnRecv, void* pvContext)
{
    m_lpfnRecv = lpfnRecv;
    m_lpRecvContext = pvContext;
    return 0;
}

BOOL SLGDEV_CALL SLG_SetDataRecvFromCallBack(DataRecvFromCallBack lpfnRecv, void* pvContext)
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
BOOL SLGDEV_CALL SLG_UnInitSock()
{
    if (m_pProc)
    {
        delete m_pProc;
        m_pProc = NULL;
    }

    m_lpfnListen = NULL;
    m_lpListenContext = NULL;
    m_lpfnRecv = NULL;
    m_lpRecvContext = NULL;
    gInitialized = false;
    UINITSOCK();
    return 0;
}

//��������: Init
//����ʱ��: 2011/07/08 16:44
//�� ��: Model �׽��ִ���ģ��
// nMemPoolSize �ڴ�ش�С
//�ļ�����: lcy
//����˵��: �����׽���ģ��
BOOL SLGDEV_CALL SLG_InitSock(NET_MODEL Model, uint32 uMemPoolSize, uint32 uMemPoolUnit)
{
    if (gInitialized)
    {
        return true;
    }

    INITSOCK();

    if (uMemPoolSize <= 0)
    {
        uMemPoolSize = MEMRESERVER_SIZE;
    }

    if (uMemPoolUnit <= 0)
    {
        uMemPoolUnit = MEMRESERVER_UNIT;
    }

    DEBUG_INFO("%s" , "CNetManager_Init");

    if (m_pProc)
    {
        return true;
    }

    try
    {
        switch (Model)
        {
        case NET_OVERLAPPED:
        {
            DEBUG_INFO("%s" , "NET_OVERLAPPED");
#ifdef WIN32
#ifndef WINCE
            COpenSsl::Obj().Init(true);

            if (NULL == m_pProc)
            {
                m_pProc = new COverlappedProcessor(uMemPoolSize, uMemPoolUnit);
            }

#endif
#else
            assert(false);
#endif
        }

        break;

        case NET_SELECTED:
        {
            DEBUG_INFO("%s" , "NET_SELECTED");
            COpenSsl::Obj().Init(true);

            if (NULL == m_pProc)
            {
                m_pProc = new CSelectProcessor(uMemPoolSize, uMemPoolUnit);
            }

        }

        break;

        default:
            assert(false);
            break;
        }

    }

    catch (std::bad_alloc)
    {
        return false;
    }

    gInitialized = m_pProc != NULL;

    if (!gInitialized)
    {
        SLG_UnInitSock();
    }

    return gInitialized;
}

//��������: CreateSocket
//����ʱ��: 2011/07/08 16:46
//�� ��: type �׽�������(�ֽ��������ݱ�)
//�ļ�����: lcy
//����˵��: �����׽���
SOCKET SLGDEV_CALL SLG_CreateSock(int af, int nSOCKType)
{
    SOCKET sock = sock_create(af, nSOCKType);
    DEBUG_INFO("SLG_CreateSock %d", sock);
    return sock;
}

//��������: ReleaseSocket
//����ʱ��: 2011/07/08 16:47
//�� ��: sock �׽���
//�ļ�����: lcy
//����˵��: �ͷ��׽���
BOOL SLGDEV_CALL SLG_ReleaseSock(SOCKET sock, CSockBase* user)
{
    DEBUG_INFO("SLG_ReleaseSock %d", sock);
    m_pProc->StopRecv(sock, user);
    sock_close(sock);
    return 0;
}

//��������: Connect
//����ʱ��: 2011/07/08 16:47
//�� ��: sock �׽���
//PeerAddr ��������ַ
//�ļ�����: lcy
//����˵��: ���ӷ�����
BOOL SLGDEV_CALL SLG_Connect(SOCKET sock, const addr_in* addr)
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
BOOL SLGDEV_CALL SLG_Bind(SOCKET sock, const addr_in* addr)
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
BOOL SLGDEV_CALL SLG_Listen(SOCKET sock, CSockBase* user)
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
BOOL SLGDEV_CALL SLG_AddInIPGroup(SOCKET sock, const addr_in* addr)
{
    //assert(0);
    return 0;//SLG_Bind(sock, ANYIP(af), GroupIPAddr.Port) && sock_join_group(sock, GroupIPAddr.IP_Address);
}

//��������: DropIPGroup
//����ʱ��: 2011/07/08 16:50 s
//�� ��: sock �׽���
// GroupIPAddr �ಥ���ַ
//�ļ�����: lcy
//����˵��: �˳��ಥ��
BOOL SLGDEV_CALL SLG_DropOutIPGroup(SOCKET sock, const addr_in* GroupIPAddr)
{
    sock_drop_group(sock, GroupIPAddr);
    return 0;
}

//��������: SetRecvBuf
//����ʱ��: 2011/07/08 16:52
//�� ��: sock �׽���
// nSize ��������С
//�ļ�����: lcy
//����˵��: ���ý��ܻ�������С
BOOL SLGDEV_CALL SLG_SetRecvBufSize(SOCKET sock, int32 nSize)
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
BOOL SLGDEV_CALL SLG_SetSendBufSize(SOCKET sock, int32 nSize)
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
BOOL SLGDEV_CALL SLG_GetPeerAddr(SOCKET sock, addr_in* addr)
{
    return sock_addr(sock, addr);
}

int cmp_cnt_bytes(const void* a, const void* b, int n)
{
    const unsigned char* ua = (const unsigned char*)a;
    const unsigned char* ub = (const unsigned char*)b;
    int i, d = 0;

    for (i = 0; i < n; ++i)
    {
        d += ua[i] != ub[i];
    }

    return d;
}

BOOL SLGDEV_CALL SLG_GetLocalIP(const addr_in* addr, int port, addr_in* addr_out)
{
    int ret = 0;
#ifdef _WIN32
    char hostname[255] = {0}; //������
    //const char* addr;
    int ilRc;
    int d = 10000000;

    if (AF_INET == addr->family)
    {
        const sockaddr_in* addr1 = (sockaddr_in*)addr;
        sockaddr_in* addr2 = (sockaddr_in*)addr_out;
#ifdef _WIN32
        PHOSTENT hostinfo;
        gethostname(hostname, sizeof(hostname));

        if ((hostinfo = gethostbyname(hostname)) == NULL)   //��ñ���ipv4��ַ
        {
            fprintf(stderr, "gethostbyname Error:%d\n", GetLastError());
            return 1;
        }

        while (*(hostinfo->h_addr_list) != NULL)   //���ipv4��ַ
        {
            struct in_addr addr3 = *(struct in_addr*) *hostinfo->h_addr_list;
            int dd = cmp_cnt_bytes(&addr1->sin_addr, &addr3, 4);

            if (dd < d)
            {
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
            DEBUG_INFO("hostname: %s/naddress list: ", hent->h_name);

            for (i = 0; hent->h_addr_list[i]; i++)
            {
                struct in_addr addr3 = *(struct in_addr*)(hent->h_addr_list[i]);
                int dd = cmp_cnt_bytes(&addr1->sin_addr, &addr3, 4);

                if (dd < d)
                {
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

    else if (AF_INET6 == addr->family)
    {
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

        if (ilRc < 0)
        {
            char str_error[100];
            strcpy(str_error, (char*)gai_strerror(GetLastError()));
            DEBUG_INFO("str_error = %s", str_error);
            return 0;
        }

        if (ailist == NULL)
        {
            DEBUG_INFO("%s", "sorry not find the IP address,please try again \n");
        }

        for (aip = ailist; aip != NULL; aip = aip->ai_next)   // ��ʾ��ȡ����Ϣ
        {
            assert(aip->ai_family == AF_INET6);
            struct sockaddr_in6* addr3 = (struct sockaddr_in6*)aip->ai_addr; // Ϊʲô��for ѭ�� �������¿�
            int dd = cmp_cnt_bytes(&addr1->sin6_addr, &addr3->sin6_addr, sizeof(in6_addr));

            if (dd < d)
            {
                d = dd;
                *addr2 = *addr3;
                ret = sizeof(sockaddr_in6);
            }

        }

    }
#endif

    return ret;
}

BOOL SLGDEV_CALL SLG_GetSockName(SOCKET sock, addr_in* lpLocalIP)
{
    return sock_name(sock, lpLocalIP);
}

int SLGDEV_CALL SLG_GetLastError()
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
BOOL SLGDEV_CALL SLG_Send(SOCKET hSOCK, const char* pbData, int* lpLen)
{
    if (NULL == pbData || NULL == lpLen)
    {
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
    if (NULL == m_pProc || NULL == pbData)
    {
        return false;
    }

    return m_pProc->Send(sock, pbData, nLen, lpAddr);
}

BOOL SLGDEV_CALL SLG_SendTo(SOCKET hSOCK, const addr_in* addr, const char* pbData, int* lpLen)
{
    if (NULL == m_pProc || NULL == pbData || NULL == lpLen)
    {
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
BOOL SLGDEV_CALL SLG_Recv(SOCKET sock , CSockBase* user)
{
    return m_pProc->StartRecv(sock , user);
    return FALSE;
}

//��������: StartWork
//����ʱ��: 2011/07/08 16:55
//�� ��: bStartUp ������ر����ݴ�������
//�ļ�����: lcy
//����˵��: ������ر����ݴ�������
BOOL SLGDEV_CALL SLG_StartWork(BOOL bStartUp)
{
    return m_pProc->StartUpWork(bStartUp);
}

//��������: OnListen
//����ʱ��: 2011/07/08 16:56
//�� ��: sock �׽���
// Addr �ͻ��˵�ַ
//�ļ�����: lcy
//����˵��: �����ص�
void CNetManager_OnListen(SOCKET sock, const addr_in* Addr, CSockBase* user)
{
    if (m_lpfnListen)
    {
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
void CNetManager_OnRecv(SOCKET sock, const int8* pbData, int32 nLen, const addr_in* lpAddr, CSockBase* user)
{
    if (lpAddr == NULL)
    {
        if (m_lpfnRecv)
        {
            m_lpfnRecv(sock, pbData, nLen, m_lpRecvContext, user);
        }

    }

    else
    {
        if (m_lpfnRecvFrom)
        {
            m_lpfnRecvFrom(sock, pbData, nLen, lpAddr, m_lpRecvFromContext, user);
        }

    }

}

