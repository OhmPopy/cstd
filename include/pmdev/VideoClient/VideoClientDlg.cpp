// VideoClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VideoClient.h"
#include "VideoClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoClientDlg dialog

CVideoClientDlg::CVideoClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVideoClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVideoClientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVideoClientDlg, CDialog)
	//{{AFX_MSG_MAP(CVideoClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoClientDlg message handlers

BOOL CVideoClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	HWND hWnd = GetDlgItem( IDC_VIDE )->m_hWnd;

	/*
	  ������������:
	  1����ʼ������ʼ�������Լ���Ƶͼ������ض�����
	  2�������豸������������TCP���ݵ��̣߳����ڽ��տ����豸���豸����Ӧ��Ϣ��
	  3����������Ӧ�������ֱ������ӡ�
	  4���ٷ���½�豸�����Ȩ����֤��
	  5������UDP������Ƶ���ݰ����׽��֣�����������Ƶ���ݰ����̣߳����豸��������Ƶ���ݵĿ������
      6�����������һ֡��Ƶ���ݹ���һ��Ϊ200��K����Ҫ�ְ����ͣ����շ���Ҫ��һ���滺�嵱����Ƶ֡�����ݣ������շ�Ҫ�����
	  7�������Ϻ󲥷ţ�����������Ƶͼ��������YUV��Ҫת����RGB��ת���㷨�����õ��ǲ��������1��Ҫ��ת�����ʼ����
	  8�����豸����ֹͣ������Ƶ���ݵ����
	  9���ر�TCP/UDP�׽����Լ���Ӧ���ݽ����̡߳�

      ע�����
	  1��������ɣ���ͷ+��������ɡ����԰�ͷ������У�顣
	  2���������ɣ�����ͷ+����������ɡ�
	  3�����������Ҫ�����ֽڶ��롣
	  4������һ��UDP�������ݻ��泤�ȱ���4�ֽڶ��룬�Ҳ�����Ҫ���յ����ݵĳ��ȡ�
	  5��Ҫ��ʼ��YUVתRGB�ı�
	  6��UDP�˿��������ȡ�Ŀ��ö˿ںš�
	*/

	//��ʼ��
	m_objVideoSDK.SDKInit(  hWnd, "192.168.1.121", "192.168.1.3", 13333 );
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVideoClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVideoClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVideoClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVideoClientDlg::OnOK() 
{
	// TODO: Add extra validation here

	//��������
	m_objVideoSDK.SDKConnect();
    
	//������Ƶ
	m_objVideoSDK.SDKStartVideo();

}

void CVideoClientDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	//�ر���Ƶ
	m_objVideoSDK.SDKStopVideo();

    //�Ͽ�����
	m_objVideoSDK.SDKDisConnect();

	::InvalidateRect(GetDlgItem(IDC_VIDE)->m_hWnd,NULL,TRUE);
}

void CVideoClientDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_objVideoSDK.SDKUInit();
	CDialog::OnClose();
}
