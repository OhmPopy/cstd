/********************************************************************

	iseetsvc.h

	----------------------------------------------------------------
    ������֤ �� GPL
	��Ȩ���� (C) 2003 VCHelp coPathway ISee workgroup.
	----------------------------------------------------------------
	��һ����������������������������������������GNU ͨ�ù�����
	��֤�������޸ĺ����·�����һ���򡣻��������֤�ĵڶ��棬���ߣ���
	�����ѡ�����κθ��µİ汾��

    ������һ�����Ŀ����ϣ�������ã���û���κε���������û���ʺ��ض�
	Ŀ�ص������ĵ���������ϸ����������GNUͨ�ù������֤��

    ��Ӧ���Ѿ��ͳ���һ���յ�һ��GNUͨ�ù������֤�ĸ�������Ŀ¼
	GPL.txt�ļ����������û�У�д�Ÿ���
    The Free Software Foundation, Inc.,  675  Mass Ave,  Cambridge,
    MA02139,  USA
	----------------------------------------------------------------
	�������ʹ�ñ����ʱ��ʲô������飬�������µ�ַ������ȡ����ϵ��

			http://isee.126.com
			http://cosoft.org.cn/projects/iseeexplorer
			
	���ŵ���

			isee##vip.163.com
	----------------------------------------------------------------
	���ļ���;��	ͼ���д���棭C���԰棭VC������������������
					�ඨ���ļ���

	���ļ���д�ˣ�	
					YZ				yzfree##sina.com

	���ļ��汾��	30318
	����޸��ڣ�	2003-3-18

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������

	������ʷ��

		2003-3		���Ӹ߾��ȼ�ʱ����
		2003-1		��һ�����԰淢��

********************************************************************/

#if !defined(AFX_ISEETSVC_H__4D09C7E7_8005_440E_B040_D14C2BA6F470__INCLUDED_)
#define AFX_ISEETSVC_H__4D09C7E7_8005_440E_B040_D14C2BA6F470__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CIseetsvcApp;

/////////////////////////////////////////////////////////////////////////////
// CTimeCounter - �߾��ȼ�ʱ���ඨ��

class CTimeCounter
{
public:
	CTimeCounter();
	~CTimeCounter();

	void BeginCount(DWORD space = 0);
	BOOL IsEnd(void);
	DWORD GetTime(void);

protected:
	LARGE_INTEGER	m_liStart;
	LARGE_INTEGER	m_liEnd;
	LARGE_INTEGER	m_liFrequ;
	DWORD			m_dwSpace;
};


/////////////////////////////////////////////////////////////////////////////
// CLogRecorder - ������¼�ඨ��

class CLogRecorder : private CFile
{
public:
	CLogRecorder();
	~CLogRecorder();

	int	OpenRecorder(LPCSTR logname);
	int	CloseRecorder(void);

	int	Record(LPCSTR str);
	int	Record(LPCSTR str, int cont);
	int	Record(LPCSTR str, int cont, int ipar);
	int	Record(LPCSTR str, int cont, LPCSTR ppar);

protected:
	static char	clr_welcome[];
	static char	clr_spek[];
};


// ��ȡȫ�ּ�¼���ַ
CLogRecorder & ISeeTsVCDebugStrRecorder(void);


/////////////////////////////////////////////////////////////////////////////
// CIseetsvcApp:
// See iseetsvc.cpp for the implementation of this class
//

class CIseetsvcApp : public CWinApp
{
public:
	CIseetsvcApp();

	DWORD GetDllVersion(LPCTSTR lpszDllName);

// Attrib
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CLogRecorder	m_clRecord;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIseetsvcApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CIseetsvcApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISEETSVC_H__4D09C7E7_8005_440E_B040_D14C2BA6F470__INCLUDED_)
