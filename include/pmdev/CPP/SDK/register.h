// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CHSYSTEMREGISTER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CHSYSTEMREGISTER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifndef _CH_SYSTEM_REGISTER_H
#define _CH_SYSTEM_REGISTER_H

#ifdef CHSYSTEMREGISTER_EXPORTS
#define CPMREG_API __declspec(dllexport)
#else
#define CPMREG_API __declspec(dllimport)
#endif

#define IN 
#define OUT 
#define INOUT 

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef STDCALL
#define STDCALL __stdcall
#endif

#ifndef WCHAR
typedef wchar_t WCHAR;
#endif

#ifdef __cplusplus
extern "C"{
#endif

	#define ARRAY_LEN 64
	#define CODE_LEN  35

	typedef struct  
	{
		char m_keyName[ARRAY_LEN+1];
		char m_keyValue[ARRAY_LEN+1];
	}REG_KEY;

	typedef struct  
	{
		int		  m_nCount; //�ṹ���С
		REG_KEY*  m_pRegKey;
	}REG_CONTENT;

	/************************************************************************
	* ��������: CPM_GetLocalMachineCode
	* ��������: ��ȡ���ػ�����
	* �����б�: mCode �������Ż���
	* ����ֵ: ��ȡ״̬
	* ˵  ��:��ȡ�ɹ�������true��ʧ���򷵻�false
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_GetLocalMachineCode( OUT char (&mCode)[CODE_LEN] );

	/************************************************************************
	* ��������: CPM_InitReg
	* ��������: ��ʼ��ע��
	* �����б�: cFile ע���ļ�
	* ����ֵ: ��ʼ��״̬
	* ˵   ��:�˽ӿڼȿ����ڻ�ȡע��״̬�ֿ���������ע���ļ�
			 �����ڻ�ȡע��״̬ʱ���ⲿ�ɵ���CPM_GetRegStatus��CPM_GetRegContent��ȡ
			 ����״̬��Ϣ
			 ����������ע���ļ�ʱ���ⲿ�ɵ���CPM_BeginReg��CPM_SetRegContent��CPM_DelRegContent
			 ��CPM_EndReg���ע�ᣬע����Ϣ��д�뵽 cFile ��
			 ������ʹ�ñ�ģ��ʱ����CPM_UnInitReg���з���ʼ��
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_InitReg( IN const WCHAR (&cFile)[MAX_PATH] );

	/************************************************************************
	* ��������: CPM_GetRegStatus
	* ��������: ��ȡע��״̬
	* �����б�: void
	* ����ֵ: �����Ƿ�ע��
	* ˵  ��: �˽ӿ����ڻ�ȡ����ע��״̬
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_GetRegStatus();

	/************************************************************************
	* ��������: CPM_GetRegStatus
	* ��������: ��ȡע������
	* �����б�: regID ע���ʶ			  	           
	* ����ֵ:  ע������
	* ˵  ��: 
	*************************************************************************/
	CPMREG_API REG_CONTENT* STDCALL CPM_GetRegContent( IN int regID );

	/************************************************************************
	* ��������: CPM_BeginReg
	* ��������: ����ע��
	* �����б�: mCode ������			  	           
	* ����ֵ:  
	* ˵  ��:  ���ô˽ӿڻὫmcode����д��ע���ļ���
	          �˺�ɵ���CPM_SetRegContent��CPM_DelRegContent��ע�����ݽ������Ӻ�ɾ��
			  ����CPM_EndReg�����ע��
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_BeginReg( IN const char (&mCode)[CODE_LEN] );

	/************************************************************************
	* ��������: CPM_SetRegContent
	* ��������: ����ע������
	* �����б�: regID      ע���ʶ
			  regContent ע������
	* ����ֵ:  
	* ˵  ��:  
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_SetRegContent( IN int regID, IN const REG_CONTENT& regContent );

	/************************************************************************
	* ��������: CPM_DelRegContent
	* ��������: ɾ��ע������
	* �����б�: regID      ע���ʶ
	* ����ֵ:  
	* ˵  ��:  
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_DelRegContent( IN int regID );

	/************************************************************************
	* ��������: CPM_EndReg
	* ��������: ���ע��
	* �����б�: 
	* ����ֵ:  
	* ˵  ��:  ���ô˽ӿڽ����ע������ȫ��д���ļ�
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_EndReg( void );
	
	/************************************************************************
	* ��������: CPM_UnInitReg
	* ��������: ����ʼ��ע��ģ��
	* �����б�: 
	* ����ֵ:  
	* ˵  ��:  �ϲ�ģ���������CPM_InitReg�ӿ�����ô˽ӿڽ���һ������ʼ��
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_UnInitReg( void );

#ifdef __cplusplus
}
#endif


#endif
