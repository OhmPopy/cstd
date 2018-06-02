// һ���򵥵�������̽��
#include "sniffer.h"
void ProcessIPPacket(char* packet, int size)
{
  IPHeader* header = (IPHeader*)packet;
  int len = getHeaderLength(header);
  printf("IP : %s => %s\n", inttoip(header->mSrcIP, 0), inttoip(header->mDstIP, 0));
  printf("size : %d\n", size);
  if (header->mProtocolType == IPPROTO_TCP) {
    TCPHeader* header = (TCPHeader*)(packet + len);
    printf("Got TCP packet\n");
    printf("port : %d => %d\n", header->mSrcPort, header->mDstPort);
    printf("len : %dB\n", (int)header->mLengthAndReserve);
  }
  else if (header->mProtocolType == IPPROTO_UDP) {
    UDPHeader* header = (UDPHeader*)(packet + len);
    printf("Got UDP packet\n");
    printf("port : %d => %d\n", header->mSrcPort, header->mDstPort);
    printf("len : %dB\n", header->mLength);
  }
  else if (header->mProtocolType == IPPROTO_ICMP) {
    ICMPHeader* header = (ICMPHeader*)(packet + len);;
    printf("Got ICMP packet\n");
  }
}
void SocketDis()
{
  WSADATA data;
  char vc[128 * 1024];
  char name[ 1024 ];
  SOCKET rawSock;
  struct hostent* ent;
  DWORD dummy;
  SOCKADDR_IN addr;
  WSAStartup(MAKEWORD(2, 2), &data);
  rawSock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
  gethostname(name, sizeof(name));
  ent = gethostbyname(name);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8888);
  addr.sin_port = htons(80);
  addr.sin_port = htons(8080);
  memcpy(&addr.sin_addr.S_un.S_addr, ent->h_addr_list[ 0 ], ent->h_length);
  bind(rawSock, (PSOCKADDR)&addr, sizeof(addr));
  WSAIoctl(rawSock, SIO_RCVALL, &dummy, sizeof(dummy), &dummy, sizeof(dummy), &dummy, NULL , NULL);
  //The WSAIoctl function controls the mode of a socket.
  for (;;) {
    ProcessIPPacket(vc, recv(rawSock, vc, countof(vc), 0));
  }
}
void PortScan() //�˿�ɨ�躯��
{
  int iportFrom = 80, iportTo = 180;
  int testsocket;
  int iopenedport = 0;
  struct sockaddr_in target_addr;
  WSADATA wsaData;
  WORD wVersionRequested = MAKEWORD(1, 1);
  char hostip[64] = ""; // input IP
  int i;
  if (iportFrom > iportTo) {
    printf("���󣡿�ʼ�˿ںű���С�ڽ����˿ں�\n");
    exit(1);
  }
  else {
    if (WSAStartup(wVersionRequested , &wsaData)) {
      printf("����socket��ʧ�ܣ�����汾���Ƿ�Ϊ1.1\n");
      exit(1);
    }
    for (i = iportFrom; i <= iportTo; i++) {
      printf("���ڽ���socket................................\n");
      if ((testsocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket����ʧ�ܣ�\n");
        exit(0);
      }
      target_addr.sin_family = AF_INET;
      target_addr.sin_port = htons(i);
      target_addr.sin_addr.s_addr = inet_addr(hostip);
      printf("����ɨ��˿ڣ�%d\n", i);
      if (connect(testsocket, (struct sockaddr*) &target_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) { //���Ӷ˿�
        printf("�˿�%d�ر�\n", i);
      }
      else {
        iopenedport++;
        printf("�˿�%d����\n", i);
      }
    }
    printf("Ŀ������%s��%d--%d����%d���˿ڿ���\n", hostip, iportFrom, iportTo, iopenedport);
    closesocket(testsocket);
    WSACleanup();
  }
}
int test_sniffer()
{
  if (1) {
    SocketDis();
  }
  if (0) {
    // Catch socke
    PortScan();
  }
  return 0;
}

