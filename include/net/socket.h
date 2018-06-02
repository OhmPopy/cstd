
#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <windows.h>
#ifndef _WINSOCKAPI_           // û�а���winsock.h
#define _WINSOCKAPI_           // �����ٰ���winsock.h
#ifndef _WINSOCK2API_      // û�а���winsock2.h
#define _WINSOCK2API_      // �����ٰ���winsock2.h
#include <winsock2.h>
#pragma comment(lib, "ws32_2.lib")
#endif
#endif
#pragma comment(lib,"ws2_32.lib")

typedef unsigned char BYTE;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

#ifndef TRACE
#ifdef _DEBUG
#define TRACE  printf
#else
int trace_impl(const char* fmt, ...)
{
  return 0;
}
#define TRACE  trace_impl
#endif
#endif

// �ͷ�WinSock.
static void net_exit(void)
{
  WSACleanup();
}

static int net_init(BYTE minorVer, BYTE majorVer)
{
  static int wasInitialized = 0;

  // check initialization status
  if (!wasInitialized) {
    //��ʼ��WS2_32.DLL
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(minorVer, majorVer);
    int nr = WSAStartup(wVersionRequested, &wsaData);

    if (nr != 0) {
      return 1;
    }

    // ��� WinSock �汾.
    if (wsaData.wVersion != wVersionRequested) {
      TRACE("WinSock version not supported\n");
      WSACleanup();
      return 0;
    }

    atexit(net_exit);
    wasInitialized = 1;
    return 1;
  }

  return 0;
}

typedef struct socket_t {
  SOCKET s;
} socket_t;

static int socket_open(socket_t* s, int af, int type, int protocol)
{
  SOCKET so;
  net_init(2, 2);
  so = socket(af, type, protocol);

  if (so != INVALID_SOCKET) {
    s->s = so;
    return 1;
  }

  return 0;
}
static int socket_close(socket_t* s)
{
  if (s->s) {
    closesocket(s->s);
    s->s = INVALID_SOCKET;
  }

  return 0;
}
static int socket_connect(socket_t* s, const void* name, int namelen)
{
  int ret = connect(s->s, (PSOCKADDR)name, namelen);
  return ret != SOCKET_ERROR;
}
static int socket_last_error()
{
  return WSAGetLastError();
}
static int socket_send(socket_t* s, const void* buf, int len, int flags)
{
  return send(s->s, (const char*)buf, len, flags);
}
static int socket_recv(socket_t* s, void* buf, int len, int flags)
{
  return recv(s->s, (char*)buf, len, flags);
}
static int socket_safe_send(socket_t* s, const void* buf, int len, int flags)
{
  int i, n = 0;
  const char* bufc = (const char*)buf;

  for (; n < len; n += i) {
    if ((i = send(s->s, bufc + n, len - n, flags)) == SOCKET_ERROR) {
      return SOCKET_ERROR;
    }

    n += i;
  }

  return len;
}
static int socket_safe_recv(socket_t* s, void* buf, int len, int flags)
{
  int i, n = 0;
  char* bufc = (char*)buf;

  for (; n < len; n += i) {
    if ((i = recv(s->s, bufc + n, len - n, flags)) == SOCKET_ERROR) {
      return SOCKET_ERROR;
    }
  }

  return len;
}
static int socket_sendstr(socket_t* s, const void* szHttp, int iLen)
{
  int ret;

  if (iLen < 0) {
    iLen = strlen((const char*)szHttp);
  }

  ret = socket_send(s, szHttp, iLen, 0);

  if (ret == SOCKET_ERROR) {
    socket_close(s);
    TRACE("��������ʧ��!\n");
    return FALSE;
  }

  return TRUE;
}

static int socket_connect_host(socket_t* s, const char* host, int port)
{
  SOCKADDR_IN saServer; // �������׽��ֵ�ַ.
  PHOSTENT phostent = NULL; // ָ��HOSTENT�ṹָ��

  // ����һ���󶨵���������TCP/IP�׽���.
  if (!socket_open(s, AF_INET, SOCK_STREAM, 0)) {
    TRACE("Allocating socket failed. Error: %d\n", WSAGetLastError());
    return 0;
  }

  // ʹ��TCP/IPЭ��.
  saServer.sin_family = AF_INET;

  // ��ȡ��������ص���Ϣ.
  if ((phostent = gethostbyname(host)) == NULL) {
    TRACE("Unable to get the host name. Error: %d\n", WSAGetLastError());
    socket_close(s);
    return 0;
  }

  // ���׽���IP��ַ��ֵ.
  memcpy((char*) & (saServer.sin_addr), phostent->h_addr, phostent->h_length);

  // �趨�׽��ֶ˿ں�.
  saServer.sin_port = htons(port);

  // ���������������׽�������.
  if (!socket_connect(s, (PSOCKADDR) &saServer, sizeof(saServer))) {
    TRACE("Connecting to the server failed. Error: %d\n", socket_last_error());
    socket_close(s);
    return 0;
  }

  return 1;
}


#define MAX_PENDING_CONNECTS 4 // �ȴ����еĳ���.

static int socket_listening(socket_t* s, int port)
{
  SOCKADDR_IN local_sin; // �����׽��ֵ�ַ.
  s->s = INVALID_SOCKET;

  // ����TCP/IP�׽���.
  if ((s->s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    TRACE("Allocating socket failed. Error: %d\n", WSAGetLastError());
    return 0;
  }

  // ���׽�����Ϣ�ṹ��ֵ.
  local_sin.sin_family = AF_INET;
  local_sin.sin_port = htons(port);
  local_sin.sin_addr.s_addr = htonl(INADDR_ANY);

  // ���б�����ַ������׽��ְ�.
  if (bind(s->s, (struct sockaddr*) &local_sin, sizeof(local_sin)) == SOCKET_ERROR) {
    TRACE("Binding socket failed. Error: %d\n", WSAGetLastError());
    closesocket(s->s);
    s->s = 0;
    return 0;
  }

  // �����׽��ֶ��ⲿ���ӵļ���.
  if (listen(s->s, MAX_PENDING_CONNECTS) == SOCKET_ERROR) {
    TRACE("Listening to the client failed. Error: %d\n", WSAGetLastError());
    closesocket(s->s);
    s->s = 0;
    return 0;
  }

  return 1;
}
static int socket_accept(socket_t* s, struct sockaddr* addr, IN OUT int* addrlen, socket_t* sClient)
{
  sClient->s = accept(s->s, addr, addrlen);
  return sClient->s != INVALID_SOCKET;
}

TCHAR* WSAGetLastErrMsg()
{
  static TCHAR buf[70];
  int error = WSAGetLastError();
  buf[0] = 0;
  _snprintf(buf, sizeof(buf) / sizeof(buf[0]), "err=%d, %s, %s", error, "", "");

  switch (error) {
#define WSA_ERROR_DEF(a,b,c)  case a: {_snprintf(buf, sizeof(buf)/sizeof(buf[0]), "err=%d, %s, %s", a, #b, #c );}break;
#include "WSAGetLastError.inl"
#undef WSA_ERROR_DEF
  };

  return buf;
}


#endif // _NET_H_
