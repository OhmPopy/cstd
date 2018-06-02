// MemPool.cpp: implementation of the CMemPool class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "MemPool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemPool::CMemPool():
m_pHead(NULL),
m_pCur(NULL),
m_pLast(NULL),
m_pbMemPool(NULL)
{
}

CMemPool::~CMemPool()
{
	FreeAllMem();
}

CMemPool& CMemPool::GetInstance()
{
	static CMemPool ins;
	return ins;
}

/********************************************************************  
/*��������:	CreateMemPool		
/*����ʱ��: 2011/07/08 16:22 
/*��	��:	nPoolSize  �ڴ�ش�С
nUnitSize  �ڴ�ػ�����Ԫ
/*�ļ�����: lcy  
/*����˵��: ����һ��nPoolSize��С���ڴ棬�������ڴ�ֽ�Ϊ����nUnitSize
��С������Ԫ��ͨ������������Ԫ����������  
*********************************************************************/
void CMemPool::CreateMemPool( uint32 nPoolSize, uint32 nUnitSize )
{
	FreeAllMem();
	m_nUnitsize = ALIGN(nUnitSize, 4);
	int32 blocks= CalcueMemBlock( ALIGN(nPoolSize,4) );
	m_nPoolSize = blocks*m_nUnitsize;
	m_pHead = new MEMBLOCK[blocks+1];
	m_pLast = m_pHead+blocks;
	memset(m_pHead, 0x0, sizeof(MEMBLOCK)*(blocks+1));
	m_pbMemPool = (int8*)(new int32[m_nPoolSize>>2]);
	AssignMemToBlock(m_pHead, m_nPoolSize, m_pbMemPool);
	m_pCur = m_pHead;
}

/********************************************************************  
/*��������:	AssinMemToBlock		
/*����ʱ��: 2011/07/08 16:31 
/*��	��:	pFirst �����׵�ַ
nPoolSize �ڴ�ش�С
pRawMem �ڴ���׵�ַ
/*�ļ�����: lcy  
/*����˵��: ���ڴ�طֽ�Ϊ���ɸ������飬����������󶨵�����ڵ�   
*********************************************************************/ 
void CMemPool::AssignMemToBlock(PMEMBLOCK pFirst, uint32 nPoolSize, int8* pRawMem)
{
	int32 nChunks = CalcueMemBlock(nPoolSize);
	pFirst[0].m_bFirst = true;
	for ( int32 i = 0; i < nChunks; ++i )
	{
		pFirst[i].m_nSize = nPoolSize - i*m_nUnitsize;
		pFirst[i].m_nUsed = 0;
		pFirst[i].m_pbBlock = pRawMem + i*m_nUnitsize;
		pFirst[i].m_pNext = &(pFirst[i+1]);
	}
}

/********************************************************************  
/*��������:	CalcueMemBlock		
/*����ʱ��: 2011/07/08 16:34 
/*��	��:	size �ڴ�ش�С
/*�ļ�����: lcy  
/*����˵��: �����ڴ�ػ�����Ԫ��С�����ʵ�ʽ�Ҫ������ڴ�ش�С  
*********************************************************************/ 
int32 CMemPool::CalcueMemBlock( uint32 size )
{
	int32 blocks = size / m_nUnitsize;
	if ( size % m_nUnitsize ) blocks += 1;
	return blocks;
}

/********************************************************************  
/*��������:	GetMem		
/*����ʱ��: 2011/07/08 16:36 
/*��	��:	nSize �ڴ��С
/*�ļ�����: lcy  
/*����˵��: ���ڴ��������nSize��С���ڴ�  
*********************************************************************/ 
int8* CMemPool::GetMem( uint32 nSize, bool bCreate )
{
	if ( nSize <= 0 ) return NULL;
	
	AoutLock lock( &m_syn );
	PMEMBLOCK pRet = FindSuitbleBlock( ALIGN(nSize,4) );
	if ( !pRet && bCreate ) //���ڴ����ʣ����е��ڴ��СС��nsizeʱ��������һ���ڴ�
	{
		int32 nNeedChunks = CalcueMemBlock( ALIGN(nSize,4) );
		int nNeedSize = nNeedChunks*m_nUnitsize;
		int8* pNewMem = (int8*)(new int32[nNeedSize>>2]);
		PMEMBLOCK pNewBlock = new MEMBLOCK[nNeedChunks+1];
		memset( pNewBlock, 0x0, sizeof(MEMBLOCK)*(nNeedChunks+1));
		m_pLast->m_pNext = pNewBlock;
		m_pLast = pNewBlock+nNeedChunks;
		AssignMemToBlock(pNewBlock, nNeedSize, pNewMem);
		m_nPoolSize += nNeedSize;
		pRet = pNewBlock;
		pRet->m_nUsed = nNeedSize;
		DEBUG_INFO1( _T("CreateMemPool=%d"), nNeedSize );
	} 
	if ( pRet == NULL ) return NULL;
	m_pCur = SkipBlock( pRet );
	return pRet->m_pbBlock;
}

/********************************************************************  
/*��������:	FindSuitbleBlock		
/*����ʱ��: 2011/07/08 16:37 
/*��	��:	nSize �ڴ��С
/*�ļ�����: lcy  
/*����˵��: ���ڴ������һ����ڻ����nszie���ڴ�  
*********************************************************************/ 
PMEMBLOCK CMemPool::FindSuitbleBlock( uint32 nSize )
{
	int32 nChunks = CalcueMemBlock(m_nPoolSize);
	int32 nNeedChunks = CalcueMemBlock( nSize );
	int32 nNeedSize = nNeedChunks*m_nUnitsize;
	
	PMEMBLOCK pRet = NULL;
	PMEMBLOCK pCur = m_pCur;
	for ( int32 i = 0; (i < nChunks) && (NULL != pCur); ++i )
	{
		if ( pCur == m_pLast )
		{
			pCur = m_pHead;
		}
		if ( pCur->m_nSize >= nNeedSize )
		{
			pRet = pCur;
			bool bFind = true;
			for ( int32 j = 0; j < nNeedChunks; ++j )
			{
				assert( m_pLast != pCur );
				assert( pCur->m_nSize );
				if ( pCur->m_nUsed )
				{
					bFind = false;
					break;
				}
				pCur = pCur->m_pNext;
			}
			if ( bFind )
			{	
				break;
			}
			else
			{
				pRet = NULL;
			}
		}
		pCur = SkipBlock( pCur );
	}
	if ( pRet )
	{
		pRet->m_nUsed = nNeedSize;
	}
	return pRet;
}

/********************************************************************  
/*��������:	SkipBlock		
/*����ʱ��: 2011/07/08 16:39 
/*��	��:	pCur   ����ڵ�
nSkips ����ڵ���Ծ����
/*�ļ�����: lcy  
/*����˵��: ��pCur��ԾnSkips���ڵ�  
*********************************************************************/ 
PMEMBLOCK CMemPool::SkipBlock( PMEMBLOCK pCur )
{
	int nSkips = CalcueMemBlock( pCur->m_nUsed );
	if( nSkips <= 0 ) nSkips = 1;
	int32 i = 0;
	for ( ; i < nSkips; ++i )
	{
		pCur = pCur->m_pNext;
	}
	return pCur;
}

/********************************************************************  
/*��������:	FreeMem		
/*����ʱ��: 2011/07/08 16:42 
/*��	��:	pMem �ڴ��׵�ַ
nMemSize �ڴ��С
/*�ļ�����: lcy  
/*����˵��: �ͷ��ڴ浽�ڴ�أ��Թ�����ʹ��  
*********************************************************************/ 
void CMemPool::FreeMem( int8* pMem )
{
	if ( pMem == NULL ) return ;
	AoutLock lock( &m_syn );
	PMEMBLOCK tmp = m_pHead;
	while ( tmp != m_pLast )
	{
		if ( tmp->m_pbBlock == pMem )
		{		
			tmp->m_nUsed = 0;
			m_pCur = tmp;
			break;
		}
		tmp = tmp->m_pNext;
	}
	assert( m_pCur == tmp );
}

/********************************************************************
/*��������:	FreeAllMem		
/*����ʱ��: 2011/07/08 16:42 
/*��	��:	
/*�ļ�����: lcy  
/*����˵��: �ͷ��ڴ�� 
*********************************************************************/ 
void CMemPool::FreeAllMem()
{
	PMEMBLOCK tmp  = NULL;
	PMEMBLOCK pCur = m_pHead;
	while ( pCur )
	{	
		tmp  = pCur;
		pCur = pCur->m_pNext;
		while ( pCur != NULL && !(pCur->m_bFirst) )
		{
			pCur = pCur->m_pNext;
		}
		assert( tmp->m_bFirst  );
		delete [] tmp->m_pbBlock;
		delete [] tmp;
	}
	m_pHead     = NULL;
	m_pCur      = NULL;
	m_pLast     = NULL;
	m_pbMemPool = NULL;
}
