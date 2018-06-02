
////////////////////////////////////////////////////////////////////*/
//��������: SLGServer.dll
//��    ��: �����sockͨ�ţ�ʹ��TCP�������������ͨ��,����dsp�������ݴ���
//˵    ��: �˿��Ѿ����������ݵĲ����������̣��ϲ�Ӧ���������ͽ��
//            �����ֻ��һ���ͻ�������
//ʱ    ��: 2010/11/25
////////////////////////////////////////////////////////////////////*/

#ifndef __SLGSERVER_SOCK_H__
#define __SLGSERVER_SOCK_H__

#include "../slgdev.h"

#ifdef WIN32
#ifdef SLGSERVER_EXPORTS
#define SLGSERVER_API
#else
#define SLGSERVER_API
#endif
#else
#define SLGSERVER_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define MEMRESERVER_SIZE   (256*1024) //�ڴ��С

#define MULTYGROUP_PORT  13332           //�ಥ��˿�
#define MULTYGROUP_IP    "224.0.1.100"   //�ಥ���ַ
#define UDP_PORT     13334
#define OFFSET_SIZE    20

    typedef enum {
        SERVER_OVERLAPPED = 0x01,
        SERVER_SELECTED,
    }
    SERVER_MODEL;

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
    //�������� SLG_S_Init
    //��    �� ��ʼ�������
    //��    ��
    //��    ��
    //˵    ��  �˽ӿ����ڳ�ʼ���ͻ��˲��趨Ԥ���ڴ��С���˴�С����������ڴ��
    //      ��Ĭ�ϴ�СΪ192KB�������ݽ��ܹ����н���ʹ�ô��ڴ��
    //      Ӧ�ò����ʵ������һ��Ԥ��
    //        ��ֵ������ڻ���ڴ��������ݵ����ֵ���������ݽ��ܽ�������
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_Init(int af, unsigned int uSize = MEMRESERVER_SIZE, SERVER_MODEL eModel = SERVER_SELECTED);

    ////////////////////////////////////////////////////*/
    //�������� SLG_S_UnInit
    //��    �� ����ʼ���ͻ���
    //��    ��
    //��    ��
    //˵    ��
    ////////////////////////////////////////////////////*/
    SLGSERVER_API void SLG_S_UnInit();

    ////////////////////////////////////////////////////*/
    //�������� SLG_S_SetRecvCallBack
    //��    �� �������ݽ��ջص�����
    //��    �� cb     �ص�����
    //           param  �ص�������
    //��    ��
    //˵    ��
    ////////////////////////////////////////////////////*/
    SLGSERVER_API void SLG_S_SetRecvCallBack(SRecvCallBack lpfnRecv, void* pvContext);

    ////////////////////////////////////////////////////*/
    //�������� SLG_S_SetRecvFromCallBack
    //��    �� �������ݽ��ջص�����
    //��    �� cb     �ص�����
    //           param  �ص�������
    //��    ��
    //˵    ��
    ////////////////////////////////////////////////////*/
    SLGSERVER_API void SLG_S_SetRecvFromCallBack(SRecvFromCallBack lpfnRecv, void* pvContext);


    ////////////////////////////////////////////////////*/
    //�������� SLG_S_SetCNCallBack
    //��    �� ���÷���������״̬���ѻص�
    //��    �� cb     �ص�����
    //           param  �ص�������
    //��    ��
    //˵    ��
    ////////////////////////////////////////////////////*/
    SLGSERVER_API void SLG_S_SetCNCallBack(SCNCallBack lpfnCN, void* pvContext);


    ////////////////////////////////////////////////////*/
    //�������� SLG_S_AddInMultyGroup
    //��    �� ����ಥ��
    //��    �� flag �������( true ����addr����ʶ���� false �˳�addr����ʶ����)
    //��    �� true  �ɹ�
    //           false ʧ��
    //˵    ��
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_AddInMultyGroup(BOOL bJoinIn, const addr_in* Addr, CSockBase* user);

    ////////////////////////////////////////////////////*/
    //�������� SLG_S_StartListen
    //��    �� �����������������
    //��    �� flag �������( true ���� false ֹͣ)
    //��    �� true  �ɹ�
    //           false ʧ��
    //˵    ��
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_StartListen(BOOL bStartUp);

    ////////////////////////////////////////////////////*/
    //�������� SLG_S_StartWork
    //��    �� ���������������
    //��    �� flag
    //              true  ����
    //      false ֹͣ
    //��    �� true  �ɹ�
    //     false ʧ��
    //˵    �� �����������̺��ڲ��������շ����֪ͨ���ⲿ
    //     ֹͣ�������̺��ڲ����پ��������շ�
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_StartWork(BOOL bStartUp);

    ////////////////////////////////////////////////////*/
    //�������� SLG_S_CreateUDPSock
    //��    �� �����׽���
    //��    �� pAddr �׽��ְ󶨵�ַ
    //��    �� �׽��־��
    //˵    ��
    ////////////////////////////////////////////////////*/
    SLGSERVER_API SOCKET SLG_S_CreateUDPSock(const addr_in* pAddr); //�����׽���

    ////////////////////////////////////////////////////*/
    //�������� SLG_S_SendData
    //��    �� ��������
    //��    �� hSock       �׽��־��
    //           buf         ��������
    //       lpLen       �������ݴ�С
    //��    ��
    //˵    ��
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_SendData(SOCKET hSOCK, const char* pbData, unsigned int* lpLen);

    ////////////////////////////////////////////////////*/
    //�������� SLG_S_SendData
    //��    �� ��������
    //��    �� hSock       �׽��־��
    //           buf         ��������
    //       lpLen       �������ݴ�С
    //��    ��
    //˵    ��
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_SendDataTo(SOCKET hSOCK, const char* pbData, unsigned int* lpLen, const addr_in* Addr);


    ////////////////////////////////////////////////////*/
    //�������� SLG_S_DCN
    //��    ��  �Ͽ�����
    //��    ��
    //��    ��
    //˵    ��
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_DCN(SOCKET hSOCK);

    ////////////////////////////////////////////////////*/
    //�������� SLG_S_Address
    //��    �� ��ȡ�Եȷ�IP��ַ
    //��    �� hSock       �׽��־��
    //           ip          �Եȷ�IP��ַ(����192.168.1.1)
    //��    ��
    //˵    ��
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_Address(SOCKET hSOCK, addr_in* addr);

#ifdef __cplusplus
}
#endif

#endif