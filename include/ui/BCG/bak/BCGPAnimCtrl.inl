//****
// COPYRIGHT NOTES
// ---------------
// This is a part of BCGControlBar Library Professional Edition
// Copyright (C) 1998-2009 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
 //****

// BCGAnimCtrl.cpp : implementation file
//


#include "BCGPAnimCtrl.h"
#include "Bcgglobals.h"

#ifndef BCGP_EXCLUDE_ANIM_CTRL

#ifndef _BCGSUITE_
#include "BCGPPopupMenu.h"
#endif

static const int iAnimEventId = 1;

IMPLEMENT_DYNAMIC(CBCGPAnimCtrl, CStatic)

/////////////////////////////////////////////////////////////////////////////
// CBCGPAnimCtrl

CBCGPAnimCtrl::CBCGPAnimCtrl()
{
	m_clrBack = (COLORREF)-1;
	m_sizeFrame = CSize (0, 0);
	m_iCurrFrame = 0;
	m_iFrameCount = -1;
	m_pImagesAnim = NULL;
	m_bIsRunning = FALSE;
	m_uiFrameRate = 500;
}

CBCGPAnimCtrl::~CBCGPAnimCtrl()
{
}


BEGIN_MESSAGE_MAP(CBCGPAnimCtrl, CStatic)
	//{{AFX_MSG_MAP(CBCGPAnimCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPAnimCtrl message handlers

BOOL CBCGPAnimCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
//****
void CBCGPAnimCtrl::OnTimer(UINT_PTR nIDEvent) 
{
	if (nIDEvent == iAnimEventId)
	{
		if (++ m_iCurrFrame == m_iFrameCount)
		{
			m_iCurrFrame = 0;
		}

		Invalidate (FALSE);
		UpdateWindow();

		CWnd* pMenu = CBCGPPopupMenu::GetActiveMenu();

		if (pMenu != NULL && CWnd::FromHandlePermanent (pMenu->GetSafeHwnd()) != NULL)
		{
			CRect rectScreen;
			GetWindowRect (&rectScreen);

			CBCGPPopupMenu::UpdateAllShadows (rectScreen);
		}
	}

	CStatic::OnTimer(nIDEvent);
}
//****
void CBCGPAnimCtrl::OnPaint(CDC* pDC) 
{
	CPaintDC dc(this); // device context for painting
	CBCGPMemDC memDC (dc, this);
	CDC* pDC = &memDC.GetDC();

	CRect rect;
	GetClientRect (rect);

	if (m_clrBack != (COLORREF)-1)
	{
		CBrush brBack (m_clrBack);
		pDC->FillRect (rect, &brBack);
	}
	else
	{
		globalData.DrawParentBackground(c, pDC);
	}

	if (m_pImagesAnim != NULL &&
		m_pImagesAnim->GetSafeHandle() != NULL)
	{
		m_pImagesAnim->Draw (pDC, m_iCurrFrame, 
			CPoint ((rect.Width() - m_sizeFrame.w) / 2,
					(rect.Height() - m_sizeFrame.h) / 2),
			ILD_NORMAL);
	}
}
//****
BOOL CBCGPAnimCtrl::SetBitmap (UINT uiBmpId, int nFrameWidth,
							  COLORREF clrTransparent,
							  BOOL bSizeToContent)
{
	ASSERT_VALID (this);

	if (nFrameWidth <= 0)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	CBitmap bmp;
	if (!bmp.LoadBitmap (uiBmpId))
	{
		TRACE(_T ("Can't load bitmap: %x\n"), uiBmpId);
		return FALSE;
	}

	if (m_imagesAnim.IsValid())
	{
		m_imagesAnim.DeleteImageList();
	}

	BITMAP bmpObj;
	bmp.GetBitmap (&bmpObj);

	UINT nFlags = (clrTransparent == (COLORREF) -1) ? 0 : ILC_MASK;

	switch (bmpObj.bmBitsPixel)
	{
	case 4:
	default:
		nFlags |= ILC_COLOR4;
		break;

	case 8:
		nFlags |= ILC_COLOR8;
		break;

	case 16:
		nFlags |= ILC_COLOR16;
		break;

	case 24:
		nFlags |= ILC_COLOR24;
		break;

	case 32:
		nFlags |= ILC_COLOR32;
		break;
	}

	m_imagesAnim.Create (nFrameWidth, bmpObj.bmHeight, nFlags, 0, 0);
	m_imagesAnim.Add (&bmp, clrTransparent);

	return SetBitmap (&m_imagesAnim, bSizeToContent);
}
//****
BOOL CBCGPAnimCtrl::SetBitmap (CImageList* pImagesAnim, BOOL bSizeToContent)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pImagesAnim);

	if (m_imagesAnim.IsValid() &&
		m_imagesAnim.GetSafeHandle() != pImagesAnim->GetSafeHandle())
	{
		::DeleteObject (m_imagesAnim.Detach());
	}

	if (m_bIsRunning)
	{
		KillTimer (iAnimEventId);
		m_bIsRunning = FALSE;
	}

	m_pImagesAnim = pImagesAnim;

	IMAGEINFO imageInfo;
	pImagesAnim->GetImageInfo (0, &imageInfo);

	CRect rectImage = imageInfo.rcImage;

	m_sizeFrame.w = rectImage.Width();
	m_sizeFrame.h = rectImage.Height();

	m_iFrameCount = pImagesAnim->GetImageCount();

	if (bSizeToContent)
	{
		SetWindowPos (NULL, -1, -1, m_sizeFrame.w, m_sizeFrame.h,
			SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}

	Invalidate();
	UpdateWindow();

	return TRUE;
}
//****
BOOL CBCGPAnimCtrl::Play (UINT uiFrameRate)
{
	ASSERT_VALID (this);

	if (uiFrameRate != 0)
	{
		m_uiFrameRate = uiFrameRate;
	}

	if (m_pImagesAnim == NULL ||
		m_pImagesAnim->GetSafeHandle() == NULL ||
		m_bIsRunning)
	{
		return FALSE;
	}

	SetTimer (iAnimEventId, m_uiFrameRate, NULL);
	m_bIsRunning = TRUE;

	return TRUE;
}
//****
BOOL CBCGPAnimCtrl::Stop()
{
	if (m_pImagesAnim == NULL ||
		m_pImagesAnim->GetSafeHandle() == NULL ||
		!m_bIsRunning)
	{
		return FALSE;
	}

	KillTimer (iAnimEventId);
	m_iCurrFrame = 0;
	m_bIsRunning = FALSE;

	Invalidate();
	UpdateWindow();

	return TRUE;
}
//****
void CBCGPAnimCtrl::SetFrameRate (UINT uiFrameRate)
{
	ASSERT_VALID (this);

	if (uiFrameRate == 0)
	{
		ASSERT (FALSE);
		return;
	}

	if (m_uiFrameRate != uiFrameRate)
	{
		m_uiFrameRate = uiFrameRate;

		if (m_bIsRunning)
		{
			KillTimer (iAnimEventId);
			SetTimer (iAnimEventId, uiFrameRate, NULL);
		}
	}
}

#endif	// BCGP_EXCLUDE_ANIM_CTRL
