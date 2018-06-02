/********************************************************************

	iseetsvcDoc.h

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
					�ඨ���ļ���

	���ļ���д�ˣ�	
					YZ				yzfree##sina.com

	���ļ��汾��	30118
	����޸��ڣ�	2003-1-18

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������

	������ʷ��

		2003-1		��һ�����԰淢��

********************************************************************/

#if !defined(AFX_ISEETSVCDOC_H__68859D6C_A067_4883_8E30_0761C83E4C50__INCLUDED_)
#define AFX_ISEETSVCDOC_H__68859D6C_A067_4883_8E30_0761C83E4C50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CIseetsvcDoc : public CDocument
{
protected: // create from serialization only
	CIseetsvcDoc();
	DECLARE_DYNCREATE(CIseetsvcDoc)

// Attributes
public:
	CShellPath		m_spath;							// ShellPath����

	LPSPEC_DATAPACK	m_pPack;							// ��ǰרҵ�����ݰ�
	BOOL			m_bPkst;							// ��ǰ���ݰ��ɲ���״̬
	BITMAPINFO_EX	m_dInfo;							// ��ʾ����Ϣ

// Operations
public:
	BOOL OnIdleProc(LONG count);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIseetsvcDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIseetsvcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	static CDisplay		*m_pView;						// ����
	static HWND			m_hView;						// �Ӵ��ھ��

	static int __cdecl _cbfun(LPSPEC_DATAPACK psdp, PFUN_COMM comm, int imgnum, int sl, int lcnt, int rw);
	

// Generated message map functions
protected:
	//{{AFX_MSG(CIseetsvcDoc)
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISEETSVCDOC_H__68859D6C_A067_4883_8E30_0761C83E4C50__INCLUDED_)
