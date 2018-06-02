/********************************************************************

	NewSpliWnd.cpp

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
					��ʵ���ļ���

	���ļ���д�ˣ�	
					YZ				yzfree##sina.com

	���ļ��汾��	30122
	����޸��ڣ�	2003-1-22

	ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
		��ַ�ռ������

	������ʷ��

		2003-1		��һ�����԰淢��

********************************************************************/

#include "stdafx.h"
#include "iseetsvc.h"
#include "NewSpliWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CNewSpliWnd

IMPLEMENT_DYNCREATE(CNewSpliWnd, CSplitterWnd)

CNewSpliWnd::CNewSpliWnd()
{
	count = 0;

	rc.SetRectEmpty();
}

CNewSpliWnd::~CNewSpliWnd()
{
}


BEGIN_MESSAGE_MAP(CNewSpliWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CNewSpliWnd)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CNewSpliWnd message handlers

BOOL CNewSpliWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CSplitterWnd::PreCreateWindow(cs);
}


// �µĻ��ƺ���
void CNewSpliWnd::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)
{
	if (pDC == NULL)
	{
		CSplitterWnd::OnDrawSplitter(pDC, nType, rectArg);
		return;
	}		

	CRect		rect = rectArg;
	COLORREF	col = (COLORREF)RGB(215,255,215);
	POINT		ptst, pten;

	switch (nType)
	{
	case splitBorder:
		if ((GetPane(0, 0)->GetStyle() & WS_VSCROLL)&&(!(GetParent()->GetStyle() & WS_CHILDWINDOW)))
		{
			CSplitterWnd::OnDrawSplitter(pDC, nType, rectArg);

			count = 0;
			rc.SetRectEmpty();

			break;
		}
		
		if (count++ != (GetRowCount()*GetColumnCount()-1))
		{
			rc.UnionRect(rc, rect);
		}
		else
		{
			rc.UnionRect(rc, rect);

			// ���Ѵ����еķ��Ѵ��ڲ��ٻ��Ʊ߿�
			if (!(GetParent()->GetStyle() & WS_CHILDWINDOW))
			{
				pDC->Draw3dRect(rc, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DHILIGHT));
				rc.InflateRect(-1, -1);
				pDC->Draw3dRect(rc, GetSysColor(COLOR_WINDOWFRAME), GetSysColor(COLOR_3DFACE));
			}

			count = 0;
			rc.SetRectEmpty();
		}
		break;		
	case splitIntersection:
	case splitBox:
		break;											// �����ָ����Ļ���
	case splitBar:
		if ((GetPane(0, 0)->GetStyle() & WS_VSCROLL)&&(!(GetParent()->GetStyle() & WS_CHILDWINDOW)))
		{
			CSplitterWnd::OnDrawSplitter(pDC, nType, rectArg);
		}
		else
		{
			rect.InflateRect(CNSW_FILL_WIDTH_CURTAIL, CNSW_FILL_HEIGHT_CURTAIL);

			pDC->FillSolidRect(rect, col);
		
			if ((rect.Width() > CNSW_FILL_WIDTH_MINI)&&(rect.Height() > CNSW_FILL_HEIGHT_MINI))
			{
				ptst.x = (rect.right-rect.left)/2+rect.left;
				ptst.y = rect.top+CNSW_LINE_CURTAIL;

				pten.x = ptst.x;
				pten.y = rect.bottom-CNSW_LINE_CURTAIL;

				//pDC->MoveTo(ptst);
				//pDC->LineTo(pten);							// ֻ����һ���ָ����
			}
		}
		break;
	default:
		ASSERT(FALSE);
	}
	
	return;
}


// �·��������ƺ���
void CNewSpliWnd::OnInvertTracker(const CRect& rect)
{
	ASSERT_VALID(this);
	ASSERT(!rect.IsRectEmpty());
	ASSERT((GetStyle() & WS_CLIPCHILDREN) == 0);
	
	CDC* pDC = GetDC();

	// ������ˢ��ʽ
	CBrush* pBrush = CDC::GetHalftoneBrush();
	HBRUSH hOldBrush = NULL;

	if (pBrush != NULL)
	{
		hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);
	}

	CRect	rt = rect;

	if ((GetPane(0, 0)->GetStyle() & WS_VSCROLL)&&(!(GetParent()->GetStyle() & WS_CHILDWINDOW)))
	{
		rt.InflateRect(0, 0);
	}
	else
	{
		rt.InflateRect(0, -CNSW_LINE_CURTAIL);
	}

	// �����϶���Ӱ
	pDC->PatBlt(rt.left, rt.top, rt.Width(), rt.Height(), PATINVERT);

	if (hOldBrush != NULL)
	{
		SelectObject(pDC->m_hDC, hOldBrush);
	}

	ReleaseDC(pDC);
}


BOOL CNewSpliWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
