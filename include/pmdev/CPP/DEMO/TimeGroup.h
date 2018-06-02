// TimeGroup.h: interface for the CTimeGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEGROUP_H__5EB16F76_CCFA_4C2A_B9C4_EDAAE9DB1031__INCLUDED_)
#define AFX_TIMEGROUP_H__5EB16F76_CCFA_4C2A_B9C4_EDAAE9DB1031__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTimeGroup  
{
public:
	CTimeGroup();
	virtual ~CTimeGroup();

public:
	//����ʼ������
	void UnInitData();
	//��ʼ������
	BOOL InitData( DBManager& DB  );
	
public:
	//��ȡ������Ϣ
	list< PTIMEGROUP >& Get()
	{
		return m_lTimeGroupLst;
	}
	//��ȡ���Ÿ���
	int	Size()
	{
		return m_lTimeGroupLst.size();	
	}
	
public:
	//ʱ����ID
	BOOL TimeGroup_NewID( DBManager& DB, int& nTimeGroupID );
	//���һ��ʱ����
	BOOL TimeGroup_Add( DBManager& DB, TIMEGROUP& stTimeGroupData );
	//�޸�һ��ʱ������Ϣ
	BOOL TimeGroup_Alter( DBManager& DB, TIMEGROUP& DeptData );
	//ɾ��һ��ʱ������Ϣ
	BOOL TimeGroup_Del( DBManager& DB, int nTimgGroupID );
	//�����Ƿ����
	BOOL TimeGroup_IsExist( DBManager& DB, int nTimgGroupID, BOOL& bExist );
	//�����Ƿ����
	BOOL TimeGroup_IsExist( DBManager& DB, TCHAR* szTimeGroupName,int nTimeGroupID, BOOL& bExist );
	//����һ�����ŵ���Ϣ
	PTIMEGROUP Find( int TimeGroupID );
	
protected:
	MemoryPool				m_Pool;		//�ڴ��
	list< PTIMEGROUP >		m_lTimeGroupLst;	//ʱ������Ϣ
};

#endif // !defined(AFX_TIMEGROUP_H__5EB16F76_CCFA_4C2A_B9C4_EDAAE9DB1031__INCLUDED_)
