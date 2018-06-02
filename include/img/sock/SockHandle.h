// SockHandle.h: interface for the CSockHandle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKHANDLE_H__EB40C5EF_A282_4A5E_B954_2B31F67D9BB5__INCLUDED_)
#define AFX_SOCKHANDLE_H__EB40C5EF_A282_4A5E_B954_2B31F67D9BB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common.h"
class CThreadPool;

/*****************************************************************************/
/*���������������ⲿ�߳̽�����ֱ�ӷ��ͳ�ȥ
/*�����������ڴ�������Ĺ��̣��Լ�TCP�ַ���������ñ���ÿ�ν���״̬
/*����ʹ��һ����������ÿ���׽������ݽ���״̬
/*���ݽ�����ͨ��overlapped��selectģ��֪ͨ
/*****************************************************************************/
class CDataRecvProcess
{
public:
	CDataRecvProcess( SOCKET sock, CThreadPool* pThreadPool = NULL );
	~CDataRecvProcess();
	
public:
	void  Accept();
	bool  Recv();
	void  RecvFrom();
	void  CheckTimeOut( int32 nTimeOut );
	int32 Send( const int8* pbData, int32 nLen, const SOCK_ADDRESS* pAddr);
	long  AddRef( void );
	long  Release( void );
	
private:
	CriticalSection        m_synIdentfiy;
	volatile unsigned long m_lIdentify;
	SOCKET                 m_SOCK;
	CThreadPool*           m_pThreadPool;	
	
private:
	int32 m_PackHdr[(sizeof(int32)+sizeof(SOCK_DATA_HEADER))>>2];//���հ�ͷ
	typedef std::map<uint32, int8*> DATA_MAP_EX;
	DATA_MAP_EX m_mapDataEx;
	
	long            m_RefCount;
#ifndef WIN32
	CriticalSection m_SynRef;
#endif	
#ifndef WIN32
	CUtimer    m_Timer;
#endif
	
};

int32 Send( SOCKET sock, const int8* data, int32 len, uint32 nSeq, const SOCK_ADDRESS* pAddr = NULL );
/************************2011/06/22 modify ***************************/


#endif // !defined(AFX_SOCKHANDLE_H__EB40C5EF_A282_4A5E_B954_2B31F67D9BB5__INCLUDED_)
