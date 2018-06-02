//////////////////////////////////////////////////////////////////
//																//
//		��; : ��/�̲߳���										//
//		���� : [Foolish] / 2001-1-15							//
//		���� : 2002-4-21										//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_PROCESS_H__
#define		__FOO_PROCESS_H__
#include <windows.h>
#include <TCHAR.H>
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	/****************************************************************/
	/*				�ж��Ƿ�������9xϵͳ��							*/
	/****************************************************************/
	bool  fooIsWindows9x () ;

	/****************************************************************/
	/*					��������									*/
	/*			������ShowWindow, SetPriorityClass					*/
	/****************************************************************/
	BOOL  fooCreateProcess (PCTSTR	szFileName,
							int		nCmdShow = SW_SHOWNORMAL,
							DWORD	dwPriority = NORMAL_PRIORITY_CLASS) ;

	/****************************************************************/
	/*					�����߳�									*/
	/*		������SetThreadPriority,  CloseHandle�������������߳�	*/
	/****************************************************************/
	HANDLE  fooCreateThread (void	* pStartAddress,
							 void	* pParameter = NULL,
							 DWORD	dwPriority = THREAD_PRIORITY_NORMAL) ;

	/****************************************************************/
	/*					ע��DLL����������							*/
	/*			������_L()��szDllFile��ΪUNICODE�ַ���				*/
	/****************************************************************/
	BOOL  fooInjectDLL_W (DWORD dwProcessId, PCWSTR szDllFile) ;

	/****************************************************************/
	/*				�õ���ǰ���̵���ʵ���							*/
	/*	::GetCurrentProcess() ��õ���α���, ���Ƿ���0x7FFFFFFF	*/
	/****************************************************************/
	HANDLE  fooGetRealProcessHandle () ;

	/****************************************************************/
	/*				������/�ر�/��¼ϵͳ							*/
	/****************************************************************/
	void  fooRebootSys () ;
	void  fooShutDownSys  () ;
	void  fooLogoSys () ;

	/****************************************************************/
	/*				��ȡpPrivilegeNameȨ��	(win2000)				*/
	/*			pPrivilegeName��WINNT.h�ж��� : SE_***				*/
	/****************************************************************/
	BOOL  fooEnablePrivilege (PCTSTR pPrivilegeName) ;

	/****************************************************************/
	/*		��ȡDebugȨ����(����OpenProcess)�򿪽���  (win2000)		*/
	/****************************************************************/
	BOOL  fooEnableDebugPrivilege () ;

	/****************************************************************/
	/*					��ֹ/�������������							*/
	/****************************************************************/
	void  fooEnableTaskDlg (BOOL bEnable) ;

//===================================================================
inline BOOL  fooEnableDebugPrivilege () {
	return fooEnablePrivilege (SE_DEBUG_NAME) ;
}
inline HANDLE  fooGetRealProcessHandle () {
	HANDLE		hProcess ;
	DuplicateHandle (::GetCurrentProcess (), ::GetCurrentProcess(), ::GetCurrentProcess(),
					 &hProcess, 0, FALSE, DUPLICATE_SAME_ACCESS) ;
	return hProcess ;
}
inline bool  fooIsWindows9x () {
	OSVERSIONINFO	osv ;
	::GetVersionEx (&osv) ;
	return (osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) ;
}
inline void  fooRebootSys () {
	if (!::fooIsWindows9x ())
		::fooEnablePrivilege (SE_SHUTDOWN_NAME) ;
	::ExitWindowsEx (EWX_REBOOT, 0) ;
}
inline void  fooShutDownSys () {
	if (!::fooIsWindows9x ())
		::fooEnablePrivilege (SE_SHUTDOWN_NAME) ;
	ExitWindowsEx (EWX_POWEROFF, 0) ;
}
inline void  fooLogoSys () {
	ExitWindowsEx (EWX_LOGOFF, 0) ;
}

#ifdef __cplusplus
}
#endif
#endif