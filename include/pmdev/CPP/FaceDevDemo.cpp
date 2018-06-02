// FaceDevDemo.cpp : Defines the entry point for the console application.
//

#include "FaceDev.h"

#include "net/socket.h"
#include "thread.h"

typedef struct cpmdev_t {
  thread_t t[1];
  socket_t s[1];
}cpmdev_t;

#include "CPMDev_net.inl"

int test_net() {
  cpmdev_t s[1] = {0};
  const char* ip = "192.168.1.133";
  thread_create(s->t, CPMDev_callback, s);
  if (!socket_connect_host(s->s, ip, SERVER_PORT)) {
    TRACE("Allocating socket failed. Error: %d\n", WSAGetLastError());
    return 0;
  }
  if (!ULDevAuth(s->s, ip, "admin", "201031", 1)) {
    TRACE("Allocating socket failed. Error: %d\n", WSAGetLastError());
    return 0;
  }
  thread_join(s->t, -1);
  return 0;
}


int test_CPMDev()
{
  //return test_VideoSDK();
  CFaceDevMrg n_DevMgr = {0};
  const char* ip = "192.168.1.133";
  //return test_net();
  printf("�豸�ײ�����ʼ��....\n");
  BOOL bRet = FALSE;
  bRet = n_DevMgr.Init();
  printf("��������ģʽΪ����ģʽ��ֱ���豸��ͨ����ת������ת��....\n");
  bRet = n_DevMgr.SetWorkMode(LocalMode);
  printf("�󶨻ص�....\n");
  n_DevMgr.BindingCallBack();//�ص��İ�һ��Ҫ�ڳ�ʼ����ģʽ����֮��
  printf("���豸��������....\n");
  bRet = n_DevMgr.Connect(ip);

  if (1) {
    Sleep(8000);

#if 0
    printf("�豸%s:���������û�...\n", ip);
    bRet = n_DevMgr.DLoadAllUserInfo(ip);
    Sleep(1500 * 2);

    printf("�豸%s:��������ʶ���¼...\n", ip);
    bRet = n_DevMgr.DLoadAllIdentifyRecord(ip);
    Sleep(1500 * 2);


    printf("�豸%s:��ȡ�豸�������...\n", ip);
    bRet = n_DevMgr.GetNetConfigInfo(ip);
#endif
    Sleep(1500 * 2);
  }

  return 0;
}
