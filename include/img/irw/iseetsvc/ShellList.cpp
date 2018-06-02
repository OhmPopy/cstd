/********************************************************************

	ShellList.cpp

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
	���ļ���;��	ͼ���д���棭C���԰棭VC���������������ShellList
					�ؼ���ʵ���ļ���

	���ļ���д�ˣ�	
					YZ				yzfree##sina.com

	���ļ��汾��	30315
	����޸��ڣ�	2003-3-15

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������

	������ʷ��

		2003-3		������˽�����ݽṹ������
		2003-2		��һ�����԰淢��

********************************************************************/

#include "stdafx.h"
#include "iseetsvc.h"

#include "ShellPidl.h"
#include "ShellTree.h"
#include "ShellList.h"
#include "ShellPath.h"

#include "Display.h"
#include "LeftView.h"
#include "RightView.h"

#include "MainFrm.h"
#include "IseetsvcDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CShellList

CShellTree	*CShellList::m_pTree   = 0;
CShellPath	*CShellList::m_pPath   = 0;

CWinThread	*CShellList::m_pThread = 0;

#pragma pack(push)
#pragma pack(4)
int			CShellList::m_iCount   = 0;
int			CShellList::m_iSN      = 0;
int			CShellList::m_iCurrPt  = 0;
int			CShellList::m_iNum     = 0;
#pragma pack(pop)

CString		CShellList::m_sFilter  = "*.*";
CString		CShellList::m_sCurrPath;

int			CShellList::m_iAutoPlayTime = 4;
int			CShellList::m_iAutoPlay     = 1;



CShellList::SIBCOL	CShellList::sc[COLNUM] = 
{
	{COL_NAME, "����", LVCFMT_LEFT, 160},
	{COL_SIZE, "��С", LVCFMT_RIGHT, 50},
	{COL_TYPE, "����", LVCFMT_LEFT, 100},
	{COL_NOTE, "��ע", LVCFMT_LEFT, 120}
};


CShellList::CShellList()
{
}

CShellList::~CShellList()
{
}


BEGIN_MESSAGE_MAP(CShellList, CListCtrl)
	//{{AFX_MSG_MAP(CShellList)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteitem)
	ON_NOTIFY_REFLECT(LVN_DELETEALLITEMS, OnDeleteallitems)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CShellList message handlers

// �б�ؼ�����߳�
UINT CShellList::ShellLProc(LPVOID parm)
{
	CShellList		*pList = (CShellList*)parm;
	MSG				msg;
	MSG				msgtmp;
	BOOL			bRet;
	int				sn, count, i, apc;
	
	// ֪ͨ���̣߳����������̵߳���Ϣ����
	pList->PostMessage(WM_LVN_PROCCREATE);
	
	// ��Ϣѭ��
	while((bRet=::GetMessage(&msg, NULL, 0, 0)) != 0)
    { 
        if (bRet == -1)
        {
            continue;
        }
        else
        {
			if (msg.message == WM_LVN_QUIT)				// �˳���Ϣ
			{
				break;
			}
			else if (msg.message == WM_LVN_AUTOSEL)		// �Զ�������Ϣ
			{
				apc = msg.wParam;

				while (m_iAutoPlay)
				{
					// �ӳ�һ����ʱ��
					for (i=m_iAutoPlayTime*1000; i>0; i-=APTIMEUNIT)
					{
						// ������µ���������������ǰ�Ĳ��Ź���
						if (::PeekMessage(&msgtmp, NULL, WM_LVN_NEWCOMM, WM_LVN_QUIT, PM_NOREMOVE))
						{
							break;
						}
					
						// �ӳ�һ����Сʱ�䵥λ
						::Sleep(APTIMEUNIT);
					}

					if ((i <= 0)&&(m_iAutoPlay))
					{
						// ʱ�䵽�������̷߳���ѡ����һ��ͼ�����Ϣ
						pList->PostMessage(WM_LVN_AUTONEXT, (WPARAM)apc, (LPARAM)0);
					}
					else
					{
						// ���µġ�ָʾ����������һ����Ϣ:)
						break;
					}
				}
			}
			else if (msg.message == WM_LVN_NEWCOMM)		// ��������Ϣ
			{
				// ��ȡ�������
				sn    = msg.wParam;
				count = msg.lParam;
				
				if (count == 0)
				{
					continue;
				}
				
				// ȥ�����ܲ�������Ϣ�����е� WM_LVN_AUTOSEL ��Ϣ
				while (::PeekMessage(&msgtmp, NULL, WM_LVN_AUTOSEL, WM_LVN_AUTOSEL, PM_REMOVE)){;}

				// �������������Ϣ
				for (i=0; i<count; i++)
				{
					// ������µ���������������ǰ������
					if (::PeekMessage(&msgtmp, NULL, WM_LVN_NEWCOMM, WM_LVN_QUIT, PM_NOREMOVE))
					{
						break;
					}
					
					if (m_iCount < MAX_SUBITEMCNT)
					{
						// �����̷߳��������Ϣ
						pList->PostMessage(WM_LVN_SUBITEMDATA, (WPARAM)sn, (LPARAM)i);
						::InterlockedIncrement((LPLONG)&m_iCount);
						::Sleep(0);						// Ϊ���߳���������������Ϣ��ʱ��
					}
					else
					{
						i--;
						::Sleep(0);						// ��̫�෢��������û�б�������ͣ����
					}
				}
				
				if (i < count)
				{
					continue;							// ֱ�ӷ��أ�ȥ��ȡ�µ�����
				}

				// ����������Զ����ţ��򴥷����Ź���
				if (m_iAutoPlay)
				{
					::PostThreadMessage(pList->m_pThread->m_nThreadID, WM_LVN_AUTOSEL, count, 0);
				}
			}
			else
			{
				continue;
			}
        }
    } 
	
	// �˳��߳�
	::AfxEndThread(0, TRUE);

	return 0;
}


// �б�ؼ�����ص�����
int CALLBACK CShellList::CFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	CString    strItem1  = pListCtrl->GetItemText(lParam1, 0);
	CString    strItem2  = pListCtrl->GetItemText(lParam2, 0);
	
	return strcmp(strItem2, strItem1);
}


// ��ʼ���б�ؼ�
int CShellList::InitShellList(CShellTree *pst, CShellPath *psp, CString filter)
{
	ISeeTsVCDebugStrRecorder().Record("��ʼ��ʼ�� ShellList ...", 0, 0);

	m_pTree = pst;
	m_pPath = psp;
	m_sCurrPath = "C:\\";
	m_sFilter   = filter;

	// ��������߳�
	if (!(m_pThread=::AfxBeginThread(ShellLProc, (LPVOID)this)))
	{
		return -1;
	}

	for (int i=0; i<COLNUM; i++)
	{
		InsertColumn(sc[i].index, (LPCTSTR)sc[i].name, sc[i].ali, sc[i].width, -1);
	}
	
	// ����ϵͳͼ���б�
	EnableImages();

	// �趨�Զ����ű�־
	m_iAutoPlay = AfxGetApp()->GetProfileInt(ITSVCMAIN_RKEYSEC, ITSVCLIST_RKEY_AUTOPLAY, 0);

	ISeeTsVCDebugStrRecorder().Record("ShellList ��ʼ����ϡ�", 0, 0);
	
	return 0;
}


// �ر��б�ؼ�
void CShellList::CloseShellList()
{
	// �˳�ǰ�������е��б���
	DeleteAllItems();

	// �趨�Զ����ű�־
	AfxGetApp()->WriteProfileInt(ITSVCMAIN_RKEYSEC, ITSVCLIST_RKEY_AUTOPLAY, m_iAutoPlay);

	// ��������߳�
	::PostThreadMessage(m_pThread->m_nThreadID, WM_LVN_QUIT, 0, 0);
	
	::WaitForSingleObject(m_pThread->m_hThread, 1000);

	m_sFilter.Empty();
	m_sCurrPath.Empty();

	m_pThread = 0;

	m_pTree = 0;
	m_pPath = 0;

	ISeeTsVCDebugStrRecorder().Record("ShellList �رա�", 0, 0);
}


// ��ɾ����Ӧ
void CShellList::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW		*pNMListView = (NM_LISTVIEW*)pNMHDR;
	LPLVITEMDATA	plvid = (LPLVITEMDATA)GetItemData(pNMListView->iItem);

	if (plvid)
	{
		// �ͷ��ļ������PIDL
		if (plvid->lpi)
		{
			m_pTree->g_malloc->Free(plvid->lpi);
		}

		// �ͷŸ��ļ��нӿ�
		if (plvid->lpsfParent)
		{
			plvid->lpsfParent->Release();
		}

		delete plvid;
	}

	*pResult = 0;
}


// ������ɾ����Ӧ
void CShellList::OnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	*pResult = FALSE;
}


// Ϊ�б�ؼ�����ͼ���б�
void CShellList::EnableImages()
{
	SetImageList(m_pPath->m_cImageList.GetLIconList(), LVSIL_NORMAL);
	SetImageList(m_pPath->m_cImageList.GetSIconList(), LVSIL_SMALL);
}


// ��ʼһ���µ������̣����ӿؼ�������
LRESULT CShellList::OnFillBegin(WPARAM wParam, LPARAM lParam)
{
	// ɾ������
	DeleteAllItems();

	// �ı䵱ǰ·��
	m_sCurrPath = (LPCTSTR)(const char *)wParam;

	m_iNum = 0;

	return 0;
}


// ����һ��������
LRESULT CShellList::OnFillEnd(WPARAM wParam, LPARAM lParam)
{
	// ������
	SortItems(CFunc, (DWORD)(LPVOID)this);

	Arrange(LVA_DEFAULT);

	// ���κŸ���
	m_iSN++;
	m_iNum = wParam;
	
	// �����ʼ��
	m_iCurrPt = 0;
	
	// ������ǰ·�����ļ�
	m_pPath->ObjectChanged((LPCTSTR)m_sCurrPath, (LPCTSTR)0, m_iSN);

	// ֪ͨ����߳̿�ʼ���������Ϣ
	::PostThreadMessage(m_pThread->m_nThreadID, WM_LVN_NEWCOMM, (WPARAM)m_iSN, (LPARAM)wParam);

	ISeeTsVCDebugStrRecorder().Record("ShellList �ĵ�ǰ·�����ı�Ϊ <%s>��", 1, (LPCSTR)m_sCurrPath);
	ISeeTsVCDebugStrRecorder().Record("ShellList ��ǰ·���µ��ļ������Ϊ <%d>��", 1, m_iNum);
	
	return 0;
}


// �Զ�ѡ����һ��ͼ���Զ�����ʱ���ã�
LRESULT CShellList::OnAutoNext(WPARAM wParam, LPARAM lParam)
{
	POSITION	pos  = GetFirstSelectedItemPosition();
	int			nCnt = GetItemCount();
	int			nItem;
	UINT		nState;

	// ��ȡ��ǰѡ���������
	if (pos == NULL)
	{
		// ����δ��ѡ�е�ӵ�н������
		if ((nItem=GetNextItem(-1, LVNI_ALL|LVNI_FOCUSED)) >= 0)
		{
			nState = LVIS_FOCUSED;
			SetItemState(nItem, 0, LVIS_FOCUSED);
		}
		else
		{
			nItem  = -1;
			nState = 0;
		}
	}
	else
	{
		// ��ȡѡ��������
		nItem = GetNextSelectedItem(pos);

		// �ж�ԭ���Ƿ�ӵ�н���
		nState = GetItemState(nItem, LVIS_FOCUSED);

		// ȥ����ǰ���ѡ��״̬������
		SetItemState(nItem, 0, LVIS_SELECTED);

		if (nState == LVIS_FOCUSED)
		{
			SetItemState(nItem, 0, LVIS_FOCUSED);
		}
	}

	// ���ƹ���
	if (++nItem >= nCnt)
	{
		nItem = 0;
	}

	// ��������Ľ��㼰ѡ��״̬
	if (nState == LVIS_FOCUSED)
	{
		SetItemState(nItem, LVIS_FOCUSED, LVIS_FOCUSED);
	}

	SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);

	return 0;
}


// ��ȡ�Զ����ű�־
int CShellList::GetAutoPlay()
{
	return m_iAutoPlay;
}


// �����Զ�����
void CShellList::SetAutoPlay(int type)
{
	// type���壺��0 �����Զ����ţ�0 �ر��Զ�����
	if (type)
	{
		// �����ǰ״̬Ϊ�ر�
		if (m_iAutoPlay == 0)
		{
			m_iAutoPlay = 1;

			// �����ǰĿ¼�ѱ������ϣ����ֶ������Զ�������Ϣ
			if (m_iNum)
			{
				::PostThreadMessage(m_pThread->m_nThreadID, WM_LVN_AUTOSEL, m_iNum, 0);
			}
		}
	}
	else
	{
		m_iAutoPlay = 0;
	}
}


// ����б������ϸ���ݣ�������߳�������
LRESULT CShellList::OnSubItemDataFill(WPARAM wParam, LPARAM lParam)
{
	static	WIN32_FILE_ATTRIBUTE_DATA	wfa;
	static	CString	spath;
	static	CString	cs;
	static	char	buff[MAX_PATH];
	
	// ����ǹ��������ֱ�ӷ��أ��������κ��б�
	if (wParam != (WPARAM)m_iSN)
	{
		::InterlockedDecrement((LPLONG)&m_iCount);
		return 0;
	}

	CRect			rc;
	UINT			flags = 0, style = 0;
	int				index, numperpage, itemcount, i, lastidx;
	DWORD			dwl;
	LPSPEC_DATAPACK	psdp;
	LPLVITEMDATA	pdata;
	
	itemcount  = GetItemCount();						// ��ȡ�б�����������ĸ���
	numperpage = GetCountPerPage();						// ��ȡ�ӵ�ÿһҳ�п���ʾ��ĸ���
	style      = GetStyle();							// ��ȡ�ӵķ��

	// �����б��ӡ������ӷ���µ�ǰ���ӷ�Χ���Ƿ����δ����������ݵ���
	if (((style&LVS_TYPEMASK) == LVS_REPORT)||((style&LVS_TYPEMASK) == LVS_LIST))
	{
		index      = GetTopIndex();						// ��ȡ��һ�������������
		lastidx    = min((itemcount-index), numperpage);

		ASSERT(index != -1);

		for (i=0; i<lastidx; i++)						// ���ҿ��ӷ�Χ�ڵ�δ�����
		{
			pdata = (LPLVITEMDATA)GetItemData(index+i);
			ASSERT(pdata);

			if (pdata->state == SPTS_EMPTY)				
			{
				index = index+i;
				break;
			}
		}

		index = (i == numperpage) ? -1 : index;			// ���û���ҵ�������-1
	}
	else
	{
		index = -1;
	}

	CMainFrame	*cmf = (CMainFrame*)AfxGetMainWnd();
	CString		prs;
	BOOL		ismark = FALSE;

	if (index != -1)
	{
		// ��ȡ�ļ���
		GetItemText(index, 0, (LPTSTR)buff, MAX_PATH);
		
		// �ϳ�ȫ·����
		spath = m_sCurrPath;
		spath += (LPCTSTR)buff;

		// �����ļ���Ӧ��ͼ��
		i = m_pPath->m_cImageList.GetIconIndex(AfxGetMainWnd()->GetSafeHwnd(), pdata->lpsfParent, pdata->lpi, (LPCSTR)spath);
		SetItem(index, 0, LVIF_IMAGE, (LPCTSTR)0, i, 0, 0, 0);

		// ��ȡ�ļ�������Ϣ
		if (GetFileAttributesEx((LPCSTR)spath, GetFileExInfoStandard, (LPVOID)&(pdata->wfad)))
		{
			pdata->state |= SPTS_FATT;					// ��ӵ���ļ�������Ϣ

			dwl = (pdata->wfad.nFileSizeLow+1023)/1024;
			if (dwl > 1024)
				cs.Format("%.1fMB", (float)dwl/1024);
			else
				cs.Format("%dKB", dwl);
			
			SetItem(index, 1, LVIF_TEXT, (LPCTSTR)cs, 0, 0, 0, 0);
		}
		
		// ��ȡͼ�������Ϣ
		if ((psdp=isirw_s_get_img_info(ISEEIO_CONFER_LOC_FILE, (LPCTSTR)spath, 0, 0, m_iSN)) != 0)
		{
			if (isirw_s_wait(psdp, 0) == ER_SUCCESS)
			{
				// ��ȡͼ����Ϣ�����˽�����ݽṹ
				pdata->width    = (DWORD)psdp->org_img->width;
				pdata->height   = (DWORD)psdp->org_img->height;
				pdata->bitcount = (DWORD)psdp->org_img->bitcount;
				
				pdata->state   |= SPTS_IMGINF;

				// ��ʾͼ��Ŀ��ߵ�����
				cs.Format("%d��%d��%d", psdp->org_img->width, psdp->org_img->height, psdp->org_img->bitcount);
				SetItem(index, 2, LVIF_TEXT, (LPCTSTR)cs, 0, 0, 0, 0);

				ismark = TRUE;
			}
			isirw_free_SPEC_DATAPACK(psdp);
		}

		if (ismark == FALSE)
		{
			cs.Format("�����ͼ���ļ���");
			SetItem(index, 3, LVIF_TEXT, (LPCTSTR)cs, 0, 0, 0, 0);
		}

		::InterlockedDecrement((LPLONG)&m_iCount);

		if (lParam == (m_iNum-1))
		{
			((CRightView*)this)->m_bFillMark = 2;		// ������

			prs.Format("��%d��ͼƬ - 100%%", m_iNum);
		}
		else
		{
			prs.Format("��%d��ͼƬ - %d%%", m_iNum, (lParam*100)/m_iNum);
		}

		// ����״̬���е������Ϣ
		cmf->m_wndStatusBar.SetPaneText(cmf->m_wndStatusBar.CommandToIndex(IDS_PROGRESS), (LPCTSTR)prs);
		
		return 1;
	}
	else if (m_iCurrPt < itemcount)
	{
		index = m_iCurrPt;

		while (TRUE)
		{
			m_iCurrPt++;

			pdata = (LPLVITEMDATA)GetItemData(index);

			if (pdata->state == SPTS_EMPTY)
			{
				GetItemText(index, 0, (LPTSTR)buff, MAX_PATH);
				
				spath = m_sCurrPath;
				spath += (LPCTSTR)buff;
				
				// �����ļ���Ӧ��ͼ��
				i = m_pPath->m_cImageList.GetIconIndex(AfxGetMainWnd()->GetSafeHwnd(), pdata->lpsfParent, pdata->lpi, (LPCSTR)spath);
				SetItem(index, 0, LVIF_IMAGE, (LPCTSTR)0, i, 0, 0, 0);
				
				// ��ȡ�ļ�������Ϣ
				if (GetFileAttributesEx((LPCSTR)spath, GetFileExInfoStandard, (LPVOID)&(pdata->wfad)))
				{
					pdata->state |= SPTS_FATT;					// ��ӵ���ļ�������Ϣ
					
					dwl = (pdata->wfad.nFileSizeLow+1023)/1024;
					if (dwl > 1024)
						cs.Format("%.1fMB", (float)dwl/1024);
					else
						cs.Format("%dKB", dwl);
					
					SetItem(index, 1, LVIF_TEXT, (LPCTSTR)cs, 0, 0, 0, 0);
				}
		
				// ��ȡͼ�������Ϣ
				if ((psdp=isirw_s_get_img_info(ISEEIO_CONFER_LOC_FILE, (LPCTSTR)spath, 0, 0, m_iSN)) != 0)
				{
					if (isirw_s_wait(psdp, 0) == ER_SUCCESS)
					{
						// ��ȡͼ����Ϣ�����˽�����ݽṹ
						pdata->width    = (DWORD)psdp->org_img->width;
						pdata->height   = (DWORD)psdp->org_img->height;
						pdata->bitcount = (DWORD)psdp->org_img->bitcount;
						
						pdata->state   |= SPTS_IMGINF;
						
						// ��ʾͼ��Ŀ��ߵ�����
						cs.Format("%d��%d��%d", psdp->org_img->width, psdp->org_img->height, psdp->org_img->bitcount);
						SetItem(index, 2, LVIF_TEXT, (LPCTSTR)cs, 0, 0, 0, 0);

						ismark = TRUE;
					}
					isirw_free_SPEC_DATAPACK(psdp);
				}

				if (ismark == FALSE)
				{
					cs.Format("�����ͼ���ļ���");
					SetItem(index, 3, LVIF_TEXT, (LPCTSTR)cs, 0, 0, 0, 0);
				}

				::InterlockedDecrement((LPLONG)&m_iCount);
				
				if (lParam == (m_iNum-1))
				{
					((CRightView*)this)->m_bFillMark = 2;		// ������

					prs.Format("��%d��ͼƬ - 100%%", m_iNum);
				}
				else
				{
					prs.Format("��%d��ͼƬ - %d%%", m_iNum, (lParam*100)/m_iNum);
				}

				cmf->m_wndStatusBar.SetPaneText(cmf->m_wndStatusBar.CommandToIndex(IDS_PROGRESS), (LPCTSTR)prs);

				return 1;
			}
			
			index = m_iCurrPt;

			if (index >= itemcount)
			{
				break;
			}
		}
	}
	
	ASSERT(m_iCount == 0);

	return 0;
}


// ��ȡ�ļ����˴�
CString	&CShellList::GetFilterString()
{
	return m_sFilter;
}


// �����ļ����˴�
void CShellList::SetFilterString(CString &nsp)
{
	m_sFilter = nsp;
}


// �ж�ָ���ļ��Ƿ��ܱ��б�ؼ����ܣ������ļ����˴���
BOOL CShellList::IsDestFile(LPCTSTR fname)
{
	if (m_sFilter.Find("*.*",0) != -1)
	{
		return TRUE;									// "*.*"��ʾ�κ��ļ�������
	}

	CString	stmp = fname;
	int		dot  = 0;
	
	// ���˴�ΪСд��������Ҫ��ת��
	stmp.MakeLower();

	// �ж��Ƿ������չ��
	if ((dot=stmp.ReverseFind('.')) == -1)
	{
		return FALSE;									// ����չ���ļ�������Ϊ��Ŀ�����
	}

	// ��ȡ��չ����������'.'���ţ�
	CString	stmp2 = stmp.Right(stmp.GetLength()-dot);

	// ���Ե��ǵ�һЩ�����Ϲ涨���ļ�����
	if (stmp2.GetLength() == 1)
	{
		return FALSE;									// ����չ�����ļ�
	}

	// �жϸ������Ƿ���Ϲ�������
	if ((dot=m_sFilter.Find((LPCTSTR)stmp2,0)) == -1)
	{
		return FALSE;									// ��֧�ֵ��ļ�����
	}

	LPCTSTR		ps = (LPCSTR)m_sFilter;

	// ���ϸ���ж�
	if ((ps[dot+stmp2.GetLength()] != ',')&&(ps[dot+stmp2.GetLength()] != '\0'))
	{
		return FALSE;
	}

	// ���Ϸ���TRUE
	return TRUE;
}

