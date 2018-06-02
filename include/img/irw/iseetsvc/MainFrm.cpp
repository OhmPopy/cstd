/********************************************************************

	MainFrm.cpp

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
	���ļ���;��	ͼ���д���棭C���԰棭VC�����������������ܴ���
					��ʵ���ļ���

	���ļ���д�ˣ�	
					YZ				yzfree##sina.com

	���ļ��汾��	30603
	����޸��ڣ�	2003-6-3

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������

	������ʷ��

		2003-6		������������޷���Win98ϵͳ�����е�BUG
					����BUG��ע�����ʺ����ڲ��ڲ�ͬϵͳ���в�ͬ����������ģ�
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

#include "IseetsvcDoc.h"
#include "MainFrm.h"
#include "PluginSet.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VAT_CENTER, OnVatCenter)
	ON_UPDATE_COMMAND_UI(ID_VAT_CENTER, OnUpdateVatCenter)
	ON_COMMAND(ID_VAT_LD, OnVatLd)
	ON_UPDATE_COMMAND_UI(ID_VAT_LD, OnUpdateVatLd)
	ON_COMMAND(ID_VAT_LT, OnVatLt)
	ON_UPDATE_COMMAND_UI(ID_VAT_LT, OnUpdateVatLt)
	ON_COMMAND(ID_VAT_RD, OnVatRd)
	ON_UPDATE_COMMAND_UI(ID_VAT_RD, OnUpdateVatRd)
	ON_COMMAND(ID_VAT_RT, OnVatRt)
	ON_UPDATE_COMMAND_UI(ID_VAT_RT, OnUpdateVatRt)
	ON_COMMAND(ID_VIEW_PLAYTYPE, OnViewPlaytype)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PLAYTYPE, OnUpdateViewPlaytype)
	ON_COMMAND(ID_VRT_AUTO, OnVrtAuto)
	ON_UPDATE_COMMAND_UI(ID_VRT_AUTO, OnUpdateVrtAuto)
	ON_COMMAND(ID_VRT_L200, OnVrtL200)
	ON_UPDATE_COMMAND_UI(ID_VRT_L200, OnUpdateVrtL200)
	ON_COMMAND(ID_VRT_L300, OnVrtL300)
	ON_UPDATE_COMMAND_UI(ID_VRT_L300, OnUpdateVrtL300)
	ON_COMMAND(ID_VRT_L400, OnVrtL400)
	ON_UPDATE_COMMAND_UI(ID_VRT_L400, OnUpdateVrtL400)
	ON_COMMAND(ID_VRT_ORG, OnVrtOrg)
	ON_UPDATE_COMMAND_UI(ID_VRT_ORG, OnUpdateVrtOrg)
	ON_COMMAND(ID_VRT_S20, OnVrtS20)
	ON_UPDATE_COMMAND_UI(ID_VRT_S20, OnUpdateVrtS20)
	ON_COMMAND(ID_VRT_S40, OnVrtS40)
	ON_UPDATE_COMMAND_UI(ID_VRT_S40, OnUpdateVrtS40)
	ON_COMMAND(ID_VRT_S60, OnVrtS60)
	ON_UPDATE_COMMAND_UI(ID_VRT_S60, OnUpdateVrtS60)
	ON_COMMAND(ID_VRT_S80, OnVrtS80)
	ON_UPDATE_COMMAND_UI(ID_VRT_S80, OnUpdateVrtS80)
	ON_COMMAND(ID_VIEW_PLUGININFO, OnViewPlugininfo)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PLUGININFO, OnUpdateViewPlugininfo)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	IDS_IMGINFO,
	IDS_TIMECNT,
	IDS_PROGRESS, 
};



/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}


CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	WINDOWPLACEMENT	*wp;
	UINT			swp;
	
	// ��ȡ��һ�δ��ڵ�λ����Ϣ
	AfxGetApp()->GetProfileBinary(ITSVCMAIN_RKEYSEC, ITSVCMAIN_RKEY_WINPOS, (LPBYTE*)&wp, &swp);
	
	// ���ô���λ��
	if (swp == sizeof(WINDOWPLACEMENT))
	{
		SetWindowPlacement(wp);
		delete []wp;
	}

	ISeeTsVCDebugStrRecorder().Record("����ܴ��ڴ�����ϣ����潫����״̬����", 0, 0);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;
	}

	ISeeTsVCDebugStrRecorder().Record("״̬��������ϡ�", 0, 0);

	UINT	idx = m_wndStatusBar.CommandToIndex(ID_SEPARATOR);

	// ������ʾ����Ϊ�������
	m_wndStatusBar.SetPaneStyle(idx, m_wndStatusBar.GetPaneStyle(idx)|SBPS_STRETCH);

	// ����ʱ����ʾ������
	idx = m_wndStatusBar.CommandToIndex(IDS_TIMECNT);
	m_wndStatusBar.SetPaneInfo(idx, IDS_TIMECNT, m_wndStatusBar.GetPaneStyle(idx), 50);

	// ����ͼ����Ϣ������
	idx = m_wndStatusBar.CommandToIndex(IDS_IMGINFO);
	m_wndStatusBar.SetPaneInfo(idx, IDS_IMGINFO, m_wndStatusBar.GetPaneStyle(idx), 120);

	EnableDocking(CBRS_ALIGN_ANY);

	// ֪ͨ�б��ӣ����д����Ѵ������
	GetRightPane()->SendMessage(WM_RV_CREATED);

	return 0;
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	ISeeTsVCDebugStrRecorder().Record("��ʼ�������Ѵ��ڼ����ӡ��б��ӡ���ͼ�ӡ�", 0, 0);

	// ��ʾ��ӭ����
	CSplashWnd::ShowSplashScreen(this);
	
	// ����A���Ѵ���
	if (!m_spa.CreateStatic(this, 1, 2))
	{
		return FALSE;
	}

	LPRECT	rc  = 0;
	UINT	src = 0;
	
	// �ָ���һ������ʱ���ӡ��б��Ӵ��ڵĳߴ�
	if (!AfxGetApp()->GetProfileBinary(ITSVCMAIN_RKEYSEC, ITSVCMAIN_RKEY_LEFTWINPOS, (LPBYTE*)&rc, &src))
	{
		rc  = new CRect(0,0,200,400);
		src = sizeof(CRect);
	}

	// ��������
	if (!m_spa.CreateView(0, 0, pContext->m_pNewViewClass, CSize(rc->right-rc->left, rc->bottom-rc->top), pContext))
	{
		if (rc)
		{
			delete rc;
		}

		m_spa.DestroyWindow();
		return FALSE;
	}
	
	if (rc)
	{
		delete rc;
	}

	// ����B���Ѵ���
	if (!m_spb.CreateStatic(&m_spa, 1, 2, WS_CHILD|WS_VISIBLE|WS_BORDER, m_spa.IdFromRowCol(0,1)))
	{
		m_spa.DestroyWindow();
		return FALSE;
	}

	if (!AfxGetApp()->GetProfileBinary(ITSVCMAIN_RKEYSEC, ITSVCMAIN_RKEY_DISPWINPOS, (LPBYTE*)&rc, &src))
	{
		rc  = new CRect(0,0,200,400);
		src = sizeof(CRect);
	}
	
	// ����ͼ�Ӻ��б���
	if ((!m_spb.CreateView(0, 0, RUNTIME_CLASS(CDisplay), CSize(rc->right-rc->left, rc->bottom-rc->top), pContext))||
		(!m_spb.CreateView(0, 1, RUNTIME_CLASS(CRightView), CSize(0, 0), pContext)))
	{
		if (rc)
		{
			delete rc;
		}

		m_spb.DestroyWindow();
		m_spa.DestroyWindow();
		return FALSE;
	}
	
	if (rc)
	{
		delete rc;
	}

	ISeeTsVCDebugStrRecorder().Record("���ӡ��б��ӡ�ͼ�Ӵ�����ϡ�", 0, 0);

	// ��������Ϊ��ǰ����
	m_spa.SetActivePane(0, 0);

	CIseetsvcDoc *pid = (CIseetsvcDoc*)GetActiveDocument();

	// ��ʼ��������ָ��
	pid->m_pView = (CDisplay*)m_spb.GetPane(0,0);
	pid->m_hView = pid->m_pView->GetSafeHwnd();

	ISeeTsVCDebugStrRecorder().Record("��ʼ��װ����...", 0, 0);
	// ��ʼ��ShellPath����
	pid->m_spath.InitShellPath((CTreeCtrl*)&(((CTreeView*)m_spa.GetPane(0,0))->GetTreeCtrl()),
		(CListCtrl*)&(((CListView*)m_spb.GetPane(0,1))->GetListCtrl()),
		(CFrameWnd*)this,
		CString((LPCTSTR)isirw_get_shellstr()));
	ISeeTsVCDebugStrRecorder().Record("������װ��ϡ�", 0, 0);

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

// ��ȡ�б���ָ��
CRightView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_spb.GetPane(0, 1);
	CRightView* pView = DYNAMIC_DOWNCAST(CRightView, pWnd);
	return pView;
}


// ��ȡ����ָ��
CLeftView* CMainFrame::GetLeftPane()
{
	CWnd* pWnd = m_spa.GetPane(0, 0);
	CLeftView* pView = DYNAMIC_DOWNCAST(CLeftView, pWnd);
	return pView;
}


// ��ȡ��ʾ��ָ��
CDisplay* CMainFrame::GetDisplayPane()
{
	CWnd* pWnd = m_spb.GetPane(0, 0);
	CDisplay* pView = DYNAMIC_DOWNCAST(CDisplay, pWnd);
	return pView;
}


// �б��ӷ��ѡ�񹤾�
void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.

	CRightView* pView = GetRightPane(); 

	// if the right-hand pane hasn't been created or isn't a view,
	// disable commands in our range

	if (pView == NULL)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// if the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// otherwise, use dots to reflect the style of the view
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetCheck(bChecked ? 1 : 0);
		}
	}
}


// �޸��б��ӷ��
void CMainFrame::OnViewStyle(UINT nCommandID)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.
	CRightView* pView = GetRightPane();

	// if the right-hand pane has been created and is a CRightView,
	// process the menu commands...
	if (pView != NULL)
	{
		DWORD dwStyle = -1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// ask the list control to snap to grid
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT;
			break;
		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON;
			break;
		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON;
			break;
		case ID_VIEW_LIST:
			dwStyle = LVS_LIST;
			break;
		}

		// change the style; window will repaint automatically
		if (dwStyle != -1)
		{
			switch (dwStyle)
			{
			case LVS_LIST:
				ISeeTsVCDebugStrRecorder().Record("�޸��б��ӷ��Ϊ <�б�> ���", 0, 0);
				break;
			case LVS_ICON:
				ISeeTsVCDebugStrRecorder().Record("�޸��б��ӷ��Ϊ <��ͼ��> ���", 0, 0);
				break;
			case LVS_SMALLICON:
				ISeeTsVCDebugStrRecorder().Record("�޸��б��ӷ��Ϊ <Сͼ��> ���", 0, 0);
				break;
			case ID_VIEW_DETAILS:
				ISeeTsVCDebugStrRecorder().Record("�޸��б��ӷ��Ϊ <��ϸ�б�> ���", 0, 0);
				break;
			default:
				ISeeTsVCDebugStrRecorder().Record("������δ֪���б��ӷ��ָ����", 0, 0);
			}

			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
		}
	}
}


void CMainFrame::OnClose() 
{
	CIseetsvcDoc	*pid = (CIseetsvcDoc*)GetActiveDocument();

	// �ر�ShellPath����
	pid->m_spath.CloseShellPath();

	ISeeTsVCDebugStrRecorder().Record("�رտ�ܴ��ڡ�", 0, 0);

	CWnd		*pWnd = m_spa.GetPane(0, 0);
	CLeftView	*pLView = DYNAMIC_DOWNCAST(CLeftView, pWnd);
	RECT		rc;

	// �������ӡ��б��ӡ���ʾ���ڳߴ�
	pLView->GetWindowRect(&rc);
	AfxGetApp()->WriteProfileBinary(ITSVCMAIN_RKEYSEC, ITSVCMAIN_RKEY_LEFTWINPOS, (LPBYTE)&rc, sizeof(RECT));

	pWnd = m_spb.GetPane(0, 1);
	CRightView* pRView = DYNAMIC_DOWNCAST(CRightView, pWnd);

	pRView->GetWindowRect(&rc);
	AfxGetApp()->WriteProfileBinary(ITSVCMAIN_RKEYSEC, ITSVCMAIN_RKEY_RIGHTWINPOS, (LPBYTE)&rc, sizeof(RECT));

	pWnd = m_spb.GetPane(0, 0);
	CDisplay* pDView = DYNAMIC_DOWNCAST(CDisplay, pWnd);

	pDView->GetWindowRect(&rc);
	AfxGetApp()->WriteProfileBinary(ITSVCMAIN_RKEYSEC, ITSVCMAIN_RKEY_DISPWINPOS, (LPBYTE)&rc, sizeof(RECT));
	

	WINDOWPLACEMENT	wp;
	GetWindowPlacement(&wp);

	// ���浱ǰ���ڵ�λ����Ϣ
	AfxGetApp()->WriteProfileBinary(ITSVCMAIN_RKEYSEC, ITSVCMAIN_RKEY_WINPOS, (LPBYTE)&wp, sizeof(WINDOWPLACEMENT));

	CFrameWnd::OnClose();
}


BOOL CMainFrame::OnIdleProc(LONG count)
{
	return FALSE;
}


void CMainFrame::OnVatCenter() 
{
	CDisplay *pDisp = GetDisplayPane();
	pDisp->m_iInitPos = CDisplay::DPT_CENTER;
}

void CMainFrame::OnUpdateVatCenter(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();

	if (pDisp->m_iInitPos != CDisplay::DPT_CENTER)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

void CMainFrame::OnVatLd() 
{
	CDisplay *pDisp = GetDisplayPane();
	pDisp->m_iInitPos = CDisplay::DPT_LEFTBOTTOM;
}

void CMainFrame::OnUpdateVatLd(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if (pDisp->m_iInitPos != CDisplay::DPT_LEFTBOTTOM)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

void CMainFrame::OnVatLt() 
{
	CDisplay *pDisp = GetDisplayPane();
	pDisp->m_iInitPos = CDisplay::DPT_LEFTTOP;
}

void CMainFrame::OnUpdateVatLt(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if (pDisp->m_iInitPos != CDisplay::DPT_LEFTTOP)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

void CMainFrame::OnVatRd() 
{
	CDisplay *pDisp = GetDisplayPane();
	pDisp->m_iInitPos = CDisplay::DPT_RIGHTBOTTOM;
}

void CMainFrame::OnUpdateVatRd(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if (pDisp->m_iInitPos != CDisplay::DPT_RIGHTBOTTOM)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

void CMainFrame::OnVatRt() 
{
	CDisplay *pDisp = GetDisplayPane();
	pDisp->m_iInitPos = CDisplay::DPT_RIGHTTOP;
}

void CMainFrame::OnUpdateVatRt(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if (pDisp->m_iInitPos != CDisplay::DPT_RIGHTTOP)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

void CMainFrame::OnViewPlaytype() 
{
	CShellList* pList = (CShellList*)GetRightPane();
	
	pList->SetAutoPlay((pList->GetAutoPlay()) ? 0 : 1);
}

void CMainFrame::OnUpdateViewPlaytype(CCmdUI* pCmdUI) 
{
	CShellList* pList = (CShellList*)GetRightPane();
	
	if (pList->GetAutoPlay())
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtAuto() 
{
	CDisplay *pDisp = GetDisplayPane();

	pDisp->m_iTalAutResize = TRUE;

	pDisp->SetDispLocInfo();
	
	// �������ű���
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// ����ͼ�����ʾλ��
	pDisp->SetFrmeLocInfo();
	
	// ˢ����ʾ
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtAuto(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();

	if (pDisp->m_iTalAutResize)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtL200() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)2.0;
	
	pDisp->SetDispLocInfo();
	
	// �������ű���
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// ����ͼ�����ʾλ��
	pDisp->SetFrmeLocInfo();
	
	// ˢ����ʾ
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtL200(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if ((pDisp->m_fTalRatio == (float)2.0)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtL300() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)3.0;
	
	pDisp->SetDispLocInfo();
	
	// �������ű���
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// ����ͼ�����ʾλ��
	pDisp->SetFrmeLocInfo();
	
	// ˢ����ʾ
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtL300(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();

	if ((pDisp->m_fTalRatio == (float)3.0)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtL400() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)4.0;
	
	pDisp->SetDispLocInfo();
	
	// �������ű���
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// ����ͼ�����ʾλ��
	pDisp->SetFrmeLocInfo();
	
	// ˢ����ʾ
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtL400(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if ((pDisp->m_fTalRatio == (float)4.0)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtOrg() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)1.0;
	
	pDisp->SetDispLocInfo();
	
	// �������ű���
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// ����ͼ�����ʾλ��
	pDisp->SetFrmeLocInfo();
	
	// ˢ����ʾ
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtOrg(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if ((pDisp->m_fTalRatio == (float)1.0)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtS20() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)0.2;
	
	pDisp->SetDispLocInfo();
	
	// �������ű���
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// ����ͼ�����ʾλ��
	pDisp->SetFrmeLocInfo();
	
	// ˢ����ʾ
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtS20(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if ((pDisp->m_fTalRatio == (float)0.2)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtS40() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)0.4;
	
	pDisp->SetDispLocInfo();
	
	// �������ű���
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// ����ͼ�����ʾλ��
	pDisp->SetFrmeLocInfo();
	
	// ˢ����ʾ
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtS40(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if ((pDisp->m_fTalRatio == (float)0.4)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtS60() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)0.6;
	
	pDisp->SetDispLocInfo();
	
	// �������ű���
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// ����ͼ�����ʾλ��
	pDisp->SetFrmeLocInfo();
	
	// ˢ����ʾ
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtS60(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if ((pDisp->m_fTalRatio == (float)0.6)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtS80() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)0.8;
	
	pDisp->SetDispLocInfo();
	
	// �������ű���
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// ����ͼ�����ʾλ��
	pDisp->SetFrmeLocInfo();
	
	// ˢ����ʾ
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtS80(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if ((pDisp->m_fTalRatio == (float)0.8)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnViewPlugininfo() 
{
	CPluginSet		dlg;
	LPIRWMF_HEADER	pib;

	// �ȴ������������
	while (isirw_is_busy())
	{
		::Sleep(0);
	}

	// ����ͼ���д����
	pib = isirw_lock();

	if (!pib)
	{
		AfxMessageBox("ISeeͼ���д�����޷�������������ڲ��ȶ�״̬������رձ������");
		return;
	}

	// ���öԻ���Ĳ����Ϣ���ַ
	dlg.SetData(pib);

	// �����öԻ���
	if (dlg.DoModal() == IDOK)
	{
		// ����û������˲�����������޸Ĳ����Ϣ��
		for (int i=0; i<dlg.plicnt; i++)
		{
			if (dlg.plda[i].inst != dlg.plda[i].iost)
			{
				if (dlg.plda[i].inst == CPluginSet::PLINSTATUS_NOUSE)
				{
					isirw_dismissal(dlg.plda[i].id);	// ��Ͳ��
				}
				else
				{
					isirw_employ(dlg.plda[i].id);		// ¼�ò��
				}
			}
		}

		CIseetsvcDoc	*pid = (CIseetsvcDoc*)GetActiveDocument();
		CString			filt((LPCTSTR)isirw_get_shellstr());

		// �����ļ����˴�
		pid->m_spath.SetFilterString(filt);
		
		// ˢ���б���
		GetLeftPane()->PostMessage(WM_TVN_UPDATE);
	}

	// �������
	isirw_unlock(pib);
}

void CMainFrame::OnUpdateViewPlugininfo(CCmdUI* pCmdUI) 
{
	if (GetRightPane()->m_bFillMark < 2)
	{
		pCmdUI->Enable(FALSE);							// �����Ŀ¼���ڼ䣬���ܴ򿪲�����öԻ���
	}
	else
	{
		pCmdUI->Enable();
	}
}

void CMainFrame::OnFileSaveAs() 
{
	return;
}

void CMainFrame::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	CIseetsvcDoc	*pid = (CIseetsvcDoc*)GetActiveDocument();
	
	if (pid->m_bPkst)
	{
		pCmdUI->Enable();
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}
