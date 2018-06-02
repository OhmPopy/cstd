/********************************************************************

	ShellList.h

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
					�ؼ��ඨ���ļ���

	���ļ���д�ˣ�	
					YZ				yzfree##sina.com

	���ļ��汾��	30220
	����޸��ڣ�	2003-2-20

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������

	������ʷ��

		2003-2		��һ�����԰淢��

********************************************************************/

#if !defined(AFX_SHELLLIST_H__F535BD83_1ACB_4492_8ED1_CD1BF1171241__INCLUDED_)
#define AFX_SHELLLIST_H__F535BD83_1ACB_4492_8ED1_CD1BF1171241__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ShellList.h : header file
//
#include <shlobj.h>
#include "ShellPidl.h"

class CShellTree;
class CShellPath;


/////////////////////////////////////////////////////////////////////////////
// CShellList window

class CShellList : public CListCtrl, public CShellPidl
{
// Construction
public:
	CShellList();

	enum {MAX_SUBITEMCNT = 1};							// ����̷߳�����Ϣ��������
	enum {HITTEST_POS_X = 4, HITTEST_POS_Y = 4};		// ���Ե�
	enum {COLNUM = 4};									// �б��ӵ�����
	enum {APTIMEUNIT = 10};								// �Զ������ӳ���С��λ�����룩

	enum Cols											// �б�������
	{
		COL_NAME,
		COL_SIZE,
		COL_TYPE,
		COL_NOTE,
		COL_MAX
	};

	typedef struct _tagSibCol							// �в�������
	{
		enum Cols	index;
		char		name[64];
		int			ali;
		int			width;
	} SIBCOL, *LPSIBCOL;


// Attributes
public:
	int		InitShellList(CShellTree *pst, CShellPath *psp, CString filter);
	void	CloseShellList(void);
	
	void	EnableImages(void);

	LRESULT OnFillBegin(WPARAM wParam, LPARAM lParam);
	LRESULT OnFillEnd(WPARAM wParam, LPARAM lParam);
	LRESULT OnSubItemDataFill(WPARAM wParam, LPARAM lParam);
	LRESULT OnAutoNext(WPARAM wParam, LPARAM lParam);

	CString	&GetFilterString(void);
	void	SetFilterString(CString &nsp);
	BOOL	IsDestFile(LPCTSTR fname);

	int		GetAutoPlay(void);
	void	SetAutoPlay(int type);

// Operations
public:

private:
	static CWinThread	*m_pThread;						// ����߳�

	static int			m_iSN;							// ��ǰ���κ�
	static int			m_iNum;							// ��ǰ���ε��ļ��ܸ���
	static int			m_iCount;						// δ���������������
	static int			m_iCurrPt;						// ˳��ʽ����һ�����ܵ�����

	static int			m_iAutoPlayTime;				// �Զ�����ͼ����ӳ�ʱ�䣨�룩
	static int			m_iAutoPlay;					// �Զ����ű�־����0Ϊ�Զ����ţ�

	static UINT ShellLProc(LPVOID parm);

	static int  CALLBACK CFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);


// Overrides
	
// Overrides	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShellList();

	// Generated message map functions
	//{{AFX_MSG(CShellList)
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	static CShellTree			*m_pTree;				// ���ؼ�ָ��
	static CShellPath			*m_pPath;				// ����ָ��
	static CString				m_sFilter;				// �б�����˴�
	static CString				m_sCurrPath;			// ��ǰ·��
	
	static SIBCOL				sc[COLNUM];

	friend class CShellTree;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLLIST_H__F535BD83_1ACB_4492_8ED1_CD1BF1171241__INCLUDED_)
