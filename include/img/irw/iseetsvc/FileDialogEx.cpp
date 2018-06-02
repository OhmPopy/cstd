/********************************************************************

	FileDialogEx.cpp

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
					�Ի�����ʵ���ļ���

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

#include "stdafx.h"
#include "iseetsvc.h"

#include "shellpidl.h"
#include "shelllist.h"
#include "shelltree.h"
#include "shellpath.h"

#include "Display.h"
#include "LeftView.h"
#include "RightView.h"

#include "MainFrm.h"
#include "iseetsvcDoc.h"
#include "FileDialogEx.h"

#include <dlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDialogEx

IMPLEMENT_DYNAMIC(CFileDialogEx, CFileDialog)

CFileDialogEx::CFileDialogEx(LPSPEC_DATAPACK pack) : CFileDialog(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL)
{
	LPIRWMF_HEADER		pib;
	LPIRWMF_INFOBLOCK	pinfo;
	int					i;

	ASSERT(pack);

	// �ȴ����������
	isirw_s_wait(pack, 0);
	
	m_pPack       = pack;
	m_saveinfocnt = 0;
	m_ctrlcnt     = 0;

	m_ext.Empty();

	memset((void*)m_saveinfo, 0, sizeof(SAVEPLUGININFO)*ISEEIRW_MAX_PLUGIN_CNT);
	memset((void*)m_pStatic, 0, sizeof(CStatic*)*ISD_CTLCNT);
	memset((void*)m_pComboBox, 0, sizeof(CComboBox*)*ISD_CTLCNT);
	memset((void*)m_save.para_value, 0, sizeof(SAVESTR));
	
	// ���治�ܴ�������״̬
	ASSERT(isirw_is_busy() != 3);
	
	// �ȴ������������
	while (isirw_is_busy())
	{
		::Sleep(0);
	}
	
	// ����ͼ���д����
	pib = isirw_lock();
	
	pinfo = pib->puh;
	
	// �����б��湦�ܵĲ����Ϣ��ȡ�������������Ϣ������
	while (pinfo)
	{
		if (pinfo->ifo.irwp_function & IRWP_WRITE_SUPP)
		{
			m_saveinfo[m_saveinfocnt].index = m_saveinfocnt;
			m_saveinfo[m_saveinfocnt].piid  = pinfo->ifo.plug_id;
			strcpy((char*)m_saveinfo[m_saveinfocnt].str, (const char *)_strupr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)" ��ʽ (*.");
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)_strlwr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)")|");
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)"*.");
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)_strlwr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)"|");
			memcpy((void*)&(m_saveinfo[m_saveinfocnt].cont), (const void *)&(pinfo->ifo.irwp_save), sizeof(IRW_SAVE_DESC));
			strcpy((char*)m_saveinfo[m_saveinfocnt].ext, (const char *)_strlwr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			m_saveinfocnt++;
		}
		pinfo = pinfo->next;
	}
	
	isirw_unlock(pib);
	
	m_ofn.lpstrDefExt    = "";
	m_ofn.Flags			&= ~OFN_ENABLESIZING;
	m_ofn.Flags			&= ~OFN_OVERWRITEPROMPT;
	
	strcpy((char*)m_szFileName, "δ����");
	
	// ��MFC���Ĺ��˴�ת��Ϊϵͳ���
	if (m_saveinfocnt)
	{
		for (i=0, m_strFilter.Empty(); i<m_saveinfocnt; i++)
		{
			m_strFilter += (LPCTSTR)m_saveinfo[i].str;
		}

		LPTSTR pch = m_strFilter.GetBuffer(0);

		// �� '\0' �滻'|'����
		while ((pch = _tcschr(pch, '|')) != NULL)
		{
			*pch++ = '\0';
		}

		m_ofn.lpstrFilter = m_strFilter;
	}

	m_iRunMark = 0;
}


BEGIN_MESSAGE_MAP(CFileDialogEx, CFileDialog)
	//{{AFX_MSG_MAP(CFileDialogEx)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CFileDialogEx::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
	CWnd		*wndDlg = GetParent();
	CRect		Rect;
	int			i;

	// ��ʱ���б��뺬��ͼ�����������
	if (!(m_pPack->data_state&SDP_DATA_STATE_ORG_PIX))
	{
		m_iRunMark++;
	}

	for (i=0, Rect.SetRectEmpty(); i<ISD_CTLCNT; i++)
	{
		m_pStatic[i]   = new CStatic;
		m_pComboBox[i] = new CComboBox;
		
		// ���������ؼ����ڲ���������
		if (m_pStatic[i])
		{
			m_pStatic[i]->Create((LPCTSTR)"", WS_CHILD|WS_TABSTOP|SS_LEFT|SS_LEFTNOWORDWRAP, Rect, wndDlg, IDC_SAVEASCNT_STC+i);
			m_pStatic[i]->SetFont(wndDlg->GetFont());
		}
		else
		{
			m_iRunMark++;
		}

		if (m_pComboBox[i])
		{
			m_pComboBox[i]->Create(WS_CHILD|CBS_DROPDOWN|WS_VSCROLL|WS_TABSTOP|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|CBS_HASSTRINGS, Rect, wndDlg, IDC_SAVEASCNT_CON+i);
			m_pComboBox[i]->SetFont(wndDlg->GetFont());
		}
		else
		{
			m_iRunMark++;
		}
	}

	_SetFileType(0);
		
	return TRUE;
}

void CFileDialogEx::OnDestroy() 
{
	CFileDialog::OnDestroy();
	
	for (int i=0; i<ISD_CTLCNT; i++)
	{
		if (m_pStatic[i])
		{
			delete m_pStatic[i];
		}
		if (m_pComboBox[i])
		{
			delete m_pComboBox[i];
		}
	}
}


// ���ݸ������ַ������Ҳ������
int CFileDialogEx::_GetPluginIndex(LPCTSTR sou)
{
	for (int i=0; i<m_saveinfocnt; i++)
	{
		if (!memcmp((const void *)sou, (const void *)m_saveinfo[i].str, 12))
		{
			return i;
		}
	}
	return -1;
}


// �����˴�����ת��Ϊ�����Ϣ�����Ӧ������
int CFileDialogEx::_GetPluginIndexFromFilterIndex(int idx)
{
	char	*p = (char*)m_ofn.lpstrFilter;
	int		i;

	ASSERT(p);

	for (i=1; i<idx; i++)
	{
		p += strlen((const char *)p)+1;					// �ƹ����˴��ԣ�2�Σ�
		p += strlen((const char *)p)+1;
	}

	i = _GetPluginIndex((LPCTSTR)p);

	return i;
}


void CFileDialogEx::OnSize(UINT nType, int cx, int cy) 
{
	CFileDialog::OnSize(nType, cx, cy);
}


BOOL CFileDialogEx::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	OFNOTIFY			*pNotify = (OFNOTIFY*)lParam;
	
	switch(pNotify->hdr.code)
	{
	case CDN_TYPECHANGE:
		_SetFileType(pNotify->lpOFN->nFilterIndex);
		return TRUE;
	case CDN_FILEOK:
		*pResult = _OK();
		return TRUE;
	}

	return CFileDialog::OnNotify(wParam, lParam, pResult);
}


int CFileDialogEx::_SetFileType(int index)
{
	CWnd		*wndDlg = GetParent();
	int			i, j, k;
	DWORD		savebits;
	CString		stmp;

	// �����ʼ��δ�ɹ�����ִ������Ĳ���
	if (m_iRunMark)
	{
		return -1;
	}

	UpdateData(TRUE);

	static CRect	rcwnd(0,0,0,0);
	
	// �����Ի���ߴ�
	if (rcwnd.IsRectEmpty())
	{
		wndDlg->GetWindowRect(rcwnd);
	}
	
	// ȡ�õ�ǰ�������
	i = _GetPluginIndexFromFilterIndex(index);

	ASSERT(i >= 0);

	const UINT nControls = 5;	
	
	// ��ȡԭʼ�Ի�����ؿؼ����ڵĳߴ�����
	UINT  Controls[nControls] = {stc3, stc2, edt1, cmb1, lst1};
	CRect rc[nControls];							
	
	// ����ԭʼ���ڼ��ؼ����ε�����
	for (j=0; j<nControls; j++)
	{
		CWnd *wndCtrl = wndDlg->GetDlgItem(Controls[j]);
		wndCtrl->GetWindowRect(&rc[j]);
		wndDlg->ScreenToClient(&rc[j]);					// ת��Ϊ�ͻ�������
	}

	// ����һ���ؼ���������ĸ߶�
	int iExtraSize = rc[1].Height()+(rc[1].top-rc[0].bottom);
	
	// ����λ���ѡ����ƴ��ڱ���
	m_pStatic[0]->SetWindowText("λ���ѡ��:");
	// �����ǰ��λ��ѡ��
	m_pComboBox[0]->ResetContent();

	// ��ǰ���֧�ֵı����������
	savebits = m_saveinfo[i].cont.bitcount;
	
	// ���λ�������Ͽ�
	for (j=0; j<ISEEIRW_MAX_BITCOUNT; j++)
	{
		if ((savebits>>j)&1)
		{
			stmp.Format("%d λ", j+1);
			k = m_pComboBox[0]->AddString((LPCTSTR)stmp);
			m_pComboBox[0]->SetItemData(k, j+1);
		}
	}

	// ȱʡѡ��Ϊ��һ��
	m_pComboBox[0]->SetCurSel(0);

	// λ��ȿؼ��������ǿɼ���
	m_pStatic[0]->SetWindowPos(NULL, rc[1].left, rc[1].top+iExtraSize, rc[1].Width(), rc[1].Height(), 0);
	m_pStatic[0]->ShowWindow(SW_SHOW);
	m_pComboBox[0]->SetWindowPos(NULL, rc[3].left, rc[3].top+iExtraSize, rc[3].Width(), rc[3].Height()+ISD_COMBOBOXLIST_HEIGHT, 0);
	m_pComboBox[0]->ShowWindow(SW_SHOW);

	// ������ǰ�εĿؼ�����
	for (j=1; j<m_ctrlcnt+1; j++)
	{
		m_pStatic[j]->ShowWindow(SW_HIDE);
		m_pComboBox[j]->ShowWindow(SW_HIDE);
	}

	// ���������б����
	m_ctrlcnt = m_saveinfo[i].cont.count;

	// �����µĿؼ�����
	for(j=1; j<(m_ctrlcnt+1); j++)
	{
		CString strTxt;
		CRect   cr = rc[1];

		cr.OffsetRect(0, iExtraSize*(j+1));

		if(m_pStatic[j]->GetSafeHwnd())
		{
			m_pStatic[j]->SetWindowPos(NULL, cr.left, cr.top, cr.Width(), cr.Height(), 0);
			m_pStatic[j]->ShowWindow(SW_SHOW);
			strTxt = m_saveinfo[i].cont.para[j-1].desc;
			m_pStatic[j]->SetWindowText(strTxt+":");
		}
		else
		{
			strTxt = m_saveinfo[i].cont.para[j-1].desc;
			m_pStatic[j]->Create(strTxt+":", WS_CHILD|WS_VISIBLE, cr, wndDlg, IDC_SAVEASCNT_STC+j);
			m_pStatic[j]->SetFont(wndDlg->GetFont());
		}

		// ���ɻ��߸��������ؼ�
		cr = rc[3];

		cr.OffsetRect(0, iExtraSize*(j+1));
		cr.bottom += ISD_COMBOBOXLIST_HEIGHT;

		if(m_pComboBox[j]->GetSafeHwnd())
		{
			m_pComboBox[j]->SetWindowPos(NULL, cr.left, cr.top, cr.Width(), cr.Height(), 0);
			m_pComboBox[j]->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pComboBox[j]->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_TABSTOP|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|CBS_HASSTRINGS, cr, wndDlg, IDC_SAVEASCNT_CON+j);
			m_pComboBox[j]->SetFont(wndDlg->GetFont());
		}

		//��������ؼ�������
		m_pComboBox[j]->ResetContent();

		// ����ѡ�����ִ�
		for (k=0; k<m_saveinfo[i].cont.para[j-1].count; k++)
		{
			stmp.Format("%s", (LPCTSTR)m_saveinfo[i].cont.para[j-1].value_desc[k]);
			m_pComboBox[j]->AddString((LPCTSTR)stmp);
		}

		m_pComboBox[j]->SetCurSel(0);
	}

	// ��������ؼ�
	for(k=m_ctrlcnt+1; k<ISD_CTLCNT; k++)
	{
		if(m_pStatic[k]->GetSafeHwnd())
		{
			m_pStatic[k]->ShowWindow(SW_HIDE);
		}

		if(m_pComboBox[k]->GetSafeHwnd())
		{
			m_pComboBox[k]->ShowWindow(SW_HIDE);
		}
	}

	// ����ȱʡ��չ��
	m_ext = (LPCSTR)m_saveinfo[i].ext;

	// �����Ի���ߴ�
	wndDlg->SetWindowPos(NULL, 0, 0, rcwnd.Width(), rcwnd.Height()+iExtraSize*(m_ctrlcnt), SWP_NOMOVE);
	
	UpdateData(FALSE);

	return 0;
}


// �����ļ�
BOOL CFileDialogEx::_OK()
{
	// ��ʱ���б��뺬��ͼ�����������
	if (!(m_pPack->data_state&SDP_DATA_STATE_ORG_PIX))
	{
		m_iRunMark++;
	}
	
	UpdateData(TRUE);

	// ��ȡĿ���ļ�·��
	CString		strExt      = GetFileExt();
	CString		strFileName = GetFileName();
	CString		strPathName = GetPathName();
	CFileFind	fileFind;
	CString		strMsg;

	// �����ļ���չ��
	if (strExt.IsEmpty())
	{
		strPathName += ".";
		strPathName += m_ext;
		strFileName += ".";
		strFileName += m_ext;
	}
	else
	{
		if (_stricmp((const char *)strExt, (const char *)m_ext))
		{
			strPathName += ".";
			strPathName += m_ext;
			strFileName += ".";
			strFileName += m_ext;
		}
	}

	// �ж��ļ��Ƿ����
	if(fileFind.FindFile(strPathName))
	{
		strMsg.Format("�ļ� %s �Ѿ����ڣ�Ҫ�滻���ļ���", strFileName);

		if(MessageBox(strMsg, "���Ϊ", MB_ICONQUESTION|MB_YESNO) == IDNO)
		{
			UpdateData(FALSE);
			return TRUE;
		}
	}

	// �ռ��û�ѡ����Ϣ
	for(int i=0; i<m_ctrlcnt; i++)
	{
		m_save.para_value[i] = m_pComboBox[i+1]->GetCurSel();
	}

	// ��ȡĿ��ͼ��Ϊ�����Ϣ
	int	bitcount = (int)m_pComboBox[0]->GetItemData(m_pComboBox[0]->GetCurSel());

	// ����ԭת��������
	IRWE_CNVMARK	cnvmode    = m_pPack->cnvmark;
	LPINFOSTR		oldcnvpack = m_pPack->cnv_img;

	m_pPack->cnv_img = 0;
	m_pPack->cnvmark = IRWE_CNVMARK_NOT;

	if (oldcnvpack)
	{
		m_pPack->data_state &= ~SDP_DATA_STATE_CNV;
	}

	// ��ͼ��ת��Ϊ�û�ָ����λ��
	if (isirw_cnv(m_pPack, _GetStdIDFromBitCnt(bitcount), 0) >= 0)
	{
		// ����ͼ��
		if (isirw_s_write_img(ISEEIO_CONFER_LOC_FILE_SEQUENTIAL, (const char *)strPathName, 0, 0, m_pPack, &m_save, 0) == 0)
		{
			// �ȴ��������
			AfxGetApp()->DoWaitCursor(1);
			int retwait = isirw_s_wait(m_pPack, 0);
			AfxGetApp()->DoWaitCursor(0);
			
			switch (retwait)
			{
			case	ER_SUCCESS:
				break;
			case	ER_USERBREAK:
				_DispLastErrMess(ISIRW_LASTERR_USER);
				break;
			case	ER_MEMORYERR:
				_DispLastErrMess(ISIRW_LASTERR_MEM);
				break;
			case	ER_FILERWERR:
				_DispLastErrMess(ISIRW_LASTERR_RW);
				break;
			case	ER_SYSERR:
				_DispLastErrMess(ISIRW_LASTERR_OSERR);
				break;
			case	ER_NSIMGFOR:
				ASSERT(0);								// ����������
				break;
			default:
				ASSERT(0);								// ������������ֵ
				break;
			}
		}
		else
		{
			_DispLastErrMess(isirw_s_get_last_err());
		}
	}
	else
	{
		_DispLastErrMess(ISIRW_LASTERR_MEM);
	}

	// �ͷ�ת����
	if (m_pPack->cnv_img)
	{
		isirw_free_INFOSTR(m_pPack->cnv_img);
	}

	if (oldcnvpack)
	{
		m_pPack->data_state |= SDP_DATA_STATE_CNV;
	}
	
	// �ָ�ԭ״̬
	m_pPack->cnv_img = oldcnvpack;
	m_pPack->cnvmark = cnvmode;

	UpdateData(FALSE);

	return FALSE;
}


// ��λ��ֵת��Ϊ��׼��ʽID
IRWE_CNVMARK CFileDialogEx::_GetStdIDFromBitCnt(int bitcnt)
{
	IRWE_CNVMARK ret = IRWE_CNVMARK_32;

	switch (bitcnt)
	{
	case	1:
		ret = IRWE_CNVMARK_1;
		break;
	case	4:
		ret = IRWE_CNVMARK_4;
		break;
	case	8:
		ret = IRWE_CNVMARK_8;
		break;
	case	16:
		ret = IRWE_CNVMARK_16_555;						// ���湦��ֻ֧��555��ʽ
		break;
	case	24:
		ret = IRWE_CNVMARK_24;
		break;
	case	32:
		ret = IRWE_CNVMARK_32;
		break;
	default:
		ASSERT(0);										// ������湦���ṩ��λ����ܳ�����׼��ʽ֮��
		break;
	}
	
	return ret;
}


// ��ʾ������Ϣ
void CFileDialogEx::_DispLastErrMess(int err)
{
	CString		str;

	str.Empty();

	switch (err)
	{
	case	ISIRW_LASTERR_SUCCESS:				// û�д���
		break;
	case	ISIRW_LASTERR_SYSCLOSE:				// ����ϵͳδ����
		str = "ISeeͼ���д����ϵͳδ�������޷�����ͼ��";
		break;
	case	ISIRW_LASTERR_MEM:					// �ڴ治��
		str = "�ڴ治�㣬��������޷���ɣ�";
		break;
	case	ISIRW_LASTERR_RW:					// ���ʶ�д����
		str = "�������̶�д���󣬱�������޷���ɣ�";
		break;
	case	ISIRW_LASTERR_USER:					// �������û��ж�
		str = "�û��жϱ���������ò�����δ��ɡ�";
		break;
	case	ISIRW_LASTERR_IMGERR:				// ͼ�����(������ͼ��)
		str = "ͼ�����(������ͼ��)��";
		break;
	case	ISIRW_LASTERR_OSERR:				// ����ϵͳ����
		str = "����ϵͳ���󣡱�������޷���ɣ�";
		break;
	default:
		ASSERT(0);
		str = "����쳣�������˳�����";
		break;
	}

	if (!str.IsEmpty())
	{
		AfxMessageBox((LPCTSTR)str);
	}
}

