/********************************************************************

	iseetsvc.cpp

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
					��ʵ���ļ���

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

#include "stdafx.h"
#include "iseetsvc.h"

#include "shellpidl.h"
#include "shelllist.h"
#include "shelltree.h"
#include "shellpath.h"

#include "Display.h"
#include "LeftView.h"
#include "RightView.h"

#include "MainFrm.h"
#include "iseetsvcDoc.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTimeCounter::CTimeCounter()
{
	m_liStart.QuadPart = (LONGLONG)0;
	m_liEnd.QuadPart   = (LONGLONG)0;
	m_dwSpace          = 0UL;

	// ��ȡ��ʱ��Ƶ��
	::QueryPerformanceFrequency(&m_liFrequ);

	// ת��Ϊ���뵥λ��Ƶ��
	m_liFrequ.QuadPart = m_liFrequ.QuadPart / 1000;
}

CTimeCounter::~CTimeCounter()
{
}

// ��ʼ��ʱ
void CTimeCounter::BeginCount(DWORD space/* ���ʱ�䣨���뵥λ��*/)
{
	m_dwSpace = space;

	::QueryPerformanceCounter(&m_liStart);
}

// �ж��Ƿ�ʱ
BOOL CTimeCounter::IsEnd()
{
	::QueryPerformanceCounter(&m_liEnd);

	if (((m_liEnd.QuadPart-m_liStart.QuadPart)/m_liFrequ.QuadPart) >= (LONGLONG)m_dwSpace)
	{
		return TRUE;									// �ѳ����趨ʱ��
	}

	return FALSE;										// δ��ʱ��
}

// ��ȡ�ӿ�ʼ��ʱ�𣬵���ǰ�����ŵ�ʱ�䣨���뵥λ��
DWORD CTimeCounter::GetTime()
{
	::QueryPerformanceCounter(&m_liEnd);

	return (DWORD)((m_liEnd.QuadPart-m_liStart.QuadPart)/m_liFrequ.QuadPart);
}




char	CLogRecorder::clr_welcome[] = "\r\n\t\t\tISee�����ò�����¼�ļ�\r\n=======================================================================\r\n\r\n";
char	CLogRecorder::clr_spek[]    = "-------------------------------------------------------------------\r\n";


CLogRecorder::CLogRecorder() : CFile()
{
}


CLogRecorder::~CLogRecorder()
{
}


int	CLogRecorder::OpenRecorder(LPCSTR logname)
{
	ASSERT(m_hFile == hFileNull);						// �����ظ��򿪼�¼�ļ�

	TCHAR	szBuff[MAX_PATH];
	int		iLen;
	
	// ȡ��Ӧ�ó����ȫ·��
	::GetModuleFileName(NULL, (LPTSTR)szBuff, MAX_PATH);
	
	iLen = lstrlen((char*)szBuff);
	
	// ȥ��β����Ӧ�ó�����
	while ((szBuff[--iLen] != '\\')&&(iLen > 0))
	{
		szBuff[iLen] = '\0';
	}
	
	// �ϳɼ�¼�ļ���
	lstrcat((LPSTR)szBuff, logname);

	// ������¼�ļ�
	if (Open((LPCTSTR)szBuff, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate) == 0)
	{
		return -1;										// ������¼�ļ�ʧ��
	}

	SeekToEnd();

	Write((const void *)clr_welcome, lstrlen((LPCSTR)clr_welcome));

	CTime	time = CTime::GetCurrentTime();
	CString	ft   = time.Format((LPCSTR)"[��ʼ��¼ʱ�䣺%Y-%m-%d  %H:%M:%S]\r\n");
	Write((const void *)(LPCSTR)ft, ft.GetLength());
	Write((const void *)clr_spek, lstrlen((LPCSTR)clr_spek));

	return 0;
}


int	CLogRecorder::CloseRecorder()
{
	if (m_hFile == CFile::hFileNull)
	{
		return -1;										// ��¼�ļ���û�б�����
	}

	Write((const void *)"\r\n", 2);
	Write((const void *)clr_spek, lstrlen((LPCSTR)clr_spek));

	CTime	time = CTime::GetCurrentTime();
	CString	ft   = time.Format((LPCSTR)"[������¼ʱ�䣺%Y-%m-%d  %H:%M:%S]\r\n");
	Write((const void *)(LPCSTR)ft, ft.GetLength());
	Write((const void *)"\r\n\r\n\r\n\r\n", 8);

	Flush();
	Close();

	return 0;
}


//
int CLogRecorder::Record(LPCSTR str, int cont, int ipar)
{
	if (m_hFile == CFile::hFileNull)
	{
		return -1;										// ��¼�ļ���û�б�����
	}

	static int	flcnt = 0;

	CTime	time = CTime::GetCurrentTime();
	CString	ft   = time.Format((LPCSTR)"[%H:%M:%S] - ");
	Write((const void *)(LPCSTR)ft, ft.GetLength());
	
	while (cont--)
	{
		Write((const void *)"\t", 1);
	}

	CString	stFS;

	stFS.Format(str, (int)ipar);

	Write((const void *)(LPCSTR)stFS, stFS.GetLength());
	Write((const void *)"\r\n", 2);

	if (!(flcnt % 10))
	{
		Flush();
	}

	return 0;
}


// 
int CLogRecorder::Record(LPCSTR str, int cont, LPCSTR ppar)
{
	if (m_hFile == CFile::hFileNull)
	{
		return -1;										// ��¼�ļ���û�б�����
	}
	
	static int	flcnt = 0;
	
	CTime	time = CTime::GetCurrentTime();
	CString	ft   = time.Format((LPCSTR)"[%H:%M:%S] - ");
	Write((const void *)(LPCSTR)ft, ft.GetLength());
	
	while (cont--)
	{
		Write((const void *)"\t", 1);
	}
	
	CString	stFS;
	
	stFS.Format(str, ppar);
	
	Write((const void *)(LPCSTR)stFS, stFS.GetLength());
	Write((const void *)"\r\n", 2);
	
	if (!(flcnt % 10))
	{
		Flush();
	}
	
	return 0;
}


// 
CLogRecorder & ISeeTsVCDebugStrRecorder()
{
	return ((CIseetsvcApp*)AfxGetApp())->m_clRecord;
}

/////////////////////////////////////////////////////////////////////////////
// CIseetsvcApp

BEGIN_MESSAGE_MAP(CIseetsvcApp, CWinApp)
	//{{AFX_MSG_MAP(CIseetsvcApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CIseetsvcApp construction

CIseetsvcApp::CIseetsvcApp()
{
}



/////////////////////////////////////////////////////////////////////////////
// The one and only CIseetsvcApp object

CIseetsvcApp theApp;


#define PACKVERSION(major,minor) MAKELONG(minor,major)

/////////////////////////////////////////////////////////////////////////////
// CIseetsvcApp initialization

BOOL CIseetsvcApp::InitInstance()
{
	// ��ʼ��COM��
	if (CoInitialize(NULL) != S_OK)
	{
		return FALSE;
	}

	// ���������ò�����¼�ļ�
	m_clRecord.OpenRecorder("iseetsvc.log");
	
	m_clRecord.Record("�����ʼ���С�", 0, 0);

	// �޶��û��Ĳ���ϵͳ
	if(GetDllVersion(TEXT("comctl32.dll")) < PACKVERSION(4,71))
	{
		m_clRecord.Record("IE�İ汾̫�ͣ����δ�����С�", 0, 0);
		::AfxMessageBox("�������ҪWin98+IE4.0���ϵĲ���ϵͳ������ϵͳ�����ϴ����Ҫ����������˳���");
		CoUninitialize();
		return FALSE;
	}
	
	AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();
#else
	Enable3dControlsStatic();
#endif

	SetRegistryKey(_T("ISee Workgroup"));

	LoadStdProfileSettings();

	TCHAR	szBuff[MAX_PATH];
	int		iLen;
	
	// ȡ��Ӧ�ó����ȫ·������Ϊ���DLLҲ�������Ŀ¼�У�
	::GetModuleFileName(NULL, (LPTSTR)szBuff, MAX_PATH);
	
	iLen = lstrlen((char*)szBuff);
	
	// ȥ��β����Ӧ�ó�����
	while ((szBuff[--iLen] != '\\')&&(iLen > 0))
	{
		szBuff[iLen] = '\0';
	}
	
	// ��ʼ��ISeeͼ���д����
	if (isirw_init((char*)(LPCSTR)szBuff, 0))
	{
		m_clRecord.Record("ͼ���д���洴��ʧ�ܣ�����˳���", 0, 0);
		::AfxMessageBox("ISeeͼ���д���洴��ʧ�ܣ����򼴽��˳���");
		CoUninitialize();
		return FALSE;
	}

	// ������������ػ�����
	isirw_enbuffer(IRWE_BUFTYPE_ORG|IRWE_BUFTYPE_CNV);

	m_clRecord.Record("ͼ���д���洴���ɹ���", 0, 0);
	m_clRecord.Record("��ǰͼ���д����֧�ֵ�ͼ���ʽ��չ����%s", 0, (LPCSTR)isirw_get_shellstr());

	CSingleDocTemplate* pDocTemplate;

	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CIseetsvcDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLeftView));

	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ������ӭ���湦��
	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CIseetsvcApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CIseetsvcApp message handlers


int CIseetsvcApp::ExitInstance() 
{
	m_clRecord.Record("��������˳�...", 0, 0);

	// �رյ����ü�¼�ļ�
	m_clRecord.CloseRecorder();

	// �ر���������ػ�����
	isirw_dibuffer(IRWE_BUFTYPE_ORG|IRWE_BUFTYPE_CNV);

	// �ر�ISeeͼ���д����
	isirw_close();

	CoUninitialize();

	return CWinApp::ExitInstance();
}


// ��ȡDLL�ļ��İ汾��ͨ���ͣ���ȡ��MSDN��
DWORD CIseetsvcApp::GetDllVersion(LPCTSTR lpszDllName)
{
	HINSTANCE	hinstDll;
	DWORD		dwVersion = 0;

	// ����DLL�ļ�
	hinstDll = LoadLibrary(lpszDllName);

	if(hinstDll)
	{
		DLLGETVERSIONPROC pDllGetVersion;

		// ȡ��DllGetVersionͨ�ýӿں����ĵ�ַ
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");

		// ��IE3.0֮ǰ��DLL�п��ܲ����ڴ˽ӿں�����������Ҫ�ж�
		if(pDllGetVersion)
		{
			DLLVERSIONINFO	dvi;
			HRESULT			hr;

			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);

			// ��ȡDLL��Ϣ
			hr = (*pDllGetVersion)(&dvi);

			if(SUCCEEDED(hr))
			{
				// �����汾�����Ӱ汾��ѹ����һ��DWORD������
				dwVersion = PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
			}
		}

		// �ͷ�DLL
		FreeLibrary(hinstDll);
	}

	// ���ذ汾�ţ����ʧ�ܷ���0
	return dwVersion;
}


BOOL CIseetsvcApp::OnIdle(LONG lCount) 
{
	// ���ȴ���MFC�ڲ��Ŀ��д���
	BOOL bRet = CWinApp::OnIdle(lCount);

	CMainFrame	*pmf = (CMainFrame*)AfxGetMainWnd();

	// ���ĵ��༰������ṩ���д���Ļ���
	if (pmf&&pmf->GetSafeHwnd())
	{
		bRet |= pmf->OnIdleProc(lCount);

		CIseetsvcDoc *pid = (CIseetsvcDoc*)pmf->GetActiveDocument();

		if (pid)
		{
			bRet |= pid->OnIdleProc(lCount);
		}
	}
	
	return bRet;
}

BOOL CIseetsvcApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}
