#include <winsock2.h> /*windows socket��ͷ�ļ���ϵͳ�����*/
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib") /*����API�������Ws2_32.lib��̬��*/
#define MAX_HOSTNAME_LAN 255
#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)
#define MAX_ADDR_LEN 16
struct ipheader {
  unsigned char ip_hl: 4; /*header length(��ͷ���ȣ�*/
  unsigned char ip_v: 4; /*version(�汾)*/
  unsigned char ip_tos; /*type os service��������*/
  unsigned short int ip_len; /*total length (�ܳ���)*/
  unsigned short int ip_id; /*identification (��ʶ��)*/
  unsigned short int ip_off; /*fragment offset field(����λ��)*/
  unsigned char ip_ttl; /*time to live (����ʱ��)*/
  unsigned char ip_p; /*protocol(Э��)*/
  unsigned short int ip_sum; /*checksum(У���)*/
  unsigned int ip_src; /*source address(Դ��ַ)*/
  unsigned int ip_dst; /*destination address(Ŀ�ĵ�ַ)*/
}; /* total ip header length: 20 bytes (=160 bits) */
typedef struct tcpheader {
  unsigned short int sport; /*source port (Դ�˿ں�)*/
  unsigned short int dport; /*destination port(Ŀ�Ķ˿ں�)*/
  unsigned int th_seq; /*sequence number(�������к�)*/
  unsigned int th_ack; /*acknowledgement number(ȷ��Ӧ���)*/
  unsigned char th_x: 4; /*unused(δʹ��)*/
  unsigned char th_off: 4; /*data offset(����ƫ����)*/
  unsigned char Flags; /*��־ȫ*/
  unsigned short int th_win; /*windows(����)*/
  unsigned short int th_sum; /*checksum(У���)*/
  unsigned short int th_urp; /*urgent pointer(����ָ��)*/
} TCP_HDR;
typedef struct udphdr {
  unsigned short sport; /*source port(Դ�˿ں�)*/
  unsigned short dport; /*destination port(Ŀ�Ķ˿ں�)*/
  unsigned short len; /*udp length(udp����)*/
  unsigned short cksum; /*udp checksum(udpУ���)*/
} UDP_HDR;
void main()
{
  SOCKET sock;
  WSADATA wsd;
  DWORD dwBytesRet;
  unsigned int optval = 1;
  unsigned char* dataudp, *datatcp;
  int i, pCount = 0, lentcp, lenudp;
  SOCKADDR_IN sa, saSource, saDest;
  struct hostent FAR* pHostent;
  char FAR name[MAX_HOSTNAME_LAN];
  char szSourceIP[MAX_ADDR_LEN], szDestIP[MAX_ADDR_LEN], RecvBuf[65535] = {0};
  struct udphdr* pUdpheader;
  struct ipheader* pIpheader;
  struct tcpheader* pTcpheader;
  WSAStartup(MAKEWORD(2, 1), &wsd);
  if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP)) == SOCKET_ERROR) {
    exit(1);
  }
  gethostname(name, MAX_HOSTNAME_LAN);
  pHostent = gethostbyname(name);
  sa.sin_family = AF_INET;
  sa.sin_port = htons(6000);
  memcpy(&sa.sin_addr.S_un.S_addr, pHostent->h_addr_list[0], pHostent->h_length);
  bind(sock, (SOCKADDR*)&sa, sizeof(sa)); /*bind()�趨�Լ�������IP��ַ�Ͷ˿ں�*/
  if ((WSAGetLastError()) == 10013) {
    exit(1);
  }
  WSAIoctl(sock, SIO_RCVALL, &optval, sizeof(optval), NULL, 0, &dwBytesRet, NULL, NULL);
  pIpheader = (struct ipheader*)RecvBuf;
  pTcpheader = (struct tcpheader*)(RecvBuf + sizeof(struct ipheader));
  pUdpheader = (struct udphdr*)(RecvBuf + sizeof(struct ipheader));
  while (1) {
    memset(RecvBuf, 0, sizeof(RecvBuf));
    recv(sock, RecvBuf, sizeof(RecvBuf), 0);
    saSource.sin_addr.s_addr = pIpheader->ip_src;
    strncpy(szSourceIP, inet_ntoa(saSource.sin_addr), MAX_ADDR_LEN);
    saDest.sin_addr.s_addr = pIpheader->ip_dst;
    strncpy(szDestIP, inet_ntoa(saDest.sin_addr), MAX_ADDR_LEN);
    lentcp = (ntohs(pIpheader->ip_len) - (sizeof(struct ipheader) + sizeof(struct tcpheader)));
    lenudp = (ntohs(pIpheader->ip_len) - (sizeof(struct ipheader) + sizeof(struct udphdr)));
    if ((pIpheader->ip_p) == IPPROTO_TCP && lentcp != 0) {
      printf("*******************************************\n");
      pCount++;
      datatcp = (unsigned char*) RecvBuf + sizeof(struct ipheader) + sizeof(struct tcpheader);
      printf("-TCP-\n");
      printf("\nĿ��IP��ַ��%s\n", szDestIP);
      printf("\nĿ�Ķ˿ڣ�%i\n", ntohs(pTcpheader->dport));
      printf("datatcp address->%x\n", datatcp);
      printf("size of ipheader->%i\n", sizeof(struct ipheader));
      printf("size of tcpheader->%i\n", sizeof(struct tcpheader));
      printf("size of the hole packet->%i\n", ntohs(pIpheader->ip_len));
      printf("\nchar Packet%i [%i]=\"", pCount, lentcp - 1);
      for (i = 0; i < lentcp; i++) {
        printf("\\x%.2x", *(datatcp + i));
        if (i % 10 == 0) {
          printf("\"\n\"");
        }
      }
      printf("\";\n\n\n");
      for (i = 0; i < lentcp; i++) {
        if (*(datatcp + i) <= 127 && *(datatcp + i) >= 20) {
          printf("%c", *(datatcp + i));
        }
        else {
          printf(".");
        }
      }
      printf("\n\n*******************************************\n");
    }
    if ((pIpheader->ip_p) == IPPROTO_UDP && lentcp != 0) {
      pCount++;
      dataudp = (unsigned char*) RecvBuf + sizeof(struct ipheader) + sizeof(struct udphdr);
      printf("-UDP-\n");
      printf("\nĿ��IP��ַ��%s\n", szDestIP);
      printf("\nĿ�Ķ˿ڣ�%d\n", ntohs(pTcpheader->dport));
      printf("UDP���ݵ�ַ��%x\n", dataudp);
      printf("IPͷ�����ȣ�%i\n", sizeof(struct ipheader));
      printf("UDPͷ�����ȣ�%i\n", sizeof(struct udphdr));
      printf("���Ĵ�С��%i\n", ntohs(pIpheader->ip_len));
      printf("\nchar Packet%i [%i]=\"", pCount, lenudp - 1);
      for (i = 0; i < lenudp; i++) {
        printf("\\x%.2x", *(dataudp + i));
        if (i % 10 == 0) {
          printf("\"\n\"");
        }
      }
      printf("\";\n\n\n");
      for (i = 0; i < lenudp; i++) {
        if (*(dataudp + i) <= 127 && *(dataudp + i) >= 20) {
          printf("%c", *(dataudp + i));
        }
        else {
          printf(".");
        }
      }
      printf("\n\n*******************************************\n");
    }
  }
}

