////////////////////////////////////////////////////////////////////*/
//��������: CPMServer.dll
//��    ��: �����sockͨ�ţ�ʹ��TCP�������������ͨ��,����dsp�������ݴ���
//˵    ��: �˿��Ѿ����������ݵĲ����������̣��ϲ�Ӧ���������ͽ��
//            �����ֻ��һ���ͻ�������
//ʱ    ��: 2010/11/25
////////////////////////////////////////////////////////////////////*/

#ifndef __CPMSERVER_SOCK_H__
#define __CPMSERVER_SOCK_H__


#ifdef WIN32
#ifdef CPMSERVER_EXPORTS
#define CPMSERVER_API
#else
#define CPMSERVER_API
#endif
#else
#define CPMSERVER_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define MEMRESERVER_SIZE   (256*1024) //�ڴ��С

#define MULTYGROUP_PORT  13332           //�ಥ��˿�
#define MULTYGROUP_IP    "224.0.1.100"  //�ಥ���ַ
#define UDP_PORT     13334
#define OFFSET_SIZE    20

  typedef enum {
    SERVER_OVERLAPPED = 0x01,
    SERVER_SELECTED,
  }
  SERVER_MODEL;

  //typedef void* SOCKET;
  /////////////////////////////////////////////////////////////////////////////////////
  //SRecvCallBack ���ݽ��ջص�ָ��
  //��    �� �ڲ����������׽����Ϸ��͹���������
  //��    ��
  //           buf   ���ݻ���
  //       len   ���ݴ�С
  //       para  Ӧ��������
  //˵    �� ���׽����������ݷ��͹���ʱ���ڲ����յ������ݷ��͸��ϲ�Ӧ��
  //           ��len=0ʱ�������Եȷ��Ѿ��ر���������
  /////////////////////////////////////////////////////////////////////////////////////
  typedef void (*SRecvCallBack)(SOCKET hSOCK, const char* pbData, unsigned int uLen, void* pvContext);

  /////////////////////////////////////////////////////////////////////////////////////
  //SRecvCallBack ���ݽ��ջص�ָ��
  //��    �� �ڲ����������׽����Ϸ��͹���������
  //��    ��
  //           buf   ���ݻ���
  //       len   ���ݴ�С
  //       para  Ӧ��������
  //˵    �� ���׽����������ݷ��͹���ʱ���ڲ����յ������ݷ��͸��ϲ�Ӧ��
  //           ��len=0ʱ�������Եȷ��Ѿ��ر���������
  /////////////////////////////////////////////////////////////////////////////////////
  typedef void (*SRecvFromCallBack)(SOCKET hSOCK, const char* pbData, unsigned int uLen, const addr_in* Addr, void* pvContext);

  /////////////////////////////////////////////////////////////////////////////////////
  //SCNCallBack �ͻ�������״̬�ص�
  //��    �� �ͻ�����������
  //��    �� hSOCK  ���ͻ��������׽��־��
  //                       flag  ����״̬(true �������ӣ�false �Ͽ�����)
  //                       para  Ӧ��������
  // ��   �� �׽��ּ������� <0 �����˴����� 0�����м��� 1��ͻ��˽���SSL��������
  //˵    ��
  /////////////////////////////////////////////////////////////////////////////////////
  typedef int (*SCNCallBack)(SOCKET hSOCK, BOOL bCN, void* pvContext);

  ////////////////////////////////////////////////////*/
  //�������� CPM_S_Init
  //��    �� ��ʼ�������
  //��    ��
  //��    ��
  //˵    ��  �˽ӿ����ڳ�ʼ���ͻ��˲��趨Ԥ���ڴ��С���˴�С����������ڴ��
  //      ��Ĭ�ϴ�СΪ192KB�������ݽ��ܹ����н���ʹ�ô��ڴ��
  //      Ӧ�ò����ʵ������һ��Ԥ��
  //        ��ֵ������ڻ���ڴ��������ݵ����ֵ���������ݽ��ܽ�������
  ////////////////////////////////////////////////////*/
  CPMSERVER_API BOOL CPM_S_Init(int af, unsigned int uSize = MEMRESERVER_SIZE, SERVER_MODEL eModel = SERVER_SELECTED);

  ////////////////////////////////////////////////////*/
  //�������� CPM_S_UnInit
  //��    �� ����ʼ���ͻ���
  //��    ��
  //��    ��
  //˵    ��
  ////////////////////////////////////////////////////*/
  CPMSERVER_API void CPM_S_UnInit();

  ////////////////////////////////////////////////////*/
  //�������� CPM_S_SetRecvCallBack
  //��    �� �������ݽ��ջص�����
  //��    �� cb     �ص�����
  //           param  �ص�������
  //��    ��
  //˵    ��
  ////////////////////////////////////////////////////*/
  CPMSERVER_API void CPM_S_SetRecvCallBack(SRecvCallBack lpfnRecv, void* pvContext);

  ////////////////////////////////////////////////////*/
  //�������� CPM_S_SetRecvFromCallBack
  //��    �� �������ݽ��ջص�����
  //��    �� cb     �ص�����
  //           param  �ص�������
  //��    ��
  //˵    ��
  ////////////////////////////////////////////////////*/
  CPMSERVER_API void CPM_S_SetRecvFromCallBack(SRecvFromCallBack lpfnRecv, void* pvContext);


  ////////////////////////////////////////////////////*/
  //�������� CPM_S_SetCNCallBack
  //��    �� ���÷���������״̬���ѻص�
  //��    �� cb     �ص�����
  //           param  �ص�������
  //��    ��
  //˵    ��
  ////////////////////////////////////////////////////*/
  CPMSERVER_API void CPM_S_SetCNCallBack(SCNCallBack lpfnCN, void* pvContext);


  ////////////////////////////////////////////////////*/
  //�������� CPM_S_AddInMultyGroup
  //��    �� ����ಥ��
  //��    �� flag �������( true ����addr����ʶ���� false �˳�addr����ʶ����)
  //��    �� true  �ɹ�
  //           false ʧ��
  //˵    ��
  ////////////////////////////////////////////////////*/
  CPMSERVER_API BOOL CPM_S_AddInMultyGroup(BOOL bJoinIn, const addr_in* Addr);

  ////////////////////////////////////////////////////*/
  //�������� CPM_S_StartListen
  //��    �� �����������������
  //��    �� flag �������( true ���� false ֹͣ)
  //��    �� true  �ɹ�
  //           false ʧ��
  //˵    ��
  ////////////////////////////////////////////////////*/
  CPMSERVER_API BOOL CPM_S_StartListen(BOOL bStartUp);

  ////////////////////////////////////////////////////*/
  //�������� CPM_S_StartWork
  //��    �� ���������������
  //��    �� flag
  //              true  ����
  //      false ֹͣ
  //��    �� true  �ɹ�
  //     false ʧ��
  //˵    �� �����������̺��ڲ��������շ����֪ͨ���ⲿ
  //     ֹͣ�������̺��ڲ����پ��������շ�
  ////////////////////////////////////////////////////*/
  CPMSERVER_API BOOL CPM_S_StartWork(BOOL bStartUp);

  ////////////////////////////////////////////////////*/
  //�������� CPM_S_CreateUDPSock
  //��    �� �����׽���
  //��    �� pAddr �׽��ְ󶨵�ַ
  //��    �� �׽��־��
  //˵    ��
  ////////////////////////////////////////////////////*/
  CPMSERVER_API SOCKET CPM_S_CreateUDPSock(const addr_in* pAddr); //�����׽���

  ////////////////////////////////////////////////////*/
  //�������� CPM_S_SendData
  //��    �� ��������
  //��    �� hSock       �׽��־��
  //           buf         ��������
  //       lpLen       �������ݴ�С
  //��    ��
  //˵    ��
  ////////////////////////////////////////////////////*/
  CPMSERVER_API BOOL CPM_S_SendData(SOCKET hSOCK, const char* pbData, unsigned int* lpLen);

  ////////////////////////////////////////////////////*/
  //�������� CPM_S_SendData
  //��    �� ��������
  //��    �� hSock       �׽��־��
  //           buf         ��������
  //       lpLen       �������ݴ�С
  //��    ��
  //˵    ��
  ////////////////////////////////////////////////////*/
  CPMSERVER_API BOOL CPM_S_SendDataTo(SOCKET hSOCK, const char* pbData, unsigned int* lpLen, const addr_in* Addr);


  ////////////////////////////////////////////////////*/
  //�������� CPM_S_DCN
  //��    ��  �Ͽ�����
  //��    ��
  //��    ��
  //˵    ��
  ////////////////////////////////////////////////////*/
  CPMSERVER_API BOOL CPM_S_DCN(SOCKET hSOCK);

  ////////////////////////////////////////////////////*/
  //�������� CPM_S_Address
  //��    �� ��ȡ�Եȷ�IP��ַ
  //��    �� hSock       �׽��־��
  //           ip          �Եȷ�IP��ַ(����192.168.1.1)
  //��    ��
  //˵    ��
  ////////////////////////////////////////////////////*/
  CPMSERVER_API BOOL CPM_S_Address(SOCKET hSOCK, addr_in* addr);

#ifdef __cplusplus
}
#endif

#endif
