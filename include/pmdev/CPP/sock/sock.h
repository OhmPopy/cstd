
#ifndef __MY_SOCK_H__
#define __MY_SOCK_H__

#ifdef WIN32
#ifdef SOCK_EXPORTS
#define SOCK_API 
#else
#define SOCK_API 
#endif
#define SOCK_CALL
#else
#define SOCK_CALL
#define SOCK_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define MEMRESERVER_SIZE (256*1024)
#define MEMRESERVER_UNIT (8*1024)
#define NET_SOCK_STREAM 1
#define NET_SOCK_DGRAM  2

  typedef void* HANDLE;

  typedef enum {
    NET_NONE       = 0x00,
    NET_OVERLAPPED,
    NET_SELECTED
  } NET_MODEL;

  typedef struct {
    unsigned short Port;                //�˿�
    char           IP_Address[16];      //���ʮ����IP��ַ
  } SOCK_ADDRESS, *PSOCK_ADDRESS;

  typedef void (*ListenCallBack)(HANDLE hSOCK, const SOCK_ADDRESS& Addr, void* pvContext);
  typedef void (*DataRecvCallBack)(HANDLE hSOCK, const char* pbData, int nLen, void* pvContext);
  typedef void (*DataRecvFromCallBack)(HANDLE hSOCK, const char* pbData, int nLen, const SOCK_ADDRESS& addr, void* pvContext);

  /****************************************************/
  /*�������� CPM_InitSock
  /*��    �� ��ʼ��sock��
  /*��    ��
  /*��    ��
  /*˵    ��  �˽ӿ����ڳ�ʼ���ͻ��˲��趨Ԥ���ڴ��С���˴�С����������ڴ��
        ��Ĭ�ϴ�СΪ192KB�������ݽ��ܹ����н���ʹ�ô��ڴ��
  /****************************************************/
  SOCK_API BOOL CPM_InitSock(NET_MODEL NetModel, unsigned int uMemPoolSize = MEMRESERVER_SIZE, unsigned int uMemPoolUnit = MEMRESERVER_UNIT);

  /****************************************************/
  /*�������� CPM_UnInitSock
  /*��    �� ����ʼ��sock��
  /*��    ��
  /*��    ��
  /*˵    ��
  /****************************************************/
  SOCK_API void CPM_UnInitSock();

  /****************************************************/
  /*�������� CPM_SetListenCallBack
  /*��    �� ���������������ص�����
  /*��    �� lpfnListen �ص�����ָ��
         pvContext �ص�����������
  /*��    ��
  /*˵    �� ���������пͻ�������ʱ�ص����ⲿ
  /****************************************************/
  SOCK_API void CPM_SetListenCallBack(ListenCallBack lpfnListen, void* pvContext);

  /****************************************************/
  /*�������� CPM_SetDataRecvCallBack
  /*��    �� ���ݽ��ܻص�
  /*��    �� lpfnRecv �ص�����ָ��
         pvContext �ص�����������
  /*��    ��
  /*˵    �� �����ܵ��ͻ��˷��͹���������ʱ�ص����ⲿ(TCP)
  /****************************************************/
  SOCK_API void CPM_SetDataRecvCallBack(DataRecvCallBack lpfnRecv, void* pvContext) ;

  /****************************************************/
  /*�������� CPM_SetDataRecvFromCallBack
  /*��    �� ���ݽ��ܻص�
  /*��    �� lpfnRecv �ص�����ָ��
         pvContext �ص�����������
  /*��    ��
  /*˵    �� �����ܵ�����Է����͹���������ʱ�ص����ⲿ(UDP)
  /****************************************************/
  SOCK_API void CPM_SetDataRecvFromCallBack(DataRecvFromCallBack lpfnRecv, void* pvContext) ;

  /****************************************************/
  /*�������� CPM_StartWork
  /*��    �� �����׽������ݴ�������
  /*��    �� bStartUp �������
  /*��    ��
  /*˵    �� �����ܵ��ͻ��˷��͹���������ʱ�ص����ⲿ
  /****************************************************/
  SOCK_API BOOL CPM_StartWork(BOOL bStartUp);

  /****************************************************/
  /*�������� CPM_CreateSock
  /*��    �� �����׽���
  /*��    �� nSOCKType �׽������ͣ��ֽ��������ݱ���
  /*��    �� �׽��־��
  /*˵    ��
  /****************************************************/
  SOCK_API HANDLE CPM_CreateSock(int nSOCKType);  //�����׽���

  /****************************************************/
  /*�������� CPM_ReleaseSock
  /*��    �� �ͷ��׽���
  /*��    �� hSOCK �׽��־��
  /*��    ��
  /*˵    ��
  /****************************************************/
  SOCK_API void CPM_ReleaseSock(HANDLE hSOCK);   //�ͷ��׽���

  /****************************************************/
  /*�������� CPM_Bind
  /*��    �� ��֡���˿�
  /*��    �� hSOCK �׽��־��
         Addr  ������ַ
  /*��    ��
  /*˵    ��
  /****************************************************/
  SOCK_API BOOL CPM_Bind(HANDLE hSOCK, const SOCK_ADDRESS& Addr);

  /****************************************************/
  /*�������� CPM_Connect
  /*��    �� ����ĳ����ַ
  /*��    �� hSOCK �׽��־��
         Addr  �����ַ
  /*��    ��
  /*˵    ��
  /****************************************************/
  SOCK_API BOOL CPM_Connect(HANDLE hSOCK, const SOCK_ADDRESS& Addr);

  /****************************************************/
  /*�������� CPM_AddInIPGroup
  /*��    �� ����ಥ��
  /*��    �� hSOCK �׽��־��
         GroupIPAddr  �鲥��ַ
  /*��    ��
  /*˵    ��
  /****************************************************/
  SOCK_API BOOL CPM_AddInIPGroup(HANDLE hSOCK, const SOCK_ADDRESS& GroupIPAddr);

  /****************************************************/
  /*�������� CPM_DropOutIPGroup
  /*��    �� �˳��ಥ��
  /*��    �� hSOCK �׽��־��
         GroupIPAddr  �鲥��ַ
  /*��    ��
  /*˵    ��
  /****************************************************/
  SOCK_API void CPM_DropOutIPGroup(HANDLE hSOCK, const SOCK_ADDRESS& GroupIPAddr);

  /****************************************************/
  /*�������� CPM_Listen
  /*��    �� ����֡��
  /*��    �� hSOCK �׽��־��
  /*��    ��
  /*˵    ��
  /****************************************************/
  SOCK_API void CPM_Listen(HANDLE hSOCK);


  /****************************************************/
  /*�������� CPM_SetSendBufSize
  /*��    �� ���÷��ͻ����С
  /*��    �� hSOCK �׽��־��
         nSize �����С
  /*��    ��
  /*˵    ��
  /****************************************************/
  SOCK_API void CPM_SetSendBufSize(HANDLE hSOCK, int nSize);

  /****************************************************/
  /*�������� CPM_SetRecvBufSize
  /*��    �� ���ý��ܻ����С
  /*��    �� hSOCK �׽��־��
         nSize �����С
  /*��    ��
  /*˵    ��
  /****************************************************/
  SOCK_API void CPM_SetRecvBufSize(HANDLE hSOCK, int nSize);

  /****************************************************/
  /*�������� CPM_Send
  /*��    �� ��������
  /*��    �� hSOCK �׽��־��
         pbData �����ڴ�
         lpLen �����ڴ��С
  /*��    ��
  /*˵    �� TCPЭ�鷢������
  /****************************************************/
  SOCK_API BOOL CPM_Send(HANDLE hSOCK, const char* pbData, int* lpLen);

  /****************************************************/
  /*�������� CPM_SendTo
  /*��    �� ��������
  /*��    �� hSOCK �׽��־��
         addr  Ŀ�ĵ�ַ
         pbData �����ڴ�
         lpLen �����ڴ��С
  /*��    ��
  /*˵    �� UDP��������
  /****************************************************/
  SOCK_API BOOL CPM_SendTo(HANDLE hSOCK, const SOCK_ADDRESS& addr, const char* pbData, int* lpLen);

  /****************************************************/
  /*�������� CPM_Recv
  /*��    �� ���ݽ���
  /*��    �� hSOCK �׽��־��
  /*��    ��
  /*˵    ��  ���ô˽ӿں󣬵�hSOCK��������ʱ���Իص���ʽ֪ͨ���ⲿ
  /****************************************************/
  SOCK_API void CPM_Recv(HANDLE hSOCK);  //���ݽ��ܣ����ô˽ӿں󣬵�psock��������ʱ���Իص���ʽ֪ͨ���ⲿ

  /****************************************************/
  /*�������� CPM_GetPeerAddr
  /*��    �� ��ȡ�Եȷ���ַ
  /*��    �� hSOCK �׽��־��
  /*��    �� pAddr �Եȷ���ַ
  /*˵    ��
  /****************************************************/
  SOCK_API BOOL CPM_GetPeerAddr(HANDLE hSOCK, PSOCK_ADDRESS pAddr);

  /****************************************************/
  /*�������� CPM_GetLocalIP
  /*��    �� ��ȡ����IP
  /*��    �� hSOCK �׽��־��
  /*��    ��
  /*˵    ��
  /****************************************************/
  SOCK_API BOOL CPM_GetLocalIP(HANDLE hSOCK, PSOCK_ADDRESS pAddr);

  /****************************************************/
  /*�������� CPM_GetSockName
  /*��    �� ��ȡ����IP
  /*��    �� hSOCK �׽��־��
  /*��    ��
  /*˵    ��
  /****************************************************/
  SOCK_API BOOL CPM_GetSockName(HANDLE hSOCK, PSOCK_ADDRESS pAddr);


  /****************************************************/
  /*�������� CPM_GetLastError
  /*��    �� ��ȡ�������
  /*��    ��
  /*��    ��
  /*˵    ��
  /****************************************************/
  SOCK_API int  CPM_GetLastError(); //��ȡ�������

#ifdef __cplusplus
}
#endif

#endif