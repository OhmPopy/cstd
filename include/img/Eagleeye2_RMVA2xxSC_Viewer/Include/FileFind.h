//////////////////////////////////////////////////////////////////
//																//
//		��; : �ļ�����											//
//		���� : [Foolish] / 2001-3-27							//
//		���� : 2002-4-24										//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_FILEFIND_H__
#define		__FOO_FILEFIND_H__
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#pragma once

//===================================================================
//	�ļ�������
//===================================================================
class FCFileFind
{
public :
	FCFileFind () ;
	~FCFileFind () ;

	//	����
	BOOL	FindFile (PCTSTR szPathName) ;	// �� : "c:\\Pic\\jpg\\*.*"
	bool	IsNoPath   ( ) const ;	// ��FindFile����ã�ΪTRUE���򲻴���·����
	BOOL	FindNext ( ) ;
	bool	IsDirectory ( ) const ;
	bool	IsNoMore   ( ) const ;	// ��FindNext����ã�ΪTRUE�����޸���ƥ�䡣���������ң�
	void	FindClose( ) ;
	bool	IsExistFile (PCTSTR szPathName) ; // TRUE�����

	//	��ȡ����
	DWORD	GetSize  ( ) const ;
	DWORD	GetAttribute ( ) const ;
	PCTSTR	GetFullName ( ) const ;
	PCTSTR	GetFileName ( ) ;	// ����·������չ�����ļ���
	PCTSTR	GetExtName  ( ) ;	// ������ : ".bmp"
	PCTSTR	GetDriver   ( ) ;	// ������ : "E:"
	PCTSTR	GetPath		( ) ;	// ������ : "\Pic\bmp\"

protected :
	void	Initialize ( ) ;
	void	SpliteName ( ) ;
protected :
	HANDLE			m_hFileFind ;		// �����ļ����
	DWORD			m_dwFileAttribute ;	// �ļ�����
	DWORD			m_dwFileSize ;		// �ļ���С

	TCHAR	m_szFindPath[_MAX_PATH] ;	// ����·����

	TCHAR	m_szFullName[_MAX_PATH] ;	// ��·���ļ���
	TCHAR	m_szDriver[_MAX_DRIVE] ;	// ��ǰ��
	TCHAR	m_szPath[_MAX_DIR] ;		// ��ǰ·����
	TCHAR	m_szFileName[_MAX_FNAME] ;	// ����·���ļ���, ������չ��
	TCHAR	m_szFileExt[_MAX_EXT] ;		// �ļ���չ��
} ;

//===================================================================
//	Implement
//===================================================================
inline FCFileFind::FCFileFind () {
	this->Initialize ( ) ;
}
inline FCFileFind::~FCFileFind () {
	this->FindClose () ;
}
inline void  FCFileFind::Initialize ( ) {
	m_hFileFind		  = INVALID_HANDLE_VALUE ;
	m_dwFileAttribute = 0xFFFFFFFF ;
	m_dwFileSize      = -1 ;
	m_szFindPath[0]	  = TEXT('\0') ;
	m_szFileName[0]	  = TEXT('\0') ;
	m_szFileExt[0]	  = TEXT('\0') ;
	m_szFullName[0]   = TEXT('\0') ;
	m_szDriver[0]     = TEXT('\0') ;
	m_szPath[0]		  = TEXT('\0') ;
}
inline void  FCFileFind::FindClose ( ) {
	if (m_hFileFind != INVALID_HANDLE_VALUE)
		::FindClose (m_hFileFind) ;
	this->Initialize ( ) ;
}
inline PCTSTR  FCFileFind::GetFullName ( ) const {
	return m_szFullName ;
}
inline void  FCFileFind::SpliteName ( ) {
	_splitpath (m_szFullName, m_szDriver, m_szPath, m_szFileName, m_szFileExt) ;
}
inline PCTSTR  FCFileFind::GetExtName ( ) {
	this->SpliteName () ;
	return m_szFileExt ;
}
inline PCTSTR  FCFileFind::GetFileName ( ) {
	this->SpliteName () ;
	return m_szFileName ;
}
inline PCTSTR  FCFileFind::GetDriver ( ) {
	this->SpliteName () ;
	return m_szDriver ;
}
inline PCTSTR  FCFileFind::GetPath ( ) {
	this->SpliteName () ;
	return m_szPath ;
}
inline DWORD  FCFileFind::GetSize ( ) const {
	return m_dwFileSize ;
}
inline DWORD  FCFileFind::GetAttribute ( ) const {
	return m_dwFileAttribute ;
}
inline bool  FCFileFind::IsNoMore ( ) const {
	return (::GetLastError () == ERROR_NO_MORE_FILES) ;
}
inline bool  FCFileFind::IsExistFile (PCTSTR szPathName) {
	this->FindFile (szPathName) ;
	bool	bRet = (::GetLastError () != ERROR_FILE_NOT_FOUND) ;
	this->FindClose () ;
	return bRet ;
}
inline bool  FCFileFind::IsNoPath ( ) const {
	return (::GetLastError () == ERROR_PATH_NOT_FOUND) ;	
}
inline bool  FCFileFind::IsDirectory ( ) const {
	return (m_dwFileAttribute == FILE_ATTRIBUTE_DIRECTORY) ;
}

#endif