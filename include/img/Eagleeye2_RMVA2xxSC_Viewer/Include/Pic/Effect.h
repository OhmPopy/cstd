//////////////////////////////////////////////////////////////////
//																//
//		��; : 32 λ DIB ��Ч����								//
//		���� : [Foolish] / 2001-4-14							//
//		���� : 2002-11-6										//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_DIBEFFECT_H__
#define		__FOO_DIBEFFECT_H__
#include "Pic.h"
#include <list>
#pragma once
using namespace std ;

//====================================================================
//	DIBЧ��
//====================================================================
/************************************************************************/
/*	1 . ���к�������ر�ָ����ֻ�� 32 λɫ��Ч, ������ConvertTo32Bit.	*/
/*	2 . ����rect, �Һ��±߽���ͼ��ѧ��һ��Ϊ������ (��������)			*/
/*		��ѡȫͼΪ: (0, 0, 800, 600) ������ (0, 0, 799, 599)			*/
/*	3 . ��һ����ͼ��ǰҪ����->ClearAllList () ;						*/
/*	4 . ���ע�� : (֧�� AUTO-Undo/Redo)								*/
/*		��m_UndoFlag == trueʱ, �����Զ����浱ǰDIB					*/
/************************************************************************/
class FCDibEffect : public FCDib
{
public :
	FCDibEffect (bool bUndo = false) ;
	FCDibEffect (FCDibEffect & dib) ;
	virtual ~FCDibEffect () ;
	void	operator= (const FCDibEffect & pic) ;

public :
	// Region selection support
	enum SelectType {SEL_CREATE, SEL_ADD, SEL_SUB} ;
	void	SetRectRgn (int nLeft, int nTop, int nRight, int nBottom, SelectType nType = FCDibEffect::SEL_CREATE) ;
	void	SetRoundRectRgn (int nLeft, int nTop, int nRight, int nBottom, int nWidthEllipse, int nHeightEllipse, SelectType nType = FCDibEffect::SEL_CREATE) ;
	void	SetEllipticRgn (int nLeft, int nTop, int nRight, int nBottom, SelectType nType = FCDibEffect::SEL_CREATE) ;
	void	SetPolygonRgn (const POINT * ppt, int cNumPoint, int nFillMode = ALTERNATE, SelectType nType = FCDibEffect::SEL_CREATE) ;
//	void	MagicStickRgn (int x, int y, RGBQUAD rgb, SelectType nType = FCDibEffect::SEL_CREATE) ; // ħ����ѡȡ
	void	OffsetRgn (int xOffset, int yOffset) ;
	void	ClearSelection () ; // ClearSelection����ѡȫͼ
	bool	HasSelection () ;
	bool	IsInSelection (int x, int y) ;
	void	GetBoundBox (RECT * rect) ;
	HRGN	GetSelectHRgn () ;
	void	FillRgn (HBRUSH hBrush) ; // ���
	void	FrameRgn (HBRUSH hBrush) ; // ����
	void	MoveRgn (int xOffset, int yOffset, bool bHold = false) ;
	void	CopySelectionDib (FCDib * RgnDib) ;
	BOOL	MakeThumbnail (FCDib * RgnDib, int nThumbW, int nThumbH) ; // ָ��RgnDib�Ĵ�С

	// Multi level Undo/Redo support
	void	Undo () ;	// ����һ������
	void	Redo () ;	// ����һ������
	void	AddToUndoList () ; // ��ǰѡ������
	void	AddImageToUndoList () ;
	void	AddToUndoList (const RECT & rect) ; // ָ������
	bool	IsUndoEnable () ;
	bool	IsRedoEnable () ;
	void	SetUndoEnable (bool bEnable = false) ; // Undo/Redo flag
	BOOL	GetUndoFlag () ;
//	void	SetUndoSize(int nUndoSize);
//	int		GetUndoSize() const { return m_nUndoSize; };
	void	ClearRedoList () ;
	void	ClearUndoList () ;
	void	ClearAllList () ; // ���Undo/Redo����

	//	ƽ�Ʊ任, �ñ߽�ɫ���հ����� (AUTO-Undo/Redo)
	void	operator << (int iWidth) ;
	void	operator >> (int iWidth) ;
	void	ShiftUp (int iHeight) ;
	void	ShiftDown (int iHeight) ;
	void	ColShiftUp (int iCol, int iHeight) ;	// ������
	void	ColShiftDown (int iCol, int iHeight) ;	// ������

	// ��λ (֧�� Undo/Redo����֧������)
		// ��/���߿� (�ú�ɫ���߿�)
	void	AddFrame (int iLeft, int iTop, int iRight, int iBottom) ;
	void	EraseFrame (int iLeft, int iTop, int iRight, int iBottom) ;
	void	DuplicateFrame (int iLeft, int iTop, int iRight, int iBottom) ;
		// ˳ʱ����ת 90'
	void	Rotate90 () ;
		// ˳ʱ����ת 270'
	void	Rotate270 () ;
		// ˳ʱ����ת�����Ƕ� iAngle��[1..89]
	void	Rotate (int iAngle) ;
		// iType == TRUE Ϊ��߳ɱ�������
	void	Stretch (DWORD dwWidth, DWORD dwHeight, int iType = TRUE) ;

	//	��Ч���� (֧�� Undo/Redo)
		// ���·�ת
	void	Flip () ;
		// ���Ҿ���
	void	Mirror () ;
		//	����RGBֵ
	void	AdjustRGB (int iValueR, int iValueG, int iValueB) ;
		//	����	bFlag == TRUE, ��iValueΪ������
		//			bFlag == FALSE, ��iValueΪ���ٷֱ�, ��150 (means 150%)
	void	AdjustBrightness (int iValue, BOOL bFlag = TRUE) ;
	void	AdjustBrightnessPixel (int x, int y, int iValue, BOOL bFlag = TRUE) ;
		//	�Աȶ� (iPercentΪ���ٷֱȵ�����)
	void	AdjustContrast (int iPercent) ;
	void	AdjustContrastPixel (int x, int y, int iPercent) ;
		//	ɫ��
	void	AdjustHue (int iPercent) ;
		//	���Ͷ�
	void	AdjustSaturation (int iPercent) ;
		//	ɫ�� && ���Ͷ�
	void	AdjustHS (int iPercentHue, int iPercentSat) ;
		//	��ɫ��ת
	void	Negate () ;
		//	����Ч��
	void	Emboss (int nStep = 2, int nDirection = DIRECT_RIGHT) ;
		//  �绯Ч��
		//	nStep Ϊ�ƶ����� : [1..x]
		//	nDirection Ϊ�绯����
	void	Windy (int nStep, int nDirection = DIRECT_RIGHT, bool bSmooth = true) ;
	void	WindySoftLine (int nLine, int nStep, int nDirection) ;
		//	ģ��Ч��, ʹ��ƽ���㷨
		//	iBlockLenΪС����߳�, ��[2..32]
	void	SmoothAverage (int iBlockLen) ;
	void	SmoothAverageNoEdge (int iBlockLen) ;
		//	��Ч��
	void	Sharp (int nStep = 1) ;
		//	������Ч��
		//	iBlockLenΪС����߳� ( >=2 )
	void	Mosaic (int iBlockLen) ;
		//	����Ч�� (�ɽ�)
		//	iBlockLenΪС����߳�, ��[2..100]
	void	Glass (int iBlockLen) ;
		//	��Ч��
	void	Sphere () ;
		//	����Ч��
	void	Column () ;
		//	׶(��)����nStep : [1..100]
	void	ConeSunken (int nStep = 50) ;
		//	��״Ч�� (iSwing : ���[0..100]		iFrequency : Ƶ��)
	void	Stripe (int iSwing, int iFrequency) ;
		//	�������, iElement�Ӿ������Ͻǿ�ʼ, iLengthΪ������
	void	ConvoluteDIB (int * iElement, int iLength, int iDivisor) ;
	void	ConvoluteDIBNoEdge (int * iElement, int iLength, int iDivisor) ;
		//	���ھ������Ч�㷨
	void	Algorithm (int iAlgorithm) ; // iAlgorithm��StdDefine.h

public :
	void	__CopyCol (FCDib & dibDest, int iColD, const FCDib & dibSrc, int iCol) ; // ��copy
	void	__BltBlock (const FCDib & Dib, int x, int y) ; // Cover block on current DIB, only the region specified by m_hSelRgn
	void	__SetPixelData (BYTE * pDest, int x, BYTE * pSrc) ;
protected :
	// Undo/Redo used
	enum UNDOTYPE {UNDO_COVER, UNDO_REPLACE} ;
	typedef struct tagUNDO_NODE
	{
		UNDOTYPE	UndoMode ;
		FCDib		* pPrior ;
		POINT		point ;
	} UNDO_NODE, * PUNDO_NODE ;
	void	__AddToUndoList (const RECT & rect, bool bClearRedo) ;
	void	__AddImageToUndoList (bool bClearRedo) ;
	void	__AddToUndoList (const UNDO_NODE & UndoNode) ;
	void	__AddToRedoList (const RECT & rect) ;
	void	__AddImageToRedoList () ;
	void	__AddToRedoList (const UNDO_NODE & UndoNode) ;
	void	__BoundSelectedRgn () ;
private :
	void	__ConeSunken (int nStep, UINT32 * nPercent) ;
	void	__Stripe (int iSwing, int iFrequency, UINT32 * nPercent) ;
	void	__Flip (UINT32 * nPercent) ;
	void	__Mirror (UINT32 * nPercent) ;
	void	__Negate (UINT32 * nPercent) ;
	void	__AdjustRGB (int iValueR, int iValueG, int iValueB, UINT32 * nPercent) ;
	void	__AdjustBrightness (int iValue, BOOL bFlag, UINT32 * nPercent) ;
	void	__AdjustContrast (int iPercent, UINT32 * nPercent) ;
	void	__AdjustHS (int iPercentHue, int iPercentSat, UINT32 * nPercent) ;
	void	__Emboss (int nStep, int nDirection, UINT32 * nPercent) ;
	void	__Windy (int nStep, int nDirection, bool bSmooth, UINT32 * nPercent) ;
	void	__Mosaic (int iBlockLen, UINT32 * nPercent) ;
	void	__Sharp (int nStep, UINT32 * nPercent) ;
	void	__SmoothAverageNoEdge (int iBlockLen, UINT32 * nPercent) ;
protected :
	BOOL				m_UndoFlag ;	// �Ƿ�֧��Undo/Redo
	RECT				m_SelRect ;		// �������Χ����
	HRGN				m_hSelRgn ;		// ѡ������
	list<UNDO_NODE *>	m_UndoList, m_RedoList ; // Undo/Redo����
} ;

//===================================================================
//	Implement
//===================================================================
inline void  FCDibEffect::SetUndoEnable (bool bEnable) {
	m_UndoFlag = bEnable ;
}
inline bool  FCDibEffect::IsRedoEnable () {
	return !m_RedoList.empty () ;
}
inline bool  FCDibEffect::IsUndoEnable () {
	return !m_UndoList.empty () ;
}
inline BOOL  FCDibEffect::GetUndoFlag () {
	return m_UndoFlag ;
}
inline void  FCDibEffect::AdjustBrightnessPixel (int x, int y, int iPercent, BOOL bFlag) {
	RGBQUAD		* pPixel = (RGBQUAD *) this->GetBits (x, y) ;
	if (!bFlag)
		iPercent = 128 * iPercent / 100 ; // ת����x/128
	pPixel->rgbBlue = max (min (bFlag ? (pPixel->rgbBlue + iPercent)
									  : (pPixel->rgbBlue * iPercent / 128), 255), 0) ;
	pPixel->rgbGreen = max (min (bFlag ? (pPixel->rgbGreen + iPercent)
									   : (pPixel->rgbGreen * iPercent / 128), 255), 0) ;
	pPixel->rgbRed = max (min (bFlag ? (pPixel->rgbRed + iPercent)
									 : (pPixel->rgbRed * iPercent / 128), 255), 0) ;
}
inline void  FCDibEffect::AdjustContrastPixel (int x, int y, int iPercent) {
	RGBQUAD		* pPixel = (RGBQUAD *)this->GetBits (x, y) ;
	iPercent = 128 * iPercent / 100 ; // ת����x/128
	pPixel->rgbBlue = max (min (128 + (pPixel->rgbBlue - 128) * iPercent / 128, 255), 0) ;
	pPixel->rgbGreen = max (min (128 + (pPixel->rgbGreen - 128) * iPercent / 128, 255), 0) ;
	pPixel->rgbRed   = max (min (128 + (pPixel->rgbRed - 128) * iPercent / 128, 255), 0) ;
}
inline void  FCDibEffect::__CopyCol (FCDib & dibDt, int iColD, const FCDib & dibSr, int iCol) {
	BYTE		* pDest = dibDt.GetBits (iColD, 0),
				* pSrc = dibSr.GetBits (iCol, 0) ;
	int			nSpan = dibDt.ColorBits() / 8 ; // ÿ�����ֽ���2, 3, 4
	DWORD		dwOldPitch = dibSr.GetPitch (),
				dwPitch = dibDt.GetPitch () ;
	for (int y=0 ; y < dibDt.Height() ; y++, pDest -= dwPitch, pSrc -= dwOldPitch)
		CopyMemory (pDest, pSrc, nSpan) ;
}
__forceinline void  FCDibEffect::__SetPixelData (BYTE * pDest, int x, BYTE * pSrc) {
	switch (this->ColorBits())
	{
		case 8  :	*(pDest + x) = *pSrc ; break ;
		case 16 :	CopyMemory ((WORD *)pDest + x, pSrc, 2) ; break ;
		case 24 :	CopyMemory ((RGBTRIPLE *)pDest + x, pSrc, 3) ; break ;
		case 32 :	CopyMemory ((DWORD *)pDest + x, pSrc, 4) ; break ;
	}
}
inline void  FCDibEffect::ColShiftUp (int iCol, int iHeight) {
	if ((this->ColorBits() >= 8) && (iCol > 0) && (iCol < this->Width()) && (iHeight > 0) && (iHeight < this->Height()))
	{
		BYTE		* pSrc = this->GetBits (iCol, iHeight),
					* pDest = this->GetBits (iCol, 0) ;
		int			iMax = Height() - iHeight,
					nSpan = this->ColorBits() / 8 ;
		DWORD		dwPitch = this->GetPitch () ;
		for (int i=0 ; i < iMax ; i++, pSrc -= dwPitch, pDest -= dwPitch)
			CopyMemory (pDest, pSrc, nSpan) ;
		for (int m=0 ; m < iHeight ; m++, pDest -= dwPitch)
			FillMemory (pDest, nSpan, 0xFF) ; // ����ɫ
	}
}
inline void  FCDibEffect::ColShiftDown (int iCol, int iHeight) {
	if ((this->ColorBits() >= 8) && (iCol > 0) && (iCol < this->Width()) && (iHeight > 0) && (iHeight < this->Height()))
	{
		BYTE		* pSrc = this->GetBits (iCol, Height() - 1 - iHeight),
					* pDest = this->GetBits (iCol, Height() - 1) ;
		int			iMax = Height() - iHeight,
					nSpan = this->ColorBits() / 8 ;
		DWORD		dwPitch = this->GetPitch () ;
		for (int i=0 ; i < iMax ; i++, pSrc += dwPitch, pDest += dwPitch)
			CopyMemory (pDest, pSrc, nSpan) ;
		for (int m=0 ; m < iHeight ; m++, pDest += dwPitch)
			FillMemory (pDest, nSpan, 0xFF) ; // ����ɫ
	}
}
inline void  FCDibEffect::__BoundSelectedRgn () {
	HRGN	hBound = ::CreateRectRgn (0, 0, Width(), Height()) ;
	::CombineRgn (m_hSelRgn, m_hSelRgn, hBound, RGN_AND) ;
	::DeleteObject (hBound) ;
}
inline void  FCDibEffect::GetBoundBox (RECT * rect) {
	if (rect != NULL)
		if (this->HasSelection())
			::CopyRect (rect, &m_SelRect) ;
		else
			::SetRect (rect, 0, 0, this->Width(), this->Height()) ;
}
inline bool  FCDibEffect::HasSelection () {
	return ((m_hSelRgn != NULL) && !::IsRectEmpty (&m_SelRect)) ;
}
inline bool  FCDibEffect::IsInSelection (int x, int y) {
	return (::PtInRegion (m_hSelRgn, x, y) != 0) ;
}
inline void  FCDibEffect::ClearSelection () {
	if (HasSelection ())
		::DeleteObject (m_hSelRgn) ;
	m_hSelRgn = NULL ;
	::SetRectEmpty (&m_SelRect) ;
}
inline void  FCDibEffect::ConeSunken (int nStep) {
	this->__ConeSunken (nStep, &m_FinishedPer) ;
}
inline void  FCDibEffect::CopySelectionDib (FCDib * RgnDib) {
	if (!this->HasSelection ())
		* RgnDib = * (FCDib *) this ;
	else
		FCDibEffect::MakeThumbnail (RgnDib, m_SelRect.right-m_SelRect.left, m_SelRect.bottom-m_SelRect.top) ;
}
inline HRGN  FCDibEffect::GetSelectHRgn () {
	return m_hSelRgn ;
}
inline void  FCDibEffect::OffsetRgn (int xOffset, int yOffset) {
	if (this->HasSelection ()) {
		::OffsetRgn (m_hSelRgn, xOffset, yOffset) ;
		this->__BoundSelectedRgn () ;
		::GetRgnBox (m_hSelRgn, &m_SelRect) ;
	}
}
inline void  FCDibEffect::AddToUndoList () {
	if (this->HasSelection ()) {
		RECT	rc ;
		this->GetBoundBox (&rc) ;
		this->AddToUndoList (rc) ;
	}
	else
		this->AddImageToUndoList () ;
}
inline void  FCDibEffect::AddImageToUndoList () {
	this->__AddImageToUndoList (true) ;
}
inline void  FCDibEffect::AddToUndoList (const RECT & rect) {
	this->__AddToUndoList (rect, true) ;
}
inline void  FCDibEffect::ClearAllList () {
	this->ClearRedoList () ;
	this->ClearUndoList () ;
}
inline void  FCDibEffect::Stripe (int iSwing, int iFrequency) {
	this->__Stripe (iSwing, iFrequency, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::Flip () {
	this->__Flip (&this->m_FinishedPer) ;
}
inline void  FCDibEffect::Mirror () {
	this->__Mirror (&this->m_FinishedPer) ;
}
inline void  FCDibEffect::AdjustRGB (int iValueR, int iValueG, int iValueB) {
	this->__AdjustRGB (iValueR, iValueG, iValueB, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::AdjustBrightness (int iPercent, BOOL bFlag) {
	this->__AdjustBrightness (iPercent, bFlag, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::AdjustContrast (int iPercent) {
	this->__AdjustContrast (iPercent, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::AdjustHue (int iPercentHue) {
	this->__AdjustHS (iPercentHue, 100, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::AdjustSaturation (int iPercentSat) {
	this->__AdjustHS (100, iPercentSat, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::AdjustHS (int iPercentHue, int iPercentSat) {
	this->__AdjustHS (iPercentHue, iPercentSat, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::Negate () {
	this->__Negate (&this->m_FinishedPer) ;
}
inline void  FCDibEffect::Emboss (int nStep, int nDirection) {
	this->__Emboss (nStep, nDirection, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::Windy (int nStep, int nDirection, bool bSmooth) {
	this->__Windy (nStep, nDirection, bSmooth, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::Sharp (int nStep) {
	this->__Sharp (nStep, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::Mosaic (int iBlockLen) {
	this->__Mosaic (iBlockLen, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::SmoothAverageNoEdge (int iBlockLen) {
	this->__SmoothAverageNoEdge (iBlockLen, &this->m_FinishedPer) ;
}

#endif