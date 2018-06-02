/********************************************************************

	ShellPath.cpp

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
	���ļ���;��	ͼ���д���棭C���԰棭VC���������������shell path
					��ʵ���ļ���

	���ļ���д�ˣ�	
					YZ				yzfree##sina.com

	���ļ��汾��	30309
	����޸��ڣ�	2003-3-9

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������

	������ʷ��

		2003-3		����CShellImageList�࣬�Լӿ��ļ�ͼ�����ȡ�ٶ�
		2003-2		��һ�����԰淢��

********************************************************************/

#include "stdafx.h"
#include "iseetsvc.h"

#include "ShellPidl.h"
#include "ShellTree.h"
#include "ShellList.h"
#include "ShellPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CShellImageList

CShellImageList::CShellImageList()
{
	m_info.RemoveAll();

	m_cLIcon.m_hImageList = 0;
	m_cSIcon.m_hImageList = 0;
}


CShellImageList::~CShellImageList()
{
	m_info.RemoveAll();

	if (m_cLIcon.m_hImageList)
	{
		m_cLIcon.DeleteImageList();
	}

	if (m_cSIcon.m_hImageList)
	{
		m_cSIcon.DeleteImageList();
	}
}


// ��ʼ��ͼ���б���
int CShellImageList::OpenImageList()
{
	ASSERT(m_cLIcon.m_hImageList == 0);

	m_info.RemoveAll();
	m_info.SetSize(0, CSIL_ICONGROW);

	m_iLIconCX = GetSystemMetrics(SM_CXICON);			// ��ȡϵͳ��Сͼ��ĳߴ�
	m_iLIconCY = GetSystemMetrics(SM_CYICON);
	m_iSIconCX = GetSystemMetrics(SM_CXSMICON);
	m_iSIconCY = GetSystemMetrics(SM_CYSMICON);

	if (!m_cLIcon.Create(m_iLIconCX, m_iLIconCY, ILC_COLOR32|ILC_MASK, CSIL_MAXREVICONUM, CSIL_ICONGROW))
	{
		return -1;										// ��ͼ��ͼ���б���ʧ��
	}

	if (!m_cSIcon.Create(m_iSIconCX, m_iSIconCY, ILC_COLOR32|ILC_MASK, CSIL_MAXREVICONUM, CSIL_ICONGROW))
	{
		m_cLIcon.DeleteImageList();
		m_cLIcon.m_hImageList = 0;
		return -2;										// Сͼ��ͼ���б���ʧ��
	}

	m_cLIcon.SetBkColor(CLR_NONE);
	m_cSIcon.SetBkColor(CLR_NONE);

	m_cLIcon.SetImageCount(CSIL_MAXREVICONUM);			// Ԥ��CSIL_MAXREVICONUM������ͼ��λ��
	m_cSIcon.SetImageCount(CSIL_MAXREVICONUM);

	m_cLIcon.Replace(CSILIDX_VDOC, AfxGetApp()->LoadIcon(IDI_SHELLICON1));
	m_cLIcon.Replace(CSILIDX_VEXE, AfxGetApp()->LoadIcon(IDI_SHELLICON2));

	m_cSIcon.Replace(CSILIDX_VDOC, AfxGetApp()->LoadIcon(IDI_SHELLICON1));
	m_cSIcon.Replace(CSILIDX_VEXE, AfxGetApp()->LoadIcon(IDI_SHELLICON2));

	return 0;
}


// �ر�ͼ���б���
int CShellImageList::CloseImageList()
{
	m_info.RemoveAll();
	
	if (m_cLIcon.m_hImageList)
	{
		m_cLIcon.DeleteImageList();
		m_cLIcon.m_hImageList = 0;
	}
	
	if (m_cSIcon.m_hImageList)
	{
		m_cSIcon.DeleteImageList();
		m_cSIcon.m_hImageList = 0;
	}

	return 0;
}


// ��ȡָ���������б��е�ͼ��������
int CShellImageList::GetIconIndex(HWND hwnd, LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, LPCSTR filename)
{
	static int	iMdiPos = 0;							// ����ͼ�������ͼ��������ֽ��

	ASSERT(lpsf&&lpi&&filename);

	IExtractIcon	*piei;
	
	char	szFileIcon[MAX_PATH];
	INT		fiIndex;
	UINT	fiComp;
	int		index = -3;

	// ��ȡ�����IExtractIcon�ӿ�
	if (FAILED(lpsf->GetUIObjectOf(hwnd, 1, (const struct _ITEMIDLIST **)&lpi, IID_IExtractIcon, 0, (LPVOID*)&piei)))
	{
		return -1;
	}

	// ��ȡ����ͼ�����ڵ�λ��
	if (FAILED(piei->GetIconLocation(GIL_FORSHELL, (LPSTR)szFileIcon, MAX_PATH, &fiIndex, &fiComp)))
	{
		piei->Release();
		return -2;
	}

	HICON	hLIcon;
	HICON	hSIcon;
	SHELLICONINFO	cShellIcon;
	
	cShellIcon.index = -1;
	cShellIcon.szFileExt[0] = '\0';

	// ֱ�Ӵ�����Щӵ���Լ�ͼ��Ķ���
	if (!(fiComp & GIL_PERCLASS))
	{
		// ���������Ͷ����ͼ���Ƿ��ѱ���¼
		for (int i=iMdiPos; i<m_info.GetSize(); i++)
		{
			if (lstrcmpi((LPCSTR)m_info[i].szFileExt, (LPCSTR)filename) == 0)
			{
				piei->Release();
				return m_info[i].index;
			}
		}
		
		// ��ȡͼ��
		if (piei->Extract((LPCSTR)szFileIcon, fiIndex, &hLIcon, &hSIcon, (m_iLIconCX&0xffff)|((m_iSIconCX<<16)&0xffff0000)) != NOERROR)
		{
			piei->Release();
			return CShellImageList::CSILIDX_VEXE;
		}

		// �����ͼ���б�
		if ((index=m_cLIcon.Add(hLIcon)) == -1)
		{
			piei->Release();
			DestroyIcon(hLIcon);
			DestroyIcon(hSIcon);
			return CShellImageList::CSILIDX_VEXE;
		}

		// ����Сͼ���б�
		if (index != m_cSIcon.Add(hSIcon))
		{
			m_cLIcon.Remove(index);
			piei->Release();
			DestroyIcon(hLIcon);
			DestroyIcon(hSIcon);
			return CShellImageList::CSILIDX_VEXE;
		}

		// �ͷŴ�ϵͳ��õ�ͼ����
		DestroyIcon(hLIcon);
		DestroyIcon(hSIcon);

		cShellIcon.index = index;

		// ���������Ķ�������
		lstrcpy((LPSTR)cShellIcon.szFileExt, (LPCSTR)filename);
		
		// ��ͼ�����������������Ϣ���м�¼
		m_info.Add(cShellIcon);
	}
	else
	{
		// ��λ������չ����λ��
		char *p = strrchr((const char *)filename, (int)(unsigned int)(uchar)'.');

		// ���������Ͷ����ͼ���Ƿ��ѱ���¼
		for (int i=0; i<iMdiPos; i++)
		{
			if (lstrcmpi((LPCSTR)m_info[i].szFileExt, (LPCSTR)p) == 0)
			{
				piei->Release();
				return m_info[i].index;
			}
		}

		// ��ȡͼ��
		if (piei->Extract((LPCSTR)szFileIcon, fiIndex, &hLIcon, &hSIcon, (m_iLIconCX&0xffff)|((m_iSIconCX<<16)&0xffff0000)) != NOERROR)
		{
			piei->Release();
			return CShellImageList::CSILIDX_VDOC;
		}
		
		// �����ͼ���б�
		if ((index=m_cLIcon.Add(hLIcon)) == -1)
		{
			piei->Release();
			DestroyIcon(hLIcon);
			DestroyIcon(hSIcon);
			return CShellImageList::CSILIDX_VDOC;
		}
		
		// ����Сͼ���б�
		if (index != m_cSIcon.Add(hSIcon))
		{
			m_cLIcon.Remove(index);
			piei->Release();
			DestroyIcon(hLIcon);
			DestroyIcon(hSIcon);
			return CShellImageList::CSILIDX_VDOC;
		}

		// �ͷŴ�ϵͳ��õ�ͼ����
		DestroyIcon(hLIcon);
		DestroyIcon(hSIcon);
		
		cShellIcon.index = index;

		// ֻ�������ͼ��������չ����
		lstrcpy((LPSTR)cShellIcon.szFileExt, (LPCSTR)p);
		
		// ��ͼ�����������������Ϣ���м�¼
		m_info.InsertAt(0, cShellIcon);

		// ���·ֽ��(��ͼ��������ͼ��ķֽ��)
		iMdiPos++;
	}

	piei->Release();

	return index;
}



/////////////////////////////////////////////////////////////////////////////
// CShellPath

IMPLEMENT_DYNCREATE(CShellPath, CCmdTarget)

CShellPath::CShellPath()
{
	m_pTree = 0;
	m_pList = 0;
	m_pFwnd = 0;

	m_iCurrIdx      = -1;
	m_pCurrItemData = 0;
	
	a_bChangeTag = FALSE;
	m_sCurrPath.Empty();
	m_sCurrFile.Empty();
	a_cPath.Empty();

	a_bChangeTag = FALSE;								// ��������־
	a_dParam1    = 0;									// ���Ӳ���1
	a_dParam2    = 0;									// ���Ӳ���2
	a_dNumber    = 0;									// ��ˮ��
	a_dSN        = 0;									// ���κ�

	a_cPath.Empty();									// ·��+�ļ���
	a_cName.Empty();									// ��������
}

CShellPath::~CShellPath()
{
	m_pTree = 0;
	m_pList = 0;
	m_pFwnd = 0;
}


BEGIN_MESSAGE_MAP(CShellPath, CCmdTarget)
	//{{AFX_MSG_MAP(CShellPath)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CShellPath message handlers

// ��ʼ��ShellPath����
int CShellPath::InitShellPath(CTreeCtrl *ptc, CListCtrl *plc, CFrameWnd *pfw, CString filter)
{
	int		ret = 0;

	ASSERT(ptc&&plc&&pfw);

	if (m_cImageList.OpenImageList())
	{
		return -1;
	}

	m_pTree   = (CShellTree*)ptc;						// ���ؼ�ָ��
	m_pList   = (CShellList*)plc;						// �б�ؼ�ָ��
	m_pFwnd   = (CFrameWnd*)pfw;						// ��ܴ���ָ��
	
	// ��ʼ�����ؼ�
	if ((ret=m_pTree->InitShellTree(m_pList, this)) != 0)
	{
		return ret;
	}

	// ��ʼ���б�ؼ�
	if ((ret=m_pList->InitShellList(m_pTree, this, filter)) != 0)
	{
		return ret;
	}

	// �������ؼ�ͼ��
	m_pTree->EnableImages();

	// �����б�ؼ�ͼ��
	m_pList->EnableImages();

	m_sCurrPath.Empty();
	m_sCurrFile.Empty();

	a_bChangeTag = FALSE;								// ��������־
	a_dParam1    = 0;									// ���Ӳ���1
	a_dParam2    = 0;									// ���Ӳ���2
	a_dSN        = 0;									// ���κ�
	
	a_cPath.Empty();									// ·��+�ļ���
	a_cName.Empty();									// ��������
	
	return ret;
}


// �ر�ShellPath����
void CShellPath::CloseShellPath()
{
	m_pList->CloseShellList();
	m_pTree->CloseShellTree();

	m_pTree = 0;
	m_pList = 0;
	m_pFwnd = 0;

	m_cImageList.CloseImageList();
}


// ����ShellPath��ĳ�ʼ·��
int CShellPath::SetInitPath(LPCSTR path)
{
	ASSERT(m_sCurrPath.IsEmpty());						// �ú��������ظ�����
	ASSERT(m_pTree);

	m_sCurrPath = path;

	// ��װ��
	return m_pTree->PopulateTree(m_sCurrPath);
	//0     - �ɹ�
	//-1    - ʧ�ܣ�δ�ܻ�ȡ�����ļ��нӿ�
	//-2    - ʧ�ܣ�δ�ܻ�ȡ�����ļ��е�PIDL
	//-3    - ʧ�ܣ�δ�ܻ�ȡ�����ļ�����ʾ��
}


// ��ȡ��ǰ·��
CString CShellPath::GetCurrPath()
{
	return m_sCurrPath;
}


// ��ȡ���ؼ�ָ��
inline CShellTree *CShellPath::GetShellTree()
{
	return m_pTree;
}


// ��ȡ�б�ؼ�ָ��
inline CShellList *CShellPath::GetShellList()
{
	return m_pList;
}


// ��ȡ��ǰ���ļ����˴�
CString	&CShellPath::GetFilterString()
{
	return m_pList->GetFilterString();
}


// �����ļ����˴�
void CShellPath::SetFilterString(CString &nsp)
{
	m_pList->SetFilterString(nsp);
}


// ·�����ļ�������
void CShellPath::ObjectChanged(LPCTSTR path, LPCTSTR fname, int idx)
{
	ASSERT(path||fname);

	// �ϳɱ�����ݣ�·�����ļ�������ͬʱ�����
	if (path)
	{
		m_sCurrPath = path;
		m_sCurrFile = "";
		m_iCurrIdx  = -1;
			
		a_cName     = "";
		a_cPath     = "";
		a_dParam1   = 0;
		a_dParam2   = 0;
		a_dNumber   ++;
		a_dSN       = idx;								// ��ǰ���κ�

		m_pCurrItemData = 0;
	}
	else if (fname)
	{
		ASSERT(!m_sCurrPath.IsEmpty());
		m_sCurrFile = fname;
		m_iCurrIdx  = idx;

		a_cName     = m_sCurrFile;
		a_cPath     = m_sCurrPath + m_sCurrFile;
		a_dParam1   = 0;
		a_dParam2   = 0;
		a_dNumber   ++;

		// ȡ�ø��ļ����б���˽������
		m_pCurrItemData = (LPLVITEMDATA)m_pList->GetItemData(idx);
	}

	a_bChangeTag= TRUE;

	// �ӳټ�ʱ��ʼ
	a_cTime.BeginCount(ISSHELLPATH_MAX_IDLTIM);
}

