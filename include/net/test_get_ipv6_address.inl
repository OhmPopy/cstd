// ��ȡ��������ipv4��ַ��ipv6��ַ - Ϊ����Ա����
#include <Winsock2.h>
#include <stdio.h>
#include <assert.h>
#include <ws2tcpip.h>
#include "net/mynet.h"
#include "cstd.h"
#pragma comment(lib, "ws2_32.lib ") //linking to the library

typedef struct {
  unsigned short family;
  unsigned char data[26];
  } addr_in;

int get_ip(const addr_in* addr)
{
  struct addrinfo* ailist, *aip;
  struct addrinfo hint;
  struct sockaddr_in6* sinp6;
  PHOSTENT hostinfo;
  char hostname[255] = {0}; //������
  char* port = "3294"; //�˿ں�
  //const char* addr;
  int ilRc;

    gethostname(hostname, sizeof(hostname));
    if ((hostinfo = gethostbyname(hostname)) == NULL) { //��ñ���ipv4��ַ
      errno = GetLastError();
      fprintf(stderr, "gethostbyname Error:%d\n", errno);
      return 1;
    }
    LPCSTR ip;
    while (*(hostinfo->h_addr_list) != NULL) { //���ipv4��ַ
      ip = inet_ntoa(*(struct in_addr*) *hostinfo->h_addr_list);
      printf("ipv4 addr = %s\n\n", ip);
      hostinfo->h_addr_list++;
    }

  hint.ai_family = AF_INET6; // hint ���޶����� 
  hint.ai_socktype = SOCK_STREAM; // ����������� socket type ���� SOCK����DGRAM 
  hint.ai_flags = AI_PASSIVE; // flags �ı�־�ܶ� �����õ���AI_CANONNAME;
  hint.ai_protocol = 0; // ����Э�� һ��Ϊ0��Ĭ�� 
  hint.ai_addrlen = 0; // ���治�������ã�Ϊ0������ΪNULL 
  hint.ai_canonname = NULL;
  hint.ai_addr = NULL;
  hint.ai_next = NULL;
  ilRc = getaddrinfo(hostname, port, &hint, &ailist); //ͨ����������õ�ַ��Ϣ
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
    sinp6 = (struct sockaddr_in6*)aip->ai_addr;  // Ϊʲô��for ѭ�� �������¿� 
    sockaddr_in6* addr6 = (struct sockaddr_in6*)addr;  // Ϊʲô��for ѭ�� �������¿� 
    int i, d;
    d = hamming_distance(&addr6->sin6_addr, &sinp6->sin6_addr, sizeof(struct in6_addr)/2);
    printf("ipv6 addr = ");
    for (i = 0; i < 16; i++) {
      if (((i - 1) % 2) && (i > 0)) {
        printf(":");
      }
      printf("%02x", sinp6->sin6_addr.u.Byte[i]);
    }
    printf("  %d \n", d);
    printf(" \n");
  }
}

int cmp_cnt_bytes(const void* a, const void* b, int n) {
  const uchar* ua = (const uchar*)a;
  const uchar* ub = (const uchar*)b;
  int i, d=0;
  for (i=0; i<n; ++i) {
    d += ua[i]!=ub[i];
  }
  return d;
}

int get_ip2(const addr_in* addr, int port, addr_in* addr_out)
{
  char hostname[255] = {0}; //������
  //const char* addr;
  int ilRc;
  int i, d = 10000000, ret = 0;
  if (AF_INET==addr->family) {
    PHOSTENT hostinfo;
    const sockaddr_in* addr1 = (sockaddr_in*)addr;
    sockaddr_in* addr2 = (sockaddr_in*)addr_out;
    gethostname(hostname, sizeof(hostname));
    if ((hostinfo = gethostbyname(hostname)) == NULL) { //��ñ���ipv4��ַ
      errno = GetLastError();
      fprintf(stderr, "gethostbyname Error:%d\n", errno);
      return 1;
    }
    while (*(hostinfo->h_addr_list) != NULL) { //���ipv4��ַ
      struct in_addr addr3 = *(struct in_addr*) *hostinfo->h_addr_list;
      int dd = cmp_cnt_bytes(&addr1->sin_addr, &addr3, 4);
      if (dd<d) {
        d = dd;
        addr2->sin_family = AF_INET;
        addr2->sin_port = htons(port);
        addr2->sin_addr = addr3;
        ret = sizeof(sockaddr_in);
      }
      //ip = inet_ntoa(*(struct in_addr*) *hostinfo->h_addr_list);
      //printf("ipv4 addr = %s\n\n", ip);
      hostinfo->h_addr_list++;
    }
  } else if (AF_INET6==addr->family) {
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
      struct sockaddr_in6* addr3 = (struct sockaddr_in6*)aip->ai_addr;  // Ϊʲô��for ѭ�� �������¿� 
      int dd = cmp_cnt_bytes(&addr1->sin6_addr, &addr3->sin6_addr, sizeof(in6_addr));
      if (dd<d) {
        d = dd;
        *addr2 = *addr3;
        ret = sizeof(sockaddr_in6);
      }
    }
  }
  return ret;
}
int test_get_ipv6_address()
{
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;
  addr_in addr[1] = {0};
  addr_in addr2[1] = {0};
  char buf[256];
  char buf2[256];
  sock_init();
  net_addr("ry.ch6", 0, addr);
  net_ntop(addr, buf, 256);
  wVersionRequested = MAKEWORD(1, 1);
  err = WSAStartup(wVersionRequested, &wsaData);  //initiate the ws2_32.dll and match the version
  if (err != 0) {
    return 0;
  }
  if (LOBYTE(wsaData.wVersion) != 1 ||    //if the version is not matched ,then quit and terminate the ws3_32.dll
      HIBYTE(wsaData.wVersion) != 1) {
    WSACleanup();
    return 0;
  }
  get_ip2(addr, 123, addr2);
  net_ntop(addr2, buf2, 256);
  return 0;
}

