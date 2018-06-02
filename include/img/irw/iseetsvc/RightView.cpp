/********************************************************************

	RightView.cpp

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
	���ļ���;��	ͼ���д���棭C���԰棭VC����������������б���
					��ʵ���ļ���

	���ļ���д�ˣ�	
					YZ				yzfree##sina.com

	���ļ��汾��	30222
	����޸��ڣ�	2003-2-22

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������

	������ʷ��

		2003-2		��һ�����԰淢��

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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CRightView

IMPLEMENT_DYNCREATE(CRightView, CListView)

CRightView::CRightView()
{
	m_bFrmMark  = FALSE;
	m_bFillMark = 2;			// �����ϱ�־
}

CRightView::~CRightView()
{
}


BEGIN_MESSAGE_MAP(CRightView, CListView)
	//{{AFX_MSG_MAP(CRightView)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_MESSAGE(WM_LVN_FILLBEGIN, OnFillBegin)
	ON_MESSAGE(WM_LVN_FILLEND, OnFillEnd)
	ON_MESSAGE(WM_LVN_AUTONEXT, OnAutoNext)
	ON_MESSAGE(WM_LVN_SUBITEMDATA, OnSubItemDataFill)
	ON_MESSAGE(WM_RV_CREATED, OnWndCreated)
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteitem)
	ON_NOTIFY_REFLECT(LVN_DELETEALLITEMS, OnDeleteallitems)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_ITEMACTIVATE, OnActive)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRightView construction/destruction

BOOL CRightView::PreCreateWindow(CREATESTRUCT& cs)
{
	UINT type = AfxGetApp()->GetProfileInt(ITSVCMAIN_RKEYSEC, ITSVCRIGHT_RKEY_LISTYPE, LVS_REPORT);

	cs.style |= LVS_SORTASCENDING | LVS_NOSORTHEADER | LVS_SHAREIMAGELISTS | LVS_SINGLESEL | LVS_AUTOARRANGE | type;

	return CListView::PreCreateWindow(cs);
}



/////////////////////////////////////////////////////////////////////////////
// CRightView drawing

void CRightView::OnDraw(CDC* pDC)
{
	CIseetsvcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CListCtrl& refCtrl = GetListCtrl();
}


void CRightView::OnInitialUpdate()
{
	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE);

	CListView::OnInitialUpdate();
}



/////////////////////////////////////////////////////////////////////////////
// CRightView diagnostics

#ifdef _DEBUG
void CRightView::AssertValid() const
{
	CListView::AssertValid();
}


void CRightView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}


CIseetsvcDoc* CRightView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIseetsvcDoc)));
	return (CIseetsvcDoc*)m_pDocument;
}
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CRightView message handlers

void CRightView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	GetListCtrl().Arrange(LVA_DEFAULT);
}


void CRightView::OnDestroy() 
{
	DWORD dwStyle = GetStyle() & LVS_TYPEMASK;

	AfxGetApp()->WriteProfileInt(ITSVCMAIN_RKEYSEC, ITSVCRIGHT_RKEY_LISTYPE, dwStyle);

	CListView::OnDestroy();
}


int CRightView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ��ʼ���б�ؼ���ַ
	m_pListCtrl = (CShellList*)this;
	
	return 0;
}


// ��ʼһ���µ������̣������ؼ�������
LRESULT CRightView::OnFillBegin(WPARAM wParam, LPARAM lParam)
{
	// ��������ˢ�£�������˸
	LockWindowUpdate();

	m_bFillMark = 0;

	// ����б�ؼ�
	m_pListCtrl->OnFillBegin(wParam, lParam);

	return 0;
}


// ����һ�������̣������ؼ�������
LRESULT CRightView::OnFillEnd(WPARAM wParam, LPARAM lParam)
{
	// ֪ͨ�б�ؼ��������
	m_pListCtrl->OnFillEnd(wParam, lParam);

	// �ļ���������
	m_bFillMark = 1;

	// �ָ�����ˢ��
	UnlockWindowUpdate();

	return 0;
}


// �Զ�ѡ����һ��ͼ���Զ�����ʱ���ã�
LRESULT CRightView::OnAutoNext(WPARAM wParam, LPARAM lParam)
{
	return m_pListCtrl->OnAutoNext(wParam, lParam);
}


// ����б������ϸ���ݣ�������߳�������
LRESULT CRightView::OnSubItemDataFill(WPARAM wParam, LPARAM lParam)
{
	return m_pListCtrl->OnSubItemDataFill(wParam, lParam);
}


void CRightView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);

	int		item = -1;

	// ʹѡ����ֿɼ�
	if ((item=GetListCtrl().GetNextItem(item, LVNI_SELECTED)) != -1)
	{
		GetListCtrl().EnsureVisible(item, 1);
	}
	
	// ����״̬������Ϣ����ĳߴ�
	if (m_bFrmMark)
	{
		CStatusBar	*cbr = &(((CMainFrame*)AfxGetMainWnd())->m_wndStatusBar);
		UINT		idx = cbr->CommandToIndex(IDS_PROGRESS);

		WINDOWPLACEMENT	wpt;

		wpt.length  = sizeof(WINDOWPLACEMENT);
		wpt.showCmd	= 0;

		if (!AfxGetMainWnd()->GetWindowPlacement(&wpt))
		{
			return;
		}

		if (wpt.showCmd == SW_SHOWMAXIMIZED)
		{
			cx = ((cx-2) < 0) ? 0 : cx-2;
		}
		else
		{
			cx = ((cx-GetSystemMetrics(SM_CXHSCROLL)-4) < 0) ? 0 : cx-GetSystemMetrics(SM_CXHSCROLL)-4;
		}

		if (GetStyle()&WS_VSCROLL)
		{
			cx += GetSystemMetrics(SM_CXVSCROLL);
		}

		cbr->SetPaneInfo(idx, IDS_PROGRESS, cbr->GetPaneStyle(idx), cx);
	}

	return;
}


// ����ܴ��ڴ��������еĴ��ں󣬽�����WM_RV_CREATED��Ϣ������������Ӧ����Ϣ
LRESULT CRightView::OnWndCreated(WPARAM wParam, LPARAM lParam)
{
	m_bFrmMark = TRUE;

	return 0;
}


void CRightView::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pListCtrl->OnDeleteitem(pNMHDR, pResult);
}


void CRightView::OnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pListCtrl->OnDeleteallitems(pNMHDR, pResult);
}


void CRightView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMITEMACTIVATE	lpnmitem = (LPNMITEMACTIVATE)pNMHDR;

	int	item = m_pListCtrl->GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);

	


	*pResult = 0;
}


void CRightView::OnActive(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
}



void CRightView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW	pnmv = (LPNMLISTVIEW)pNMHDR;

	if ((pnmv->iItem != -1)&&(pnmv->uChanged == LVIF_STATE)&&(pnmv->uNewState&LVIS_SELECTED))
	{
		char	buf[MAX_PATH];

		m_pListCtrl->GetItemText(pnmv->iItem, 0, (LPTSTR)buf, MAX_PATH);

		m_pListCtrl->m_pPath->ObjectChanged((LPCTSTR)0, (LPCTSTR)buf, pnmv->iItem);

		ISeeTsVCDebugStrRecorder().Record("ShellList ��ǰѡ���ļ�Ϊ <%s>��", 1, (LPCTSTR)buf);
	}
	
	*pResult = 0;
}


