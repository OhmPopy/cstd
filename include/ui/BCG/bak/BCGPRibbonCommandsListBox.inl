
//
// BCGPRibbonCommandsListBox.cpp : implementation file
//

#include "BCGPRibbonCommandsListBox.h"
#include "BCGPRibbonBar.h"
#include "BCGPRibbonCategory.h"

#ifndef BCGP_EXCLUDE_RIBBON

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonCommandsListBox

CBCGPRibbonCommandsListBox::CBCGPRibbonCommandsListBox(CBCGPRibbonBar* pRibbonBar,
													   BOOL bIncludeSeparator/* = TRUE*/,
													   BOOL bDrawDefaultIcon/* = FALSE*/)
{
	ASSERT_VALID (pRibbonBar);

	m_pRibbonBar = pRibbonBar;
	m_nTextOffset = 0;
	m_bDrawDefaultIcon = bDrawDefaultIcon;

	if (bIncludeSeparator)
	{
		m_pSeparator = new CBCGPRibbonSeparator (TRUE);
	}
	else
	{
		m_pSeparator = NULL;
	}
}
//****
CBCGPRibbonCommandsListBox::~CBCGPRibbonCommandsListBox()
{
	if (m_pSeparator != NULL)
	{
		delete m_pSeparator;
	}
}

BEGIN_MESSAGE_MAP(CBCGPRibbonCommandsListBox, CListBox)
	//{{AFX_MSG_MAP(CBCGPRibbonCommandsListBox)
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_MEASUREITEM_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonCommandsListBox message handlers

void CBCGPRibbonCommandsListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	ASSERT_VALID (pDC);

	CRect rect = lpDIS->rcItem;

	if (lpDIS->itemID == (UINT)-1)
	{
		return;
	}

	BOOL bIsRibbonImageScale = globalData.IsRibbonImageScaleEnabled();
	globalData.EnableRibbonImageScale (FALSE);

	pDC->SetBkMode (TRANSPARENT);

	BOOL bIsHighlighted = 
		(lpDIS->itemState & ODS_SELECTED) && (lpDIS->itemState & ODS_FOCUS);
	BOOL bIsSelected = (lpDIS->itemState & ODS_SELECTED);

	CBCGPBaseRibbonElement* pCommand = (CBCGPBaseRibbonElement*) GetItemData (lpDIS->itemID);
	ASSERT_VALID (pCommand);

	CString strText;
	GetText (lpDIS->itemID, strText);

	if (bIsHighlighted)
	{
		::FillRect (pDC, rect, GetSysColorBrush (COLOR_HIGHLIGHT));
		pDC->SetTextColor (_GetSysColor(CLR_HIGHLIGHTTEXT));
	}
	else if (bIsSelected)
	{
		pDC->FillRect (rect, &globalData.brBtnFace);
		pDC->SetTextColor (globalData.clrBtnText);
	}
	else
	{
		pDC->FillRect (rect, &globalData.brWindow);
		pDC->SetTextColor (globalData.clrWindowText);
	}

	BOOL bDrawDefaultIconSaved = pCommand->m_bDrawDefaultIcon;
	pCommand->m_bDrawDefaultIcon = m_bDrawDefaultIcon;

	pCommand->OnDrawOnList (pDC, strText, m_nTextOffset, rect, bIsSelected, bIsHighlighted);

	pCommand->m_bDrawDefaultIcon = bDrawDefaultIconSaved;

	globalData.EnableRibbonImageScale (bIsRibbonImageScale);
}
//****
void CBCGPRibbonCommandsListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	ASSERT (lpMIS != NULL);

	CClientDC dc (this);
	CFont* pOldFont = (CFont*) dc.SelectStockObject (DEFAULT_GUI_FONT);
	ASSERT_VALID (pOldFont);

	TEXTMETRIC tm;
	dc.GetTextMetrics (&tm);

	lpMIS->itemHeight = tm.tmHeight + 6;

	dc.SelectObject (pOldFont);
}
//****
void CBCGPRibbonCommandsListBox::FillFromCategory (CBCGPRibbonCategory* pCategory)
{
	ASSERT_VALID (this);

	ResetContent();
	m_nTextOffset = 0;

	if (pCategory == NULL)
	{
		return;
	}

	ASSERT_VALID (pCategory);

	CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*> arElements;
	pCategory->GetElements (arElements);

	FillFromArray (arElements, TRUE, TRUE);

	if (m_pSeparator != NULL)
	{
		ASSERT_VALID (m_pSeparator);
		m_pSeparator->AddToListBox (this, FALSE);
	}
}
//****
void CBCGPRibbonCommandsListBox::FillFromArray (
		const CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arElements, BOOL bDeep,
		BOOL bIgnoreSeparators)
{
	ASSERT_VALID (this);

	ResetContent();
	m_nTextOffset = 0;

	BOOL bIsRibbonImageScale = globalData.IsRibbonImageScaleEnabled();
	globalData.EnableRibbonImageScale (FALSE);

	for (int i = 0; i < arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = arElements [i];
		ASSERT_VALID (pElem);

		if (bIgnoreSeparators && pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonSeparator)))
		{
			continue;
		}

		pElem->AddToListBox (this, bDeep);

		int nImageWidth = pElem->GetImageSize (CBCGPBaseRibbonElement::RibbonImageSmall).w;

		m_nTextOffset = max (m_nTextOffset, nImageWidth + 2);
	}

	if (GetCount() > 0)
	{
		SetCurSel (0);
	}

	globalData.EnableRibbonImageScale (bIsRibbonImageScale);
}
//****
void CBCGPRibbonCommandsListBox::FillFromIDs (const CList<UINT,UINT>& lstCommands,
											  BOOL bDeep)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pRibbonBar);

	CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*> arElements;

	for (POSITION pos = lstCommands.GetHeadPosition(); pos != NULL;)
	{
		UINT uiCmd = lstCommands.GetNext (pos);

		if (uiCmd == 0)
		{
			if (m_pSeparator != NULL)
			{
				arElements.Add (m_pSeparator);
			}

			continue;
		}

		CBCGPBaseRibbonElement* pElem = m_pRibbonBar->FindByID (uiCmd, FALSE);
		if (pElem == NULL)
		{
			continue;
		}

		ASSERT_VALID (pElem);
		arElements.Add (pElem);
	}

	FillFromArray (arElements, bDeep, FALSE);
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonCommandsListBox::GetSelected() const
{
	ASSERT_VALID (this);

	int nIndex = GetCurSel();

	if (nIndex < 0)
	{
		return NULL;
	}

	return GetCommand (nIndex);
}
//****
CBCGPBaseRibbonElement* CBCGPRibbonCommandsListBox::GetCommand (int nIndex) const
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement* pCommand = 
		(CBCGPBaseRibbonElement*) GetItemData (nIndex);
	ASSERT_VALID (pCommand);

	return pCommand;
}
//****
int CBCGPRibbonCommandsListBox::GetCommandIndex (UINT uiID) const
{
	ASSERT_VALID (this);

	for (int i = 0; i < GetCount(); i++)
	{
		CBCGPBaseRibbonElement* pCommand = (CBCGPBaseRibbonElement*) GetItemData (i);
		ASSERT_VALID (pCommand);

		if (pCommand->GetID() == uiID)
		{
			return i;
		}
	}

	return -1;
}
//****
BOOL CBCGPRibbonCommandsListBox::AddCommand (CBCGPBaseRibbonElement* pCmd, BOOL bSelect, BOOL bDeep)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pCmd);

	int nIndex = GetCommandIndex (pCmd->GetID());
	if (nIndex >= 0 && pCmd->GetID() != 0)
	{
		return FALSE;
	}

	// Not found, add new:

	if (m_nTextOffset == 0)
	{
		BOOL bIsRibbonImageScale = globalData.IsRibbonImageScaleEnabled();
		globalData.EnableRibbonImageScale (FALSE);

		m_nTextOffset = pCmd->GetImageSize (CBCGPBaseRibbonElement::RibbonImageSmall).w + 2;

		globalData.EnableRibbonImageScale (bIsRibbonImageScale);
	}

	nIndex = pCmd->AddToListBox (this, bDeep);		

	if (bSelect)
	{
		SetCurSel (nIndex);
	}

	return TRUE;
}

#endif	// BCGP_EXCLUDE_RIBBON