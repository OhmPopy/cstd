/********************************************************************

	Display.cpp

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
	���ļ���;��	ͼ���д���棭C���԰棭VC���������������ͼ��
					��ʵ���ļ���

	���ļ���д�ˣ�	
					YZ				yzfree##sina.com

	���ļ��汾��	30627
	����޸��ڣ�	2003-6-27

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������

	������ʷ��

		2003-6		ѹ��DrawDibDraw��ʹ�ø߶ȵ�һ������(���� mssoft##sina.com)
		2003-5		ʵ���Զ����š�Ԥ����ʾλ�ù���
		2003-4		�����������š��϶����ܣ�����ͼ����ʾ�ٶ�
		2003-1		��һ�����԰淢��

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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CDisplay

IMPLEMENT_DYNCREATE(CDisplay, CView)

CDisplay::CDisplay()
{
	m_hDrawDib	= DrawDibOpen();						// ��DrawDib��
	m_sImgInfo.Empty();

	m_iViewModl     = ISDISP_NOL;						// ȱʡΪ����ģʽ
	m_iTalAutResize = TRUE;								// ȫ���Զ����ű�־(ȱʡΪ���Զ�����)
	m_fTalRatio		= DPTRS_DEFAULT;					// ȫ�����ű���(ȱʡΪ��ʵģʽ)
	m_iAutoResize	= m_iTalAutResize;
	m_fRatio		= m_fTalRatio;
	m_iPassMark		= FALSE;

	m_iInitPos		= DPT_CENTER;						// ȱʡΪ�е����
	m_iSwitch		= ISDISP_SWH_FAST;					// ȱʡΪ����ͼ���л�ģʽ
	
	m_rcPict.SetRectEmpty();							// ����ͼ��ߴ磨�������ţ�
	m_rcBmpRc.SetRectEmpty();							// ͼ��ԭʼ�ߴ�
	m_rcFTRc.SetRectEmpty();
	m_rcPTRc.SetRectEmpty();

	m_rcHeader.SetRectEmpty();
	m_rcImage.SetRectEmpty();
	m_rcFrme.SetRectEmpty();							// ��ΧͼƬ�����ο�
	m_rcClient.SetRectEmpty();

	m_cpPrePt.x = m_cpPrePt.y = 0;

	m_bMoveing     = FALSE;
	m_pLineBuf     = 0;
	m_pDispBuf     = 0;
	m_cCurrPackCnt = 0;
	m_icbTrueState = 0;
	m_icbResize    = 0;
	m_lBuffSize    = 0;

	m_hcHSd = ::AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hcHFr = ::AfxGetApp()->LoadCursor(IDC_CUR_HANDFREE);
	m_hcHOd = ::AfxGetApp()->LoadCursor(IDC_CUR_HANDFOLD);
	m_hcHPs = ::AfxGetApp()->LoadCursor(IDC_CUR_PASS);

	m_iCurrFr     = 0;
	m_cAniThread  = 0;
	m_bDiableMark = FALSE;
	m_iInfo       = 0;
}


CDisplay::~CDisplay()
{
	if (m_hDrawDib)
	{
		DrawDibClose(m_hDrawDib);						// �ر�DrawDib��
	}
}


BEGIN_MESSAGE_MAP(CDisplay, CView)
	//{{AFX_MSG_MAP(CDisplay)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_DISP_INTOPLUG, OnDispIntoPlug)
	ON_MESSAGE(WM_DISP_BUILDMI, OnDispBuildMi)
	ON_MESSAGE(WM_DISP_BUILDSI, OnDispBuildSi)
	ON_MESSAGE(WM_DISP_PROCESS, OnDispProcess)
	ON_MESSAGE(WM_DISP_OVER, OnDispOver)
	ON_MESSAGE(WM_DISP_OK, OnDispOK)
	ON_MESSAGE(WM_DISP_FAILD, OnDispFaild)
	ON_MESSAGE(WM_DANI_NEXT, OnDispAniNext)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ISDBTNID_DISPTYPE, OnBtn1)
	ON_BN_CLICKED(ISDBTNID_RS, OnBtn2)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// ���������߳�
UINT CDisplay::AniProc(LPVOID parm)
{
	CDisplay		*pDisp = (CDisplay*)parm;
	LPSPEC_DATAPACK	pPack  = 0;

	MSG				msg;
	MSG				msgtmp;
	BOOL			bRet, bAbi, bNew, bTnl;
	int				idx, count, i, l, k, tm;
	
	// ֪ͨ���̶߳����̳߳�ʼ�����
	pDisp->PostMessage(WM_DANI_INIT);
	
	// ��Ϣѭ��
	while((bRet=::GetMessage(&msg, NULL, 0, 0)) != 0)
    { 
        if (bRet == -1)
        {
            continue;
        }
        else
        {
			if (msg.message == WM_DANI_QUIT)			// �˳���Ϣ
			{
				break;
			}
			else if (msg.message == WM_DANI_NEWCOMM)	// ��������Ϣ
			{
				// ��ȡרҵͼ�����ѭ�����ű�־
				pPack = (LPSPEC_DATAPACK)msg.wParam;
				bAbi  = (BOOL)msg.lParam;
				
				if (pPack == 0)
				{
					ASSERT(0);
					continue;
				}
				
				// ����Ǿ�̬ͼ�󣨵�֡�����������
				if (pPack->org_img->imgnumbers <= 1)
				{
					continue;
				}

				bNew                 = FALSE;
				pDisp->m_bDiableMark = FALSE;

				// ���Ŷ���ͼ��
				while (bNew == FALSE)
				{
					// ������ڲ��ű��򰴲��ű���
					count = (pPack->org_img->play_order != 0) ? pPack->org_img->play_number : pPack->org_img->imgnumbers;

					for (i=0; i<count; i++)
					{
						idx = (pPack->org_img->play_order != 0) ? (int)pPack->org_img->play_order[i] : i;

						// ������ʾָ����ͼ���֪ͨ��Ϣ
						pDisp->PostMessage(WM_DANI_NEXT, (WPARAM)pPack, (LPARAM)idx);

						// ��ȡ�ӳ�ʱ�䣨���룩
						if (idx == 0)
						{
							tm = (int)pPack->org_img->time;
						}
						else
						{
							tm  = isirw_s_get_subimg(pPack, idx, 0)->time;
						}

						// ̫���̫С���ӳ�ֵ��������
						tm = min(tm, ISDISP_MAXANI_TIME);
						tm = max(tm, ISDISP_MINANI_TIME);
						
						k    = tm/ISDISP_MINANI_TIME;
						bTnl = FALSE;

						// �ӳ٣���ʹͼ��ͣ��ָ��ʱ��
						for (l=0; l<k; l++)
						{
							::Sleep(ISDISP_MINANI_TIME);

							// ������µ���������������ǰ������
							if (::PeekMessage(&msgtmp, NULL, WM_DANI_NEWCOMM, WM_DANI_QUIT, PM_NOREMOVE))
							{
								bTnl = TRUE;
								bNew = TRUE;
								break;
							}
						}

						if (bTnl == TRUE)
						{
							break;
						}
					}

					// ���δ��ѭ����־���򲥷�һ����˳�
					if (bAbi == FALSE) 
					{
						bNew = TRUE;
					}
					else
					{
						::Sleep(0);
					}
				}
			}
			else if (msg.message == WM_DANI_DIABLE)		// ������ǰ����
			{
				pDisp->m_bDiableMark = TRUE;
				continue;
			}
			else
			{
				continue;
			}
        }
    }
	
	// �˳��߳�
	::AfxEndThread(0, TRUE);
	
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CDisplay drawing

void CDisplay::OnDraw(CDC* pDC)
{
	CIseetsvcDoc* pDoc = (CIseetsvcDoc*)GetDocument();

	CTimeCounter	ctcr;
	ctcr.BeginCount();
	
	// ���Ʊ��ⲿ��
	DrawHeader(pDC, pDoc->m_spath.m_iCurrIdx);

	// ����ͼ��
	DrawImage(pDC);

	DWORD ttt = ctcr.GetTime();

	return;
}



/////////////////////////////////////////////////////////////////////////////
// CDisplay diagnostics

#ifdef _DEBUG
void CDisplay::AssertValid() const
{
	CView::AssertValid();
}

void CDisplay::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CDisplay message handlers

int CDisplay::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_cbBackupBmp.LoadBitmap(IDB_BITMAP7);

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ����������ʱ�߳�
	if (!(m_cAniThread=::AfxBeginThread(AniProc, (LPVOID)this)))
	{
		return -1;
	}
	
	// ����ͼ��ת������
	if (!(m_pLineBuf=(char*)isirw_malloc(ISDISP_LINEBUFFSIZE)))
	{
		// ���ٶ�����ʱ�߳�
		m_cAniThread->PostThreadMessage(WM_DANI_QUIT, 0, 0);
		::WaitForSingleObject(m_cAniThread->m_hThread, 1000);
		return -1;
	}

	m_lBuffSize = ISDISP_MAXBUFFSIZE;
	
	// ������ʾ������
	if (!(m_pDispBuf=(char*)isirw_malloc(m_lBuffSize)))
	{
		isirw_free(m_pLineBuf);
		m_pLineBuf = 0;

		// ���ٶ�����ʱ�߳�
		m_cAniThread->PostThreadMessage(WM_DANI_QUIT, 0, 0);
		::WaitForSingleObject(m_cAniThread->m_hThread, 1000);
		return -1;
	}

	CRect	rc;

	rc.SetRect(ISDISP_MINBTN_WIDTH, ISDISP_TOP+ISDISP_HEADER_H-2-ISDISP_BTN_H, ISDISP_MINBTN_WIDTH+ISDISP_BTN_W, ISDISP_TOP+ISDISP_HEADER_H-2);
	m_cbTrue.Create("", BS_PUSHBUTTON|WS_VISIBLE|WS_CHILDWINDOW, rc, this, ISDBTNID_DISPTYPE);
	m_cbTrue.SetBitmaps(IDB_BITMAP3, 0);
	m_cbTrue.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(255,255,255), 0);
	m_cbTrue.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255,255,255), 0);
	m_cbTrue.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(0x7b, 0xd7, 0xff), 0);
	m_cbTrue.SetTooltipText("�Զ����ź�ԭʼ�ߴ��л�");

	rc.SetRect(ISDISP_MINBTN_WIDTH+ISDISP_BTN_W+ISDISP_BTN_SPEC, ISDISP_TOP+ISDISP_HEADER_H-2-ISDISP_BTN_H, ISDISP_MINBTN_WIDTH+ISDISP_BTN_W*2+ISDISP_BTN_SPEC, ISDISP_TOP+ISDISP_HEADER_H-2);
	m_cbResize.Create("", BS_PUSHBUTTON|WS_VISIBLE|WS_CHILDWINDOW, rc, this, ISDBTNID_RS);
	m_cbResize.SetBitmaps(IDB_BITMAP1, 0);
	m_cbResize.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(255,255,255), 0);
	m_cbResize.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255,255,255), 0);
	m_cbResize.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(0x7b, 0xd7, 0xff), 0);
	m_cbResize.SetTooltipText("�Ŵ�����С");
	
	return 0;
}


void CDisplay::OnSize(UINT nType, int cx, int cy) 
{
	// ������ʾ���ߴ�����
	SetDispLocInfo();

	// ���Զ�����ģʽ�£��������ű���
	if (m_iAutoResize)
	{
		SetPictLocInfo();
		SetFrmeLocInfo(TRUE);
	}

	// ����ͼ�����ʾλ��
	SetFrmeLocInfo();

	// ������ťλ��
	if ((cx > (ISDISP_LEFT+ISDISP_HEADER_W+ISDISP_MINBTN_WIDTH+ISDISP_BTN_W+ISDISP_BTN_SPEC+ISDISP_BTN_W+ISDISP_RIGHT))&&(cy > (ISDISP_TOP+ISDISP_HEADER_H+ISDISP_HISPEC)))
	{
		int x = max((ISDISP_LEFT+ISDISP_HEADER_W+ISDISP_MINBTN_WIDTH), (cx-ISDISP_RIGHT-ISDISP_BTN_W-ISDISP_BTN_SPEC-ISDISP_BTN_W));
		int y = ISDISP_TOP+ISDISP_HEADER_H-2-ISDISP_BTN_H;

		if (m_icbTrueState == 0)
		{
			m_icbTrueState = 1;
			m_cbTrue.ShowWindow(SW_SHOW);
		}
		m_cbTrue.MoveWindow(CRect(x,y,x+ISDISP_BTN_W,y+ISDISP_BTN_H));

		if (m_icbResize == 0)
		{
			m_icbResize = 1;
			m_cbResize.ShowWindow(SW_SHOW);
		}
		x += ISDISP_BTN_W+ISDISP_BTN_SPEC;
		m_cbResize.MoveWindow(CRect(x,y,x+ISDISP_BTN_W,y+ISDISP_BTN_H));
	}
	else
	{
		// ���������밴ť����С�������ذ�ť
		m_cbTrue.ShowWindow(SW_HIDE);
		m_icbTrueState = 0;
		m_cbResize.ShowWindow(SW_HIDE);
		m_icbResize = 0;
	}
	
	CView::OnSize(nType, cx, cy);
}


// ������ʾ���Ϸ��ı�ǩ
void CDisplay::DrawHeader(CDC *pDC, int ord)
{
	if (m_rcHeader.IsRectEmpty())
	{
		return;
	}

	COLORREF oldbkc = pDC->GetBkColor();

	// ���������ű���ɫ
	pDC->SetBkColor((COLORREF)(DWORD)ISCOL_HEADER_BG);
	
	// ������������ʾ������
	pDC->FillSolidRect(m_rcHeader.left, m_rcHeader.top, ISDISP_HEADER_W, ISDISP_HEADER_H, (COLORREF)(DWORD)ISCOL_HEADER_BG);

	CPen	pen;
	CPen	*oldpen;
	LOGPEN	logpen;

	logpen.lopnStyle   = PS_SOLID;
	logpen.lopnWidth.x = 0;
	logpen.lopnWidth.y = 0;
	logpen.lopnColor   = (COLORREF)(DWORD)ISCOL_HEADER_BG;

	pen.CreatePenIndirect(&logpen);

	oldpen = (CPen*)pDC->SelectObject(&pen);

	// ���Ʊ������
	pDC->MoveTo(m_rcHeader.left, m_rcHeader.bottom-1);
	pDC->LineTo(m_rcHeader.right, m_rcHeader.bottom-1);

	pDC->SelectObject(oldpen);

	COLORREF	oldtxtcol = pDC->GetTextColor();
	CString		sbuf;

	pDC->SetTextColor(RGB(255,255,255));

	// ������������
	if (ord == -1)
	{
		sbuf = TEXT("");
	}
	else
	{
		sbuf.Format("%d", ord);
	}

	// �����������ַ���ʾλ��
	CSize		fntsize = pDC->GetOutputTabbedTextExtent(sbuf, 0, NULL);
	CRect		fntrect((POINT)CPoint(ISDISP_LEFT+(ISDISP_HEADER_W-fntsize.cx)/2, ISDISP_TOP+(ISDISP_HEADER_H-fntsize.cy)/2), fntsize);

	// ��ʾ������
	pDC->DrawText(sbuf, fntrect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	// �ָ�DC����
	pDC->SetTextColor(oldtxtcol);
	pDC->SetBkColor(oldbkc);
	
	return;
}


// ����ͼ����Χ�Ļ���
void CDisplay::DrawImage(CDC *pDC)
{
	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	ASSERT(pDoc);
	
	LPINFOSTR		pinfo;
	long n;
				
	if ((!m_rcImage.IsRectEmpty())&&(pDoc->m_pPack)&&(pDoc->m_pPack->org_img->p_bit_data))
	{
		pinfo = ((pDoc->m_pPack->exec_state == IRWE_SDPSTATE_IDLE)) ? pDoc->m_pPack->cnv_img : pDoc->m_pPack->org_img;

		// �ų�����Ҫˢ�µ�����
		pDC->ExcludeClipRect(0, 0, ISDISP_LEFT, m_rcClient.bottom);
		pDC->ExcludeClipRect(0, 0, m_rcClient.right, m_rcHeader.bottom+ISDISP_HISPEC);
		pDC->ExcludeClipRect(m_rcClient.right-ISDISP_RIGHT, 0, m_rcClient.right, m_rcClient.bottom);
		pDC->ExcludeClipRect(0, m_rcClient.bottom-ISDISP_BOTTOM, m_rcClient.right, m_rcClient.bottom);

		// ��ʾͼ��
		if (pDoc->m_pPack->exec_state != IRWE_SDPSTATE_IDLE)
		{
			if (m_iSwitch == ISDISP_SWH_FAST)
			{
				// �ڶ�ȡ�Ĺ����в���StretchDIBits()������ʾͼ����Ϊ�޷�����ԭʼͼ�����������
				StretchDIBits(pDC->GetSafeHdc(), m_rcPTRc.left, m_rcPTRc.top, 
							m_rcPTRc.Width(), m_rcPTRc.Height(), 
							m_rcBmpRc.left, m_rcBmpRc.top, 
							m_rcBmpRc.Width(), m_rcBmpRc.Height(), 
							pinfo->p_bit_data, 
							(wBITMAPINFO*)&pDoc->m_dInfo, 
							DIB_RGB_COLORS, SRCCOPY);
			}
			else
			{
				;
			}
		}
		else
		{
			n = pDoc->m_dInfo.info.biHeight;
			if(n < 0)
				pDoc->m_dInfo.info.biHeight = -n;

			DrawDibDraw(m_hDrawDib, pDC->GetSafeHdc(), m_rcPTRc.left, m_rcPTRc.top, 
					m_rcPTRc.Width(), m_rcPTRc.Height(), 
					(wBITMAPINFOHEADER*)&pDoc->m_dInfo, 
					(LPVOID)m_pDispBuf, 
					0, 0, pinfo->width, pinfo->height, 
					DDF_HALFTONE);

			pDoc->m_dInfo.info.biHeight = n;
		}

		// ���Ƹ���ͼ��
		if (!m_rcFrme.IsRectEmpty())
		{
			CPen	pen(PS_SOLID, 1, RGB(128,128,128));
			CPen	penlt(PS_SOLID, 1, RGB(225,225,225));
			CPen	penrb(PS_SOLID, 1, RGB(128,128,128));
			CPen	penrb1(PS_SOLID, 1, RGB(168,168,168));
			CPen	penrb2(PS_SOLID, 1, RGB(216,216,216));
			CPen	penrs(PS_SOLID, 1, RGB(173, 211, 255));
			CPen	*oldpen;
			int		x, y;

			oldpen = (CPen*)pDC->SelectObject(&penlt);

			// ���ƻ���
			pDC->MoveTo(m_rcFTRc.left, m_rcFTRc.top);
			pDC->LineTo(m_rcFTRc.right-2, m_rcFTRc.top);
			pDC->MoveTo(m_rcFTRc.left, m_rcFTRc.top);
			pDC->LineTo(m_rcFTRc.left, m_rcFTRc.bottom-2);
			pDC->SelectObject(&penrb);
			pDC->MoveTo(m_rcFTRc.right-2, m_rcFTRc.top);
			pDC->LineTo(m_rcFTRc.right-2, m_rcFTRc.bottom-1);
			pDC->MoveTo(m_rcFTRc.left+1, m_rcFTRc.bottom-2);
			pDC->LineTo(m_rcFTRc.right-2, m_rcFTRc.bottom-2);
			pDC->SelectObject(&penrb1);
			pDC->MoveTo(m_rcFTRc.right-1, m_rcFTRc.top+1);
			pDC->LineTo(m_rcFTRc.right-1, m_rcFTRc.bottom);
			pDC->MoveTo(m_rcFTRc.left+2, m_rcFTRc.bottom-1);
			pDC->LineTo(m_rcFTRc.right-1, m_rcFTRc.bottom-1);
			pDC->SelectObject(&penrb2);
			pDC->MoveTo(m_rcFTRc.right, m_rcFTRc.top+2);
			pDC->LineTo(m_rcFTRc.right, m_rcFTRc.bottom+1);
			pDC->MoveTo(m_rcFTRc.left+3, m_rcFTRc.bottom);
			pDC->LineTo(m_rcFTRc.right, m_rcFTRc.bottom);

			if (m_fRatio != DPTRS_REAL)
			{
				pDC->SelectObject(&penrs);

				y = m_rcFTRc.bottom - ISDISP_FRMSPEC + ((ISDISP_FRMSPEC-2)/2-3);
				x = m_rcFTRc.CenterPoint().x;

				// �������ű��
				pDC->MoveTo(x-3, y);
				pDC->LineTo(x, y);
				pDC->MoveTo(x+2, y);
				pDC->LineTo(x+5, y);

				pDC->MoveTo(x-5, y+1);
				pDC->LineTo(x, y+1);
				pDC->MoveTo(x+2, y+1);
				pDC->LineTo(x+7, y+1);

				pDC->MoveTo(x-7, y+2);
				pDC->LineTo(x+9, y+2);
				pDC->MoveTo(x-7, y+3);
				pDC->LineTo(x+9, y+3);

				pDC->MoveTo(x-5, y+4);
				pDC->LineTo(x, y+4);
				pDC->MoveTo(x+2, y+4);
				pDC->LineTo(x+7, y+4);
				
				pDC->MoveTo(x-3, y+5);
				pDC->LineTo(x, y+5);
				pDC->MoveTo(x+2, y+5);
				pDC->LineTo(x+5, y+5);
			}
			
			pDC->SelectObject(oldpen);
		}
	}
	else
	{
		if (pDoc->m_pPack)
		{
			return;
		}

		// û��ѡ��ͼ��ʱ��ֱ�ӷ���
		if (pDoc->m_spath.m_iCurrIdx == -1)
		{
			return;
		}

		// �����ǰû�п���ʾ��ͼ������ʾȱʡ����ͼ��
		pDC->ExcludeClipRect(0, 0, ISDISP_LEFT, m_rcClient.bottom);
		pDC->ExcludeClipRect(0, 0, m_rcClient.right, m_rcHeader.bottom+ISDISP_HISPEC);
		pDC->ExcludeClipRect(m_rcClient.right-ISDISP_RIGHT, 0, m_rcClient.right, m_rcClient.bottom);
		pDC->ExcludeClipRect(0, m_rcClient.bottom-ISDISP_BOTTOM, m_rcClient.right, m_rcClient.bottom);

		CDC			cmdc;
		CBitmap		*oldbmp;
		CRect		rct;
		CRect		rcbmp;
		BITMAP		bim;

		memset((void*)&bim, 0, sizeof(BITMAP));
		m_cbBackupBmp.GetBitmap(&bim);

		rcbmp.SetRect(0, 0, bim.bmWidth, bim.bmHeight);

		if (cmdc.CreateCompatibleDC(pDC))
		{
			oldbmp = (CBitmap*)cmdc.SelectObject(&m_cbBackupBmp);

			rct.SetRect(m_rcImage.CenterPoint().x-rcbmp.CenterPoint().x, 
						m_rcImage.CenterPoint().y-rcbmp.CenterPoint().y, 
						(m_rcImage.CenterPoint().x-rcbmp.CenterPoint().x)+rcbmp.Width(), 
						(m_rcImage.CenterPoint().y-rcbmp.CenterPoint().y)+rcbmp.Height());

			pDC->BitBlt(rct.left, rct.top, rct.Width(), rct.Height(), &cmdc, 0, 0, SRCCOPY);

			cmdc.SelectObject(oldbmp);
		}
	}
}


// ����ͼƬԭʼ�ߴ硢���ųߴ缰����ߴ磨������ͼ�����ʱ���ã�
void CDisplay::SetPictLocInfo(BOOL init)
{
	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	LPSPEC_DATAPACK pdsp = pDoc->m_pPack;

	// ���û��ͼ�������򵥵ķ���
	if (pdsp == 0)
	{
		return;
	}

	if (init == TRUE)
	{
		// ����ͼ����ʵ��ԭʼ���ߴ�ṹ
		m_rcBmpRc.SetRect(0, 0, pdsp->org_img->width, pdsp->org_img->height);

		// ��ȫ�ֱ�ǩ�л�ȡ���Ų���
		m_iAutoResize = m_iTalAutResize;
		m_fRatio      = m_fTalRatio;
	}

	// ������Զ�����
	if (m_iAutoResize)
	{
		int		w, h;
		float	wr, hr;

		w = m_rcImage.Width() - ISDISP_FRMSPEC*2;
		h = m_rcImage.Height() - ISDISP_FRMSPEC*2;

		w = max(1, w);
		h = max(1, h);

		wr = (float)w/(float)m_rcBmpRc.Width();			// ������ߵ����ű���
		hr = (float)h/(float)m_rcBmpRc.Height();
			
		if ((wr >= 1.0)&&(hr >= 1.0))
		{
			m_fRatio = DPTRS_REAL;						// ��ʾ���ߴ��ͼ��󣬲�����ʵ�ߴ�
		}
		else
		{
			m_fRatio = max((min(wr,hr)), DPTRS_MIN);	// ������С�ߴ�(�޶�����)
		}
	}
	else
	{
		ASSERT(m_fRatio >= DPTRS_MIN);
		ASSERT(m_fRatio <= DPTRS_MAX);
	}

	m_rcFrme.left = 0;
	m_rcFrme.top  = 0;
	
	// ����ͼ����ʾ�ߴ磨�������ţ������߿���ʾ�ߴ�
	if (m_fRatio == DPTRS_REAL)							// ʵ�ʳߴ�
	{
		m_rcPict = m_rcBmpRc;

		if ((m_rcPict.Width() < ISDISP_MIN_WIDTH)&&(m_rcPict.Height() < ISDISP_MIN_HEIGHT))
		{
			m_rcFrme.right  = ISDISP_MIN_WIDTH+ISDISP_FRMSPEC*2;
			m_rcFrme.bottom = ISDISP_MIN_HEIGHT+ISDISP_FRMSPEC*2;
		}
		else
		{
			m_rcFrme.right  = m_rcPict.Width()+ISDISP_FRMSPEC*2;
			m_rcFrme.bottom = m_rcPict.Height()+ISDISP_FRMSPEC*2;
		}
	}
	else if (m_fRatio > DPTRS_REAL)						// �Ŵ�
	{
		ASSERT(m_fRatio <= DPTRS_MAX);

		m_rcPict.SetRect(0, 0, (int)((float)m_rcBmpRc.Width()*m_fRatio+0.5), (int)((float)m_rcBmpRc.Height()*m_fRatio+0.5));

		if ((m_rcPict.Width() < ISDISP_MIN_WIDTH)&&(m_rcPict.Height() < ISDISP_MIN_HEIGHT))
		{
			m_rcFrme.right  = ISDISP_MIN_WIDTH+ISDISP_FRMSPEC*2;
			m_rcFrme.bottom = ISDISP_MIN_HEIGHT+ISDISP_FRMSPEC*2;
		}
		else
		{
			m_rcFrme.right  = m_rcPict.Width()+ISDISP_FRMSPEC*2;
			m_rcFrme.bottom = m_rcPict.Height()+ISDISP_FRMSPEC*2;
		}
	}
	else												// ��С
	{
		ASSERT(m_fRatio >= DPTRS_MIN);

		m_rcPict.SetRect(0, 0, max(((int)((float)m_rcBmpRc.Width()*m_fRatio+0.5)),1), max(((int)((float)m_rcBmpRc.Height()*m_fRatio+0.5)),1));

		if ((m_rcPict.Width() < ISDISP_MIN_WIDTH)&&(m_rcPict.Height() < ISDISP_MIN_HEIGHT))
		{
			m_rcFrme.right  = ISDISP_MIN_WIDTH+ISDISP_FRMSPEC*2;
			m_rcFrme.bottom = ISDISP_MIN_HEIGHT+ISDISP_FRMSPEC*2;
		}
		else
		{
			m_rcFrme.right  = m_rcPict.Width()+ISDISP_FRMSPEC*2;
			m_rcFrme.bottom = m_rcPict.Height()+ISDISP_FRMSPEC*2;
		}
	}
}


// ���±�ǩ��ͼ���ĳߴ�
void CDisplay::SetDispLocInfo()
{
	// ���¿ͻ����ߴ�
	GetClientRect(m_rcClient);

	// ���±�ǩ���Σ���������ʾ��λ����ͼ���
	if ((m_rcClient.Width() <= (ISDISP_LEFT+ISDISP_RIGHT))||(m_rcClient.Height() <= (ISDISP_TOP+ISDISP_HEADER_H)))
	{
		m_rcHeader.SetRectEmpty();
		m_rcImage.SetRectEmpty();
	}
	else
	{
		m_rcHeader.SetRect(ISDISP_LEFT, ISDISP_TOP, m_rcClient.Width()-ISDISP_RIGHT, ISDISP_TOP+ISDISP_HEADER_H);
		
		if (m_rcClient.Height() <= (ISDISP_TOP+ISDISP_HEADER_H+ISDISP_HISPEC))
		{
			m_rcImage.SetRectEmpty();
		}
		else
		{
			m_rcImage.SetRect(ISDISP_LEFT, ISDISP_TOP+ISDISP_HEADER_H+ISDISP_HISPEC, m_rcClient.Width()-ISDISP_RIGHT, m_rcClient.Height()-ISDISP_BOTTOM);
		}
	}
}


// ����ͼ�����ʾ�������
void CDisplay::SetFrmeLocInfo(BOOL init)
{
	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	LPSPEC_DATAPACK pdsp = pDoc->m_pPack;

	// ��û��ͼƬ����ʱֻ�򵥵ķ���
	if (pdsp == 0)
	{
		return;
	}

	// ����ͼ����ʾ�������
	if (init == FALSE)									// ��������ı�ߴ�ʱ��
	{
		int off;

		// ����X��λ�ã���ʹͼ�����Զ����ٴ���ߴ�ı仯
		if (m_rcImage.Width() >= m_rcFTRc.Width())
		{
			m_rcFTRc.left  = m_rcImage.CenterPoint().x - m_rcFTRc.Width()/2;
			m_rcFTRc.right = m_rcFTRc.left+m_rcFrme.Width();
			m_rcPTRc.left  = m_rcImage.CenterPoint().x - m_rcPTRc.Width()/2;
			m_rcPTRc.right = m_rcPTRc.left+m_rcPict.Width();
		}
		else
		{
			if (m_rcImage.right > m_rcFTRc.right)
			{
				off = m_rcImage.right-m_rcFTRc.right;

				m_rcFTRc.OffsetRect(off, 0);
				m_rcPTRc.OffsetRect(off, 0);
			}
			else
			{
				if (m_rcFTRc.left > m_rcImage.left)
				{
					off = m_rcImage.left-m_rcFTRc.left;

					m_rcFTRc.OffsetRect(off, 0);
					m_rcPTRc.OffsetRect(off, 0);
				}
			}
		}

		// ����Y��λ��
		if (m_rcImage.Height() >= m_rcFTRc.Height())
		{
			m_rcFTRc.top    = m_rcImage.CenterPoint().y - m_rcFTRc.Height()/2;
			m_rcFTRc.bottom	= m_rcFTRc.top+m_rcFrme.Height();
			m_rcPTRc.top    = m_rcImage.CenterPoint().y - m_rcPTRc.Height()/2;
			m_rcPTRc.bottom = m_rcPTRc.top+m_rcPict.Height();
		}
		else
		{
			if (m_rcImage.bottom > m_rcFTRc.bottom)
			{
				off = m_rcImage.bottom-m_rcFTRc.bottom;

				m_rcFTRc.OffsetRect(0, off);
				m_rcPTRc.OffsetRect(0, off);
			}
			else
			{
				if (m_rcFTRc.top > m_rcImage.top)
				{
					off = m_rcImage.top-m_rcFTRc.top;

					m_rcFTRc.OffsetRect(0, off);
					m_rcPTRc.OffsetRect(0, off);
				}
			}
		}

		return;
	}
	else												// ��������ͼ�����ʱ���ã�
	{
		// ���ͼ����ʾ��̫С���򲻽��н�һ���ļ���
		if (m_rcImage.IsRectEmpty())
		{
			return;
		}

		ASSERT(!m_rcFrme.IsRectEmpty());				// ͼ��ĳߴ����ݱز�Ϊ��
		ASSERT(!m_rcPict.IsRectEmpty());

		CPoint	cpt = m_rcImage.CenterPoint();
		int		xf, yf, xb, yb, x1, y1, x2, y2;
		
		// ���ݶ��뷽ʽ���趨ͼ���ʼ��ʾλ��
		switch (m_iInitPos)
		{
		case	DPT_LEFTTOP:							// ���ϽǶ���
			xf = m_rcImage.CenterPoint().x-m_rcFrme.Width()/2;
			yf = m_rcImage.CenterPoint().y-m_rcFrme.Height()/2;
			xb = m_rcImage.CenterPoint().x-m_rcPict.Width()/2;
			yb = m_rcImage.CenterPoint().y-m_rcPict.Height()/2;
			
			x1 = max(xf, m_rcImage.left);
			y1 = max(yf, m_rcImage.top);
			x2 = x1+m_rcFrme.Width();
			y2 = y1+m_rcFrme.Height();
			m_rcFTRc.SetRect(x1, y1, x2, y2);

			x1 = max(xb, m_rcImage.left+ISDISP_FRMSPEC);
			y1 = max(yb, m_rcImage.top+ISDISP_FRMSPEC);
			x2 = x1+m_rcPict.Width();
			y2 = y1+m_rcPict.Height();
			m_rcPTRc.SetRect(x1, y1, x2, y2);
			break;
		case	DPT_RIGHTTOP:							// ���ϽǶ���
			xf = m_rcImage.CenterPoint().x+m_rcFrme.Width()/2;
			yf = m_rcImage.CenterPoint().y-m_rcFrme.Height()/2;
			xb = m_rcImage.CenterPoint().x+m_rcPict.Width()/2;
			yb = m_rcImage.CenterPoint().y-m_rcPict.Height()/2;
			
			x2 = min(xf, m_rcImage.right);
			y1 = max(yf, m_rcImage.top);
			x1 = x2-m_rcFrme.Width();
			y2 = y1+m_rcFrme.Height();
			m_rcFTRc.SetRect(x1, y1, x2, y2);
			
			x2 = min(xb, m_rcImage.right-ISDISP_FRMSPEC);
			y1 = max(yb, m_rcImage.top+ISDISP_FRMSPEC);
			x1 = x2-m_rcPict.Width();
			y2 = y1+m_rcPict.Height();
			m_rcPTRc.SetRect(x1, y1, x2, y2);
			break;
		case	DPT_LEFTBOTTOM:							// ���½Ƕ���
			xf = m_rcImage.CenterPoint().x-m_rcFrme.Width()/2;
			yf = m_rcImage.CenterPoint().y+m_rcFrme.Height()/2;
			xb = m_rcImage.CenterPoint().x-m_rcPict.Width()/2;
			yb = m_rcImage.CenterPoint().y+m_rcPict.Height()/2;
			
			x1 = max(xf, m_rcImage.left);
			y2 = min(yf, m_rcImage.bottom);
			x2 = x1+m_rcFrme.Width();
			y1 = y2-m_rcFrme.Height();
			m_rcFTRc.SetRect(x1, y1, x2, y2);
			
			x1 = max(xb, m_rcImage.left+ISDISP_FRMSPEC);
			y2 = min(yb, m_rcImage.bottom-ISDISP_FRMSPEC);
			x2 = x1+m_rcPict.Width();
			y1 = y2-m_rcPict.Height();
			m_rcPTRc.SetRect(x1, y1, x2, y2);
			break;
		case	DPT_RIGHTBOTTOM:						// ���½Ƕ���
			xf = m_rcImage.CenterPoint().x+m_rcFrme.Width()/2;
			yf = m_rcImage.CenterPoint().y+m_rcFrme.Height()/2;
			xb = m_rcImage.CenterPoint().x+m_rcPict.Width()/2;
			yb = m_rcImage.CenterPoint().y+m_rcPict.Height()/2;
			
			x2 = min(xf, m_rcImage.right);
			y2 = min(yf, m_rcImage.bottom);
			x1 = x2-m_rcFrme.Width();
			y1 = y2-m_rcFrme.Height();
			m_rcFTRc.SetRect(x1, y1, x2, y2);
			
			x2 = min(xb, m_rcImage.right-ISDISP_FRMSPEC);
			y2 = min(yb, m_rcImage.bottom-ISDISP_FRMSPEC);
			x1 = x2-m_rcPict.Width();
			y1 = y2-m_rcPict.Height();
			m_rcPTRc.SetRect(x1, y1, x2, y2);
			break;
		case	DPT_CENTER:								// �е����
			m_rcFTRc.SetRect(m_rcImage.CenterPoint().x-m_rcFrme.Width()/2, 
				m_rcImage.CenterPoint().y-m_rcFrme.Height()/2,
				(m_rcImage.CenterPoint().x-m_rcFrme.Width()/2)+m_rcFrme.Width(), 
				(m_rcImage.CenterPoint().y-m_rcFrme.Height()/2)+m_rcFrme.Height());
			
			m_rcPTRc.SetRect(m_rcImage.CenterPoint().x-m_rcPict.Width()/2, 
				m_rcImage.CenterPoint().y-m_rcPict.Height()/2,
				(m_rcImage.CenterPoint().x-m_rcPict.Width()/2)+m_rcPict.Width(), 
				(m_rcImage.CenterPoint().y-m_rcPict.Height()/2)+m_rcPict.Height());
			break;
		default:
			ASSERT(0);
			break;
		}
	}
}


// ��̬������ǰ������
BOOL CDisplay::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CRect	rc;
	CPoint	pt;
	SHORT	kbs;
	
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	
	// �жϹ���Ƿ���ͼ������
	rc.IntersectRect((LPCRECT)m_rcPTRc, (LPCRECT)m_rcImage);

	kbs = GetAsyncKeyState(VK_CONTROL) & 0x8000;

	if ((kbs)||(m_iPassMark))
	{
		// ��ʾ�Ŵ󾵹��
		if (rc.PtInRect(pt))
		{
			SetCursor(m_hcHPs);
		}
		else
		{
			SetCursor(m_hcHSd);
		}
	}
	else
	{
		// �����ǰͼ���ѱ���ȫ��ʾ�����ˣ�����ʾ��׼��ͷ���
		if (IsDrop() == FALSE)
		{
			SetCursor(m_hcHSd);
			return TRUE;
		}
		
		// ��������ͼ�����棬����ʾ���ι��
		if (rc.PtInRect(pt))
		{
			SetCursor(m_hcHFr);
		}
		else
		{
			SetCursor(m_hcHSd);
		}
	}

	return TRUE;
}


// ������ѹ�´���
void CDisplay::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	LPSPEC_DATAPACK pdsp = pDoc->m_pPack;

	if (pdsp == 0)
	{
		CView::OnLButtonDown(nFlags, point);
		return;
	}

	CRect	rc;
	
	rc.IntersectRect((LPCRECT)m_rcPTRc, (LPCRECT)m_rcImage);
	
	// �жϹ���Ƿ���ͼ���Ϸ�
	if (rc.PtInRect(point))
	{
		if ((nFlags&MK_CONTROL)||(m_iPassMark))		// �Ŵ�ģʽ
		{
			_RSL(point);
		}
		else if (IsDrop())			// ��������ͼ�����棬����ͼ��û�б���ȫ��ʾ��������׽��꣬��ʼһ���϶�����
		{
			SetCursor(m_hcHOd);
			::SetCapture((HWND)(*this));
			m_bMoveing = TRUE;
			m_cpPrePt = point;
		}
		else
		{
			;
		}
	}
	
	CView::OnLButtonDown(nFlags, point);
}


// �������ɿ�����
void CDisplay::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// �����ǰ�����϶������У����ͷŹ�겶׽�����ָ��������
	if (m_bMoveing == TRUE)
	{
		m_cpPrePt = point;
		m_bMoveing = FALSE;
		::ReleaseCapture();
		SetCursor(m_hcHFr);
	}
	
	CView::OnLButtonUp(nFlags, point);
}


// ������˫������
void CDisplay::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if ((nFlags&MK_CONTROL)||(m_iPassMark))		// �Ŵ�ģʽ
	{
		OnLButtonDown(nFlags, point);
		return;
	}

	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	LPSPEC_DATAPACK pdsp = pDoc->m_pPack;
	
	if (pdsp == 0)
	{
		CView::OnLButtonDblClk(nFlags, point);
		return;
	}

	CMainFrame*	pMain = (CMainFrame*)AfxGetMainWnd();
	CRect		rc;

	if (m_iViewModl == ISDISP_NOL)
	{
		// ���浱ǰ�����ڵ�λ�ü��ߴ���Ϣ
		pMain->GetWindowPlacement(&m_wp);

		pMain->m_spa.GetPane(0,0)->GetWindowRect(rc);
		m_iOTW = rc.Width();
		pMain->m_spa.GetPane(0,1)->GetWindowRect(rc);
		m_iORW = rc.Width();
		pMain->m_spb.GetPane(0,0)->GetWindowRect(rc);
		m_iODW = rc.Width();
		pMain->m_spb.GetPane(0,1)->GetWindowRect(rc);
		m_iOLW = rc.Width();

		// �����ܴ���δ�������״̬����ʹ֮���
		if (m_wp.showCmd != SW_SHOWMAXIMIZED)
		{
			WINDOWPLACEMENT	wp;

			memcpy((void*)&wp, (const void *)&m_wp, sizeof(WINDOWPLACEMENT));
			wp.showCmd = SW_SHOWMAXIMIZED;
			pMain->SetWindowPlacement(&wp);
		}
		
		// �������ӡ���С�б��ӣ�������ʾ��
		pMain->GetClientRect(rc);
		pMain->m_spa.SetColumnInfo(0, ISDISP_TREE_MIN, 0);
		pMain->m_spa.SetColumnInfo(1, rc.Width(), 0);
		pMain->m_spb.SetColumnInfo(0, rc.Width()-ISDISP_LISTSS, 0);
		pMain->m_spb.SetColumnInfo(1, ISDISP_LISTSS, 0);

		m_iViewModl = ISDISP_VIEWMODL;
	}
	else
	{
		// �������ڳߴ�ָ����������ģʽ����ĳߴ�
		pMain->SetWindowPlacement(&m_wp);

		pMain->m_spa.SetColumnInfo(0, m_iOTW, 0);
		pMain->m_spa.SetColumnInfo(1, m_iORW, 0);
		pMain->m_spb.SetColumnInfo(0, m_iODW, 0);
		pMain->m_spb.SetColumnInfo(1, m_iOLW, 0);

		m_iViewModl = ISDISP_NOL;
	}

	// ���Ѵ������²���
	pMain->m_spa.RecalcLayout();
	pMain->m_spb.RecalcLayout();

	CView::OnLButtonDblClk(nFlags, point);
}


// ����Ҽ�˫������
void CDisplay::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	if ((nFlags&MK_CONTROL)||(m_iPassMark))		// �Ŵ�ģʽ
	{
		OnRButtonDown(nFlags, point);
		return;
	}
	

	
	CView::OnRButtonDblClk(nFlags, point);
}


// ����ƶ�����
void CDisplay::OnMouseMove(UINT nFlags, CPoint point) 
{
	// �����ǰ�����϶����̣�����н�һ���Ĵ���
	if (m_bMoveing == TRUE)
	{
		CPoint	cpt   = m_cpPrePt;
		BOOL	bMark = FALSE;
		CPoint	cpoff(0,0);
		int		i;

		ASSERT(nFlags & MK_LBUTTON);
		
		cpt -= point;
	
		i = abs(cpt.x);

		if (cpt.x < 0)								// �����϶�
		{
			if (m_rcFTRc.left < m_rcImage.left)
			{
				i = ((m_rcFTRc.left+i) > m_rcImage.left) ? (m_rcImage.left-m_rcFTRc.left) : i;
				m_rcFTRc.OffsetRect(i, 0);
				m_rcPTRc.OffsetRect(i, 0);
				cpoff.x = i;
				bMark   = TRUE;
			}
		}
		else if (cpt.x > 0)							// �����϶�
		{
			if (m_rcFTRc.right > m_rcImage.right)
			{
				i = ((m_rcFTRc.right-i) < m_rcImage.right) ? (m_rcFTRc.right-m_rcImage.right) : i;
				m_rcFTRc.OffsetRect(-i, 0);
				m_rcPTRc.OffsetRect(-i, 0);
				cpoff.x = -i;
				bMark   = TRUE;
			}
		}
		else										// û�� X �᷽����϶�
		{
			;
		}


		i = abs(cpt.y);

		if (cpt.y < 0)								// �����϶�
		{
			if (m_rcFTRc.top < m_rcImage.top)
			{
				i = ((m_rcFTRc.top+i) > m_rcImage.top) ? (m_rcImage.top-m_rcFTRc.top) : i;
				m_rcFTRc.OffsetRect(0, i);
				m_rcPTRc.OffsetRect(0, i);
				cpoff.y = i;
				bMark   = TRUE;
			}
		}
		else if (cpt.y > 0)							// �����϶�
		{
			if (m_rcFTRc.bottom > m_rcImage.bottom)
			{
				i = ((m_rcFTRc.bottom-i) < m_rcImage.bottom) ? (m_rcFTRc.bottom-m_rcImage.bottom) : i;
				m_rcFTRc.OffsetRect(0, -i);
				m_rcPTRc.OffsetRect(0, -i);
				cpoff.y = -i;
				bMark   = TRUE;
			}
		}
		else										// û�� Y �᷽����϶�
		{
			;
		}

		// ���������λ�ƣ�����������е�����
		if (bMark == TRUE)
		{
			m_cpPrePt = point;
			ScrollWindowEx(cpoff.x, cpoff.y, m_rcImage, m_rcImage, 0, 0, SW_INVALIDATE|SW_ERASE);
		}
	}
	
	CView::OnMouseMove(nFlags, point);
}


// �ж��Ƿ���Ҫ�϶����ܿ���ȫͼ
BOOL CDisplay::IsDrop()
{
	CRect	rc;
	
	rc.UnionRect((LPCRECT)m_rcFTRc, (LPCRECT)m_rcImage);
	
	if (rc == m_rcImage)
	{
		return FALSE;									// �����϶����ɿ���ȫͼ	
	}
	else
	{
		return TRUE;									// ��Ҫ�϶�
	}
}


// ���ر�һ��ͼ���ļ��󣬵��ñ���������ʾλ�����ݵ�����
void CDisplay::ClearDispPosInfo()
{
	m_iCurrFr = 0;
	m_iInfo   = 0;

	m_rcBmpRc.SetRectEmpty();							// ͼ��ԭʼ�ߴ�
	
	m_rcPict.SetRectEmpty();							// ����ͼ������ųߴ�
	m_rcPTRc.SetRectEmpty();							// ͼ��Ŀͻ�����
	m_rcFrme.SetRectEmpty();							// ��ΧͼƬ�����ο�ĳߴ�
	m_rcFTRc.SetRectEmpty();							// ���ο�Ŀͻ�����
}


// �ж�ָ���İ��Ƿ��ǵ�ǰ��������ݰ�
BOOL CDisplay::IsCurrPack(LPSPEC_DATAPACK psdp)
{
	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	ASSERT(pDoc);

	return (psdp == pDoc->m_pPack);
}


// ���ƽ�����Ӧ����Ļص�֪ͨ
LRESULT CDisplay::OnDispIntoPlug(WPARAM wParam, LPARAM lParam)
{
	// �ж��Ƿ��ǹ��ڵ�����
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	// �����ȡһ��ͼ�������ʱ�䣨��ʼʱ�䣩
	if (m_cCurrPackCnt)
	{
		m_cCurrPackCnt->BeginCount();
	}

	return 0;
}


// ���ݰ�������ͼ���֪ͨ
LRESULT CDisplay::OnDispBuildMi(WPARAM wParam, LPARAM lParam)
{
	// �ж��Ƿ��ǹ��ڵ�����
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	ASSERT(pDoc);
	ASSERT(pDoc->m_pPack);

	LPINFOSTR pinfo = pDoc->m_pPack->org_img;

	// ��д��ͼ��ĳߴ硢��ʾ����Ϣ
	pDoc->m_dInfo.info.biSize          = sizeof(wBITMAPINFOHEADER);
	pDoc->m_dInfo.info.biWidth         = pinfo->width;
	pDoc->m_dInfo.info.biHeight        = (pinfo->order == 0) ? -(long)pinfo->height : pinfo->height;
	pDoc->m_dInfo.info.biPlanes        = 1;
	pDoc->m_dInfo.info.biBitCount      = (WORD)pinfo->bitcount;
	pDoc->m_dInfo.info.biCompression   = BI_RGB;
	pDoc->m_dInfo.info.biSizeImage     = 0;
	pDoc->m_dInfo.info.biXPelsPerMeter = 0;
	pDoc->m_dInfo.info.biYPelsPerMeter = 0;
	pDoc->m_dInfo.info.biClrUsed       = 0;
	pDoc->m_dInfo.info.biClrImportant  = 0;
	
	memcpy((void*)pDoc->m_dInfo.pal, (const void *)pinfo->palette, sizeof(wRGBQUAD)*MAX_PALETTE);
	
	// ���õ�ǰ֡�ţ�0����ͼ��
	m_iCurrFr = 0;

	// ���µ�ǰͼ��ĸ��ֳߴ�����
	SetPictLocInfo(TRUE);
	SetFrmeLocInfo(TRUE);

	// ����ǿ����л���ʽ����ʱˢ����ʾ
	if (m_iSwitch == ISDISP_SWH_FAST)
	{
		// ���ͼ�����ʾ����ʱ���������Ļ���Ա�����˸
		if ((m_rcFrme.Width() >= m_rcImage.Width())&&(m_rcFrme.Height() >= m_rcImage.Height()))
		{
			InvalidateRect(m_rcImage, FALSE);
		}
		else
		{
			InvalidateRect(m_rcImage, TRUE);
		}

		_UpdateHeadRgn(FALSE);

		// ˢ����ʾ
		UpdateWindow();
	}
	else
	{
		;
	}

	return 0;
}


// ���ݰ�������ͼ��֪ͨ
LRESULT CDisplay::OnDispBuildSi(WPARAM wParam, LPARAM lParam)
{
	// �ж��Ƿ��ǹ��ڵ�����
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	return 0;
}


// ���ݰ���дͼ�󲽽�֪ͨ
LRESULT CDisplay::OnDispProcess(WPARAM wParam, LPARAM lParam)
{
	// �ж��Ƿ��ǹ��ڵ�����
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	CMainFrame		*cmf = (CMainFrame*)AfxGetMainWnd();
	LPSPEC_DATAPACK	psdp = (LPSPEC_DATAPACK)wParam;
	CString			srat;
	
	// ��ʾ��ȡ����
	srat.Format("%d%%", (int)((float)(psdp->s_current*100)/(float)psdp->s_total));
	cmf->m_wndStatusBar.SetPaneText(cmf->m_wndStatusBar.CommandToIndex(IDS_TIMECNT), (LPCTSTR)srat);
	
	if (m_iSwitch == ISDISP_SWH_FAST)
	{
		// Ϊ�ӿ��ٶȣ�ֻ���ڲ����ŵ�����²Ž��в���ˢ��
		if (m_fRatio == DPTRS_REAL)
		{
			InvalidateRect(m_rcImage, FALSE);
			//UpdateWindow();//UPK
		}
	}
	else
	{
		;
	}

	return 0;
}


// ���ݰ���д��֡ͼ�����֪ͨ
LRESULT CDisplay::OnDispOver(WPARAM wParam, LPARAM lParam)
{
	// �ж��Ƿ��ǹ��ڵ�����
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	if (m_iSwitch == ISDISP_SWH_FAST)
	{
		// ˢ����ʾ��ֻ��Ӧ��ͼ��Ľ���֪ͨ��
		if ((lParam == 0)&&(m_fRatio == DPTRS_REAL)&&(((LPSPEC_DATAPACK)wParam)->org_img->imgnumbers == 1))
		{
			InvalidateRect(m_rcImage, FALSE);
			UpdateWindow();
		}
	}
	else
	{
		;
	}

	return 0;
}


// ���ݰ���������֪ͨ���ɹ���
LRESULT CDisplay::OnDispOK(WPARAM wParam, LPARAM lParam)
{
	// �ж��Ƿ��ǹ��ڵ�����
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	CIseetsvcDoc	*pDoc = (CIseetsvcDoc*)GetDocument();
	ASSERT(pDoc);
	ASSERT(m_pLineBuf);

	CMainFrame		*cmf  = (CMainFrame*)AfxGetMainWnd();
	
	// ��ʾ��ȡ��ͼ�������ѵ�ʱ�䣨���룩
	if (m_cCurrPackCnt)
	{
		DWORD		time  = m_cCurrPackCnt->GetTime();
		CString		stime;
		
		stime.Format("%d ����", time);
		if (cmf)
		{
			cmf->m_wndStatusBar.SetPaneText(cmf->m_wndStatusBar.CommandToIndex(IDS_TIMECNT), (LPCTSTR)stime);
		}
	}
	
	LPINFOSTR		pinfo = pDoc->m_pPack->cnv_img;
	SUBIMGBLOCK*	psub;

	// �ж�ͼ��Ŀ���Ƿ��ڿɴ���ķ�Χ��
	if (pinfo->width >= ISDISP_MAXWIDTH)
	{
		isirw_s_wait((LPSPEC_DATAPACK)wParam, 0);

		// �ͷ����ݰ��������ʧ�ܣ��ð������ã�
		isirw_free_SPEC_DATAPACK(pDoc->m_pPack);
		
		pDoc->m_pPack = 0;
		
		m_iCurrFr = 0;

		InvalidateRect(m_rcImage, FALSE);
		UpdateWindow();

		return -2;
	}

	DWORD	i, linesize, hecnt;
	void	*ppv;

	// ���������ͼ����תΪ����ͼ
	if (pinfo->order == 0)
	{
		linesize = DIBSCANLINE_DWORDALIGN(pinfo->width*pinfo->bitcount);
		hecnt    = pinfo->height/2;

		for (i=0; i<hecnt; i++)
		{
			memcpy((void*)m_pLineBuf, (const void *)pinfo->pp_line_addr[i], linesize);
			memcpy((void*)pinfo->pp_line_addr[i], (const void *)pinfo->pp_line_addr[pinfo->height-1-i], linesize);
			memcpy((void*)pinfo->pp_line_addr[pinfo->height-1-i], (const void *)m_pLineBuf, linesize);

			ppv = pinfo->pp_line_addr[i];
			pinfo->pp_line_addr[i] = pinfo->pp_line_addr[pinfo->height-1-i];
			pinfo->pp_line_addr[pinfo->height-1-i] = ppv;
		}

		pinfo->order = 1;
		pDoc->m_dInfo.info.biHeight = pinfo->height;
	}

	// ���������ͼ����һ����ת
	if (pinfo->imgnumbers > 1)
	{
		psub = pinfo->psubimg;

		while (psub)
		{
			if (psub->order == 0)
			{
				linesize = DIBSCANLINE_DWORDALIGN(psub->width*psub->bitcount);
				hecnt    = psub->height/2;
				
				for (i=0; i<(DWORD)hecnt; i++)
				{
					memcpy((void*)m_pLineBuf, (const void *)psub->pp_line_addr[i], linesize);
					memcpy((void*)psub->pp_line_addr[i], (const void *)psub->pp_line_addr[psub->height-1-i], linesize);
					memcpy((void*)psub->pp_line_addr[psub->height-1-i], (const void *)m_pLineBuf, linesize);
					
					ppv = psub->pp_line_addr[i];
					psub->pp_line_addr[i] = psub->pp_line_addr[psub->height-1-i];
					psub->pp_line_addr[psub->height-1-i] = ppv;
				}
				psub->order = 1;
			}
			psub = psub->next;
		}
	}

	// �ȴ��������β�������
	isirw_s_wait((LPSPEC_DATAPACK)wParam, 0);

	// ��дͼ����ʾ����Ϣ
	pDoc->m_dInfo.info.biSize          = sizeof(wBITMAPINFOHEADER);
	pDoc->m_dInfo.info.biWidth         = pinfo->width;
	pDoc->m_dInfo.info.biHeight        = pinfo->height;
	pDoc->m_dInfo.info.biPlanes        = 1;
	pDoc->m_dInfo.info.biBitCount      = (WORD)pinfo->bitcount;
	pDoc->m_dInfo.info.biCompression   = BI_RGB;
	pDoc->m_dInfo.info.biSizeImage     = 0;
	pDoc->m_dInfo.info.biXPelsPerMeter = 0;
	pDoc->m_dInfo.info.biYPelsPerMeter = 0;
	pDoc->m_dInfo.info.biClrUsed       = 0;
	pDoc->m_dInfo.info.biClrImportant  = 0;
	
	memcpy((void*)pDoc->m_dInfo.pal, (const void *)pinfo->palette, sizeof(wRGBQUAD)*MAX_PALETTE);

	if (m_iSwitch == ISDISP_SWH_FAST)
	{
		linesize = DIBSCANLINE_DWORDALIGN(pinfo->width*pinfo->bitcount);
		hecnt    = pinfo->height;

		if ((linesize*hecnt) <= m_lBuffSize)
		{
			memcpy((void*)m_pDispBuf, (const void*)pinfo->p_bit_data, linesize*hecnt);
		}
		else
		{
			char *ptmp;

			// ����������ʾ������
			if (ptmp=(char*)isirw_malloc(linesize*hecnt))
			{
				isirw_free(m_pDispBuf);
				m_pDispBuf  = ptmp;
				memcpy((void*)m_pDispBuf, (const void*)pinfo->p_bit_data, linesize*hecnt);
				m_lBuffSize = linesize*hecnt;
			}
			else
			{
				// ����ֻ����ʾ����ͼ��
				memcpy((void*)m_pDispBuf, (const void*)pinfo->p_bit_data, m_lBuffSize);
			}
		}
	}
	else
	{
		;
	}

	// ��ʼ����ͼ�����ʾ����
	m_iCurrFr = 0;
	m_iInfo   = 0;

	// ��ʼ��ͼ����Ϣ��
	m_sImgInfo.Format("%d��%d��%d��%d", pinfo->width, pinfo->height, pinfo->pater->org_img->bitcount, pinfo->imgnumbers);

	// ��״̬������ʾͼ����Ϣ��
	if (cmf)
	{
		cmf->m_wndStatusBar.SetPaneText(cmf->m_wndStatusBar.CommandToIndex(IDS_IMGINFO), (LPCTSTR)m_sImgInfo);
	}

	_UpdateHeadRgn(FALSE);

	// ˢ����ʾ
	InvalidateRect(m_rcImage, FALSE);
	UpdateWindow();
	
	// ��ɲ�����־Ϊ��
	pDoc->m_bPkst = TRUE;

	// ������ʾ֪ͨ
	if (pinfo->imgnumbers > 1)
	{
		DWORD	ttm = pinfo->time;

		// �ж��Ƿ��Ƕ���ͼ��ICO�ļ��п��ܰ������ͼ�󣬵������Ƕ�����
		for (i=1, psub=pinfo->psubimg; i<pinfo->imgnumbers; i++, psub=psub->next)
		{
			ASSERT(psub);
			ttm += psub->time;
		}

		// ����֪ͨ
		if (ttm)
		{
			m_cAniThread->PostThreadMessage(WM_DANI_NEWCOMM, (WPARAM)pDoc->m_pPack, (LPARAM)1);
		}
	}

	return 0;
}


// ���ݰ���������֪ͨ��ʧ�ܣ�
LRESULT CDisplay::OnDispFaild(WPARAM wParam, LPARAM lParam)
{
	// �ж��Ƿ��ǹ��ڵ�����
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	// �ȴ��������β�������
	isirw_s_wait((LPSPEC_DATAPACK)wParam, 0);

	CIseetsvcDoc	*pDoc = (CIseetsvcDoc*)GetDocument();
	ASSERT(pDoc);
	ASSERT(pDoc->m_pPack);

	// �ͷ����ݰ��������ʧ�ܣ��ð������ã�
	isirw_free_SPEC_DATAPACK(pDoc->m_pPack);
	
	pDoc->m_pPack = 0;

	m_iCurrFr = 0;

	// ˢ����ʾ
	InvalidateRect(m_rcImage, TRUE);
	UpdateWindow();

	return 0;
}


// ��ʾ��һ֡��ͼ��
LRESULT CDisplay::OnDispAniNext(WPARAM wParam, LPARAM lParam)
{
	// �ж��Ƿ��ǹ��ڵ�����
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	LPSPEC_DATAPACK	psdp  = (LPSPEC_DATAPACK)wParam;
	ASSERT(psdp);
	LPINFOSTR		pinfo = psdp->cnv_img;
	ASSERT(pinfo);
	SUBIMGBLOCK*	pold;
	SUBIMGBLOCK*	psub;
	
	DWORD			ckey;
	DWORD			*pdwSou, *pdwDes;
	
	int				linesize;
	int				nWidth;
	int				nHeight;
	int				i, j;

	// ������ǰ����
	switch (m_iInfo)
	{
	case	0:											// ������
		break;
	case	1:											// �򵥸���
		break;
	case	2:											// ����ɫ���
		pold = isirw_s_get_subimg(psdp, m_iCurrFr, 1);
		nWidth  = pold->width;
		nHeight = pold->height;
		linesize = DIBSCANLINE_DWORDALIGN(pinfo->width*pinfo->bitcount);
		
		for (i=0; i<nHeight; i++)
		{
			pdwSou = ((DWORD*)pinfo->pp_line_addr[i+pold->top])+pold->left;
			pdwDes = (DWORD*)(m_pDispBuf+(pinfo->height-i-1-pold->top)*linesize)+pold->left;
			
			for (j=0; j<nWidth; j++)
			{
				*pdwDes++ = *pdwSou++;
			}
		}
		break;
	case	3:											// �ָ�ԭͼ
		break;
	default:											// δ������Ϊ
		break;
	}

	// �����µ�֡��
	m_iCurrFr = (int)lParam;

	// ����ͼ��֡��ˢ����ʾ������
	if (m_iCurrFr == 0)
	{
		m_iInfo  = 0;									// ��ͼ��ȱʡ���÷�ʽΪ������:)
		linesize = DIBSCANLINE_DWORDALIGN(pinfo->width*pinfo->bitcount);
		memcpy((void*)m_pDispBuf, (const void *)pinfo->p_bit_data, pinfo->height*linesize);
	}
	else
	{
		// ���洦�÷�ʽ
		psub = isirw_s_get_subimg(psdp, m_iCurrFr, 1);
		ASSERT(psub);
		m_iInfo = psub->dowith;

		if (psub->colorkey == -1L)
		{
			nWidth  = psub->width;
			nHeight = psub->height;
			linesize = DIBSCANLINE_DWORDALIGN(pinfo->width*pinfo->bitcount);
			
			for (i=0; i<nHeight; i++)
			{
				pdwSou = (DWORD*)psub->pp_line_addr[i];
				pdwDes = (DWORD*)(m_pDispBuf+(pinfo->height-i-1-psub->top)*linesize);
				pdwDes = pdwDes+psub->left;
				
				for (j=0; j<nWidth; j++)
				{
					*pdwDes++ = *pdwSou++;
				}
			}
		}		
		else
		{
			ckey    = (DWORD)psub->colorkey;
			nWidth  = psub->width;
			nHeight = psub->height;
			linesize = DIBSCANLINE_DWORDALIGN(pinfo->width*pinfo->bitcount);
			
			// ͸��ɫ��ʽ�Ŀ���
			for (i=0; i<nHeight; i++)
			{
				pdwSou = (DWORD*)psub->pp_line_addr[i];
				pdwDes = (DWORD*)(m_pDispBuf+(pinfo->height-i-1-psub->top)*linesize);
				pdwDes = pdwDes+psub->left;
				
				for (j=0; j<nWidth; j++)
				{
					if (*pdwSou != ckey)
					{
						*pdwDes = *pdwSou;
					}
					pdwSou++;
					pdwDes++;
				}
			}
		}
	}

	// ��ʾ��ǰ֡
	InvalidateRect(m_rcImage, FALSE);
	UpdateWindow();
	
	return 0;
}


// �˳���β
void CDisplay::OnDestroy() 
{
	CView::OnDestroy();

	// ���浱ǰ����ʾ�趨
	AfxGetApp()->WriteProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_AUTORESIZE, m_iTalAutResize);
	AfxGetApp()->WriteProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_RSROT, (int)(m_fTalRatio*100.0));
	AfxGetApp()->WriteProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_INITPOS, m_iInitPos);
	AfxGetApp()->WriteProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_SWITCH, m_iSwitch);

	// ���ٶ�����ʱ�߳�
	::PostThreadMessage(m_cAniThread->m_nThreadID, WM_DANI_QUIT, 0, 0);

	::WaitForSingleObject(m_cAniThread->m_hThread, 1000);
	
	// ɾ����ʱ����
	if (m_cCurrPackCnt)
	{
		delete m_cCurrPackCnt;
		m_cCurrPackCnt = 0;
	}

	// �ͷ�ͼ��ת������
	if (m_pLineBuf)
	{
		isirw_free(m_pLineBuf);
		m_pLineBuf = 0;
	}

	// �ͷ���ʾ������
	if (m_pDispBuf)
	{
		isirw_free(m_pDispBuf);
		m_pDispBuf = 0;
	}
}


// ��ʾ���ʼ��
void CDisplay::OnInitialUpdate() 
{
	// �����ʱ��
	m_cCurrPackCnt = new CTimeCounter;

	// �ָ���һ�ε���ʾ״̬
	m_iTalAutResize = AfxGetApp()->GetProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_AUTORESIZE, 1);
	m_fTalRatio     = ((float)(AfxGetApp()->GetProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_RSROT, 100)))/(float)100.0;
	m_iInitPos      = AfxGetApp()->GetProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_INITPOS, DPT_CENTER);
	m_iSwitch       = AfxGetApp()->GetProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_SWITCH, ISDISP_SWH_FAST);

	CView::OnInitialUpdate();
}



void CDisplay::OnBtn1()
{
	if (m_iAutoResize)
	{
		m_iAutoResize = FALSE;
		m_fRatio = DPTRS_REAL;
	}
	else
	{
		m_iAutoResize = TRUE;
	}
	
	SetDispLocInfo();
	
	// �������ű���
	SetPictLocInfo();
	
	// ����ͼ�����ʾλ��
	SetFrmeLocInfo(TRUE);
	
	// ˢ����ʾ
	InvalidateRect(m_rcImage, TRUE);
	UpdateWindow();
	
	return;
}


void CDisplay::OnBtn2()
{
	if (m_iPassMark == TRUE)
	{
		m_iPassMark = FALSE;
	}
	else
	{
		m_iPassMark = TRUE;
	}

	return;
}


void CDisplay::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	
	CPoint	point;

	switch (nChar)
	{
	case	VK_CONTROL:
		m_iPassMark = TRUE;
		OnSetCursor(0, 0, 0);
		break;
	case	0xbb:
	case	VK_ADD:
		// ����ʾ���е�Ϊ�Ŵ�����
		point = m_rcImage.CenterPoint();
		_RSL(point);
		break;
	case	0xbd:
	case	VK_SUBTRACT:
		// ����ʾ���е�Ϊ��С����
		point = m_rcImage.CenterPoint();
		_RSS(point);
		break;
	}
}

void CDisplay::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CView::OnKeyUp(nChar, nRepCnt, nFlags);

	if (nChar == VK_CONTROL)
	{
		m_iPassMark = FALSE;
		OnSetCursor(0, 0, 0);
	}
}

void CDisplay::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	LPSPEC_DATAPACK pdsp = pDoc->m_pPack;
	
	if (pdsp == 0)
	{
		CView::OnRButtonDown(nFlags, point);
		return;
	}
	
	CRect	rc;
	
	rc.IntersectRect((LPCRECT)m_rcPTRc, (LPCRECT)m_rcImage);
	
	// �жϹ���Ƿ���ͼ���Ϸ�
	if (rc.PtInRect(point))
	{
		if ((nFlags&MK_CONTROL)||(m_iPassMark))		// �Ŵ�ģʽ
		{
			_RSS(point);
		}
		else
		{
			;
		}
	}
	
	CView::OnRButtonDown(nFlags, point);
}


BOOL CDisplay::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// �����������Ҽ��ѱ�ѹ��ʱ�����ٴ��������Ϣ
	if ((nFlags & MK_LBUTTON)||(nFlags & MK_RBUTTON))
	{
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}

	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	LPSPEC_DATAPACK pdsp = pDoc->m_pPack;

	// ���û��ͼ����ֱ�ӷ���
	if (pdsp == 0)
	{
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}
	
	// ��ͼ��ĸ߶�С����ʾ���߶�ʱ�����ٴ��������Ϣ
	if (m_rcFTRc.Height() <= m_rcImage.Height())
	{
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}

	BOOL	bMark = FALSE;
	CPoint	cpoff(0,0);
	int		i = abs(zDelta);
	
	i = (i/WHEEL_DELTA)*ISDWHEEL_DELTA;					// �޸Ĺ��־���

	if (zDelta > 0)										// �����϶�
	{
		if (m_rcFTRc.top < m_rcImage.top)
		{
			i = ((m_rcFTRc.top+i) > m_rcImage.top) ? (m_rcImage.top-m_rcFTRc.top) : i;
			m_rcFTRc.OffsetRect(0, i);
			m_rcPTRc.OffsetRect(0, i);
			cpoff.y = i;
			bMark   = TRUE;
		}
	}
	else if (zDelta < 0)								// �����϶�
	{
		if (m_rcFTRc.bottom > m_rcImage.bottom)
		{
			i = ((m_rcFTRc.bottom-i) < m_rcImage.bottom) ? (m_rcFTRc.bottom-m_rcImage.bottom) : i;
			m_rcFTRc.OffsetRect(0, -i);
			m_rcPTRc.OffsetRect(0, -i);
			cpoff.y = -i;
			bMark   = TRUE;
		}
	}
	
	// ���������λ�ƣ�����������е�����
	if (bMark == TRUE)
	{
		ScrollWindowEx(cpoff.x, cpoff.y, m_rcImage, m_rcImage, 0, 0, SW_INVALIDATE|SW_ERASE);
	}
	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


// ��ָ����Ϊ���ķŴ�ͼ��
void CDisplay::_RSL(CPoint point)
{
	m_iAutoResize = 0;

	// �ж��Ƿ��м����Ŵ�����
	if (m_fRatio < DPTRS_MAX)
	{
		// ȡ���·�������ͼ�������λ��
		int x = (int)(((float)(point.x-m_rcPTRc.left)/m_fRatio)+(float)0.5);
		int y = (int)(((float)(point.y-m_rcPTRc.top)/m_fRatio)+(float)0.5);
		
		m_fRatio = min(DPTRS_MAX, (m_fRatio+DPTRS_STEP));
		
		// ����ͼ�����ųߴ�
		SetPictLocInfo();
		
		// ��¼ԭͼ��ʾ��λ��
		CRect	tmp = m_rcFTRc;
		
		m_rcPTRc = m_rcPict;
		m_rcFTRc = m_rcFrme;
		
		// ����Ŵ��ǰ��ľ���
		LONG sx = point.x - (LONG)(int)((float)x * m_fRatio);
		LONG sy = point.y - (LONG)(int)((float)y * m_fRatio);
		
		// �����µ���ʾλ��
		m_rcPTRc.OffsetRect((int)sx, (int)sy);
		m_rcFTRc.OffsetRect((int)(m_rcPTRc.CenterPoint().x - m_rcFTRc.Width()/2), (int)(m_rcPTRc.CenterPoint().y - m_rcFTRc.Height()/2));
		
		int ox, orx;
		int oy, ory;
		
		// ������ʾλ�ã���ͼ��С����ʾ��ʱ����������ʾ��
		if (m_rcFTRc.Width() < m_rcImage.Width())
		{
			ox  = m_rcImage.Width()/2-m_rcFTRc.Width()/2+m_rcImage.left;
			orx	= ox - m_rcFTRc.left;
			m_rcFTRc.SetRect(ox, m_rcFTRc.top, ox+m_rcFTRc.Width(), m_rcFTRc.bottom);
			m_rcPTRc.OffsetRect(orx, 0);
		}
		else
		{
			if (m_rcFTRc.left > m_rcImage.left)
			{
				ox = m_rcImage.left - m_rcFTRc.left;
			}
			else if (m_rcFTRc.right < m_rcImage.right)
			{
				ox = m_rcImage.right - m_rcFTRc.right;
			}
			else
			{
				ox = 0;
			}
			m_rcFTRc.OffsetRect(ox, 0);
			m_rcPTRc.OffsetRect(ox, 0);
		}
		
		if (m_rcFTRc.Height() < m_rcImage.Height())
		{
			oy  = m_rcImage.Height()/2-m_rcFTRc.Height()/2+m_rcImage.top;
			ory = oy - m_rcFTRc.top;
			m_rcFTRc.SetRect(m_rcFTRc.left, oy, m_rcFTRc.right, oy+m_rcFTRc.Height());
			m_rcPTRc.OffsetRect(0, ory);
		}
		else
		{
			if (m_rcFTRc.top > m_rcImage.top)
			{
				oy = m_rcImage.top - m_rcFTRc.top;
			}
			else if (m_rcFTRc.bottom < m_rcImage.bottom)
			{
				oy = m_rcImage.bottom - m_rcFTRc.bottom;
			}
			else
			{
				oy = 0;
			}
			m_rcFTRc.OffsetRect(0, oy);
			m_rcPTRc.OffsetRect(0, oy);
		}
		
		tmp.UnionRect(tmp, m_rcFTRc);
		tmp.InflateRect(1,1);
		
		// ˢ����ʾ��
		_UpdateImageRgn(tmp);
	}
}


// ��ָ����Ϊ������Сͼ��
void CDisplay::_RSS(CPoint point)
{
	m_iAutoResize = 0;
	
	if (m_fRatio > DPTRS_MIN)
	{
		// ȡ���·�������ͼ�������λ��
		int x = (int)(((float)(point.x-m_rcPTRc.left)/m_fRatio)+(float)0.5);
		int y = (int)(((float)(point.y-m_rcPTRc.top)/m_fRatio)+(float)0.5);
		
		m_fRatio = max(DPTRS_MIN, (m_fRatio-DPTRS_STEP));
		
		// ����ͼ�����ųߴ�
		SetPictLocInfo();
		
		// ��¼ԭͼ��ʾ��λ��
		CRect	tmp = m_rcFTRc;
		
		m_rcPTRc = m_rcPict;
		m_rcFTRc = m_rcFrme;
		
		// ������С��ǰ��ľ���
		LONG sx = point.x - (LONG)(int)((float)x * m_fRatio);
		LONG sy = point.y - (LONG)(int)((float)y * m_fRatio);
		
		// �����µ���ʾλ��
		m_rcPTRc.OffsetRect((int)sx, (int)sy);
		m_rcFTRc.OffsetRect((int)(m_rcPTRc.CenterPoint().x - m_rcFTRc.Width()/2), (int)(m_rcPTRc.CenterPoint().y - m_rcFTRc.Height()/2));
		
		int ox, orx;
		int oy, ory;
		
		// ������ʾλ�ã���ͼ��С����ʾ��ʱ����������ʾ��
		if (m_rcFTRc.Width() < m_rcImage.Width())
		{
			ox  = m_rcImage.Width()/2-m_rcFTRc.Width()/2+m_rcImage.left;
			orx	= ox - m_rcFTRc.left;
			m_rcFTRc.SetRect(ox, m_rcFTRc.top, ox+m_rcFTRc.Width(), m_rcFTRc.bottom);
			m_rcPTRc.OffsetRect(orx, 0);
		}
		else
		{
			if (m_rcFTRc.left > m_rcImage.left)
			{
				ox = m_rcImage.left - m_rcFTRc.left;
			}
			else if (m_rcFTRc.right < m_rcImage.right)
			{
				ox = m_rcImage.right - m_rcFTRc.right;
			}
			else
			{
				ox = 0;
			}
			m_rcFTRc.OffsetRect(ox, 0);
			m_rcPTRc.OffsetRect(ox, 0);
		}
		
		if (m_rcFTRc.Height() < m_rcImage.Height())
		{
			oy  = m_rcImage.Height()/2-m_rcFTRc.Height()/2+m_rcImage.top;
			ory = oy - m_rcFTRc.top;
			m_rcFTRc.SetRect(m_rcFTRc.left, oy, m_rcFTRc.right, oy+m_rcFTRc.Height());
			m_rcPTRc.OffsetRect(0, ory);
		}
		else
		{
			if (m_rcFTRc.top > m_rcImage.top)
			{
				oy = m_rcImage.top - m_rcFTRc.top;
			}
			else if (m_rcFTRc.bottom < m_rcImage.bottom)
			{
				oy = m_rcImage.bottom - m_rcFTRc.bottom;
			}
			else
			{
				oy = 0;
			}
			m_rcFTRc.OffsetRect(0, oy);
			m_rcPTRc.OffsetRect(0, oy);
		}
		
		// ��Ϊ����С��������Ҫ�ϲ�ԭͼ�����ʾ��
		tmp.UnionRect(tmp, m_rcFTRc);
		tmp.InflateRect(1,1);
		
		// ˢ����ʾ��
		_UpdateImageRgn(tmp);
	}
}


// ˢ��ͼ����ʾ��
void CDisplay::_UpdateImageRgn(CRect& rect)
{
	CRect	rc;

	// ȡ����������ͼ����ʾ���Ľ�������
	rc.IntersectRect(rect, m_rcImage);

	// ˢ�´���
	InvalidateRect(rc);
	UpdateWindow();
}


// ˢ�±�����
void CDisplay::_UpdateHeadRgn(BOOL rgn)
{
	if (rgn)											// ˢ��������Ϣ����
	{
		InvalidateRect(m_rcHeader);
		UpdateWindow();
	}
	else												// ֻˢ��ͼ������
	{
		CRect	rc(m_rcHeader.left, m_rcHeader.top, m_rcHeader.left+ISDISP_HEADER_W, m_rcHeader.top+ISDISP_HEADER_H);
		InvalidateRect(rc);
		UpdateWindow();
	}
}


