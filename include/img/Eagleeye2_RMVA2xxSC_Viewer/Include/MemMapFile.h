//////////////////////////////////////////////////////////////////
//																//
//		��; : �ڴ�ӳ��� - �ļ����� ( Memory Mapping File )	//
//		���� : [Foolish] / 2001-4-19							//
//		���� : 2002-7-25										//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_MEMMAPFILE_H__
#define		__FOO_MEMMAPFILE_H__
#include <windows.h>
#pragma once

//===================================================================
//	Declaration
//===================================================================
class FCMemMapFile
{
public :
	FCMemMapFile () ;
	~FCMemMapFile () ;
	void	UnMap () ;

	/*************************************************************/
	/*				 ����ֻ���ļ���ʧ��							 */
	/*************************************************************/
	void *	CreateFile (PCTSTR szFileName,
						DWORD dwMaxFileSize,
						bool bOverWrite = false,
						DWORD dwShare = FILE_SHARE_READ) ;

	/*************************************************************/
	/*			 ��ȡ�ļ�����Ϊ0���ļ���ʧ��					 */
	/*************************************************************/
	void *	ReadFile (PCTSTR szFileName,
					  DWORD dwShare = FILE_SHARE_READ) ;

	/*************************************************************/
	/*			 ��ȡ�ļ�����Ϊ0���ļ���ʧ��					 */
	//	dwMaxFileSize == 0 ��ʾ���ı��ļ���С					 */
	//	dwMaxFileSizeС���ļ����ȣ��򲻸ı��С��������0���	 */
	/*************************************************************/
	void *	ModifyFile (PCTSTR szFileName,
						DWORD dwMaxFileSize = 0,
						DWORD dwShare = FILE_SHARE_READ) ;

	HANDLE	GetFileHandle () const ;
	HANDLE	GetMappingHandle () const ;
	void *	GetBaseAddress () const ;
	void	FlushFile () const ;
	void	SetSize (DWORD dwFileSize) ; // �˺����ĵ��ñ���������

protected :
	HANDLE		m_hFile ;
	HANDLE		m_hFileMap ;
	VOID	  * m_pMapAddress ;
} ;

//===================================================================
//	Implement
//===================================================================
inline FCMemMapFile::FCMemMapFile () {
	m_hFile = INVALID_HANDLE_VALUE ;
	m_hFileMap = NULL ;
	m_pMapAddress = NULL ;
}
inline FCMemMapFile::~FCMemMapFile () {
	this->UnMap () ;
}
inline HANDLE FCMemMapFile::GetFileHandle () const {
	return m_hFile ;
}
inline HANDLE FCMemMapFile::GetMappingHandle () const {
	return m_hFileMap ;
}
inline void * FCMemMapFile::GetBaseAddress () const {
	return m_pMapAddress ;
}
inline void FCMemMapFile::FlushFile () const {
	if (m_pMapAddress != NULL)
		::FlushViewOfFile (m_pMapAddress, 0) ;
}
inline void FCMemMapFile::UnMap () {
	if (m_pMapAddress != NULL)				::UnmapViewOfFile (m_pMapAddress) ;
	if (m_hFileMap != NULL)					CloseHandle (m_hFileMap) ;
	if (m_hFile != INVALID_HANDLE_VALUE)	CloseHandle (m_hFile) ;
	m_hFile = INVALID_HANDLE_VALUE ;
	m_hFileMap = NULL ;
	m_pMapAddress = NULL ;
}
inline void FCMemMapFile::SetSize (DWORD dwFileSize) {
	if (m_pMapAddress != NULL)		::UnmapViewOfFile (m_pMapAddress) ;
	if (m_hFileMap != NULL)			CloseHandle (m_hFileMap) ;
	if (m_hFile != INVALID_HANDLE_VALUE) {
		::SetFilePointer (m_hFile, dwFileSize, NULL, FILE_BEGIN) ;
		::SetEndOfFile (m_hFile) ;
		CloseHandle (m_hFile) ;
	}
	m_hFile = INVALID_HANDLE_VALUE ;
	m_hFileMap = NULL ;
	m_pMapAddress = NULL ;
}

#endif