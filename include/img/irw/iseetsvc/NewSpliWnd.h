/********************************************************************

	NewSpliWnd.h

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
	���ļ���;��	ͼ���д���棭C���԰棭VC����������������·��Ѵ���
					�ඨ���ļ���

	���ļ���д�ˣ�	
					YZ				yzfree##sina.com

	���ļ��汾��	30122
	����޸��ڣ�	2003-1-22

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������

	������ʷ��

		2003-1		��һ�����԰淢��

********************************************************************/

#if !defined(AFX_NEWSPLIWND_H__204CD775_5896_403B_B353_025F36CD77FF__INCLUDED_)
#define AFX_NEWSPLIWND_H__204CD775_5896_403B_B353_025F36CD77FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewSpliWnd.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CNewSpliWnd frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define CNSW_FILL_WIDTH_CURTAIL			(0)
#define CNSW_FILL_HEIGHT_CURTAIL		(-2)
#define CNSW_FILL_WIDTH_MINI			(4)
#define CNSW_FILL_HEIGHT_MINI			(16)

#define CNSW_LINE_CURTAIL				(6)


class CNewSpliWnd : public CSplitterWnd
{
	DECLARE_DYNCREATE(CNewSpliWnd)
public:
	CNewSpliWnd();           // protected constructor used by dynamic creation

// Attributes
protected:
	int		count;
	CRect	rc;

public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewSpliWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg);
	virtual void OnInvertTracker(const CRect& rect);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNewSpliWnd();

	// Generated message map functions
	//{{AFX_MSG(CNewSpliWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWSPLIWND_H__204CD775_5896_403B_B353_025F36CD77FF__INCLUDED_)
