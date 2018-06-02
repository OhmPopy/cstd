/********************************************************************

	MainFrm.h

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
					�ඨ���ļ���

	���ļ���д�ˣ�	
					YZ				yzfree##sina.com

	���ļ��汾��	30112
	����޸��ڣ�	2003-1-12

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������

	������ʷ��

		2003-1		��һ�����԰淢��

********************************************************************/

#if !defined(AFX_MAINFRM_H__93899ABB_8D39_435E_935E_B7EBD5994DA0__INCLUDED_)
#define AFX_MAINFRM_H__93899ABB_8D39_435E_935E_B7EBD5994DA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "newspliwnd.h"
#include "shellpath.h"


class CRightView;


class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CNewSpliWnd		m_spa;								// ���Ѵ���A
	CNewSpliWnd		m_spb;								// ���Ѵ���B

// Operations
public:
	BOOL OnIdleProc(LONG count);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
	CRightView* GetRightPane();
	CLeftView*	GetLeftPane();
	CDisplay*	GetDisplayPane();


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CStatusBar		m_wndStatusBar;
	CProgressCtrl	m_wndProgress;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnVatCenter();
	afx_msg void OnUpdateVatCenter(CCmdUI* pCmdUI);
	afx_msg void OnVatLd();
	afx_msg void OnUpdateVatLd(CCmdUI* pCmdUI);
	afx_msg void OnVatLt();
	afx_msg void OnUpdateVatLt(CCmdUI* pCmdUI);
	afx_msg void OnVatRd();
	afx_msg void OnUpdateVatRd(CCmdUI* pCmdUI);
	afx_msg void OnVatRt();
	afx_msg void OnUpdateVatRt(CCmdUI* pCmdUI);
	afx_msg void OnViewPlaytype();
	afx_msg void OnUpdateViewPlaytype(CCmdUI* pCmdUI);
	afx_msg void OnVrtAuto();
	afx_msg void OnUpdateVrtAuto(CCmdUI* pCmdUI);
	afx_msg void OnVrtL200();
	afx_msg void OnUpdateVrtL200(CCmdUI* pCmdUI);
	afx_msg void OnVrtL300();
	afx_msg void OnUpdateVrtL300(CCmdUI* pCmdUI);
	afx_msg void OnVrtL400();
	afx_msg void OnUpdateVrtL400(CCmdUI* pCmdUI);
	afx_msg void OnVrtOrg();
	afx_msg void OnUpdateVrtOrg(CCmdUI* pCmdUI);
	afx_msg void OnVrtS20();
	afx_msg void OnUpdateVrtS20(CCmdUI* pCmdUI);
	afx_msg void OnVrtS40();
	afx_msg void OnUpdateVrtS40(CCmdUI* pCmdUI);
	afx_msg void OnVrtS60();
	afx_msg void OnUpdateVrtS60(CCmdUI* pCmdUI);
	afx_msg void OnVrtS80();
	afx_msg void OnUpdateVrtS80(CCmdUI* pCmdUI);
	afx_msg void OnViewPlugininfo();
	afx_msg void OnUpdateViewPlugininfo(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__93899ABB_8D39_435E_935E_B7EBD5994DA0__INCLUDED_)
