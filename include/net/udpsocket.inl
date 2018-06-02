#ifndef _WINSOCKAPI_ // û�а���winsock.h
#define _WINSOCKAPI_ // �����ٰ���winsock.h
#ifndef _WINSOCK2API_ // û�а���winsock2.h
#define _WINSOCK2API_ // �����ٰ���winsock2.h
#include <winsock2.h>
#pragma comment(lib, "ws32_2.lib")
#endif
#endif
#pragma comment(lib,"ws2_32.lib")
typedef struct udpsocket {
  SOCKET m_Socket;
  SOCKADDR_IN m_LocalAddr;
  SOCKADDR_IN m_RemoteAddr;
} udpsocket;
//��ʼ��
WORD InitializeSocket()
{
  WORD wVersionRequested;
  WSADATA wsaData;
  WORD wVerGet = 0;
  wVersionRequested = MAKEWORD(2, 2);
  while (1) {
    int err;
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
      BYTE* p = (BYTE*)&wVersionRequested;
      if (p[1] > 0) {
        p[1]--;
      }
      else if (p[1] == 0) {
        p[1] = 1;
        if (p[0] > 1) {
          p[0]--;
        }
        else if (p[0] == 0) {
          break;
        }
      }
      continue;
    }
    else {
      wVerGet = wsaData.wVersion;
      break;
    }
  }
  return wVerGet;
}
int CleanUpSocket()
{
  return WSACleanup();
}
u_long Name2Inet(LPCSTR szAddress)
{
  BOOL name;
  int i;
  if (szAddress == NULL || strlen(szAddress) < 1) {
    return htonl(INADDR_ANY);
  }
  name = FALSE;
  for (i = 0; szAddress[i]; i++) {
    if ((!isdigit(szAddress[i])) && szAddress[i] != '.') {
      name = TRUE;
      break;
    }
  }
  //���Ǽ����������IP
  if (!name) {
    return inet_addr(szAddress);
  }
  //�Ǽ������
  else {
    struct hostent* host = gethostbyname(szAddress);
    if (host == NULL) {
      return htonl(INADDR_ANY);
    }
    return *(long*)host->h_addr;
  }
}
BOOL udpsocket_set(udpsocket* s, UINT uSocketPort, char* pszSockAddr)
{
  BOOL bRet = FALSE;
  if (!s->m_Socket) {
    s->m_LocalAddr.sin_family = AF_INET;
    if (pszSockAddr) {
      s->m_LocalAddr.sin_addr.s_addr = Name2Inet(pszSockAddr);
    }
    else {
      s->m_LocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    if ((s->m_Socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) != INVALID_SOCKET) {
      //�������������ݱ��׽ӿ�
      s->m_LocalAddr.sin_port = htons(uSocketPort);
      if (bind(s->m_Socket, (SOCKADDR*)&s->m_LocalAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
        s->m_Socket = 0;
        memset((void*)&s->m_LocalAddr, 0, sizeof(s->m_LocalAddr));
      }
      else {
        bRet = TRUE;
      }
    }
  }
  return bRet;
}
int udpsocket_close(udpsocket* s)
{
  int iRet;
  shutdown(s->m_Socket, 1);
  iRet = closesocket(s->m_Socket);
  memset((void*)&s->m_LocalAddr, 0, sizeof(s->m_LocalAddr));
  memset((void*)&s->m_RemoteAddr, 0, sizeof(s->m_RemoteAddr));
  s->m_Socket = 0;
  return iRet;
}
int udpsocket_sendto_s(udpsocket* s, const char* pBuf, int iLen, char* pszRemoteSockAddr, unsigned short uPort)
{
  SOCKADDR_IN RemoteAddr;
  RemoteAddr.sin_family = AF_INET;
  RemoteAddr.sin_addr.s_addr = Name2Inet(pszRemoteSockAddr);
  RemoteAddr.sin_port = htons(uPort);
  return sendto(s->m_Socket, pBuf, iLen, 0, (SOCKADDR*)&RemoteAddr, sizeof(SOCKADDR_IN));
}
int udpsocket_sendto(udpsocket* s, const char* pBuf, int iLen, SOCKADDR* pRemoteAddr)
{
  if (!pRemoteAddr) {
    pRemoteAddr = (SOCKADDR*)&s->m_RemoteAddr;
  }
  return sendto(s->m_Socket, pBuf, iLen, 0, pRemoteAddr, sizeof(SOCKADDR_IN));
}
int udpsocket_recvfrom(udpsocket* s, char* pBuf, int iLen, SOCKADDR* pRemoteAddr)
{
  int addrlen = sizeof(SOCKADDR_IN);
  SOCKADDR_IN RemoteAddr;
  if (!pRemoteAddr) {
    pRemoteAddr = (SOCKADDR*)&RemoteAddr;
  }
  return recvfrom(s->m_Socket, pBuf, iLen, 0, pRemoteAddr, &addrlen);
}
u_long GetLocal_in_addr()
{
  HOSTENT* pHostEnt;
  char szHostname[256];
  if (gethostname(szHostname, sizeof(szHostname))) {
    return htonl(INADDR_ANY) ;
  }
  //�õ�������Ϣ
  pHostEnt = gethostbyname(szHostname);
  if (pHostEnt == NULL) {
    return htonl(INADDR_ANY);
  }
  //����ַ����
  if (pHostEnt->h_length != 4) {
    return htonl(INADDR_ANY);
  }
  return ((struct in_addr*)*pHostEnt->h_addr_list)->s_addr;
}

