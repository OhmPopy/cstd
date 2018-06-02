//////////////////////////////////////////////////////////////////
//																//
//		��; : �ļ���											//
//		���� : [Foolish] / 2001-4-24							//
//		���� : 2002-1-12										//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_FOLDER_H__
#define		__FOO_FOLDER_H__
#include <windows.h>
#include <shlobj.h>
#pragma once

//===================================================================
//	��ʾѡ���ļ��е���
//===================================================================
class FCSelectFolder
{
public :
	FCSelectFolder (HWND hParent = NULL, PCTSTR szTitle = NULL) ;
	~FCSelectFolder () ;

	int		Select () ;	// return IDOK or IDCANCEL
	PCTSTR	GetPathName () ;

protected :
	//	���غ���
	virtual void  OnSelChange (ITEMIDLIST * pIdl) ;
	virtual void  BrowseDispatchMsg (HWND hWnd, UINT uMsg, LPARAM lParam) ;

	void	EnableOK (BOOL bEnable = TRUE) ;
	void	SetStatusText (PCTSTR szStatusText) ;

protected :
	BROWSEINFO	m_bi ;
	TCHAR		m_szPath[_MAX_PATH] ;
	HWND		m_hSelectDlg ;

	friend int CALLBACK __fooBrowseCallbackProc (HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData) ;
} ;

//===================================================================
inline FCSelectFolder::FCSelectFolder (HWND hParent, PCTSTR szTitle) {
	ZeroMemory (&m_bi, sizeof (BROWSEINFO)) ;
	m_bi.hwndOwner		= hParent ;
	m_bi.pidlRoot		= NULL ;
	m_bi.pszDisplayName	= m_szPath ;	
	m_bi.lpszTitle		= (szTitle == NULL) ? TEXT("ѡ���ļ���") : szTitle ;
	m_bi.ulFlags		= BIF_STATUSTEXT | 0 ;
	m_bi.lpfn			= __fooBrowseCallbackProc ;
	m_bi.lParam			= (LPARAM) this ;
	m_szPath[0]			= TEXT('\0') ;
}
inline FCSelectFolder::~FCSelectFolder () {
}
inline PCTSTR FCSelectFolder::GetPathName () {
	return (m_szPath[0] == TEXT('\0')) ? NULL : m_szPath ;
}
inline void  FCSelectFolder::EnableOK (BOOL bEnable) {
	::SendMessage (m_hSelectDlg, BFFM_ENABLEOK, (bEnable ? 1 : 0), 0) ;
}
inline void  FCSelectFolder::SetStatusText (PCTSTR szStatusText) {
	::SendMessage (m_hSelectDlg, BFFM_SETSTATUSTEXT, 0, (LPARAM) szStatusText) ;
}

#endif