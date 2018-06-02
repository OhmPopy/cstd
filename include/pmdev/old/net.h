
#ifndef _NET_H_
#define _NET_H_

#ifdef WIN32
#include <winsock.h>

#ifndef _WINSOCKAPI_           // û�а���winsock.h
#define _WINSOCKAPI_           // �����ٰ���winsock.h
#ifndef _WINSOCK2API_      // û�а���winsock2.h
#define _WINSOCK2API_      // �����ٰ���winsock2.h
#include <winsock2.h>
#pragma comment(lib, "ws32_2.lib")
#endif
#endif
#pragma comment(lib,"ws2_32.lib")


#define snprintf _snprintf
//��ʼ��
int netinit()
{
  WSADATA wsd; //WSADATA����
  static int netinited = 0;

  if (!netinited) {
    //��ʼ���׽��ֶ�̬��
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0) {
      //printf("WSAStartup failed!\n");
      return FALSE;
    }

    netinited = 1;
  }

  return 1;
}

int netexit()
{
  WSACleanup();
  return 0;
}



#else
#include <netdb.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <arpa/inet.h>



typedef char INT8;
typedef unsigned char UINT8;
typedef short INT16;
typedef unsigned short UINT16;
typedef int INT32;
typedef unsigned int UINT32;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;


typedef unsigned int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define netinit()
#define netexit()
#define ZeroMemory(p, n)   memset(p, 0, n)
#define closesocket(s)   close(s)

char* getlocalip()
{
  int inet_sock;
  struct ifreq ifr;
  inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
  char* ip = 0;
  //eth0Ϊ�ӿڵ�����

  strcpy(ifr.ifr_name, "eth1");
  //SIOCGIFADDR��־�����ȡ�ӿڵ�ַ

  if (ioctl(inet_sock, SIOCGIFADDR, &ifr) ==  0) {
    perror("ioctl");
  }

  ip = inet_ntoa(((struct sockaddr_in*) & (ifr.ifr_addr))->sin_addr);
  //printf("%s\n", ip);
  return ip;
}

#endif









#ifdef WIN32
#define SOCK_LENTH int
#define T_VOID void
#define T_HANDLE HANDLE
#define SLEEP(Milliseconds) {Sleep(Milliseconds);}
#define LASTERROR WSAGetLastError()
#define MILLISEC       1
#define MEMRESERVER_BLOCKS  10 //�ڴ���С
#else
#define SOCKET int
#define SOCK_LENTH socklen_t
#define T_HANDLE pthread_t
#define T_VOID void*
#define INVALID_SOCKET 0
#define SOCKET_ERROR   -1
#define closesocket close
#define ioctlsocket ioctl
#define SLEEP(Milliseconds)  {struct timeval tval;tval.tv_sec= 0;tval.tv_usec=(Milliseconds); select( 0 , NULL , NULL , NULL , &tval ); }
#define LASTERROR errno
#define WSAECONNRESET   ECONNRESET
#define WSAECONNABORTED ECONNABORTED
#define WSAEWOULDBLOCK  EWOULDBLOCK
#define WSAEINTR        EINTR
#define MILLISEC        1000
#define SD_BOTH         SHUT_RDWR
#define MEMRESERVER_BLOCKS  2 //�ڴ���С
#define INFINITE 0xFFFFFFFF

#endif

#define TIME_OUT      5 //10s
#define DEFAULT(x)


typedef struct sockaddr SOCKADDR, *PSOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef char  int8;
typedef int   int32;
typedef short int16;


#define IN
#define OUT

#define MAX_SEND_COUNTS 5 //������������ʹ���
#define MAX_CONNECTS    5 //һ���������������
#define RECV_TIME_OUT   (10*1000) //30s
#define PACK_SIZE (60*1024)
#define MEM_UNIT  (8*1024)

#define ALIGN(bytes, alignments) (((bytes)+((alignments)-1))&~((alignments)-1))

typedef struct {
  uint32 msg_identify; //��ʶ
  uint32 msg_offset ;  //ƫ��
  uint32 msg_len;      //���ݳ���
  uint32 msg_total;    //�����ܳ���
  uint32 msg_checksum; //�����
  //  int8   msg_data[4];  //����
} SOCK_DATA_HEADER;




typedef struct {
  unsigned short Port;                //�˿�
  char           IP_Address[16];      //���ʮ����IP��ַ
} SOCK_ADDRESS, *PSOCK_ADDRESS;







#endif // _NET_H_
