// TimeGroup.cpp: implementation of the CTimeGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimeGroup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeGroup::CTimeGroup()
{
	m_lTimeGroupLst.clear();
	m_Pool.FreePool();
}

CTimeGroup::~CTimeGroup()
{

}

//����ʼ������
void CTimeGroup::UnInitData()
{

}
//��ʼ������
BOOL CTimeGroup::InitData( DBManager& DB  )
{
	int nRlt = 0;
	//���������
	m_lTimeGroupLst.clear();
	
	//�����ڴ��
	m_Pool.ResetPool();
	
	if( m_Pool.InitPool( MAX_TIMEGROUP_NUM,sizeof(TIMEGROUP) ) == FALSE )
		return FALSE;
	
	//���ز���Ա
	nRlt = DB.TimeGroup_GetGroupInfo( m_Pool, m_lTimeGroupLst );
	
	//����ʧ��
	if( nRlt != DB_OK && nRlt != DB_NODATA )
	{
		//���������
		m_lTimeGroupLst.clear();
		
		//�����ڴ��
		m_Pool.ResetPool();	
		
		return FALSE;
	}
	
	//���سɹ��ڱ����µ�����
	return TRUE;	
}
//ʱ����ID
BOOL CTimeGroup::TimeGroup_NewID( DBManager& DB, int& nTimeGroupID )
{
	return DB.TimeGroup_NewID( nTimeGroupID ) == DB_OK?TRUE:FALSE;
}
//���һ��ʱ����
BOOL CTimeGroup::TimeGroup_Add( DBManager& DB, TIMEGROUP& stTimeGroupData )
{
	PTIMEGROUP pTimeGroup = (PTIMEGROUP)m_Pool.AllocBlock();
	if( pTimeGroup == NULL )
		return FALSE;
	
	DB_RESULT Rlt = DB.TimeGroup_Add( stTimeGroupData );
	if( Rlt != DB_OK )
	{
		m_Pool.FreeBlock( pTimeGroup );
		return FALSE;
	}
	
	CopyMemory( pTimeGroup, &stTimeGroupData, sizeof( TIMEGROUP ) );
	m_lTimeGroupLst.push_back( pTimeGroup );
	return TRUE;
}
//�޸�һ��ʱ������Ϣ
BOOL CTimeGroup::TimeGroup_Alter( DBManager& DB, TIMEGROUP& stTimeGroup )
{
	DB_RESULT Rlt = DB.TimeGroup_Alter( stTimeGroup );
	if( Rlt != DB_OK )
		return FALSE;
	
	for (list<PTIMEGROUP>::iterator iter = m_lTimeGroupLst.begin(); iter != m_lTimeGroupLst.end();iter++ )
	{
		if( (*iter)->m_nTimeGroupID == stTimeGroup.m_nTimeGroupID )
		{
			CopyMemory( *iter, &stTimeGroup, sizeof( TIMEGROUP ) );
			break;
		}
	}
	return TRUE;
}
//ɾ��һ��ʱ������Ϣ
BOOL CTimeGroup::TimeGroup_Del( DBManager& DB, int nTimgGroupID )
{
	DB_RESULT Rlt = DB.TimeGroup_Del( nTimgGroupID );
	if( Rlt != DB_OK )
		return FALSE;
	
	for (list<PTIMEGROUP>::iterator iter = m_lTimeGroupLst.begin(); iter != m_lTimeGroupLst.end();iter++ )
	{
		if( (*iter)->m_nTimeGroupID == nTimgGroupID )
		{		
			m_Pool.FreeBlock( *iter );
			m_lTimeGroupLst.erase( iter );
			break;
		}
	}
	return TRUE;
}
//�����Ƿ����
BOOL CTimeGroup::TimeGroup_IsExist( DBManager& DB, int nTimgGroupID, BOOL& bExist )
{
	return DB.TimeGroup_IsExist( nTimgGroupID, bExist ) == DB_OK?TRUE:FALSE;
}
BOOL CTimeGroup::TimeGroup_IsExist( DBManager& DB, TCHAR* szTimeGroupName,int nTimeGroupID,BOOL& bExist )
{
	return DB.TimeGroup_IsExist( szTimeGroupName, nTimeGroupID,bExist ) == DB_OK?TRUE:FALSE;
}
//����һ�����ŵ���Ϣ
PTIMEGROUP CTimeGroup::Find( int nTimeGroupID )
{
	for (list<PTIMEGROUP>::iterator iter = m_lTimeGroupLst.begin(); iter != m_lTimeGroupLst.end();iter++ )
	{
		if( (*iter)->m_nTimeGroupID == nTimeGroupID )
		{		
			return *iter;
			break;
		}
	}
	return NULL;
}