/********************************************************************

	FileDialogEx.h

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
	���ļ���;��	ͼ���д���棭C���԰棭VC�������������������
					�Ի����ඨ���ļ���

	���ļ���д�ˣ�	
					leye			louxiulin##263.net
					YZ				yzfree##sina.com

	���ļ��汾��	30528
	����޸��ڣ�	2003-5-28

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������

	������ʷ��

		2003-5		��һ�����԰淢��

********************************************************************/

#if !defined(AFX_FILEDIALOGEX_H__7C8C7557_C74F_11D3_B2C1_0020AF694846__INCLUDED_)
#define AFX_FILEDIALOGEX_H__7C8C7557_C74F_11D3_B2C1_0020AF694846__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __ISEE_IRW_INC__
#include "iseeirw.h"
#endif // __ISEE_IRW_INC__

/////////////////////////////////////////////////////////////////////////////
// ����

#define DEFAULTEXT						("bmp")

#define ISD_COMBOBOXLIST_HEIGHT			100
#define ISD_CTLCNT						(ISD_ITEM_NUM+1)

/////////////////////////////////////////////////////////////////////////////
// �¿ؼ�ID����
#define IDC_SAVEASCNT_STC				2012
#define IDC_SAVEASCNT_CON				(IDC_SAVEASCNT_STC+ISD_ITEM_NUM+1)


/////////////////////////////////////////////////////////////////////////////
// �б��湦�ܵĲ����Ϣ�ṹ
typedef struct _tagSavePluginInfo
{
	int				index;								// �������
	int				piid;								// ���ID
	char			str[64];							// ��ʾ��
	char			ext[32];							// ȱʡ��չ����
	IRW_SAVE_DESC	cont;								// ԭʼ�ı��湦������
} SAVEPLUGININFO, *LPSAVEPLUGININFO;


/////////////////////////////////////////////////////////////////////////////
// CFileDialogEx - �����Ϊ���Ի�����

class CFileDialogEx : public CFileDialog
{
	DECLARE_DYNAMIC(CFileDialogEx)

public:
	CFileDialogEx(LPSPEC_DATAPACK);


	LPSPEC_DATAPACK	m_pPack;							// �������ͼ�����ַ

	int				m_iRunMark;							// ��ʼ���ɹ���ǣ�0���ɹ���
	int				m_ctrlcnt;							// ��ǰ��ѡ����������б����
	SAVESTR			m_save;								// ��������ṹ
	CString			m_ext;								// ��ǰȱʡ��չ��

	CStatic			*m_pStatic[ISD_CTLCNT];				// ��ǩ������
	CComboBox		*m_pComboBox[ISD_CTLCNT];			// �б�������

	int				m_saveinfocnt;						// ���б��湦�ܵĲ������
	SAVEPLUGININFO	m_saveinfo[ISEEIRW_MAX_PLUGIN_CNT];	// �б��湦�ܲ������Ϣ�ṹ����


	int  _GetPluginIndex(LPCTSTR sou);
	int  _GetPluginIndexFromFilterIndex(int idx);
	int  _SetFileType(int index);
	void _DispLastErrMess(int err);

	IRWE_CNVMARK _GetStdIDFromBitCnt(int bitcnt);

	BOOL _OK(void);

protected:
	//{{AFX_MSG(CFileDialogEx)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDIALOGEX_H__7C8C7557_C74F_11D3_B2C1_0020AF694846__INCLUDED_)
