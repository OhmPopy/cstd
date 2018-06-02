/********************************************************************

	ShellTree.h

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
					�ඨ���ļ���

	���ļ���д�ˣ�	
					Selom Ofori		none
					YZ				yzfree##sina.com

	���ļ��汾��	30119
	����޸��ڣ�	2003-1-19

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������
	----------------------------------------------------------------
	����ע�ͣ�

	���ļ���ԭʼ����Ϊ Selom Ofori��Դ��û��������ơ�����YZ��д������
	ISee��Ŀ�����֤��ΪGPL���� Selom Ofori û��������ϵ��ַ������δ��
	�������������

	������ʷ��

		2003-1		��һ�����԰淢��

********************************************************************/

#if !defined(AFX_SHELLTREE_H__6B1818E3_8ADA_11D1_B10E_40F603C10000__INCLUDED_)
#define AFX_SHELLTREE_H__6B1818E3_8ADA_11D1_B10E_40F603C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <shlobj.h>
#include "ShellPidl.h"

class CShellList;
class CShellPath;

#define	SHELLTREE_DEFER			60						// �ӳ�ʱ�䣨10���뵥λ��

class CShellTree : public CTreeCtrl, public CShellPidl
{
public:
	enum FindAttribs{type_drive,type_folder,type_path};


protected:
	typedef struct _tagTREEMSGPACK
	{
		LPSHELLFOLDER	ppsf;							// ��ǰ�����IShellFolder�ӿ�
		LPITEMIDLIST	pidlfq;							// ��ǰ����ľ���PIDL
		TVITEM			item;							// ��ǰ�����TVITEM�ṹ����
	} TREEMSGPACK, *LPTREEMSGPACK;


// Construction
public:
	CShellTree();

	int		InitShellTree(CShellList *psl, CShellPath *psp);
	void	CloseShellTree(void);
	
// Attributes
public:
	void	GetContextMenu(NMHDR* pNMHDR, LRESULT* pResult);
	void	EnableImages();
	BOOL	GetSelectedFolderPath(CString &szFolderPath);

	LPSHELLFOLDER	GetParentShellFolder(HTREEITEM folderNode);
	LPITEMIDLIST	GetRelativeIDLIST(HTREEITEM folderNode);
	LPITEMIDLIST	GetFullyQualifiedIDLIST(HTREEITEM folderNode);
	int				GetPathFromHTREEITEM(HTREEITEM folderNode, LPTSTR szBuff);

// Operations
public:

	int 	PopulateTree(CString &);

	bool	SearchTree(HTREEITEM treeNode, CString szSearchName, FindAttribs attr);
	void	TunnelTree(CString szFindPath);

	void	OnFolderExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	BOOL	OnFolderSelected(NMHDR* pNMHDR, LRESULT* pResult, CString &szFolderPath);
	void	OnDeleteShellItem(NMHDR* pNMHDR, LRESULT* pResult);
	LRESULT OnDeferFillAtt(WPARAM wParam, LPARAM lParam);
	int		OnUpdateItem(WPARAM wParam, LPARAM lParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShellTree();

// Generated message map functions
protected:
	const enum ISHFILLTYPE{ ISHTYPE_TREE = 1, ISHTYPE_LIST = 2	};

	int 		FillShellView(LPSHELLFOLDER lpsf, LPITEMIDLIST lpifq, HTREEITEM hParent, int list);
	void		GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem);
	HTREEITEM	SearchByText(HTREEITEM hti, CString szFindText);

 	static  int CALLBACK TreeViewCompareProc(LPARAM, LPARAM, LPARAM);

	
private:
	static UINT ShellTProc(LPVOID parm);

	static CWinThread			*g_pwt;
	static LPMALLOC				g_malloc;
	
	static CShellList			*g_pList;
	static CShellPath			*g_pPath;

protected:
	//{{AFX_MSG(CShellTree)
	afx_msg LRESULT OnDeferFill(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	friend class CShellList;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLTREE_H__6B1818E3_8ADA_11D1_B10E_40F603C10000__INCLUDED_)
