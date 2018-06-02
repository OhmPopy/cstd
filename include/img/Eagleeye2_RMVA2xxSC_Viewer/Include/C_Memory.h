//////////////////////////////////////////////////////////////////
//																//
//		��; : �ڴ����											//
//		���� : [Foolish] / 2001-6-5								//
//		���� :													//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_MEMORY_H__
#define		__FOO_MEMORY_H__
#include <windows.h>
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	/****************************************************************/
	/*				����һ�鹲���ڴ�								*/
	/*		�������pMemName, ���ܴ���, ����NULL					*/
	/****************************************************************/
	void *  fooMallocSharedMemory (PCTSTR	pMemName,
								   int		iBlockSize,
								   HANDLE	* hMapping) ; // ����ΪNULL

	/****************************************************************/
	/*				�ͷ�һ�鹲���ڴ�								*/
	/****************************************************************/
	void  fooFreeSharedMemory (void * pBaseAddr, HANDLE hMapping) ;

	/****************************************************************/
	/*				��/дһ�鹲���ڴ�								*/
	/****************************************************************/
	BOOL  fooReadSharedMemory (PCTSTR	pMemName,
							   BYTE		* pReceive,
							   int		iExpectBytes) ;

	BOOL  fooWriteSharedMemory (PCTSTR	pMemName,
								BYTE	* pWrite,
								int		iWriteBytes) ;

	/****************************************************************/
	/*			�ж��Ƿ������ΪpMemName�Ĺ����ڴ�					*/
	/****************************************************************/
	bool  fooIsExistSharedMemory (PCTSTR pMemName) ;

//===================================================================
inline void  fooFreeSharedMemory (void * pBaseAddr, HANDLE hMapping) {
	if (pBaseAddr != NULL)
		::UnmapViewOfFile (pBaseAddr) ;
	if (hMapping != NULL)
		::CloseHandle (hMapping) ;
}
inline bool fooIsExistSharedMemory (PCTSTR pMemName) {
	HANDLE	hMap = NULL ;
	if (pMemName != NULL)
	{
		hMap = ::OpenFileMapping (FILE_MAP_READ, FALSE, pMemName) ;
		if (hMap != NULL)
			::CloseHandle (hMap) ;
	}
	return hMap ? true : false ;
}

#ifdef __cplusplus
}
#endif
#endif