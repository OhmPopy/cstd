//////////////////////////////////////////////////////////////////
//																//
//		��; : ������ز���										//
//		���� : [Foolish] / 2002-9-29							//
//		���� :													//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_NETWORK_H__
#define		__FOO_NETWORK_H__
#include <windows.h>
#include <TCHAR.H>
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	/****************************************************************/
	/*				��ʾ����ѡ��Ի��� (win2000/XP)					*/
	/*				 dwBufLenΪ��������С(�ַ���)					*/
	/*					�û���OK��, ����TRUE						*/
	/****************************************************************/
	BOOL  fooChooseDomainDlg (HWND hParent, LPWSTR wCompName, DWORD dwBufLen) ;

//===================================================================

#ifdef __cplusplus
}
#endif
#endif