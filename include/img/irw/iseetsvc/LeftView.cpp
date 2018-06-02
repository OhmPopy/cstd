/********************************************************************

	LeftView.cpp

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
	���ļ���;��	ͼ���д���棭C���԰棭VC�������������������
					��ʵ���ļ���

	���ļ���д�ˣ�	
					YZ				yzfree##sina.com

	���ļ��汾��	30218
	����޸��ڣ�	2003-2-18

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

#include "iseetsvcDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteitem)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	ON_MESSAGE(WM_TVN_DEFERFILL, OnDeferFill)
	ON_MESSAGE(WM_TVN_SELCHANGED, OnSelectChange)
	ON_MESSAGE(WM_TVN_UPDATE, OnUpdateItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	m_pTreeCtrl = 0;
}


CLeftView::~CLeftView()
{
}


BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT;

	return CTreeView::PreCreateWindow(cs);
}



/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CIseetsvcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}


void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
}



/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}


void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}


CIseetsvcDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIseetsvcDoc)));
	return (CIseetsvcDoc*)m_pDocument;
}
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ��ʼ�����ؼ���ַ
	m_pTreeCtrl = (CShellTree*)	this;

	return 0;
}


// ����չ��Ӧ
void CLeftView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	m_pTreeCtrl->OnFolderExpanding(pNMHDR, pResult);
	
	*pResult = 0;
}


// ��ɾ����Ӧ
void CLeftView::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	m_pTreeCtrl->OnDeleteShellItem(pNMHDR, pResult);

	*pResult = 0;
}


// ���ҵ����Ӧ
void CLeftView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pTreeCtrl->GetContextMenu(pNMHDR, pResult);
	
	*pResult = 0;
}


// ��ѡ������Ӧ
void CLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString	path;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	m_pTreeCtrl->OnFolderSelected(pNMHDR, pResult, path);

	*pResult = 0;
}


void CLeftView::OnDestroy() 
{
	CTreeView::OnDestroy();
}


// ����չ�����Ӧ
void CLeftView::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// �������ӵı߽����
	((CSplitterWnd*)GetParent())->RecalcLayout();

	*pResult = 0;
}


// �ӳ������Ӧ���ɿؼ����ӳ�����߳�������
LRESULT CLeftView::OnDeferFill(WPARAM wParam, LPARAM lParam)
{
	return m_pTreeCtrl->OnDeferFillAtt(wParam, lParam);
}


// ѡ������Ӧ�������ؼ����ͣ���ϵͳ���͵�ѡ������Ϣ��ͬ��
LRESULT CLeftView::OnSelectChange(WPARAM wParam, LPARAM lParam)
{
	TCHAR	szBuff[MAX_PATH];
	
	lstrcpy((LPSTR)szBuff, "");
	
	// ���ó������
	if (m_pTreeCtrl->GetPathFromHTREEITEM((HTREEITEM)wParam, (LPTSTR)szBuff) == 0)
	{
		// ������ļ�ϵͳ�ģ�����ʾ·��
		GetDocument()->SetTitle((LPCTSTR)szBuff);
	}
	else
	{
		// ������ʾ�������ʾ��
		GetDocument()->SetTitle((LPCTSTR)m_pTreeCtrl->GetItemText((HTREEITEM)wParam));
	}
	
	return 0;
}


// �����汻ˢ��֮����ã��Ը����б�����
LRESULT CLeftView::OnUpdateItem(WPARAM wParam, LPARAM lParam)
{
	return m_pTreeCtrl->OnUpdateItem(wParam, lParam);
}

