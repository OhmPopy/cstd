
#include "net/net_w32.h"

// ��������
int test_server()
{
  //����WinSock��
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;
  SOCKET lSocket, aSocket;
  SOCKADDR_IN AddrSer;
  SOCKADDR_IN AddrCli;//�ͻ��˵ĵ�ַ��Ϣ
  int len;
  char recBuf[1024];
  char senBuf[1024];

  wVersionRequested = MAKEWORD(2, 2);
  err = WSAStartup(wVersionRequested, &wsaData);

  if (err != 0) {
    return 0;
  }

  if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
    WSACleanup();
    return 0;
  }

  AddrSer.sin_family = AF_INET;
  AddrSer.sin_port = htons(5000);
  AddrSer.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //INADDR_ANY��ʾ�����κλ���������
  lSocket = socket(AF_INET, SOCK_STREAM, 0);

  //�󶨷�������ַ
  if (bind(lSocket, (SOCKADDR*)&AddrSer, sizeof(SOCKADDR)) == SOCKET_ERROR) {
    printf("���׽���ʧ��");
    return 0;
  }

  listen(lSocket, 1); //��ʼ�����ͻ��˵���������
  len = sizeof(SOCKADDR);
  aSocket = accept(lSocket, (SOCKADDR*)&AddrCli, &len); //���ӳɹ�ʱ�������µ�socket������ͻ���ͨ��

  while (1) {
    //TCP��Ϊrecv��send��UDP��Ϊrecefrom��sendto
    recv(aSocket, recBuf, 1024, 0);

    if (recBuf[0] == '#') {
      printf("�ͻ�������Ͽ�\n");
      printf("���ڶϿ�...");
      Sleep(3000);
      break;
    }

    printf("�ͻ��ˣ�%s\n", recBuf);
    printf("������:");
    gets(senBuf);//warming:gets()is unsafe

    if (senBuf[0] == '#') {
      send(aSocket, senBuf, 1024, 0);
      printf("��������Ͽ�...\n");
      Sleep(3000);
      return 0;
    }

    //printf("��������%s\n",senBuf);
    send(aSocket, senBuf, 1024, 0);
    memset(recBuf, ' ', 1024);
    memset(senBuf, ' ', 1024);
  }

  closesocket(aSocket);
  closesocket(lSocket);
  WSACleanup();
  return 0;
}

int test_client()
{
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;
  SOCKET cSocket;
  SOCKADDR_IN addrSer;
  char senBuf[1024];
  char recBuf[1024];

  wVersionRequested = MAKEWORD(2, 2);
  err = WSAStartup(wVersionRequested, &wsaData);

  if (err != 0) {
    return 0;
  }

  if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
    WSACleanup();
    return 0;
  }

  cSocket = socket(AF_INET, SOCK_STREAM, 0);
  addrSer.sin_family = AF_INET;
  addrSer.sin_port = htons(5000);
  addrSer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

  if (connect(cSocket, (SOCKADDR*)&addrSer, sizeof(SOCKADDR)) == SOCKET_ERROR) {
    printf("���ӷ�����ʧ��");
    return 0;
  }


  while (1) {
    printf("�ͻ��ˣ�");
    gets(senBuf);

    if (senBuf[0] == '#') {
      send(cSocket, senBuf, 1024, 0);
      printf("��������Ͽ�...\n");
      Sleep(3000);
      return 0;
    }

    //printf("�ͻ��ˣ�%s\n",senBuf);
    send(cSocket, senBuf, 1024, 0);
    recv(cSocket, recBuf, 1024, 0);

    if (recBuf[0] == '#') {
      printf("��������Ͽ�");
      printf("���ڶϿ�...");
      Sleep(3000);
      break;
    }

    printf("��������%s\n", recBuf);
    memset(recBuf, ' ', 1024);
    memset(senBuf, ' ', 1024);
  }

  closesocket(cSocket);
  WSACleanup();
  return 0;
}


#include "test_save_net_pic.h"
#define SERVER_IP "192.168.1.187"

int saferecv( SOCKET s, void* buf0, int n, int flags ) {
  int i = 0, re;
  char* buf = (char*)buf0;
  for ( i = 0; i < n; ) {
    if ( (re = recv( s, buf + i, n - i, flags )) <=0 ) {
      return re;
    }
    i += re;
  }
  
  return i;
}

int thd_save(int id) {
  SOCKADDR_IN addrSer;
  int h = 576, w = 720, cn = 1;
  int step = w * cn, size = h * step;
  int csock = socket(AF_INET, SOCK_STREAM, 0);
  uchar* recBuf = MALLOC(uchar, size);
  char buf[256];
  int frame=0;
  addrSer.sin_family = AF_INET;
  addrSer.sin_port = htons(port[id]);
  addrSer.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
  
  printf("%d���ӷ����� %s ...\n", id, SERVER_IP);
  
  if (connect(csock, (SOCKADDR*)&addrSer, sizeof(SOCKADDR)) == SOCKET_ERROR) {
    printf("%d���ӷ�����ʧ��", id);
    return 0;
  }

  while (1) {
    int len=0;
    //printf("�ͻ��ˣ�%s\n",senBuf);
    len = saferecv(csock, recBuf, size, 0);
    if (len==size) {
      _snprintf(buf, 256, "./out/pic%05d_ch%d.jpg", frame, recBuf[0]);
      imwrite4(buf, h, w, recBuf, step, cn);
      printf("%d, recv %d bytes, %s\n", id, len, buf);
    } else {
      printf("%d, error %d\n", id, len);
      break;
    }
  }
  
  closesocket(csock);
  free(recBuf);
  return 0;
}

int test_save_net_pic()
{
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;
  ttt_t tt[2]={0};
  int frame = 0;
  int i;

  wVersionRequested = MAKEWORD(2, 2);
  err = WSAStartup(wVersionRequested, &wsaData);

  if (err != 0) {
    return 0;
  }

  if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
    WSACleanup();
    return 0;
  }
  
  for (i=0; i<2; ++i) {
    DWORD targetThreadID;
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thd_save, (LPVOID)i,0,&targetThreadID); 
  }
  
  while(getchar()!='q')
  {
    Sleep(1000);
  } 
  _mkdir("out");

  WSACleanup();
  return 0;
}


