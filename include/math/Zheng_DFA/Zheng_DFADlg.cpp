// Zheng_DFADlg.cpp : implementation file
//

#include "stdafx.h"
#include "Zheng_DFA.h"
#include "Zheng_DFADlg.h"
#include "ExampleDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

  // Dialog Data
  enum { IDD = IDD_ABOUTBOX };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  // Implementation
protected:
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CZheng_DFADlg dialog




CZheng_DFADlg::CZheng_DFADlg(CWnd* pParent /*=NULL*/)
  : CDialog(CZheng_DFADlg::IDD, pParent)
  , m_Input_expr(_T(""))
  , m_Postfix_expr(_T(""))
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZheng_DFADlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIT1, m_Input_expr);
  DDX_Text(pDX, IDC_EDIT2, m_Postfix_expr);
}

BEGIN_MESSAGE_MAP(CZheng_DFADlg, CDialog)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  //}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_BUTTON1, CZheng_DFADlg::OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON2, CZheng_DFADlg::OnBnClickedButton2)
  ON_COMMAND(ID_32771, CZheng_DFADlg::OnShowExample)
END_MESSAGE_MAP()


// CZheng_DFADlg message handlers

BOOL CZheng_DFADlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);

  if (pSysMenu != NULL) {
    CString strAboutMenu;
    strAboutMenu.LoadString(IDS_ABOUTBOX);

    if (!strAboutMenu.IsEmpty()) {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);     // Set big icon
  SetIcon(m_hIcon, FALSE);    // Set small icon

  // TODO: Add extra initialization here

  return TRUE;  // return TRUE  unless you set the focus to a control
}

void CZheng_DFADlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  }
  else {
    CDialog::OnSysCommand(nID, lParam);
  }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CZheng_DFADlg::OnPaint()
{
  if (IsIconic()) {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
  else {
    CDialog::OnPaint();
  }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CZheng_DFADlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}

void CZheng_DFADlg::OnBnClickedButton1()   // ����  NFA  DFA  ��С��DFA
{
  // TODO: Add your control notification handler code here
  UpdateData(true);
  CString input_expr = m_Input_expr;                          // ����ı��ʽ
  CString output_expr = "";                                   // ��ʱ����
  GetDlgItem(IDC_EDIT3)->SetWindowTextA("");                  // ���
  GetDlgItem(IDC_EDIT4)->SetWindowTextA("");                  // ���
  GetDlgItem(IDC_EDIT5)->SetWindowTextA("");                  // ���
  RedrawWindow();
  dfa.ClearAll();                                             // �����������е�����

  if (dfa.IsLegal(input_expr)) {                              // �ж������Ƿ�Ϸ�
    CString Regular_expr = dfa.ToRegular_expr(input_expr);  // ת���ɿɼ����������ʽ
    dfa.SetExpr(Regular_expr);                              // ���������ַ�
    m_Postfix_expr = dfa.ToPostfix();                       // ת��Ϊ��׺�ַ�

    if (m_Postfix_expr != "ERROR") {                        // ת�����ж�
      dfa.Regular_exprToNFA(m_Postfix_expr);              // ��׺ʽת��ΪNFA

      CString show_NFA = "";
      CString show_DFA = "";
      CString show_MinimizeDFA = "";
      show_NFA = dfa.ShowNFA();                           // ��ʾNFA
      GetDlgItem(IDC_EDIT3)->SetWindowTextA(show_NFA);    // ������ʾ

      dfa.NFA_To_DFA();                                   // NFA ת��Ϊ DFA
      show_DFA = dfa.ShowDFA();                           // ��ʾ DFA
      GetDlgItem(IDC_EDIT4)->SetWindowTextA(show_DFA);    // ������ʾ

      dfa.MinimizeDFA();                                  // ��С��DFA
      dfa.CombinSameState();                              // �ϲ���ͬ״̬
      show_MinimizeDFA = dfa.ShowMinimizeDFA();           // ��ʾ ��С��DFA
      GetDlgItem(IDC_EDIT5)->SetWindowTextA(show_MinimizeDFA);
    }
    else {
      AfxMessageBox("���ʽERROR�����飡");  // ������Ϣ
    }
  }
  else {
    AfxMessageBox("Error Input Expr!");                      // ������Ϣ
  }

  UpdateData(false);                                          // ����
}

void CZheng_DFADlg::OnBnClickedButton2()   //  �����ַ��� �Ƿ�ɽ���
{
  // TODO: Add your control notification handler code here
  UpdateData(true);

  if (dfa.GetExpr().GetLength() != 0) {
    CString testStr = "";
    GetDlgItem(IDC_EDIT6)->GetWindowTextA(testStr);

    if (dfa.TestInput_MinimizeNFA(testStr)) {
      AfxMessageBox("���ʽ ƥ�䣡");  // �ɽ���
    }
    else {
      AfxMessageBox("���ʽ ��ƥ�䣡");  // ���ɽ���
    }
  }
  else {
    AfxMessageBox("û������������ʽ��");  // û������������ʽ
  }
}

void CZheng_DFADlg::OnShowExample()
{
  // TODO: Add your command handler code here
  ExampleDlg exa;
  exa.DoModal();
}
