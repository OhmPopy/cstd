/********************************************************************

	ShellPath.h

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
					�ඨ���ļ���

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

#if !defined(AFX_SHELLPATH_H__7549C0D3_70B0_49BE_9E8D_7B7DA9DE832D__INCLUDED_)
#define AFX_SHELLPATH_H__7549C0D3_70B0_49BE_9E8D_7B7DA9DE832D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ShellPath.h : header file

// ����������
class CShellTree;
class CShellList;

/////////////////////////////////////////////////////////////////////////////
// CShellImageList

// �б�����ʹ�õ�ͼ���б���
class CShellImageList
{
public:
	enum {CSIL_MAXREVICONUM = 8, CSIL_ICONGROW = 256};
	enum {CSILIDX_VDOC = 0, CSILIDX_VEXE};

	CShellImageList();
	~CShellImageList();

	int OpenImageList(void);
	int CloseImageList(void);

	CImageList *GetLIconList(void){return &m_cLIcon;};
	CImageList *GetSIconList(void){return &m_cSIcon;};

	int GetIconIndex(HWND hwnd, LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, LPCSTR filename);

private:
	typedef struct _tagSHELLICONINFO
	{
		char	szFileExt[MAX_PATH];
		int		index;
	} SHELLICONINFO, *PSHELLICONINFO;

	int			m_iLIconCX;
	int			m_iLIconCY;
	int			m_iSIconCX;
	int			m_iSIconCY;

	CArray<SHELLICONINFO, SHELLICONINFO&>	m_info;		// ��չ����ͼ��������Ӧ��ϵ��

	CImageList	m_cLIcon;								// ��ͼ���б�
	CImageList	m_cSIcon;								// Сͼ���б�
};


/////////////////////////////////////////////////////////////////////////////
// CShellPath command target

enum {ISSHELLPATH_MAX_IDLTIM = 10};						// �ļ�ѡ����������ӳ�ʱ��


class CShellPath : public CCmdTarget
{
	DECLARE_DYNCREATE(CShellPath)

public:
	CShellPath();           // protected constructor used by dynamic creation
	virtual ~CShellPath();
	
// Attributes
public:
	CShellImageList	m_cImageList;						// �б�ؼ�ͼ���б�

	CShellTree		*m_pTree;							// ���ؼ�ָ��   
	CShellList		*m_pList;							// �б�ؼ�ָ�� 
	CFrameWnd		*m_pFwnd;							// ��ܴ���ָ�� 

	int				m_iCurrIdx;							// ��ǰ�ļ����б����е�����
	LPLVITEMDATA	m_pCurrItemData;					// ��ǰ�ļ����б���˽�нṹ��ַ
	CString			m_sCurrPath;						// ��ǰ·��
	CString			m_sCurrFile;						// ��ǰ�ļ���

	BOOL					a_bChangeTag;				// ��������־
	CString					a_cName;					// ��������
	CString					a_cPath;					// ·��+�ļ���
	DWORD					a_dParam1;					// ���Ӳ���1
	DWORD					a_dParam2;					// ���Ӳ���2
	CTimeCounter			a_cTime;					// �ӳټ�ʱ
	DWORD					a_dNumber;					// ��ˮ��
	DWORD					a_dSN;						// ���κ�


// Operations
public:
	int  InitShellPath(CTreeCtrl *ptc, CListCtrl *plc, CFrameWnd *pfw, CString filter);
	void CloseShellPath(void);

	int         SetInitPath(LPCSTR path);
	CString     GetCurrPath(void);

	CShellTree *GetShellTree(void);
	CShellList *GetShellList(void);
	
	CString	   &GetFilterString(void);
	void        SetFilterString(CString &nsp);

	void		ObjectChanged(LPCTSTR path, LPCTSTR fname, int idx = -1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellPath)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShellPath)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLPATH_H__7549C0D3_70B0_49BE_9E8D_7B7DA9DE832D__INCLUDED_)
