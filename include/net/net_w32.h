
#ifndef _NETINIT_H_
#define _NETINIT_H_

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

typedef unsigned char BYTE;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

int InitSocket(BYTE minorVer, BYTE majorVer);

#define __INITSOCKET22()  InitSocket(2, 2)

// extern const int __wasInitSocket = __INITSOCKET22();

// ��a.b.c.d:port ��ʽ�趨IP�˿ں�, ���������ֽ�˳��ת��Ϊ�����ֽ�˳��
struct sockaddr_in make_sockaddr_in(const char* cp);

int net_sendtoAll(SOCKET* f, int nClientNum, const char* buf, int len, int flags);
int net_safesend(SOCKET s, const char* buf, size_t n, int flags);
int net_saferecv(SOCKET s, char* buf, size_t n, int flags);
int net_safesendtoAll(SOCKET* f, int nClientNum, const char* buf, int len, int flags);
int safesendMattoAll(SOCKET* f, int nClientNum, int h, int w, const char* A, int al, int flags);
int net_saferecvMat(SOCKET s, int h, int w, char* A, int al, int flags);

#define RFIND_OUT(f, l, minchar, maxchar)  for (; f!=l && (l[-1]<minchar|| l[-1]>maxchar); --l) {}
#define RFIND_IN(f, l, minchar, maxchar)  for (; f!=l && !(l[-1]<minchar|| l[-1]>maxchar); --l) {}

#define MAKE_SOCKADDR_IN(_OUTADDR, _FAMILY, _PORT, _ADDR)                     \
  do {                                                                          \
    (_OUTADDR).sin_family = _FAMILY;                                            \
    (_OUTADDR).sin_port = htons(_PORT);                                         \
    (_OUTADDR).sin_addr.S_un.S_addr = _ADDR;                                    \
  } while(0)

// �����׽���(AF_INET,SOCK_STREAM,IPPROTO_TCP)
// ʹ��ǰ����һ���µ��׽���
// af: ͨ�ŷ���������������ַ���ͣ�һ��ΪAF_INET����ʾ��Internet����ʹ��
// type: �׽�������
// procotol: ʹ�õ��ض�Э��
#define NET_OPEN_TCP() (socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))

// �ر��׽��֣��ͷ���ռ�е���Դ�����������˺Ϳͻ��ˣ�
#define NET_CLOSE(_SOCKET) (( _SOCKET != INVALID_SOCKET ) ? (closesocket( _SOCKET )) : ((_SOCKET)=0))

// �ж��׽������ӣ�֪ͨ�������˻�ͻ���ֹͣ���պͷ������ݡ����������˺Ϳͻ��ˣ�
// HowΪ������ֹ��Щ������ȡֵΪ��SD_RECEIVE��SD_SEND��SD_BOTH��
#define NET_SHUTDOWN(_SOCKET, _HOW)     shutdown(_SOCKET, _HOW)

// �׽��ֵ����᣺�������׽�����������׼��ͨ�š����ͻ��ˣ�
// sΪ��������Ѵ������׽��֡�
// nameΪ�������socket��ַ��
// namelenΪsocket��ַ�Ľṹ�ĳ���
#define NET_CONNECT(_SOCKET, _ADDR)  (connect(_SOCKET, (struct sockaddr*)&(_ADDR), sizeof(_ADDR)) != SOCKET_ERROR)

// ���׽��֣��������˺Ϳͻ��ˣ�
// �����ص�ַ�󶨵����������׽����ϡ�
// s: ����socket()���÷��صĲ���δ�����ӵ��׽������������׽��ֺţ���
// û�д���bind()����0������SOCKET_ERROR
#define NET_BIND(_SOCKET, _ADDR) (bind(_SOCKET, (struct sockaddr*)&(_ADDR), sizeof(_ADDR)) != SOCKET_ERROR)

// �������ӣ��������ˣ�
// �����������ӷ�������������Ը���������
#define NET_LISTEN(_SOCKET, _BACKLOG)  (listen(_SOCKET, _BACKLOG) != SOCKET_ERROR)

// �׽��ֵȴ����ӣ��������ˣ�
// �������ͻ���������������
// ����������ʱ, ���ر�����ͻ�����ַ�����׽���
// sΪ���ڼ���ģʽ���׽��֡�
// sockaddrΪ���ճɹ��󷵻ؿͻ��˵������ַ��
// addrlenΪ�����ַ�ĳ��ȡ�
#define NET_ACCEPT(_SOCKET, _ADDR, _ADDRLEN) accept(_SOCKET, (struct sockaddr*)&(_ADDR), &(_ADDRLEN))

// �׽��ַ������ݣ����������˺Ϳͻ��ˣ�
// sΪ�������˼������׽��֡�
// bufΪ���������ݻ�������ָ�롣
// lenΪ�������ݻ������ĳ��ȡ�
// flagsΪ���ݷ��ͱ�ǡ�
// ����ֵΪ�������ݵ��ַ�����
// ���ｲһ��������ͱ�ǣ�����8�����۵Ľ��ձ��Ҳһ����
// flagȡֵ����Ϊ0�������¶������ϣ�0��ʾû��������Ϊ��
// #define MSG_OOB 0x1 /* process out-of-band data */
// #define MSG_PEEK 0x2 /* peek at incoming message */
// #define MSG_DONTROUTE 0x4 /* send without using routing tables */
// MSG_OOB��ʾ����Ӧ�ô��ⷢ�ͣ���ν�������ݾ���TCP�������ݡ�
// MSG_PEEK��ʾʹ���õ����ݸ��Ƶ��������ڣ���������ϵͳ��������ɾ����
// MSG_DONTROUTE��ʾ��Ҫ����·�ɳ�ȥ��
#define NET_SEND(_SOCKET, _BUF, _LEN, _FLAGS) send(_SOCKET, (const char *)(_BUF), (_LEN)*sizeof(*(_BUF)), _FLAGS)
#define NET_SEND2(_SOCKET, _VAL, _FLAGS) send(_SOCKET, (const char *)&(_VAL), sizeof(_VAL), _FLAGS)


// ��������
// sΪ׼���������ݵ��׽��֡�
// bufΪ׼���������ݵĻ�������
// lenΪ׼���������ݻ������Ĵ�С��
// flagsΪ���ݽ��ձ�ǡ�
// ����ֵΪ���յ����ݵ��ַ�����
#define NET_RECV(_SOCKET, _BUF, _LEN, _FLAGS) recv(_SOCKET, (char *)(_BUF), (_LEN)*sizeof(*(_BUF)), _FLAGS)
#define NET_RECV2(_SOCKET, _VAL, _FLAGS) recv(_SOCKET, (char *)&(_VAL), sizeof(_VAL), _FLAGS)


// recvfrom(_SOCKET, _BUF, _LEN, _FLAGS, _FROM, _FROMLEN)

// ��·���á���select()
// �������һ�������׽���״̬��
// readfds:ָ��Ҫ��������ָ��
// writefds:ָ��Ҫ��д����ָ��
// exceptfds:ָ��Ҫ����Ƿ�����ָ��
// timeout:���ȴ�ʱ��
// int PASCAL FAR select(int nfds,fd_set FAR * readfds,fd_set FAR * writefds,
// fd_set FAR * exceptfds,const struct timeval FAR * timeout);
// ����ֵ����ֵ��select���� ��ֵ��ĳЩ�ļ��ɶ�д����� 0���ȴ���ʱ��û�пɶ�д�������ļ�

// sendto(_SOCKET, buf, len, flags, to, tolen);

// ����������#define SO_DEBUG 0x0001 /* turn on debugging info recording */
// �Ƿ����ģʽ#define SO_ACCEPTCONN 0x0002 /* socket has had listen() */
// �׽����������׽��ֵĵ�ַ��#define SO_REUSEADDR 0x0004 /* allow local address reuse */
// ��������#define SO_KEEPALIVE 0x0008 /* keep connections alive */
// ��Ҫ·�ɳ�ȥ#define SO_DONTROUTE 0x0010 /* just use interface addresses */
// ����Ϊ�㲥#define SO_BROADCAST 0x0020 /* permit sending of broadcast msgs */
// ʹ�û��ز�ͨ��Ӳ��#define SO_USELOOPBACK 0x0040 /* bypass hardware when possible */
// ��ǰ����ֵ#define SO_LINGER 0x0080 /* linger on close if data present */
// �Ƿ�����������#define SO_OOBINLINE 0x0100 /* leave received OOB data in line */
// ����LINGERѡ��#define SO_DONTLINGER (int)(~SO_LINGER)
// ���ͻ���������#define SO_SNDBUF 0x1001 /* send buffer size */
// ���ջ���������#define SO_RCVBUF 0x1002 /* receive buffer size */
// ���ͳ�ʱʱ��#define SO_SNDTIMEO 0x1005 /* send timeout */
// ���ճ�ʱʱ��#define SO_RCVTIMEO 0x1006 /* receive timeout */
// ����״̬#define SO_ERROR 0x1007 /* get error status and clear */
// �׽�������#define SO_TYPE 0x1008 /* get socket type */
// ��ȡsocket����
// sΪ����ȡ���Ե��׽��֡�
// levelΪ�׽���ѡ��ļ��𣬴�������ض�Э����׽���ר�еġ���IPЭ��ӦΪ IPPROTO_IP��
// optnameΪ��ȡѡ�������
// optvalΪ���ѡ��ֵ�Ļ�����ָ�롣
// optlenΪ�������ĳ���
// getsockopt(_SOCKET, level, optname, optval, optlen)

// ����socket����
// sΪ���������Ե��׽��֡�
// levelΪ�׽���ѡ��ļ����÷�ͬ�ϡ�
// optnameΪ����ѡ�������
// optvalΪ���ѡ��ֵ�Ļ�����ָ�롣
// optlenΪ�������ĳ���
// setsockopt(_SOCKET, level, optname, optval, optlen)

// ��Ϊ������ģʽioctlsocket(FIONBIO, &flag)
// ioctlsocket( _SOCKET, cmd, argp);

#define NET_GETSOCKNAME(_SOCKET, _ADDR, _NAMELEN) (getsockname(_SOCKET, (struct sockaddr*)&(_ADDR), &_NAMELEN) != SOCKET_ERROR)
#define NET_GETPEERNAME(_SOCKET, _ADDR, _NAMELEN) (getpeername(_SOCKET, (struct sockaddr*)&(_ADDR), &_NAMELEN) != SOCKET_ERROR)


// ��ʼ������
SOCKET net_start_server(int port);
// ��ʼ�ͻ���
SOCKET net_start_client(const char* StrAddrPort);

int net_acceptAll(SOCKET s, int nClientNum, SOCKET* pClient);

// ����recv��send֮��
int net_ssrr(SOCKET s, int k);
// ����send��recv֮��
int net_rrss(SOCKET s, int k);







#endif // _NETINIT_H_
