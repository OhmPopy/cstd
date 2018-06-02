/********************************************************************

	ShellTree.cpp

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
	���ļ���;��	ͼ���д���棭C���԰棭VC���������������shell tree
					��ʵ���ļ���

	���ļ���д�ˣ�	
					Selom Ofori		none
					YZ				yzfree##sina.com

	���ļ��汾��	30315
	����޸��ڣ�	2003-3-15

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������
	----------------------------------------------------------------
	����ע�ͣ�

	���ļ���ԭʼ����Ϊ Selom Ofori��Դ��û��������ơ�����YZ��д������
	ISee��Ŀ�����֤��ΪGPL���� Selom Ofori û��������ϵ��ַ������δ��
	�������������

	������ʷ��

		2003-3		�޸�����б��ӵķ�ʽ������ISee����ӿڵ��ã�
		2003-1		��һ�����԰淢��

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


////////////////////////////////////////////////////////////////////////////////////////
// CShellTree�ྲ̬��Ա������ʼ��

CWinThread			*CShellTree::g_pwt   = NULL;		// �ӳ��̵߳Ķ���ָ��
LPMALLOC			CShellTree::g_malloc = NULL;		// IMalloc�ӿ�

CShellList			*CShellTree::g_pList = 0;			// �б�ؼ�ָ��
CShellPath			*CShellTree::g_pPath = 0;			// �ϼ�����ָ��




////////////////////////////////////////////////////////////////////////////////////////
// CShellTree��ӿ�ʵ��

CShellTree::CShellTree()
{
}

CShellTree::~CShellTree()
{
}


BEGIN_MESSAGE_MAP(CShellTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CShellTree)
	ON_MESSAGE(WM_TVN_DEFERFILL, OnDeferFill)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/****************************************************************************
*
*    ����: ShellTProc(LPVOID parm)
*
*    ������parm - CShellTree���thisֵ
*
*    ��;: �ӳ��̣߳����û��ü����޸���ѡ����ʱ��CShellTree�ཫ�ӳ������
*          �ļ�����Ϣ
*
****************************************************************************/
UINT CShellTree::ShellTProc(LPVOID parm)
{
	CShellTree		*pst = (CShellTree*)parm;
	LPTREEMSGPACK	ptmp = 0;
	MSG				msg;
	MSG				msgtmp;
	BOOL			bRet;
	int				i;

	// ֪ͨ���̣߳����������̵߳���Ϣ����
	pst->PostMessage(WM_TVN_PROCCREATE);

	// ��Ϣѭ��
	while((bRet=::GetMessage(&msg, NULL, 0, 0)) != 0)
    { 
        if (bRet == -1)
        {
            continue;
        }
        else
        {
			if (msg.message == WM_TVN_QUIT)				// �˳���Ϣ
			{
				break;
			}
			else if (msg.message == WM_TVN_QUEUECLR)	// �����Ϣ
			{
				continue;
			}
			else if (msg.message == WM_TVN_DEFER)		// �ӳ���Ϣ
			{
				// ��ȡ���������
				ptmp = (LPTREEMSGPACK)msg.lParam;

				if (!ptmp)
				{
					continue;
				}

				// �ӳ�ָ����ʱ�䣨SHELLTREE_DEFER * 10ms��
				for (i=0; i<SHELLTREE_DEFER; i++)
				{
					// ������µ���������������ǰ�����ӳٵ�����
					if (::PeekMessage(&msgtmp, NULL, WM_TVN_QUIT, WM_TVN_DEFER, PM_NOREMOVE))
					{
						ptmp->ppsf->Release();
						delete ptmp;
						ptmp = 0;
						break;
					}
					// �ӳ�һ��ʱ�䵥λ��10ms��
					::Sleep(10);
				}

				if (i < SHELLTREE_DEFER)
				{
					continue;							// ֱ�ӷ��أ�ȥ��ȡ�µ�����
				}

				// �����̷߳��������Ϣ
				pst->PostMessage(WM_TVN_DEFERFILL, 0, (LPARAM)ptmp);

				ptmp = 0;
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


/****************************************************************************
*
*    ����: InitShellTree()
*
*    ��;: ��ʼ��CShellTree��������һЩ��Ҫ����Դ
*
*    ��ڣ�psl - ��Ӧ���б�������ָ��
*          psp - ������ָ��
*
*    ���أ�0     - �ɹ�
*          -1	 - ʧ�ܣ�Shell�ڴ����ӿ�����ʧ��
*          -2    - ʧ�ܣ��ӳ��̴߳���ʧ��
*
****************************************************************************/
int CShellTree::InitShellTree(CShellList *psl, CShellPath *psp)
{
	ISeeTsVCDebugStrRecorder().Record("��ʼ��ʼ�� ShellTree ...", 0, 0);

	// ����Shell���ڴ����ӿ�
	if (FAILED(::SHGetMalloc(&g_malloc)))
	{
		return -1;										// ��ȡShell����ӿ�ʧ��
	}

	// �����ӳ��߳�
	if (!(g_pwt=::AfxBeginThread(ShellTProc, this, THREAD_PRIORITY_NORMAL, 0, 1, NULL)))
	{
		g_malloc->Release();
		g_malloc = 0;
		return -2;										// �����ӳ��߳�ʧ��
	}

	ASSERT(psl&&psp);

	g_pList = psl;										// ��ʼ���б���Ƽ�������ָ��
	g_pPath = psp;

	::ResumeThread(g_pwt->m_hThread);					// �����߳�

	ISeeTsVCDebugStrRecorder().Record("ShellTree ��ʼ����ϡ�", 0, 0);

	return 0;
}


/****************************************************************************
*
*    ����: CloseShellTree()
*
*    ��;: �ͷ�CShellTree�е���Դ
*
****************************************************************************/
void CShellTree::CloseShellTree()
{
	DeleteAllItems();

	// �����ӳ��߳�
	::PostThreadMessage(g_pwt->m_nThreadID, WM_TVN_QUIT, 0, 0);

	// �ȴ��ӳ��߳��˳�
	::WaitForSingleObject(g_pwt->m_hThread ,INFINITE);

	g_pwt    = NULL;

	// �ͷ�IMalloc�ӿ�
	g_malloc->Release();
	g_malloc = 0;
	
	g_pList  = 0;										// �б�ؼ�ָ��
	g_pPath  = 0;										// �ϼ�����ָ��
	
	ISeeTsVCDebugStrRecorder().Record("ShellTree �رա�", 0, 0);

	return;
}


/****************************************************************************
*
*    ����: PopulateTree()
*
*    ��;: ��ϵͳShell�ĸ������棩��װĿ¼��
*
*    ��ڣ�cszLastSel    - ָ����ǰ��ѡ�����·������������Ϊ�գ�ȱʡѡ��
*
*    ���أ�0     - �ɹ�
*          -1    - ʧ�ܣ�δ�ܻ�ȡ�����ļ��нӿ�
*          -2    - ʧ�ܣ�δ�ܻ�ȡ�����ļ��е�PIDL
*          -3    - ʧ�ܣ�δ�ܻ�ȡ�����ļ�����ʾ��
*
****************************************************************************/
int CShellTree::PopulateTree(CString &cszLastSel)
{

	HTREEITEM		hti;
	TV_ITEM         tvi;
	TV_INSERTSTRUCT tvins;
	TV_SORTCB		tvscb;

	LPSHELLFOLDER	lpsf   = NULL;
	LPITEMIDLIST	lpi    = NULL;
	LPITEMIDLIST	lpifq  = NULL; 
	LPTVITEMDATA    lptvid = NULL;

	char			szBuff[MAX_PATH];

	ISeeTsVCDebugStrRecorder().Record("ShellTree ��ʼ��װ·�� <%s>��", 0, (LPCSTR)cszLastSel);

	// ��ȡ�����ļ��нӿ�
	if (FAILED(::SHGetDesktopFolder(&lpsf)))
	{
		return -1;
	}

	// ɾ����ǰ�������е���
	DeleteAllItems();

	// ��ȡ��Ԫ�صı�ʶ���б�PIDL��
	if (FAILED(::SHGetSpecialFolderLocation(0, CSIDL_DESKTOP, &lpi)))
	{
		lpsf->Release();
		return -2;
	}

	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_CHILDREN;

	// ��ȡԪ����ʾ��
	if (GetName(lpsf, lpi, SHGDN_NORMAL, szBuff) == 0)
	{
		g_malloc->Free(lpi);
		lpsf->Release();
		return -3;
	}

	// ��д��ʾ�������������ļ��б�־
	tvi.pszText    = szBuff;
	tvi.cchTextMax = MAX_PATH;
	tvi.cChildren  = 1;

	// ��ȡ���Ԫ�ع�����ͼ������
	GetNormalAndSelectedIcons(lpi, &tvi);

	// ˽�����ݶ�Ϊ�գ���Ϊ����������ģ�����һ��������
	tvi.lParam = (LPARAM)0;

	tvins.item         = tvi;
	tvins.hInsertAfter = 0;
	tvins.hParent      = 0;

	// �����Ԫ��
	hti = InsertItem(&tvins);

	// �����
	if (FillShellView(lpsf, lpi, hti, ISHTYPE_TREE) == 0)
	{
		tvscb.hParent     = hti;
		tvscb.lParam      = 0;
		tvscb.lpfnCompare = TreeViewCompareProc;
		
		// ���θ����ʾ������
		SortChildrenCB(&tvscb);
		
		// չ���θ���
		Expand(hti, TVE_EXPAND);

		if (cszLastSel.GetLength() == 0)
		{
			// ��ȡ���ҵ��ĵ���������
			hti = GetChildItem(hti);
		
			// չ����ѡ���ҵ��ĵ����ļ���
			Expand(hti, TVE_EXPAND);
			Select(hti, TVGN_CARET);
		}
		else
		{
			TunnelTree(cszLastSel);
		}
	}
	else
	{
		Select(hti, TVGN_CARET);
	}

	// �ͷ�IShellFolder�ӿڼ�����PIDL
	lpsf->Release();
	g_malloc->Free(lpi);

	ISeeTsVCDebugStrRecorder().Record("ShellTree ·�� <%s> ��װ��ϡ�", 0, (LPCSTR)cszLastSel);

	return 0;
}


/****************************************************************************
*
*  ����: FillShellView(LPSHELLFOLDER lpsf, LPITEMIDLIST lpifq, HTREEITEM hParent, int type)
*
*  ��;: ���һ���ļ����µ����ļ��������һ���ļ���ö�����µ����ļ��У�
*        Ȼ���������С�
*
*  ���:
*    lpsf         - ������ļ��е�IShellFolder�ӿ�
*    lpifq        - ������ļ��еľ���PIDL
*    hParent      - ����Ľڵ�
*    type         - �������
*
****************************************************************************/
int CShellTree::FillShellView(LPSHELLFOLDER lpsf, LPITEMIDLIST lpifq, HTREEITEM hParent, int type)
{
    TV_ITEM         tvi;
	LV_ITEM			lvi;
    TV_INSERTSTRUCT tvins;

    HTREEITEM       hPrev = NULL;
    LPENUMIDLIST    lpe   = NULL;
    LPITEMIDLIST    lpi   = NULL;
	LPITEMIDLIST    lpiTemp       = NULL;
	LPITEMIDLIST    lpifqThisItem = NULL;
    LPTVITEMDATA    lptvid        = NULL;

    UINT            uCount = 0;
	UINT			uIcon  = 0;
	int				lcnt   = 0;
	int				lindx  = 0;
    ULONG           ulFetched;
	ULONG			ulAttrs;
    char            szBuff[MAX_PATH];
    HWND            hwnd;

	static			char phroot[MAX_PATH];


	hwnd = ::GetParent(m_hWnd);

	if (type&ISHTYPE_LIST)
	{
		AfxGetApp()->DoWaitCursor(1);

		if ((g_pList->GetStyle()&LVS_TYPEMASK) == LVS_ICON)
		{
			uIcon = SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_LARGEICON | SHGFI_ICON;
		}
		else
		{
			uIcon = SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_ICON;
		}

		phroot[0] = '\0';

		::SHGetPathFromIDList(lpifq, (LPSTR)phroot);

		if (strlen(phroot))
		{
			if (phroot[strlen(phroot)-1] != '\\')
			{
				strcat((char*)phroot, "\\");
			}
		}
		else
		{
			strcat((char*)phroot, "\\");
		}
		
		g_pList->SendMessage(WM_LVN_FILLBEGIN, (WPARAM)phroot);
	}

	DWORD	obj = 0;

	if (type&ISHTYPE_TREE)
	{
		obj |= SHCONTF_FOLDERS;
	}
	if (type&ISHTYPE_LIST)
	{
		obj |= SHCONTF_NONFOLDERS;
	}

    // ��ȡIEnumIDList�ӿڣ���ö�����е�ָ��Ԫ�أ��ļ��л�/�ͷ��ļ���Ԫ�أ�
    if (SUCCEEDED(lpsf->EnumObjects(hwnd, obj, &lpe)))
    {
        // ö���ļ����µ����ļ��к�/���ļ�����
        while (S_OK == lpe->Next(1, &lpi, &ulFetched))
        {
            ulAttrs = SFGAO_FOLDER | SFGAO_FILESYSANCESTOR | SFGAO_FILESYSTEM | SFGAO_HASSUBFOLDER;

            // ��ȡԪ������
            lpsf->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lpi, &ulAttrs);

			// �����������ļ�ϵͳ�Ķ��󽫱�����
			if (!(ulAttrs & (SFGAO_FILESYSANCESTOR | SFGAO_FILESYSTEM)))
			{
				g_malloc->Free(lpi);
				lpi = 0;
				continue;
			}

			// �ļ��д���
			if (ulAttrs & SFGAO_FOLDER)
			{
				if (type&ISHTYPE_TREE)
				{
					memset((void*)&tvi, 0, sizeof(TV_ITEM));
					
					tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
					
					// �ж�Ԫ���Ƿ�ӵ�����ļ��У����и��������+�ţ������û���һ��չ��
					// ����ʱ��������װ����
					if (ulAttrs & SFGAO_HASSUBFOLDER)
					{
						tvi.cChildren = 1;
						tvi.mask |= TVIF_CHILDREN;
					}
					
					// ���������˽�����ݽṹ��TVITEMDATA��
					if((lptvid=(LPTVITEMDATA)g_malloc->Alloc(sizeof(TVITEMDATA))) == NULL)
					{
						g_malloc->Free(lpi);
						lpi = 0;
						continue;
					}
					
					// ��ȡ�ļ��С���ʾ������
					if (GetName(lpsf, lpi, SHGDN_NORMAL, szBuff) == FALSE)
					{
						g_malloc->Free(lptvid);
						g_malloc->Free(lpi);
						lptvid = 0;
						lpi = 0;
						continue;
					}
					
					tvi.pszText    = szBuff;
					tvi.cchTextMax = MAX_PATH;
					
					// ��������PIDL�������µġ����Ե�PIDL
					lpifqThisItem = ConcatPidls(lpifq, lpi);
					
					// ����һ������ԡ�PIDL��˽�����ݽṹ
					lptvid->lpi = CopyITEMID(g_malloc, lpi);
					
					GetNormalAndSelectedIcons(lpifqThisItem, &tvi);
					
					// ���游�ļ��нӿڣ�COM�����ۼӣ�
					lptvid->lpsfParent = lpsf;
					lpsf->AddRef();
					
					// �������ԡ�PIDLд��˽�����ݽṹ
					lptvid->lpifq = lpifqThisItem;
					lpifqThisItem = 0;
					
					tvi.lParam = (LPARAM)lptvid;
					
					// ����һ������
					tvins.item         = tvi;
					tvins.hInsertAfter = hPrev;
					tvins.hParent      = hParent;
					
					hPrev = InsertItem(&tvins);
				}
			}
			// �ļ�����
			else if (ulAttrs & SFGAO_FILESYSTEM)
			{
				if (type&ISHTYPE_LIST)
				{
					memset((void*)&lvi, 0, sizeof(LV_ITEM));
					
					lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
					
					// ��ȡ�ļ�����ʾ������ 
					if (GetName(lpsf, lpi, SHGDN_INFOLDER|SHGDN_FORPARSING, szBuff) == FALSE)
					{
						g_malloc->Free(lpi);
						lpi = 0;
						continue;
					}
					
					// �ж��Ƿ���Ҫ������ļ������ļ��б�
					if (g_pList->IsDestFile((LPCTSTR)szBuff) == FALSE)
					{
						g_malloc->Free(lpi);
						lpi = 0;
						continue;
					}

					// �������˽�����ݽṹ
					if ((lvi.lParam=(LPARAM) new LVITEMDATA) == 0)
					{
						g_malloc->Free(lpi);
						lpi = 0;
						continue;
					}
					
					// ��ʼ����˽�����ݽṹ
					memset((void*)lvi.lParam, 0, sizeof(LVITEMDATA));

					// ����״̬��Ϊ��
					((LPLVITEMDATA)lvi.lParam)->state = SPTS_EMPTY;
					((LPLVITEMDATA)lvi.lParam)->lpi   = CopyITEMID(g_malloc, lpi);
					((LPLVITEMDATA)lvi.lParam)->lpsfParent = lpsf;
					lpsf->AddRef();

					lvi.iItem      = lcnt++;
					lvi.pszText    = szBuff;
					lvi.cchTextMax = MAX_PATH;
					
					// ��ʱ����δ֪����ͼ��
					lvi.iImage     = 0;
					
					// ����һ���б���
					lindx = g_pList->InsertItem((const LVITEM *)&lvi);
				}
			}
            g_malloc->Free(lpi);
            lpi = 0;

        }
		lpe->Release();
		lpe = 0;
    }

	if (type&ISHTYPE_LIST)
	{
		g_pList->SendMessage(WM_LVN_FILLEND, (WPARAM)lcnt);
		
		AfxGetApp()->DoWaitCursor(-1);
	}
	
	return 0;
}


/****************************************************************************
*
*    ����: GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
*
*    ��;:  Ϊָ�������ȡ������ѡ��״̬�µġ���ϵͳͼ����е�����
*
*    ���:
*      lpifq    - ����ľ���PIDL
*      lptvitem - ָ�������TV_ITEM�ṹ�����ص������Ž���д�ڸýṹ��iImage��
*                 iSelectedImage��Ա�����С�
*
****************************************************************************/
void CShellTree::GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
{
   // �������ǲ�ȥ���GetItemIcon()�ķ���ֵ�Ƿ���ȷ����Ϊ����ú���ʧ�ܣ����Ǿͻ�
   // �д��鷳......
   // YZ ע��������sam��ԭ�����������д���ȷ�иĽ�����ء�

   lptvitem->iImage         = GetItemIcon(lpifq, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_ICON);
   lptvitem->iSelectedImage = GetItemIcon(lpifq, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON  | SHGFI_ICON);
   
   return;
}


/****************************************************************************
*
*    ����: TreeViewCompareProc(LPARAM, LPARAM, LPARAM)
*
*    ��;:  �����ܵĻص�����
*
****************************************************************************/
int CALLBACK CShellTree::TreeViewCompareProc(LPARAM lparam1, LPARAM lparam2, LPARAM lparamSort)
{
    LPTVITEMDATA lptvid1=(LPTVITEMDATA)lparam1;
    LPTVITEMDATA lptvid2=(LPTVITEMDATA)lparam2;
    HRESULT   hr;

    if (FAILED(hr=lptvid1->lpsfParent->CompareIDs(0,lptvid1->lpi,lptvid2->lpi)))
	{
		return 0;
	}

    return (short)SCODE_CODE(GetScode(hr));
}




/////////////////////////////////////////////////////////////////////////////
// CShellTree message handlers

/****************************************************************************
*
*	����:	OnFolderExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
*
*	��;:	��չָ�������������Ӧ����ӦTVN_ITEMEXPANDING��Ϣʱ���á�
*
*	ע��:	
*
*	ӳ��:	TVN_ITEMEXPANDING
*
****************************************************************************/
void CShellTree::OnFolderExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPTVITEMDATA   lptvid;
	LPSHELLFOLDER  lpsf2 = NULL;
	TV_SORTCB      tvscb;

	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pNMHDR;

	// ������ļ�����������չ����������������������
    if ((pnmtv->itemNew.state & TVIS_EXPANDEDONCE))
	{
		*pResult = 0;
		return;
	}

	// ȡ�������˽�����ݽṹ
    lptvid = (LPTVITEMDATA)pnmtv->itemNew.lParam;

    if (lptvid)
	{
		// ��ô���չ�ļ��е�IShellFolder�ӿ�
		if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi, 0, IID_IShellFolder,(LPVOID *)&lpsf2)))
		{
			// ����ļ���Ŀ¼��Ϣ
			FillShellView(lpsf2, lptvid->lpifq, pnmtv->itemNew.hItem, ISHTYPE_TREE);

			lpsf2->Release();
		}

		tvscb.hParent     = pnmtv->itemNew.hItem;
		tvscb.lParam      = 0;
		tvscb.lpfnCompare = TreeViewCompareProc;

		// �����ƶ���Ŀ¼����
		SortChildrenCB(&tvscb);
	}	
	
	*pResult = 0;
}


/****************************************************************************
*
*	����:	GetContextMenu(NMHDR* pNMHDR, LRESULT* pResult) 
*
*	��;:	Ϊһѡ����������Ҽ��˵���������Ӧ����ӦNM_RCLICK֪ͨʱ���á�
*
*	ӳ��:	NM_RCLICK;
*
****************************************************************************/
void CShellTree::GetContextMenu(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POINT			pt;
	LPTVITEMDATA	lptvid = NULL;
	LPSHELLFOLDER	lpsf2  = NULL;

	TV_HITTESTINFO	tvhti;
	TV_ITEM			tvi;

	static char		szBuff[MAX_PATH];
	

	// ��ȡ��굱ǰ����
	::GetCursorPos((LPPOINT)&pt);
	ScreenToClient(&pt);

	tvhti.pt = pt;
    HitTest(&tvhti);

    SelectItem(tvhti.hItem);

	// ֻ�е��¼��������������ʾ����ͼ����ʱ�ŵ����˵�
	if (tvhti.flags & (TVHT_ONITEMLABEL|TVHT_ONITEMICON))
	{
		ClientToScreen(&pt);

		tvi.mask  = TVIF_PARAM;
		tvi.hItem = tvhti.hItem;
		
        if (!GetItem(&tvi))
		{
			return;
		}
		
		// ȡ��˽�����ݽṹ
		lptvid = (LPTVITEMDATA)tvi.lParam;
		
		if (lptvid)
		{
			// ��������ʾ�Ҽ��˵�
			DoTheMenuThing(::GetParent(m_hWnd),	lptvid->lpsfParent, lptvid->lpi, &pt);
		}
	}
	
	*pResult = 0;
}


/****************************************************************************
*
*	����:	OnFolderSelected(NMHDR* pNMHDR, LRESULT* pResult, CString &szFolderPath) 
*
*	��;:	���û�ѡ����һ���ļ���ʱӦ���ñ�������TVN_SELCHANGED����������ļ���
*           ���ļ�ϵͳ��һ���֣��򱾺�������������·����
*
*	ӳ��:	TVN_SELCHANGED
*
****************************************************************************/
BOOL CShellTree::OnFolderSelected(NMHDR* pNMHDR, LRESULT* pResult, CString &szFolderPath) 
{
	HRESULT			hr;
	LPTVITEMDATA	lptvid;

	LPSHELLFOLDER	lpsf2  = NULL;
	HTREEITEM		hItem  = NULL;
	BOOL			bRet   = FALSE;
	TV_SORTCB		tvscb;
	int				obj;

	static char		szBuff[MAX_PATH];

	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pNMHDR;

	if((hItem = GetSelectedItem()))
	{
		// ȡ��ѡ�����˽�����ݽṹ
		lptvid = (LPTVITEMDATA)GetItemData(hItem);

		// �������û��˽���������ٽ��н�һ���Ĵ���������
		if (lptvid && lptvid->lpsfParent && lptvid->lpi)
		{
			// ��ȡѡ�����IShellFolder�ӿ�
			hr = lptvid->lpsfParent->BindToObject(lptvid->lpi, 0, IID_IShellFolder, (LPVOID *)&lpsf2);

			if (SUCCEEDED(hr))
			{
				ULONG ulAttrs = SFGAO_FILESYSTEM;

				// �ж�ѡ�����Ƿ������ļ�ϵͳ
				lptvid->lpsfParent->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);

				// �����������д·����Ϣ
				if (ulAttrs & (SFGAO_FILESYSTEM))
				{
					if(::SHGetPathFromIDList(lptvid->lpifq, szBuff))
					{
						szFolderPath = szBuff;
						bRet = TRUE;					// ����ֵ��TRUE
					}
				}

				// �����ͨ�������޸ĵ�ѡ��״̬���ӳٴ���
				if ((pnmtv->action == TVC_BYKEYBOARD)||(pnmtv->action == TVC_UNKNOWN))
				{
					LPTREEMSGPACK	ptmp = (LPTREEMSGPACK) new TREEMSGPACK;

					if (ptmp)
					{
						ptmp->ppsf   = lpsf2;
						ptmp->pidlfq = lptvid->lpifq;
						memcpy((void*)&ptmp->item, (const void *)&(pnmtv->itemNew), sizeof(TVITEM));

						// �����ӳٴ�����Ϣ
						::PostThreadMessage(g_pwt->m_nThreadID, WM_TVN_DEFER, 0, (LPARAM)ptmp);

						// lpsf2 �� ptmp �����ӳ��̻߳��ӳ���亯�������ͷ�
						lpsf2 = NULL;
						ptmp  = NULL;
					}
				}
				else
				{
					// ����ӳ��߳�����δ�����������������
					::PostThreadMessage(g_pwt->m_nThreadID, WM_TVN_QUEUECLR, 0, 0);

					obj  = ((pnmtv->itemNew.cChildren == 1)&&(!(pnmtv->itemNew.state & TVIS_EXPANDEDONCE))) ? ISHTYPE_TREE : 0;
					obj |= ISHTYPE_LIST;

					if (bRet)
						ISeeTsVCDebugStrRecorder().Record("ShellTree �������·�� <%s> �µ���Ŀ¼���ݡ�", 1, szBuff);
					else
						ISeeTsVCDebugStrRecorder().Record("ShellTree �������һ�����ļ�ϵͳ·����", 1, 0);

					// �����Ϣ
					if (FillShellView(lpsf2, lptvid->lpifq, pnmtv->itemNew.hItem, obj) == 0)
					{
						if (obj&ISHTYPE_TREE)
						{
							tvscb.hParent     = pnmtv->itemNew.hItem;
							tvscb.lParam      = 0;
							tvscb.lpfnCompare = TreeViewCompareProc;
							SortChildrenCB(&tvscb);
							
							// ���á��ѱ���չ������־����������չʱ���ӿ��ٶ�
							pnmtv->itemNew.state     |= TVIS_EXPANDEDONCE;
							pnmtv->itemNew.stateMask |= TVIS_EXPANDEDONCE;
							pnmtv->itemNew.mask      |= TVIF_STATE;
							SetItem(&pnmtv->itemNew);
						}
					}

					PostMessage(WM_TVN_SELCHANGED, (WPARAM)pnmtv->itemNew.hItem, 0);
				}
			}
			else
			{
				::PostThreadMessage(g_pwt->m_nThreadID, WM_TVN_QUEUECLR, 0, 0);

				PostMessage(WM_TVN_SELCHANGED, (WPARAM)pnmtv->itemNew.hItem, 0);
			}
		}
		else
		{
			::PostThreadMessage(g_pwt->m_nThreadID, WM_TVN_QUEUECLR, 0, 0);

			PostMessage(WM_TVN_SELCHANGED, (WPARAM)pnmtv->itemNew.hItem, 0);
		}

		if(lpsf2)
		{
			lpsf2->Release();
		}
	}	

	*pResult = 0;

	return bRet;
}


/****************************************************************************
*
*	����:	OnDeleteShellItem(NMHDR* pNMHDR, LRESULT* pResult)
*
*	��;:	����Ӧɾ������ʱ���ñ�������TVN_DELETEITEM��������Ҫ�����ͷ�
*           ���˽�����ݽṹ
*
*	ӳ��:	TVN_DELETEITEM
*
****************************************************************************/
void CShellTree::OnDeleteShellItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPTVITEMDATA	lptvid = NULL;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if ((lptvid=(LPTVITEMDATA)pNMTreeView->itemOld.lParam) != NULL)
	{
		ASSERT(g_malloc);

		lptvid->lpsfParent->Release();

		g_malloc->Free(lptvid->lpi);
		g_malloc->Free(lptvid->lpifq);
		g_malloc->Free(lptvid);
	}
}


/****************************************************************************
*
*	����:	OnDeferFillAtt()
*
*	��;:	��������Ӧ�ӳ������Ϣʱ���ã�WM_TVN_DEFERFILL��������OnDeferFill()
*           ������һ����װ��
*
*	ӳ��:	WM_TVN_DEFERFILL
*
****************************************************************************/
LRESULT CShellTree::OnDeferFillAtt(WPARAM wParam, LPARAM lParam)
{
	return OnDeferFill(wParam, lParam);
}


/****************************************************************************
*
*	����:	OnDeferFill()
*
*	��;:	������Ӧ�ӳ������Ϣʱ���á�WM_TVN_DEFERFILL��ϢΪ������Զ�����Ϣ��
*           ����ҪΪ�������ӳ�������ԡ�
*
*	ӳ��:	WM_TVN_DEFERFILL
*
****************************************************************************/
LRESULT CShellTree::OnDeferFill(WPARAM wParam, LPARAM lParam)
{
	LPTREEMSGPACK	ptmp = (LPTREEMSGPACK)lParam;
	int				obj;
	TV_SORTCB		tvscb;
	
	ASSERT(ptmp);

	if (ptmp->ppsf)
	{
		// �жϴ�������Ƿ��ѱ������ˣ���������ٽ�����Ŀ¼�����
		obj  = ((!(GetItemState(ptmp->item.hItem, TVIS_EXPANDEDONCE) & TVIS_EXPANDEDONCE))&&(ptmp->item.cChildren == 1)) ? ISHTYPE_TREE : 0;
		obj |= ISHTYPE_LIST;

		ISeeTsVCDebugStrRecorder().Record("ShellTree �����ӳ����һ��·������Ŀ¼��Ϣ��", 1, 0);

		// ���
		if (FillShellView(ptmp->ppsf, ptmp->pidlfq, ptmp->item.hItem, obj) == 0)
		{
			if (obj&ISHTYPE_TREE)
			{
				tvscb.hParent     = ptmp->item.hItem;
				tvscb.lParam      = 0;
				tvscb.lpfnCompare = TreeViewCompareProc;
				SortChildrenCB(&tvscb);
				
				// �����ѱ�����־
				ptmp->item.state     |= TVIS_EXPANDEDONCE;
				ptmp->item.stateMask |= TVIS_EXPANDEDONCE;
				ptmp->item.mask      |= TVIF_STATE;
				SetItem(&ptmp->item);
			}
		}
		// ����ѡ���޸�֪ͨ
		PostMessage(WM_TVN_SELCHANGED, (WPARAM)ptmp->item.hItem, 1);
		
		// �ͷ�IShellFolder�ӿ�
		ptmp->ppsf->Release();

		delete ptmp;
	}

	return 0;
}


/****************************************************************************
*
*	����:	EnableImages()
*
*	��;:	������ϵͳ��ͼ���б�ճ�������У�ע����Ϊ��ϵͳ��ͼ���б�����
*           ����Ҫȥ�ͷ�������
*
****************************************************************************/
void CShellTree::EnableImages()
{
    HIMAGELIST	hImageList;
    SHFILEINFO	sfi;

    hImageList = (HIMAGELIST)SHGetFileInfo((LPCSTR)"C:\\", 
                                           0,
                                           &sfi, 
                                           sizeof(SHFILEINFO), 
                                           SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

    // ճ��������
    if (hImageList)
	{
		::SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM) TVSIL_NORMAL, (LPARAM)hImageList);
	}
}


/****************************************************************************
*
*	����:	GetSelectedFolderPath(CString &szFolderPath)
*
*	��;:	��ȡ��ǰѡ�����·�����������ǰѡ������ļ�ϵͳ�У��确�ҵĵ��ԡ���
*           �򱾺�������FALSE��
*
****************************************************************************/
BOOL CShellTree::GetSelectedFolderPath(CString &szFolderPath)
{
	LPTVITEMDATA	lptvid;
	HRESULT			hr;
	BOOL			bRet  = FALSE;
	LPSHELLFOLDER	lpsf2 = NULL;
	HTREEITEM		hItem = NULL;

	static char		szBuff[MAX_PATH];
	
	// ��ȡ��ǰѡ����
	if((hItem = GetSelectedItem()))
	{
		// �õ������˽�����ݽṹ
		lptvid=(LPTVITEMDATA)GetItemData(hItem);

		if (lptvid && lptvid->lpsfParent && lptvid->lpi)
		{
			hr = lptvid->lpsfParent->BindToObject(lptvid->lpi, 0, IID_IShellFolder, (LPVOID *)&lpsf2);

			if (SUCCEEDED(hr))
			{
				ULONG ulAttrs = SFGAO_FILESYSTEM;

				// �жϸ��ļ����Ƿ����ļ�ϵͳ
				lptvid->lpsfParent->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);

				if (ulAttrs & (SFGAO_FILESYSTEM))
				{
					// ������PIDLת��Ϊ�ļ�ϵͳ��·��������������szBuff��������
					if(SHGetPathFromIDList(lptvid->lpifq, szBuff))
					{						
						szFolderPath = szBuff;
						bRet = TRUE;					// ��ȡ��·��������TRUE
					}
				}

				lpsf2->Release();
			}
		}
	}

	return bRet;
}


/****************************************************************************
*
*	����:	GetParentShellFolder(HTREEITEM folderNode)
*
*	��;:	��ȡָ���ļ��еĸ��ļ��нӿڣ�IShellFolder��
*
****************************************************************************/
LPSHELLFOLDER CShellTree::GetParentShellFolder(HTREEITEM folderNode)
{
	LPTVITEMDATA	lptvid;

	// ���ָ����ļ��У���˽�����ݽṹ
	lptvid = (LPTVITEMDATA)GetItemData(folderNode);

	if(lptvid)
		return lptvid->lpsfParent;						// ���ظ��ļ��нӿ�
	else
		return NULL;									// ���û��˽�����ݣ��򷵻�NULL
}


/****************************************************************************
*
*	����:	GetRelativeIDLIST(HTREEITEM folderNode)
*
*	��;:	��ȡָ���ļ��еġ���ԡ�PIDL����������ĸ��ļ��У�
*
****************************************************************************/
LPITEMIDLIST CShellTree::GetRelativeIDLIST(HTREEITEM folderNode)
{
	LPTVITEMDATA	lptvid;

	// ���ָ����ļ��У���˽�����ݽṹ
	lptvid = (LPTVITEMDATA)GetItemData(folderNode);

	if(lptvid)
		return lptvid->lpifq;							// ����ָ���ļ��е����PIDL
	else
		return NULL;									// ��˽�����ݽṹ�򷵻�NULL
}


/****************************************************************************
*
*	����:	GetFullyQualifiedIDLIST(HTREEITEM folderNode)
*
*	��;:	��ȡָ���ļ��еġ����ԡ�PIDL���Ӹ���ʼ��
*
****************************************************************************/
LPITEMIDLIST CShellTree::GetFullyQualifiedIDLIST(HTREEITEM folderNode)
{
	LPTVITEMDATA	lptvid;

	// ���ָ����ļ��У���˽�����ݽṹ
	lptvid = (LPTVITEMDATA)GetItemData(folderNode);

	if(lptvid)
		return lptvid->lpifq;							// ����ָ���ļ��еľ���PIDL
	else
		return NULL;
}


/****************************************************************************
*
*	����:	SearchTree(	HTREEITEM treeNode, CString szSearchName, FindAttribs attr)
*
*	��;:	�����ļ������Ƿ���ָ�������ļ��У�ֻ����һ��������ѡ����
*
*	ע��:	�ú���ֻ�ܹ������ļ�ϵͳ�����Һ�������ʵ�ֱȽϴֲڣ��д��Ľ�
*
****************************************************************************/
bool CShellTree::SearchTree(HTREEITEM treeNode, CString szSearchName, FindAttribs attr)
{
	LPTVITEMDATA	lptvid;
	HRESULT			hr;
	bool			bRet  = FALSE;
	LPSHELLFOLDER	lpsf2 = NULL;

	char	drive[_MAX_DRIVE];
	char	dir[_MAX_DIR];
	char	fname[_MAX_FNAME];
	char	ext[_MAX_EXT];

	CString	szCompare;

	// ��������ת��Ϊ��д
	szSearchName.MakeUpper();

	while(treeNode && bRet==FALSE)
	{
		// ��ȡ���˽�����ݽṹ
		lptvid = (LPTVITEMDATA)GetItemData(treeNode);

		if (lptvid && lptvid->lpsfParent && lptvid->lpi)
		{
			// ��ȡIShellFolder�ӿ�
			hr = lptvid->lpsfParent->BindToObject(lptvid->lpi, 0, IID_IShellFolder, (LPVOID *)&lpsf2);

			if (SUCCEEDED(hr))
			{
				ULONG ulAttrs = SFGAO_FILESYSTEM;

				lptvid->lpsfParent->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);

				// ֻ�е�����ļ�ϵͳʱ�Ž��н�һ���ıȽϲ���
				if (ulAttrs & (SFGAO_FILESYSTEM))
				{
					// ��ȡ���ȫ·��
					if(SHGetPathFromIDList(lptvid->lpifq, szCompare.GetBuffer(MAX_PATH)))
					{
						switch(attr)
						{
						case type_drive:
							// ����·��������ȡ���е���������
							_splitpath(szCompare, drive, dir, fname, ext);
							szCompare = drive;
							break;
						case type_folder:
							// ֻ��ȡ�����ʾ��
							szCompare = GetItemText(treeNode);
							break;
						case	type_path:
							// ����
							break;
						default:
							ASSERT(0);
							break;
						}

						// ת��Ϊ��д�����ڵ������ṩ�Ĵ����бȽ�
						szCompare.MakeUpper();

						if(szCompare == szSearchName)
						{
							// �����ͬ��ȷ������ɼ�����ѡ����
							EnsureVisible(treeNode);
							SelectItem(treeNode);
							bRet = TRUE;
						}
					}
				}
				lpsf2->Release();
			}
		}
		// ȡ����һ���ֵ�����
		treeNode = GetNextSiblingItem(treeNode);
	}

	return bRet;
}


/****************************************************************************
*
*	����:	TunnelTree(CString szFindPath)
*
*	��;:	ʹָ�����ļ������ѡ��״̬���ɼ�����Ҫʱ�����չ������
*
*	ע��:	�ú����Ƚϴֲڣ�ֻ�ܹ������ļ�ϵͳ���ļ��У��������ĸ������
*           �����档
*
****************************************************************************/
void CShellTree::TunnelTree(CString szFindPath)
{
	HTREEITEM		subNode = GetRootItem();
	CString			szPathHop;
	LPSHELLFOLDER	lpsf   = NULL;
	LPITEMIDLIST	lpi    = NULL;
	
	char			drive[_MAX_DRIVE];
	char			dir[_MAX_DIR];
	char			fname[_MAX_FNAME];
	char			ext[_MAX_EXT];
	char			delimiter[]="\\";

	char			szBuff[MAX_PATH];

	if (szFindPath.IsEmpty())
	{
		return;
	}

	if(szFindPath.ReverseFind('\\') != szFindPath.GetLength()-1)
	{
		szFindPath += "\\";
	}

	// ����·����
	_splitpath(szFindPath, drive, dir, fname, ext);

	// ������������
	szPathHop = drive;

	// ��ȡ�����ļ��нӿ�
	if (FAILED(SHGetDesktopFolder(&lpsf)))
	{
		return;
	}
	
	// ��ȡ���ҵĵ��ԡ�Ԫ�صı�ʶ���б�PIDL��
	if (FAILED(SHGetSpecialFolderLocation(0, CSIDL_DRIVES, &lpi)))
	{
		lpsf->Release();
		return;
	}

	// ��ȡԪ����ʾ��
	if (GetName(lpsf, lpi, SHGDN_NORMAL, szBuff) == 0)
	{
		g_malloc->Free(lpi);
		lpsf->Release();
		return;
	}
	
	// �ͷ���Դ
	g_malloc->Free(lpi);
	lpsf->Release();

	// ��ȡ���ҵĵ��ԡ��ļ��е�������
	if ((subNode=SearchByText(subNode, CString(szBuff))) == NULL)
	{
		return;
	}

	// �ļ������
	Expand(subNode, TVE_EXPAND);

	// ��ȡ��һ��������������
	if((subNode=GetChildItem(subNode)) != NULL)
	{
		// �����������ʼ��ѡ������
		if(SearchTree(subNode, szPathHop, CShellTree::type_drive))
		{
			char *p = strtok(dir, delimiter);

			while(p)
			{
				// ��ȡѡ����
				subNode = GetSelectedItem();

				// ���
				Expand(subNode, TVE_EXPAND);

				// ��ȡ��һ�����ļ�����
				subNode = GetChildItem(subNode);

				if(SearchTree(subNode, p, CShellTree::type_folder))
				{
					// �ҵ�һ���󣬽�p��λ����һ���ļ���
					p = strtok(NULL, delimiter);
				}
				else
				{
					p = NULL;
				}
			}
		}
	}
}


/****************************************************************************
*
*	����:	SearchByText(HTREEITEM hti, CString szFindText)
*
*	��;:	������һ���ļ������Ƿ���ָ����ʾ�������ļ��У�������򷵻���������
*
****************************************************************************/
HTREEITEM CShellTree::SearchByText(HTREEITEM hti, CString szFindText)
{
	HTREEITEM	hRet = NULL;
	HTREEITEM	hChildItem;
	
	// �ж��Ƿ�������
	if (ItemHasChildren(hti))
	{
		hChildItem = GetChildItem(hti);
		
		while (hChildItem != NULL)
		{
			// �Ƚ����ı�����ʾ����
			if (GetItemText(hChildItem) == szFindText)
			{
				hRet = hChildItem;
				break;
			}

			hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
		}
	}

	return hRet;
}


/****************************************************************************
*
*	����:	GetPathFromHTREEITEM(HTREEITEM folderNode, LPTSTR szBuff)
*
*	��;:	����ָ�������������ȫ·����szBuff�ĳߴ类����ΪMAX_PATH
*
****************************************************************************/
int CShellTree::GetPathFromHTREEITEM(HTREEITEM folderNode, LPTSTR szBuff)
{
	// ��ȡ����˽�����ݽṹ�еľ���PIDL�������ͷţ�
	LPITEMIDLIST	pidlfq = GetFullyQualifiedIDLIST(folderNode);

	if (pidlfq == NULL)
	{
		return -1;										// ��Ч���������������
	}

	if (SHGetPathFromIDList(pidlfq, szBuff) == FALSE)
	{
		return -2;										// ת��ʧ��
	}

	return 0;
}


/****************************************************************************
*
*	����:	OnUpdateItem(WPARAM wParam, LPARAM lParam)
*
*	��;:	�����汻ˢ��֮����ã��Ը����б�������
*
****************************************************************************/
int CShellTree::OnUpdateItem(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM		hItem = GetSelectedItem();
	LPTVITEMDATA	pda;

	if (hItem)
	{
		pda = (LPTVITEMDATA)GetItemData(hItem);			// ȡ�����������

		if (pda)
		{
			LPSHELLFOLDER  lpsf2 = NULL;
			
			// ��ȡѡ����Shell�ӿ�
			if (SUCCEEDED(pda->lpsfParent->BindToObject(pda->lpi, 0, IID_IShellFolder, (LPVOID *)&lpsf2)))
			{
				// ����ļ�����Ϣ
				FillShellView(lpsf2, pda->lpifq, 0, ISHTYPE_LIST);
				lpsf2->Release();
			}
		}
	}

	return 0;
}
