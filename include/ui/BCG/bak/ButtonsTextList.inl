
// ButtonsTextList.cpp : implementation file
//


#include "ButtonsTextList.h"
#include "afxadv.h"
#include "BCGPToolbar.h"
#include "BCGPToolbarButton.h"
#include "bcgprores.h"

#include "BCGGlobals.h"
#include "BCGPVisualManager.h"

/////////////////////////////////////////////////////////////////////////////
// CButtonsTextList

CButtonsTextList::CButtonsTextList() :
	m_sizeButton (0, 0)
{
}

CButtonsTextList::~CButtonsTextList()
{
}


BEGIN_MESSAGE_MAP(CButtonsTextList, CListBox)
	//{{AFX_MSG_MAP(CButtonsTextList)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonsTextList message handlers

void CButtonsTextList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CListBox::OnLButtonDown(nFlags, point);

	int iIndex = GetCurSel();
	if (iIndex == LB_ERR)
	{
		return;
	}

	//-------------------------------------------
	// Be sure that we realy click into the item!
	//-------------------------------------------
	CRect rect;
	GetItemRect (iIndex, &rect);

	if (!rect.PtInRect (point))
	{
		return;
	}

	//-----------------------------------------------------------
	// Trigger mouse up event (to change selection notification):
	//-----------------------------------------------------------
	SendMessage (WM_LBUTTONUP, nFlags, MAKELPARAM (point.x, point.y));

	//---------------------
	// Get selected button:
	//---------------------
	CBCGPToolbarButton* pButton = (CBCGPToolbarButton*) GetItemData (iIndex);
	ASSERT_VALID(pButton);

	//--------------------------------------
	// Prepare clipboard data and start drag:
	//--------------------------------------
	COleDataSource srcItem;

	pButton->m_bDragFromCollection = TRUE;
	pButton->PrepareDrag (srcItem);
	pButton->m_bDragFromCollection = FALSE;

	{
		CBCGPLocalResource locaRes;
		::SetCursor (AfxGetApp()->LoadCursor (IDC_BCGBARRES_DELETE));
	}

	srcItem.DoDragDrop
		(DROPEFFECT_COPY|DROPEFFECT_MOVE, &rect, &CBCGPToolBar::m_DropSource);
}
//****
void CButtonsTextList::MeasureItem(LPMEASUREITEMSTRUCT lpMIS) 
{
	ASSERT (lpMIS != NULL);

	UINT uiRowHeight = (UINT) m_sizeButton.h;
	if (lpMIS->itemHeight < uiRowHeight)
	{
		lpMIS->itemHeight = uiRowHeight;
	}
}
//****
void CButtonsTextList::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	ASSERT_VALID (pDC);

	CRect rect = lpDIS->rcItem;

	if (lpDIS->itemID != (UINT)-1)
	{
		CBCGPToolbarButton* pButton = (CBCGPToolbarButton*) GetItemData (lpDIS->itemID);
		ASSERT_VALID(pButton);

		CString strText = pButton->m_strText;
		GetText (lpDIS->itemID, pButton->m_strText);

		CBCGPVisualManager::GetInstance()->OnFillCommandsListBackground (pDC, rect);

		pButton->OnDrawOnCustomizeList (pDC, rect, 
			(lpDIS->itemState & ODS_SELECTED) && (lpDIS->itemState & ODS_FOCUS));
		pButton->m_strText = strText;
	}
}
//****
void CButtonsTextList::PreSubclassWindow() 
{
	CListBox::PreSubclassWindow();

	m_sizeButton = CBCGPToolBar::GetMenuImageSize();
	m_sizeButton.w += 6;
	m_sizeButton.h += 6;

	m_sizeButton.h = max (m_sizeButton.h, globalData.GetTextHeight());
}
//****
BOOL CButtonsTextList::OnEraseBkgnd(CDC* pDC)
{
	ASSERT_VALID (pDC);

	CRect rectClient;
	GetClientRect (rectClient);

	CBCGPVisualManager::GetInstance()->OnFillCommandsListBackground (pDC, rectClient);
	return TRUE;
}
