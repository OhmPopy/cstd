

// BCGFontComboBox.cpp : implementation file
//

#include "BCGPFontComboBox.h"
#include "BCGPToolBar.h"
#include "BCGPToolbarFontCombo.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"

const int nImageHeight_fcb = 16;
const int nImageWidth_fcb = 16;

BOOL CBCGPFontComboBox::m_bDrawUsingFont = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CBCGPFontComboBox

CBCGPFontComboBox::CBCGPFontComboBox() :
	m_bToolBarMode (FALSE)
{
}

CBCGPFontComboBox::~CBCGPFontComboBox()
{
}

BEGIN_MESSAGE_MAP(CBCGPFontComboBox, CBCGPComboBox)
	//{{AFX_MSG_MAP(CBCGPFontComboBox)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPFontComboBox message handlers

BOOL CBCGPFontComboBox::PreTranslateMessage(MSG* pMsg)
{
	if (m_bToolBarMode &&
		pMsg->message == WM_KEYDOWN &&
		!CBCGPToolbarFontCombo::IsFlatMode ())
	{
		CBCGPToolBar* pBar = (CBCGPToolBar*) GetParent ();

		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			if (BCGCBProGetTopLevelFrame (this) != NULL)
			{
				BCGCBProGetTopLevelFrame (this)->SetFocus ();
			}
			return TRUE;

		case VK_TAB:
			if (pBar != NULL)
			{
				pBar->GetNextDlgTabItem (this)->SetFocus();
			}
			return TRUE;

		case VK_UP:
		case VK_DOWN:
			if ((GetKeyState(VK_MENU) >= 0) && (GetKeyState(VK_CONTROL) >=0) &&
				!GetDroppedState())
			{
				ShowDropDown();
				return TRUE;
			}
		}
	}

	return CBCGPComboBox::PreTranslateMessage(pMsg);
}
//****
int CBCGPFontComboBox::CompareItem(LPCOMPAREITEMSTRUCT lpCIS)
{
	ASSERT(lpCIS->CtlType == ODT_COMBOBOX);

	int id1 = (int)(WORD)lpCIS->itemID1;
	if (id1 == -1)
	{
		return -1;
	}

	CString str1;
	GetLBText (id1, str1);

	int id2 = (int)(WORD)lpCIS->itemID2;
	if (id2 == -1)
	{
		return 1;
	}

	CString str2;
	GetLBText (id2, str2);

	return str1.Collate (str2);
}
//****
void CBCGPFontComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	if (m_Images.GetSafeHandle () == NULL)
	{
		CBCGPLocalResource locaRes;
		m_Images.Create (IDB_BCGBARRES_FONT, nImageWidth_fcb, 0, RGB (255, 255, 255));
	}

	ASSERT (lpDIS->CtlType == ODT_COMBOBOX);

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	ASSERT_VALID (pDC);

	CRect rc = lpDIS->rcItem;
	
	if (lpDIS->itemState & ODS_FOCUS)
	{
		pDC->DrawFocusRect(rc);
	}

	int nIndexDC = pDC->SaveDC ();

	CBrush brushFill;
	if (lpDIS->itemState & ODS_SELECTED)
	{
		brushFill.CreateSolidBrush (globalData.clrHilite);
		pDC->SetTextColor (globalData.clrTextHilite);
	}
	else
	{
		brushFill.CreateSolidBrush (pDC->GetBkColor());
	}

	pDC->SetBkMode(TRANSPARENT);
	pDC->FillRect(rc, &brushFill);

	int id = (int)lpDIS->itemID;
	if (id >= 0)
	{
		CFont fontSelected;
		CFont* pOldFont = NULL;

		CBCGPFontDesc* pDesc = (CBCGPFontDesc*)lpDIS->itemData;
		if (pDesc != NULL)
		{
			if (pDesc->m_nType & (DEVICE_FONTTYPE | TRUETYPE_FONTTYPE))
			{
				CPoint ptImage (rc.left, rc.top + (rc.Height () - nImageHeight_fcb) / 2);
				m_Images.Draw (pDC, (pDesc->m_nType & DEVICE_FONTTYPE) ? 0 : 1, 
					ptImage, ILD_NORMAL);
			}

			rc.left += nImageWidth_fcb + 6;
			
			if (m_bDrawUsingFont && pDesc->m_nCharSet != SYMBOL_CHARSET)
			{
				LOGFONT lf;
				globalData.fontRegular.GetLogFont (&lf);

				lstrcpy (lf.lfFaceName, pDesc->m_strName);
				
				if (pDesc->m_nCharSet != DEFAULT_CHARSET)
				{
					lf.lfCharSet = pDesc->m_nCharSet;
				}

				if (lf.lfHeight < 0)
				{
					lf.lfHeight -= 4;
				}
				else
				{
					lf.lfHeight += 4;
				}

				fontSelected.CreateFontIndirect (&lf);
				pOldFont = pDC->SelectObject (&fontSelected);
			}
		}

		CString strText;
		GetLBText (id, strText);

		pDC->DrawText (strText, rc, DT_SINGLELINE | DT_VCENTER);

		if (pOldFont != NULL)
		{
			pDC->SelectObject (pOldFont);
		}
	}

	pDC->RestoreDC (nIndexDC);
}
//****
void CBCGPFontComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	ASSERT(lpMIS->CtlType == ODT_COMBOBOX);

	CRect rc;
	GetWindowRect (&rc);
	lpMIS->itemWidth = rc.Width();

	int nFontHeight = max (globalData.GetTextHeight (), CBCGPToolbarFontCombo::m_nFontHeight);
	lpMIS->itemHeight = max (nImageHeight_fcb, nFontHeight);
}
//****
void CBCGPFontComboBox::PreSubclassWindow() 
{
	CBCGPComboBox::PreSubclassWindow();

	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState ();
	if (pThreadState->m_pWndInit == NULL)
	{
		Init ();
	}
}
//****
int CBCGPFontComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	Init ();
	return 0;
}
//****
void CBCGPFontComboBox::Init ()
{
	CWnd* pWndParent = GetParent ();
	ASSERT_VALID (pWndParent);

	m_bToolBarMode = pWndParent->IsKindOf (RUNTIME_CLASS (CBCGPToolBar));
	if (!m_bToolBarMode)
	{
		Setup ();
	}
}
//****
void CBCGPFontComboBox::CleanUp ()
{
	ASSERT_VALID (this);
	ASSERT (::IsWindow (m_hWnd));
	
	if (m_bToolBarMode)
	{
		// Font data will be destroyed by CBCGPToolbarFontCombo object
		return;
	}

	for (int i = 0; i < GetCount (); i++)
	{
		delete (CBCGPFontDesc*) GetItemData (i);
	}

	ResetContent ();
}
//****
BOOL CBCGPFontComboBox::Setup (int nFontType, BYTE nCharSet, BYTE nPitchAndFamily)
{
	ASSERT_VALID (this);
	ASSERT (::IsWindow (m_hWnd));
	
	if (m_bToolBarMode)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	CleanUp ();

	CBCGPToolbarFontCombo combo (0, (UINT)-1, nFontType, nCharSet,
								CBS_DROPDOWN, 0, nPitchAndFamily);

	for (int i = 0; i < combo.GetCount (); i++)
	{
		CString strFont = combo.GetItem (i);

		CBCGPFontDesc* pFontDescrSrc = (CBCGPFontDesc*) combo.GetItemData (i);
		ASSERT_VALID (pFontDescrSrc);

		if (FindStringExact (-1, strFont) <= 0)
		{
			CBCGPFontDesc* pFontDescr = new CBCGPFontDesc (*pFontDescrSrc);
			int iIndex = AddString (strFont);
			SetItemData (iIndex, (DWORD_PTR) pFontDescr);
		}
	}

	return TRUE;
}
//****
void CBCGPFontComboBox::OnDestroy() 
{
	CleanUp ();
	CBCGPComboBox::OnDestroy();
}
//****
BOOL CBCGPFontComboBox::SelectFont (CBCGPFontDesc* pDesc)
{
	ASSERT_VALID (this);
	ASSERT (::IsWindow (m_hWnd));
	ASSERT_VALID (pDesc);

	for (int i = 0; i < GetCount (); i++)
	{
		CBCGPFontDesc* pFontDescr = (CBCGPFontDesc*) GetItemData (i);
		ASSERT_VALID (pFontDescr);

		if (*pDesc == *pFontDescr)
		{
			SetCurSel (i);
			return TRUE;
		}
	}

	return FALSE;
}
//****
BOOL CBCGPFontComboBox::SelectFont (LPCTSTR lpszName, BYTE nCharSet/* = DEFAULT_CHARSET*/)
{
	ASSERT_VALID (this);
	ASSERT (::IsWindow (m_hWnd));
	ASSERT (lpszName != NULL);

	for (int i = 0; i < GetCount (); i++)
	{
		CBCGPFontDesc* pFontDescr = (CBCGPFontDesc*) GetItemData (i);
		ASSERT_VALID (pFontDescr);

		if (pFontDescr->m_strName == lpszName)
		{
			if (nCharSet == DEFAULT_CHARSET ||
				nCharSet == pFontDescr->m_nCharSet)
			{
				SetCurSel (i);
				return TRUE;
			}
		}
	}

	return FALSE;
}
//****
CBCGPFontDesc* CBCGPFontComboBox::GetSelFont () const
{
	ASSERT_VALID (this);
	ASSERT (::IsWindow (m_hWnd));

	int iIndex = GetCurSel ();
	if (iIndex < 0)
	{
		return NULL;
	}

	return (CBCGPFontDesc*) GetItemData (iIndex);
}
