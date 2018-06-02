// PluginSet.cpp : implementation file
//

#include "stdafx.h"
#include "iseetsvc.h"
#include "PluginSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPluginSet dialog


CPluginSet::CPluginSet(CWnd* pParent /*=NULL*/)
	: CDialog(CPluginSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPluginSet)
	m_sPluginPath = _T("");
	m_sDBG = _T("");
	m_sEngVer = _T("");
	m_sPliVer = _T("");
	//}}AFX_DATA_INIT

	pirwhead = 0;
	plicnt   = 0;
	selcnt   = 0;
	
	memset((void*)plda, 0, sizeof(LDATA)*MAX_PLUGIN_NUM);
}


void CPluginSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluginSet)
	DDX_Control(pDX, IDC_BTN_UPDATE, m_btnUpdate);
	DDX_Control(pDX, IDC_BTN_SELALL, m_btnAll);
	DDX_Control(pDX, IDC_BTN_SEL, m_btnUse);
	DDX_Control(pDX, IDC_BTN_FREE, m_btnFree);
	DDX_Control(pDX, IDC_LIST_PLUGIN, m_cList);
	DDX_Text(pDX, IDC_PLUGIN_PATH, m_sPluginPath);
	DDV_MaxChars(pDX, m_sPluginPath, 260);
	DDX_Text(pDX, IDC_STA_DBG, m_sDBG);
	DDV_MaxChars(pDX, m_sDBG, 64);
	DDX_Text(pDX, IDC_STA_ENGVER, m_sEngVer);
	DDV_MaxChars(pDX, m_sEngVer, 32);
	DDX_Text(pDX, IDC_STA_PLIVER, m_sPliVer);
	DDV_MaxChars(pDX, m_sPliVer, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPluginSet, CDialog)
	//{{AFX_MSG_MAP(CPluginSet)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PLUGIN, OnItemchangedListPlugin)
	ON_BN_CLICKED(IDC_BTN_FREE, OnBtnFree)
	ON_BN_CLICKED(IDC_BTN_SEL, OnBtnSel)
	ON_BN_CLICKED(IDC_BTN_SELALL, OnBtnSelall)
	ON_BN_CLICKED(IDC_BTN_UPDATE, OnBtnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPluginSet message handlers

BOOL CPluginSet::OnInitDialog() 
{
	int					ev, pv, dbg;

	isirw_get_eng_info(&ev, &pv, &dbg, (char*)m_sPluginPath.GetBuffer(MAX_PATH));

	m_sEngVer.Format("%d.%d��", ev/10, ev%10);
	m_sPliVer.Format("%d.%d��", pv/10, pv%10);

	if (dbg == 0)
	{
		m_sDBG = "����";
	}
	else
	{
		m_sDBG = "����";
	}
	
	CDialog::OnInitDialog();

	// �����߱����ѵ�����SetData()����
	ASSERT(pirwhead);

	m_cList.InsertColumn(0, (LPCTSTR)"����", LVCFMT_CENTER, 100);
	m_cList.InsertColumn(1, (LPCTSTR)"�汾", LVCFMT_CENTER, 36);
	m_cList.InsertColumn(2, (LPCTSTR)"״̬", LVCFMT_CENTER, 72);
	m_cList.InsertColumn(3, (LPCTSTR)"���ȼ�", LVCFMT_CENTER, 100);
	m_cList.InsertColumn(4, (LPCTSTR)"����", LVCFMT_CENTER, 36);
	m_cList.InsertColumn(5, (LPCTSTR)"���ID", LVCFMT_CENTER, 48);
	m_cList.InsertColumn(6, (LPCTSTR)"������", LVCFMT_CENTER, 300);

	memset((void*)plda, 0, sizeof(LDATA)*MAX_PLUGIN_NUM);

	plicnt = 0;
	selcnt = 0;
	
	// ��ʼ���б���
	_init_list();

	return TRUE;
}


// �������趨�����ڲ���Ϣ��ӳ���ַ
void CPluginSet::SetData(LPIRWMF_HEADER pih)
{
	ASSERT(pih);
	ASSERT(isirw_is_ok() == 0);							// ��������ѳɹ�����
	ASSERT(isirw_is_busy() == 3);						// ���浱ǰ�������ѱ�������
	pirwhead = pih;
}


void CPluginSet::OnItemchangedListPlugin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uChanged == LVIF_STATE)
	{
		int			slda[MAX_PLUGIN_NUM];
		POSITION	pos = m_cList.GetFirstSelectedItemPosition();
		int			ibuse = 0, ibfree = 0, i;
		
		memset((void*)slda, 0, sizeof(int)*MAX_PLUGIN_NUM);
		
		selcnt = 0;

		// ��ȡѡ����ĸ���
		while (pos)
		{
			slda[selcnt++] = (int)m_cList.GetItemData(m_cList.GetNextSelectedItem(pos));
		}

		// ����ѡ������¼�ò����δ�ò������Ŀ
		for (i=0; i<selcnt; i++)
		{
			if (plda[slda[i]].inst == PLINSTATUS_NOUSE)
			{
				ibfree++;
			}
			else if ((plda[slda[i]].inst == PLINSTATUS_USE)||(plda[slda[i]].inst == PLINSTATUS_ACT))
			{
				ibuse++;
			}
			else
			{
				ASSERT(0);
			}
		}

		// ֻ�д�����¼�ò��ʱ�����ð�ť����Ч
		if (ibuse)
		{
			m_btnFree.EnableWindow();
		}
		else
		{
			m_btnFree.EnableWindow(FALSE);
		}

		// ֻ�д���δ¼�ò��ʱ��¼�ð�ť����Ч
		if (ibfree)
		{
			m_btnUse.EnableWindow();
		}
		else
		{
			m_btnUse.EnableWindow(FALSE);
		}
	}

	*pResult = 0;
}


// ����ѡ��Ĳ��
void CPluginSet::OnBtnFree() 
{
	if (selcnt == 0)
	{
		return;
	}

	int	item = -1, index;

	// ��������ѡ����
	while ((item=m_cList.GetNextItem(item,LVNI_ALL|LVNI_SELECTED)) != -1)
	{
		index = m_cList.GetItemData(item);

		// ����¼�ò������
		if ((plda[index].inst == PLINSTATUS_USE)||(plda[index].inst == PLINSTATUS_ACT))
		{
			plda[index].inst = PLINSTATUS_NOUSE;
			m_cList.SetItemText(item, 2, "δ¼��");
		}
	}

	// ������ť״̬
	m_btnUse.EnableWindow();
	m_btnFree.EnableWindow(FALSE);

	m_cList.SetFocus();
}


// ¼��ѡ��Ĳ��
void CPluginSet::OnBtnSel() 
{
	if (selcnt == 0)
	{
		return;
	}
	
	int	item = -1, index;
	
	// ��������ѡ����
	while ((item=m_cList.GetNextItem(item,LVNI_ALL|LVNI_SELECTED)) != -1)
	{
		index = m_cList.GetItemData(item);
		
		// ����δ¼�õĲ��
		if (plda[index].inst == PLINSTATUS_NOUSE)
		{
			if (plda[index].iost == PLINSTATUS_ACT)
			{
				plda[index].inst = PLINSTATUS_ACT;
				m_cList.SetItemText(item, 2, "����");
			}
			else
			{
				plda[index].inst = PLINSTATUS_USE;
				m_cList.SetItemText(item, 2, "¼��δ����");
			}
		}
	}
	
	// ������ť״̬
	m_btnUse.EnableWindow(FALSE);
	m_btnFree.EnableWindow();

	m_cList.SetFocus();
}


// �������в��
void CPluginSet::OnBtnSelall() 
{
	int	item, index;

	// �������в��
	for (item=0; item<m_cList.GetItemCount(); item++)
	{
		index = m_cList.GetItemData(item);
		
		// ֻ����δ¼�õĲ��
		if (plda[index].inst == PLINSTATUS_NOUSE)
		{
			if (plda[index].iost == PLINSTATUS_ACT)
			{
				plda[index].inst = PLINSTATUS_ACT;
				m_cList.SetItemText(item, 2, "����");
			}
			else
			{
				plda[index].inst = PLINSTATUS_USE;
				m_cList.SetItemText(item, 2, "¼��δ����");
			}
		}
	}

	POSITION pos = m_cList.GetFirstSelectedItemPosition();

	// ���°�ť״̬
	if (pos)
	{
		m_btnFree.EnableWindow();
	}
	else
	{
		m_btnFree.EnableWindow(FALSE);
	}

	m_btnUse.EnableWindow(FALSE);

	m_cList.SetFocus();
}


// ˢ�²����Ϣ��
void CPluginSet::OnBtnUpdate() 
{
	ASSERT(isirw_is_busy() == 3);						// ���浱ǰ�������ѱ�������

	int		ret;
	char	mess[MAX_PATH];

	// ��ʾ�ȴ���ꡢ�����б����ػ�
	AfxGetApp()->DoWaitCursor(1);
	m_cList.LockWindowUpdate();

	// ɾ�������б���
	m_cList.DeleteAllItems();

	memset((void*)plda, 0, sizeof(LDATA)*MAX_PLUGIN_NUM);
	
	plicnt = 0;
	selcnt = 0;

	UPDATAINFO	udp;

	// ˢ�²����Ϣ��
	switch (ret=isirw_update(pirwhead, IRWE_UPDTYPE_DISMISSAL, &udp))
	{
	case	0:			/* 0: ˢ�³ɹ� */
		_init_list();	
		break;
	case	1:			/* 1: �޿��ò�� */
		strcpy((char*)mess, "��������û���κ�ͼ���д��������潫�޷�����������������ر�����������Ŀ¼��");
		break;
	case	2:			/* 2: ��д���� */
		strcpy((char*)mess, "�����ڶ�����ļ�ʱ���ִ����������ļ��Ƿ��⵽�ƻ��������ȹرձ������");
		break;
	case	3:			/* 3: �ڴ治�� */
		strcpy((char*)mess, "������ڴ���ֲ��㣬����ر�һ�����������");
		break;
	case	4:			/* 4: ϵͳ�쳣 */
	default:
		strcpy((char*)mess, "����ϵͳ�쳣������رձ����������ϵͳ��");
		break;
	}
	
	if (ret)
	{
		// ���������Ϣ
		AfxMessageBox((LPCTSTR)mess);
	}
	else
	{
		// ����ˢ�½�� 
		sprintf((char*)mess, "ˢ�½����\n---------------------------------------------------------\n * ������� %d ����\n\n * ע����Ч��� %d ����\n\n * ������� %d ����", udp.newcnt, udp.delcnt, udp.updcnt);
		AfxMessageBox((LPCTSTR)mess, MB_ICONINFORMATION|MB_OK);
	}

	UnlockWindowUpdate();
	AfxGetApp()->DoWaitCursor(0);

	m_cList.SetFocus();

	return;
}


// ��ʼ������б�ʱ�ؼ�
void CPluginSet::_init_list()
{
	int					i, j, k, l, uc, nc;
	LPIRWMF_INFOBLOCK	pib;
	LVITEM				lvi;
	char				buf[MAX_PATH];
	
	uc = pirwhead->uc;
	nc = pirwhead->nc;
	
	// ����¼�ò����Ϣ��
	for (i=0, pib=pirwhead->puh; i<uc; i++, pib=pib->next)
	{
		plda[i].id   = pib->ifo.plug_id;
		plda[i].pol  = pib;
		plda[i].iost = (pib->act) ? PLINSTATUS_ACT : PLINSTATUS_USE;
		plda[i].inst = plda[i].iost;
		
		memset((void*)&lvi, 0, sizeof(LVITEM));

		// �������
		lvi.mask     = LVIF_TEXT | LVIF_PARAM;
		lvi.iItem    = i;
		lvi.iSubItem = 0;
		lvi.pszText  = (LPTSTR)pib->ifo.irwp_name;
		lvi.lParam   = i;
		l = m_cList.InsertItem(&lvi);

		// �汾
		sprintf((char*)buf, "%d.%d", pib->ifo.irwp_version/10, pib->ifo.irwp_version%10);
		m_cList.SetItemText(l, 1, buf);

		// ״̬
		m_cList.SetItemText(l, 2, (pib->act) ? "����" : "¼��δ����");
		
		// ���ȼ�
		sprintf((char*)buf, "%d", pib->pr);
		m_cList.SetItemText(l, 3, buf);

		// ����
		m_cList.SetItemText(l, 4, (pib->ifo.irwp_build_set) ? "����" : "����");
		
		// ���ID
		sprintf((char*)buf, "%02X", (int)pib->ifo.plug_id);
		m_cList.SetItemText(l, 5, buf);
		
		// ������
		buf[0] = '\0';
		for (k=0; k<(int)pib->ifo.irwp_author_count; k++)
		{
			strcat((char*)buf, (const char *)pib->ifo.irwp_author[k].ai_name);
			if ((k+1)<(int)pib->ifo.irwp_author_count)
			{
				strcat((char*)buf, " and ");
			}
			else
			{
				strcat((char*)buf, ";");
			}
		}
		m_cList.SetItemText(l, 6, buf);
	}
	
	// ����δ�ò����Ϣ��
	for (j=0, pib=pirwhead->pnh; j<nc; j++, pib=pib->next)
	{
		plda[i+j].id   = pib->ifo.plug_id;
		plda[i+j].pol  = pib;
		plda[i+j].iost = PLINSTATUS_NOUSE;
		plda[i+j].inst = plda[i+j].iost;
		
		memset((void*)&lvi, 0, sizeof(LVITEM));
		
		// �������
		lvi.mask     = LVIF_TEXT | LVIF_PARAM;
		lvi.iItem    = i+j;
		lvi.iSubItem = 0;
		lvi.pszText  = (LPTSTR)pib->ifo.irwp_name;
		lvi.lParam   = i+j;
		l = m_cList.InsertItem(&lvi);
		
		// �汾
		sprintf((char*)buf, "%d.%d", pib->ifo.irwp_version/10, pib->ifo.irwp_version%10);
		m_cList.SetItemText(l, 1, buf);
		
		// ״̬
		m_cList.SetItemText(l, 2, "δ¼��");
		
		// ���ȼ�
		sprintf((char*)buf, "%d", pib->pr);
		m_cList.SetItemText(l, 3, buf);
		
		// ����
		m_cList.SetItemText(l, 4, (pib->ifo.irwp_build_set) ? "����" : "����");
		
		// ���ID
		sprintf((char*)buf, "%02X", (int)pib->ifo.plug_id);
		m_cList.SetItemText(l, 5, buf);
		
		// ������
		buf[0] = '\0';
		for (k=0; k<(int)pib->ifo.irwp_author_count; k++)
		{
			strcat((char*)buf, (const char *)pib->ifo.irwp_author[k].ai_name);
			if ((k+1)<(int)pib->ifo.irwp_author_count)
			{
				strcat((char*)buf, " and ");
			}
			else
			{
				strcat((char*)buf, ";");
			}
		}
		m_cList.SetItemText(l, 6, buf);
	}

	plicnt = pirwhead->plugin_num;
}

