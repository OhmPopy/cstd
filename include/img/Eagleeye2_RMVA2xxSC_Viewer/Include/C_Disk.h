//////////////////////////////////////////////////////////////////
//																//
//		��; : ���̲���											//
//		���� : [Foolish] / 2001-1-15							//
//		���� : 2002-4-21										//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_DISK_H__
#define		__FOO_DISK_H__
#include <windows.h>
#include <mmsystem.h>
#include "StdDefine.h"
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	/****************************************************************/
	/*				��ȡ���̿ռ�									*/
	/*		���� : szDrive == _T("c:\\") , ����Ϊ��Ŀ¼				*/
	/****************************************************************/
	BOOL  fooGetDiskSpace (PCTSTR	szDrive,
						   __int64	* TotalSize,
						   __int64	* FreeSize) ;

	/****************************************************************/
	/*				��ȡ������Ϣ									*/
	/*		���� : szDrive == _T("c:\\") , ����Ϊ��Ŀ¼				*/
	/****************************************************************/
	BOOL  fooGetVolumeInfo (PCTSTR		szDrive,
							VOLUMEINFO	* pVolInfo) ; // ��StdDefine.h��

	/****************************************************************/
	/*				ö�ٴ���										*/
	/*	���̵����к����ڷ�����ϵͳ������8�ֽ��������, ���������̸�	*/
	/*	��ʱ���������̵����к�Ҳ������Ϊһ����.						*/
	/*		iModeNum Ϊ��0��ʼ������, ����FALSE��ö�����			*/
	/****************************************************************/
	BOOL  fooEnumVolume (int iModeNum, VOLUMEINFO * pVolInfo) ;

	/****************************************************************/
	/*				��/�ر�CDROM����								*/
	/****************************************************************/
	void  fooOpenCdromDoor () ;
	void  fooCloseCdromDoor () ;
	bool  fooIsDiskInsert () ;

//===================================================================
inline void  fooOpenCdromDoor () {
	::mciSendString (TEXT("set cdaudio door open"), NULL, 0, 0) ;
}
inline void  fooCloseCdromDoor () {
	::mciSendString (TEXT("set cdaudio door closed"), NULL, 0, 0) ;
}
inline bool  fooIsDiskInsert () {
	TCHAR	string[256] ;
	::mciSendString (TEXT("status cdaudio ready"), string, 256, 0) ;
	return (lstrcmpi (string, TEXT("true")) == 0) ? true : false ;
}

#ifdef __cplusplus
}
#endif
#endif