//////////////////////////////////////////////////////////////////
//																//
//		��; : �ļ�����											//
//		���� : [Foolish] / 1999-10-9							//
//		���� :													//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_FILE_H__
#define		__FOO_FILE_H__
#include <windows.h>
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	/****************************************************************/
	/*		ΪNULL����ϵͳTempĿ¼�´���, Close() ʱ�Զ�ɾ��		*/
	/*		�� : szPathName == "c:\\Pic\\jpg\\"						*/
	/****************************************************************/
	HANDLE  fooCreateTempFile (PCTSTR szPathName = NULL) ;

	/****************************************************************/
	/*		�õ��ļ����� (win98)									*/
	/****************************************************************/
	DWORD  fooGetFileSize (PCTSTR szFileName) ;

	/****************************************************************/
	/*		����/ɾ���ļ�����										*/
	/****************************************************************/
	BOOL  fooDelFileAttributes (PCTSTR szFileName, DWORD dwAttribute) ;
	BOOL  fooAddFileAttributes (PCTSTR szFileName, DWORD dwAttribute) ;

//===================================================================
inline DWORD fooGetFileSize (PCTSTR szFileName) {
	WIN32_FILE_ATTRIBUTE_DATA	file_data ;
	return (::GetFileAttributesEx (szFileName, GetFileExInfoStandard, &file_data) == 0)
		    ? -1 : file_data.nFileSizeLow ;
}
inline BOOL  fooDelFileAttributes (PCTSTR szFileName, DWORD dwAttribute) {
	DWORD	dwAtt = ::GetFileAttributes (szFileName) ;
	if (dwAtt == -1)	return FALSE ;
	dwAtt &= ~dwAttribute ;
	return ::SetFileAttributes (szFileName, dwAtt) ;
}
inline BOOL  fooAddFileAttributes (PCTSTR szFileName, DWORD dwAttribute) {
	DWORD	dwAtt = ::GetFileAttributes (szFileName) ;
	if (dwAtt == -1)	return FALSE ;
	dwAtt |= dwAttribute ;
	return ::SetFileAttributes (szFileName, dwAtt) ;
}

#ifdef __cplusplus
}
#endif
#endif