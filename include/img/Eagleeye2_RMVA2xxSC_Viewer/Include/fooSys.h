//////////////////////////////////////////////////////////////////
//																//
//		��; : General ( Win32 ���ú����� )						//
//		���� : [Foolish] / 2001-3-15							//
//		���� :													//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_GENERAL_WIN32_H__
#define		__FOO_GENERAL_WIN32_H__
#include <windows.h>
#include "StdDefine.h"
#pragma once

//=============================================================================

////////////////////////////////////////////////////////////
//	��  �� ����ô���������������
////////////////////////////////////////////////////////////
PTSTR  fooGetErrorString (DWORD dwError = ::GetLastError ()) ;
////////////////////////////////////////////////////////////
//	��  �� ����Ϣ��
////////////////////////////////////////////////////////////
void  fooMsgBox (PCTSTR szMessage) ;
////////////////////////////////////////////////////////////
//	��  �� ����һ���������
//	��  �� ��
//	����ֵ ��
//	˵  �� ���������� --0		Internet����--1		��ý������--2
//			 Modem����--3		��������--4			��Դ����--5
//			 ���/ɾ������--6	�������--7			ϵͳ����--8
//			 ����/ʱ��--9		��������--10
////////////////////////////////////////////////////////////
BOOL  fooControlPad (HWND hwnd, DWORD dwType) ;
////////////////////////////////////////////////////////////
//	��  �� ������
//	��  �� ��
//	˵  �� ��uiTimeOut Ϊ����
////////////////////////////////////////////////////////////
BOOL  fooSetScreenSave (BOOL bEnable, UINT uiTimeOut) ;
BOOL  fooGetScreenSave (BOOL * bEnable, UINT * uiTimeOut) ;
////////////////////////////////////////////////////////////
//	��  �� ��������ת�����ַ���, ������","
//	��  �� ��iBufLenΪszBuffer��С
////////////////////////////////////////////////////////////
void  fooFormatCommaNumber (DWORD dwNum, TCHAR * szBuffer, int iBufLen) ;
////////////////////////////////////////////////////////////
//	��  �� ��ͨ�öԻ������
//	��  �� ��szFilter = "Executable Files (*.exe, *.dll)\0*.exe;*.dll\0"
//						"All Files (*.*)\0*.*\0\0" ;
//	˵  �� ��
////////////////////////////////////////////////////////////
BOOL  fooShowFileDialog (HWND hWnd, BOOL bOpen,
						 TCHAR * szFullName,	// ���ص��ļ���
						 PCTSTR szIniDir = NULL,	// ��ǰĿ¼
						 PCTSTR szTitle = NULL,
						 PCTSTR szFilter = NULL) ;

BOOL  fooShowColorDialog (COLORREF * ColorSelect,
						  COLORREF   InitColor[16] = NULL) ;
////////////////////////////////////////////////////////////
//	��  �� ����ȡ������ʾ��Ϣ
//	˵  �� ��
////////////////////////////////////////////////////////////
void  fooGetLargestDisplayMode (DISPLAYMODE * mode) ;
void  fooGetCurrentDisplayMode (DISPLAYMODE * mode) ;
LONG  fooSetDisplayMode (DISPLAYMODE mode) ;
////////////////////////////////////////////////////////////
//	��  �� ���ض�λ�ؼ�
//	��  �� ��rcDest,rcSrc Ϊ GetClientRect �ķ���ֵ
//			 nMethod : DLG_RESIZE_MOVE �� DLG_RESIZE_STRETCH
//	����ֵ ��
//	˵  �� ���� OnSize �е���
////////////////////////////////////////////////////////////
void  fooOnChangeDlgSize (HWND hDlg,
						  RECT rcDest, RECT rcSrc,
						  UINT nNumCtrl,
						  UINT nCtrl[],		// �ؼ�����
						  UINT nMethod[]) ;	// ��λ��������
////////////////////////////////////////////////////////////
//	��  �� ��
//	��  �� ��fExpand : TRUE��չ���Ի���
//			 DefaultBox_ID : Ԥ��Ի���ID
//	����ֵ ��
//	˵  �� ��������Initʱ����һ�� ��С(fExpand : FALSE)
////////////////////////////////////////////////////////////
void  fooExpandBox (BOOL fExpand, HWND hDlg, int DefaultBox_ID) ;

//===================================================================
//	Implement
//===================================================================
inline void  fooMsgBox (PCTSTR szMessage) {
	TCHAR	szTitle[128] ;
	::GetModuleFileName (NULL, szTitle, 128) ;
	::MessageBox (NULL, szMessage, szTitle, MB_OK) ;
}
inline BOOL  fooSetScreenSave (BOOL bEnable, UINT uiTimeOut) {
	BOOL	bResult = SystemParametersInfo (SPI_SETSCREENSAVEACTIVE, bEnable, NULL, SPIF_SENDCHANGE| SPIF_UPDATEINIFILE) ;
	if (bEnable)
		bResult &= SystemParametersInfo (SPI_SETSCREENSAVETIMEOUT, uiTimeOut*60, NULL, SPIF_SENDCHANGE| SPIF_UPDATEINIFILE) ;
	return bResult ;
}
inline BOOL  fooGetScreenSave (BOOL *bEnable, UINT *uiTimeOut) {
	BOOL	bResult = SystemParametersInfo (SPI_GETSCREENSAVEACTIVE, 0, bEnable, SPIF_SENDCHANGE) ;
	bResult &= SystemParametersInfo (SPI_GETSCREENSAVETIMEOUT, 0, uiTimeOut, SPIF_SENDCHANGE) ;
	*uiTimeOut /= 60 ;
	return bResult ;
}

//===================================================================

#endif