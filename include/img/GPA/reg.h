
#pragma once

#ifdef __cplusplus
extern "C"{
#endif
	
	typedef enum
	{
		eNORMALREG   = 0, //����ע��ʹ����
		eOUTDATAREG  = 1, //ע�����
		eNOREG       = 2  //δע��
	}REGSTATUS;

#ifndef BOOL 
#define BOOL int
#endif

	/************************************************************************
	* ��������: CPM_GetRegisterStatus
	* ��������: ��ȡ���ע��״̬
	* �����б�: file ��Կ�����ļ�
	* ����ֵ: ����ע��״̬(true ע��ɹ� false ע��ʧ��)
	* ˵  ��: 
	*************************************************************************/
	__declspec(dllexport) REGSTATUS  CPM_GetRegStatus();

	/************************************************************************
	* ��������: CPM_RegisterSys
	* ��������: ���ע��
	* �����б�: file ��Կ�����ļ�
	* ����ֵ: ����ע��״̬(true ע��ɹ� false ע��ʧ��)
	* ˵  ��: 
	*************************************************************************/
	__declspec(dllexport) BOOL  CPM_Register();

#ifdef __cplusplus
}
#endif


