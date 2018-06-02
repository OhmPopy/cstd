/********************************************************************

	iseetsvcDoc.cpp

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
	���ļ���;��	ͼ���д���棭C���԰棭VC����������������ĵ�����
					��ʵ���ļ���

	���ļ���д�ˣ�	
					YZ				yzfree##sina.com

	���ļ��汾��	30118
	����޸��ڣ�	2003-1-18

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������

	������ʷ��

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
#include "FileDialogEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDisplay		*CIseetsvcDoc::m_pView = 0;				// ����
HWND			CIseetsvcDoc::m_hView  = 0;				// �Ӵ��ھ��



/////////////////////////////////////////////////////////////////////////////
// CIseetsvcDoc

IMPLEMENT_DYNCREATE(CIseetsvcDoc, CDocument)

BEGIN_MESSAGE_MAP(CIseetsvcDoc, CDocument)
	//{{AFX_MSG_MAP(CIseetsvcDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CIseetsvcDoc construction/destruction

CIseetsvcDoc::CIseetsvcDoc()
{
	m_pPack = 0;
	m_bPkst = FALSE;
	memset(&m_dInfo, 0, sizeof(BITMAPINFO_EX));
}


CIseetsvcDoc::~CIseetsvcDoc()
{
	if (m_pPack)
	{
		isirw_free_SPEC_DATAPACK(m_pPack);
	}
}


BOOL CIseetsvcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// ��ȡ��һ�����б����ʱ����·��
	CString	csInitPath = AfxGetApp()->GetProfileString(ITSVCMAIN_RKEYSEC, ITSVCDOC_RKEY_LASTPATH, "");

	// ��װ�����б�ؼ�
	m_spath.SetInitPath((LPCSTR)csInitPath);
	
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CIseetsvcDoc serialization

void CIseetsvcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}



/////////////////////////////////////////////////////////////////////////////
// CIseetsvcDoc diagnostics

#ifdef _DEBUG
void CIseetsvcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIseetsvcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIseetsvcDoc commands

void CIseetsvcDoc::OnCloseDocument() 
{
	// ���汾�������ڵ����·��
	AfxGetApp()->WriteProfileString(ITSVCMAIN_RKEYSEC, ITSVCDOC_RKEY_LASTPATH, (LPCTSTR)m_spath.GetCurrPath());

	CDocument::OnCloseDocument();
}


int __cdecl CIseetsvcDoc::_cbfun(LPSPEC_DATAPACK psdp, PFUN_COMM comm, int imgnum, int sl, int lcnt, int rw)
{
	static int	img = 0;								// ��ǰ�����ͼ��֡�ţ�����0...��

	// �ص�������ֻ���������֪ͨ
	if (rw == 1)
	{
		return 0;
	}

	// ���෢��֪ͨ
	switch (comm)
	{
	case	PFC_INTO_PLUG:					/* ��ʼִ������ */
		::PostMessage(m_hView, WM_DISP_INTOPLUG, (WPARAM)psdp, (LPARAM)0);
		break;
	case	PFC_BUILD_MAST_IMAGE:			/* ������ͼ�� */
		img = imgnum;
		::PostMessage(m_hView, WM_DISP_BUILDMI, (WPARAM)psdp, (LPARAM)0);
		break;
	case	PFC_BUILD_SERT_IMAGE:			/* �����µ���ͼ�� */
		img = imgnum;
		::PostMessage(m_hView, WM_DISP_BUILDSI, (WPARAM)psdp, (LPARAM)imgnum);
		break;
	case	PFC_PROCESS:					/* ���� */
		::PostMessage(m_hView, WM_DISP_PROCESS, (WPARAM)psdp, (LPARAM)sl);
		break;
	case	PFC_OVER_SIR:					/* ����һ��ͼ��Ĳ�������ͼ�����ͼ��*/
		::PostMessage(m_hView, WM_DISP_OVER, (WPARAM)psdp, (LPARAM)img);
		break;
	case	PFC_OK:							/* ����ɹ�ִ�� */
		::PostMessage(m_hView, WM_DISP_OK, (WPARAM)psdp, (LPARAM)0);
		break;
	case	PFC_BREAK:						/* ����ж� */
		::PostMessage(m_hView, WM_DISP_FAILD, (WPARAM)psdp, (LPARAM)0);
		break;
	default:
		break;
	}

	return 0;
}


BOOL CIseetsvcDoc::OnIdleProc(LONG count)
{
	if (m_spath.a_bChangeTag)
	{
		if (m_spath.a_cTime.IsEnd() == TRUE)
		{
			m_spath.a_bChangeTag = FALSE;

			m_bPkst = FALSE;

			// ���پ�ͼ���
			if (m_pPack)
			{
				// �����ͼ���û�ж�ȡ��ϣ����ж����Ĳ���
				if (m_pPack->exec_state != IRWE_SDPSTATE_IDLE)
				{
					isirw_s_wait(m_pPack, -1);
				}

				// �жϵ�ǰ�Ķ������ţ�����Ƕ����ļ��Ļ���
				m_pView->m_cAniThread->PostThreadMessage(WM_DANI_DIABLE, 0, 0);

				// �ȴ������̵߳Ļ�Ӧ
				while (m_pView->m_bDiableMark == FALSE)
				{
					Sleep(0);
				}

				m_pView->m_bDiableMark = FALSE;
				m_pView->m_iCurrFr     = 0;

				// �ͷŰ�
				isirw_free_SPEC_DATAPACK(m_pPack);
				m_pPack = 0;
				
				// ������ʾλ�õ�����
				m_pView->ClearDispPosInfo();
			}
			
			if (!m_spath.a_cPath.IsEmpty())
			{
				// ��ȡ��ͼ��
				if (isirw_s_read_img_c(m_spath.a_eMedium, (LPCSTR)m_spath.a_cPath, m_spath.a_dParam1, m_spath.a_dParam2, m_spath.a_dSN, IRWE_CNVMARK_32, (void*)_cbfun, &m_pPack) != ISIRW_LASTERR_SUCCESS)
				{
					ASSERT(!m_pPack);
					m_pView->Invalidate();
					m_pView->UpdateWindow();
				}
			}
			else
			{
				CMainFrame		*cmf  = (CMainFrame*)AfxGetMainWnd();
				CString			stime;

				// ����״̬���е�ͼ����Ϣ��
				m_pView->m_sImgInfo.Format("0��0��0��0");
				
				if (cmf)
				{
					cmf->m_wndStatusBar.SetPaneText(cmf->m_wndStatusBar.CommandToIndex(IDS_IMGINFO), (LPCTSTR)m_pView->m_sImgInfo);
				}

				// ���¶�ʱ�䴰��
				stime.Format("0 ����");

				if (cmf)
				{
					cmf->m_wndStatusBar.SetPaneText(cmf->m_wndStatusBar.CommandToIndex(IDS_TIMECNT), (LPCTSTR)stime);
				}
					
				// �л�Ŀ¼ʱ��������
				m_pView->Invalidate();
				m_pView->UpdateWindow();
			}
			
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	return FALSE;
}



void CIseetsvcDoc::OnFileSaveAs() 
{
	CFileDialogEx	dlg(m_pPack);
	
	dlg.DoModal();
	
	return;
}

void CIseetsvcDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	if (m_bPkst)
	{
		pCmdUI->Enable();
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}
