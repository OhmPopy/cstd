
//
// BCGPAutoHideDockBar.cpp : implementation file
//

//#include "BCGCBPro.h"
#include "BCGPAutoHideDockBar.h"
#include "BCGPControlBar.h"
#include "BCGPDockBarRow.h"
#include "BCGPVisualManager.h"
#include "BCGPAutoHideToolBar.h"

int	  CBCGPAutoHideDockBar::m_nExtraSpace = 2;

IMPLEMENT_DYNCREATE(CBCGPAutoHideDockBar, CBCGPDockBar)

/////////////////////////////////////////////////////////////////////////////
// CBCGPAutoHideDockBar

CBCGPAutoHideDockBar::CBCGPAutoHideDockBar()
{
	m_nOffsetLeft = 0;
	m_nOffsetRight = 0;
}

CBCGPAutoHideDockBar::~CBCGPAutoHideDockBar()
{
}


BEGIN_MESSAGE_MAP(CBCGPAutoHideDockBar, CBCGPDockBar)
	//{{AFX_MSG_MAP(CBCGPAutoHideDockBar)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//----------------------------------------------------------------------------------//
void CBCGPAutoHideDockBar::DockControlBar (CBCGPControlBar* pControlBar, BCGP_DOCK_METHOD /*dockMethod*/, 
								    LPCRECT lpRect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pControlBar);

	BOOL bVertDock = !IsHorizontal();
	CSize szBarSize = pControlBar->CalcFixedLayout (FALSE, !bVertDock);

	// the control bar doesn't take up all space of the row
	int nRowHeight = bVertDock ? szBarSize.w + m_nExtraSpace 
							   : szBarSize.h + m_nExtraSpace;

	if (!m_lstControlBars.Find (pControlBar))
	{
		CBCGPDockBarRow* pRowToDock = NULL;

		if (m_lstDockBarRows.IsEmpty())
		{
			pRowToDock = AddRow (NULL, nRowHeight);
			if (GetCurrentAlignment() & CBRS_ALIGN_LEFT || 
				GetCurrentAlignment() & CBRS_ALIGN_TOP)
			{
				pRowToDock->SetExtra (m_nExtraSpace, BCGP_ROW_ALIGN_TOP);
			}
			else
			{
				pRowToDock->SetExtra (m_nExtraSpace, BCGP_ROW_ALIGN_BOTTOM);
			}
		}
		else
		{
			pRowToDock = (CBCGPDockBarRow*) m_lstDockBarRows.GetHead();
		}
		
		ASSERT_VALID (pRowToDock);
		// the bar should be placed on the existing row or new row
		pRowToDock->AddControlBar (pControlBar, BCGP_DM_RECT, lpRect, TRUE);

		ShowWindow (SW_SHOW);

		m_lstControlBars.AddTail (pControlBar);
		AdjustDockingLayout();
		CRect rectClient;
		GetClientRect (rectClient);
		RepositionBars (rectClient);
		
	}
}
//----------------------------------------------------------------------------------//
void CBCGPAutoHideDockBar::RepositionBars (CRect& /*rectNewClientArea*/)
{
	ASSERT_VALID (this);

	if (!m_lstDockBarRows.IsEmpty())
	{
		CBCGPDockBarRow* pRow = (CBCGPDockBarRow*) m_lstDockBarRows.GetHead();
		ASSERT_VALID (pRow);

		pRow->ArrangeControlBars (m_nOffsetLeft + globalData.m_nAutoHideToolBarMargin, 
									globalData.m_nAutoHideToolBarSpacing);
		
		if (CBCGPVisualManager::GetInstance()->HasOverlappedAutoHideButtons())
		{
			pRow->RedrawAll();
		}
	}
}
//----------------------------------------------------------------------------------//
void CBCGPAutoHideDockBar::UnSetAutoHideMode (CBCGPAutoHideToolBar* pAutohideToolbar)
{
	if (pAutohideToolbar == NULL)
	{
		CObList lstBars;
		lstBars.AddTail (&m_lstControlBars);

		POSITION posSave = NULL;
		POSITION pos = NULL;

		for (pos = lstBars.GetHeadPosition(); pos != NULL;)
		{
			posSave = pos;
			CBCGPAutoHideToolBar* pToolBar = (CBCGPAutoHideToolBar*) lstBars.GetNext (pos);
			if (!pToolBar->m_bFirstInGroup)
			{
				lstBars.RemoveAt (posSave);
			}
		}

		for (pos = lstBars.GetHeadPosition(); pos != NULL;)
		{
			CBCGPAutoHideToolBar* pToolBar = (CBCGPAutoHideToolBar*) lstBars.GetNext (pos);
			UnSetAutoHideMode (pToolBar);
		}
		return;
	}

	// find the group;
	CBCGPDockBarRow* pRow = RowFromControlBar (pAutohideToolbar);

	CObList lstGroup;
	if (pRow != NULL)
	{
		pRow->GetGroupFromBar (pAutohideToolbar, lstGroup);
	}

	if (lstGroup.IsEmpty())
	{
		pAutohideToolbar->UnSetAutoHideMode (NULL);
	}
	else
	{
		BOOL bFirstBar = TRUE;
		CBCGPDockingControlBar* pFirstBar = NULL;
		for (POSITION pos = lstGroup.GetHeadPosition(); pos != NULL;)
		{
			CBCGPAutoHideToolBar* pNextBar = 
				DYNAMIC_DOWNCAST (CBCGPAutoHideToolBar, lstGroup.GetNext (pos));
			if (pNextBar != NULL)
			{
				if (bFirstBar)
				{
					pFirstBar = pNextBar->GetFirstAHWindow();
					pNextBar->UnSetAutoHideMode (NULL);
					bFirstBar = FALSE;
				}
				else
				{
					pNextBar->UnSetAutoHideMode (pFirstBar);
				}
			}	
		}
	}
}
//----------------------------------------------------------------------------------//
void CBCGPAutoHideDockBar::OnPaint(CDC* pDC) 
{
	CPaintDC dc(this); // device context for painting
	CBCGPMemDC memDC (dc, this);

	CRect rectClient;
	GetClientRect (rectClient);

	CBCGPVisualManager::GetInstance()->OnFillBarBackground (&memDC.GetDC(), this,
		rectClient, rectClient);
}
//----------------------------------------------------------------------------------//
void CBCGPAutoHideDockBar::GetAlignRect (CRect& rect) const
{
	GetWindowRect (rect);

	if (IsHorizontal())  
	{
		rect.l += m_nOffsetLeft;
		rect.r -= m_nOffsetRight; 
	}
	else
	{
		 rect.t += m_nOffsetLeft;
		 rect.b -= m_nOffsetRight;
	}
}
//----------------------------------------------------------------------------------//
BOOL CBCGPAutoHideDockBar::CanAcceptBar (const CBCGPBaseControlBar* pBar) const
{
	return pBar->IsKindOf (RUNTIME_CLASS (CBCGPAutoHideToolBar));
}