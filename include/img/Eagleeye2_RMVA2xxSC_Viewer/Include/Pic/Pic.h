//////////////////////////////////////////////////////////////////
//																//
//		��; : DIB ������										//
//		���� : [Foolish] / 2001-2-13							//
//		���� : 2002-11-27										//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_32DIB_H__
#define		__FOO_32DIB_H__
#include <windows.h>
#include "..\StdDefine.h"
#include "ImageFormat.h"
#pragma once

//===================================================================
//	DIB����
//===================================================================
/********************************************************************/
/*	1 . DIB���ش����½Ǵ洢, ���ڴ��д洢��ʽΪ B-G-R-a				*/
/*	2 . �����е�����ԭ��(0,0)ȡ���Ͻ�								*/
/********************************************************************/
class FCDib
{
public :
	FCDib () ;
	FCDib (const FCDib & dib) ;
	virtual		~FCDib () ;
	void  Unload () ; // �����ǰλͼ
	FCDib & operator= (const FCDib & dib) ;
	void	RegionFill (RGBQUAD rgb, RECT * rect = NULL) ;
	bool	BoundRect (RECT & rect) ;
	BOOL	CaptureScreen (LPRECT lpRect = NULL) ;	// NULL Ϊ��׽ȫ��

	/****************************************************************/
	/*	��  �� �����ڴ��д���һ���յ�DIB							*/
	/*	��  �� ��0Ϊ��ǰ���������									*/
	/*	˵  �� ���ڲ�����->Unload (), �����ǰ��DIB					*/
	/*			 8Bit����λͼ�����յ�ɫ��							*/
	/*			 ��֧�����϶��´洢��DIB (��iHeight < 0)			*/
	/****************************************************************/
	BOOL	Create (int iWidth, int iHeight, WORD wColorBit = 0) ;
	BOOL	Create (BITMAPINFO * bmif, BYTE * pBits) ;
	BOOL	MakeThumbnail (FCDib * dib, int nThumbW, int nThumbH, RECT * rect = NULL) const ;

	/****************************************************************/
	/*	��  �� ��DDB ==> DIB										*/
	/*	˵  �� ��hBitmapΪDDB Handle�����ú����ɾ��hBitmap			*/
	/****************************************************************/
	BOOL	AttachDDB (HBITMAP hBitmap, int iColor = 32) ;

	/****************************************************************/
	/*	��  �� ��DIB ==> DDB										*/
	/*	˵  �� ����������Ļ���ݵ�HBITMAP, �����ֶ�ɾ�����صĶ���	*/
	/*			 95/98/Me : ���ܴ�������16M��DDB					*/
	/****************************************************************/
	HBITMAP	GetDDB_Handle (HDC hdc = NULL) ;

public :
	//	��ȡͼ����Ϣ (Attributes)
	BYTE  * GetBits (int iLine = 0) const ;	// ȡ�õ� iLine ��ָ��, ���Ͻ�Ϊ(0,0), ���϶���
	BYTE  * GetBits (int x, int y) const ;	// ȡ�� (x,y) ���ָ��, ���Ͻ�Ϊ(0,0), ���϶��£��������
	BYTE  * GetMemStart () const ;			// ���DIB���½�����ָ��(Ҳ����ʼ��ַ)
	DWORD	Width () const ;				// ��
	DWORD	Height () const ;				// ��
	WORD	ColorBits () const ;			// ��ɫλ��
	DWORD	GetPitch () const ;				// �п��ֽ��� (32λ����)
	HBITMAP	GetHandle () const ;			// Get DIBSECTION handle (DIB-Handle)
	int		ColorsUsed () const ;			// ͳ��ͼ��ʹ�õ���ɫ��
	BOOL	TakeKeyColor (RGBQUAD * rgb) ;	// �ҳ�һ�ֲ���ͼ���г��ֵ���ɫ (>=16Bit)
	void	GetDibInfo (BITMAPV4HEADER * bmif) const ;

	// progress control (uses in multiple threads)
	void	ResetFinishedPercent () const ;
	int		GetFinishedPercent () const ;
	void	SetFinishedPercent (int nPercent) ;

	//	��ɫת��
	void	ConvertToGray () ; // ת���ɻҶ�ͼ
	void	ConvertTo16Bit () ;	// 1, 4, 8, 24, 32 ==> 16
	void	ConvertTo24Bit () ;	// 1, 4, 8, 16, 32 ==> 24
	void	ConvertTo32Bit () ;	// 1, 4, 8, 16, 24 ==> 32
	void	ConvertColorTo (int iColor) ; // ==> iColor (��ɫʱЧ������)

	//	ֱ�����ش�ȡ (pixel operations)
	bool	IsInside (int x, int y) ;
	RGBQUAD	ParsePixelData (DWORD dwData) const ;
	DWORD	GetPixelData (int x, int y) const ;
	void	GetPixelColor (int x, int y, RGBQUAD * prgb) const ;
	RGBQUAD	GetPixelColor (int x, int y) const ;
	void	SetPixelData (int x, int y, DWORD dwPixel) ;
	void	SetPixelColor (int x, int y, RGBQUAD rgb) ;	// �� 16, 24, 32 λɫ��Ч
	void	NegatePixel (int x, int y) ;
	void	AlphaBlendPixel (int x, int y, RGBQUAD rgb) ; // ����rgb�е�alphaͨ������

	//	ͨ������ (24,32Bit��Ч)(��ͨ��λͼ��Ϊ8Bit)
	void	GetRGBChannel (FCDib * red, FCDib * green, FCDib * blue, FCDib * alpha = NULL) ;
	void	GetBlueChannel (FCDib * blue) ;
	void	GetGreenChannel (FCDib * green) ;
	void	GetRedChannel (FCDib * red) ;
	void	GetAlphaChannel (FCDib * alpha) ;
	void	CombineChannel (FCDib * red, FCDib * green, FCDib * blue, FCDib * alpha = NULL) ;
	void	AppendAlphaChannel (const FCDib & alpha) ;
	void	InvertAlphaChannel () ;

	//	��ɫ����� (palette operations) ( 8 λɫ������Ч ), index��0��ʼ
	bool	IsGrayPalette () const ;
	void	SetGrayPalette () ;
	BOOL	GetColorTable (int index, RGBQUAD * prgb) const ;
	BOOL	SetColorTable (int index, RGBQUAD * prgb) ;
	BOOL	GetColorTable (int iFirstIndex, int iNumber, RGBQUAD * pColors) const ;
	BOOL	SetColorTable (int iFirstIndex, int iNumber, RGBQUAD * pColors) ;

	//	����DIB���д���
	BOOL	GetSubBlock (FCDib * SubDib, const RECT & rcBlock) const ; // SubDib����Ϊthis
	BOOL	CoverBlock (const FCDib & Dib, int x, int y) ;
	void	AlphaBlend (const FCDib & MaskDib, int alpha, int x, int y) ; // alphaΪMaskDibλͼ��Ȩֵ [0..0xFF]
	void	AlphaBlend (const FCDib & MaskDib32, int x=0, int y=0) ; // ����MaskDib32�е�alphaͨ������
	void	LogicalOperate (const FCDib & Dib, DWORD dwRop, int x=0, int y=0) ; // dwRop��StdDefine.h
	void	AND (const FCDib & Dib, int x=0, int y=0) ;
	void	 OR (const FCDib & Dib, int x=0, int y=0) ;
	void	XOR (const FCDib & Dib, int x=0, int y=0) ;

	//	������
	BOOL	CopyToClipboard (const RECT * rect = NULL) ;
	BOOL	PasteFromClipboard (const RECT * rect = NULL) ;
	BOOL	IsPasteAvailable () ;

	//	����� (��̬)
	static RGBQUAD	fooSplit16Bit_565 (WORD wPixel) ;
	static RGBQUAD	fooSplit16Bit_555 (WORD wPixel) ;
	static WORD		fooCombine16Bit_565 (const RGBQUAD & rgb) ;
	static WORD		fooCombine16Bit_555 (const RGBQUAD & rgb) ;
	static DWORD	fooDibRowBytes (int iWidth, int iColorBit) ;
	static BYTE		fooGetGrayscale (const RGBQUAD & rgb) ;
	static void		fooSwapRGB (BYTE * prgb) ;
	static void		fooRGBtoHLS (RGBTRIPLE rgb, double * H, double * L, double * S) ;
	static RGBTRIPLE fooHLStoRGB (const double & H, const double & L, const double & S) ;
	static RGBQUAD	fooRGBtoRGBQUAD (COLORREF color) ;
	static COLORREF fooRGBQUADtoRGB (RGBQUAD rgb) ;

protected :
	BITMAPV4HEADER		m_DibInfo ; // DIB Info
	HBITMAP				m_hBitmap ; // Handle from CreateDIBSection (DIB-Handle)
	BYTE			  * m_pByte ;   // Bitmap bits, left-bottom point
	BYTE			 ** m_ppLine ;  // Line-pointer , ppLine[] ; ���϶���
	UINT32				m_FinishedPer ; // ��������ݵİٷֱ�[0..100]
protected :
	DWORD	__GetPixelData (BYTE * pBits) const ;
	BOOL	__Create (int iWidth, int iHeight, WORD wColorBit, DWORD dwType, const DWORD pField[3], bool bInitPal = true) ;
	void	__CopyPalette (const FCDib & DibSrc) ;
	static	double  __fooHuetoRGB (double m1, double m2, double h) ;
private :
	void	__ConvertToTrueColor (int iColor) ; // ת��Ϊ���ɫ��iColorΪ24 or 32
} ;

//===================================================================
//	Implement
//===================================================================
inline FCDib::FCDib () {
	m_hBitmap	= NULL ;
	m_pByte		= NULL ;
	m_ppLine	= NULL ;
	::ZeroMemory (&m_DibInfo, sizeof(m_DibInfo)) ;
}
inline FCDib::FCDib (const FCDib & dib) {
	m_hBitmap	= NULL ;
	m_pByte		= NULL ;
	m_ppLine	= NULL ;
	::ZeroMemory (&m_DibInfo, sizeof(m_DibInfo)) ;
	if (dib.GetHandle () != NULL)
		FCDib::operator = (dib) ;
}
inline FCDib::~FCDib () {
	this->Unload () ;
}
inline void  FCDib::Unload () {
	if (m_ppLine != NULL)
		delete[] m_ppLine ;
	if (m_hBitmap != NULL)
		::DeleteObject (m_hBitmap) ;
	m_hBitmap	= NULL ;
	m_pByte		= NULL ;
	m_ppLine	= NULL ;
	::ZeroMemory (&m_DibInfo, sizeof(m_DibInfo)) ;
}
inline BOOL  FCDib::Create (int iWidth, int iHeight, WORD wColorBit) {
	return this->__Create (iWidth, iHeight, wColorBit, BI_RGB, NULL) ;
}
inline void FCDib::GetDibInfo (BITMAPV4HEADER * bmif) const {
	if (bmif != NULL)
		::CopyMemory (bmif, &m_DibInfo, sizeof(m_DibInfo)) ;
}
inline BYTE * FCDib::GetBits (int iLine) const {
	return m_ppLine[iLine] ;
}
inline BYTE * FCDib::GetBits (int x, int y) const {
	return (m_ppLine[y] + x * this->ColorBits() / 8) ;
}
inline DWORD FCDib::Width () const {
	return m_DibInfo.bV4Width ;
}
inline DWORD FCDib::Height () const {
	return m_DibInfo.bV4Height ;
}
inline WORD FCDib::ColorBits () const {
	return m_DibInfo.bV4BitCount ;
}
inline HBITMAP	FCDib::GetHandle () const {
	return m_hBitmap ;
}
inline 	DWORD  FCDib::GetPitch ( ) const {
	return ( 4 * ((this->Width () * (DWORD)this->ColorBits () + 31) / 32) ) ;
}
inline BOOL  FCDib::GetColorTable (int index, RGBQUAD * prgb) const {
	return this->GetColorTable (index, 1, prgb) ;
}
inline BOOL  FCDib::SetColorTable (int index, RGBQUAD * prgb) {
	return this->SetColorTable (index, 1, prgb) ;
}
inline bool  FCDib::IsInside (int x, int y) {
	return ((0 <= y) && (y < (int)this->Height()) && (0 <= x) && (x < (int)this->Width())) ;
}
inline RGBQUAD  FCDib::fooSplit16Bit_565 (WORD wPixel) {
	RGBQUAD		rgb ;
	rgb.rgbRed   = (BYTE) ((MASK16_RED_565 & wPixel) >> 8) ;
	rgb.rgbGreen = (BYTE) ((MASK16_GREEN_565 & wPixel) >> 3) ;
	rgb.rgbBlue  = (BYTE) ((MASK16_BLUE_565 & wPixel) << 3) ;
	return rgb ;
}
inline RGBQUAD  FCDib::fooSplit16Bit_555 (WORD wPixel) {
	RGBQUAD		rgb ;
	rgb.rgbRed   = (BYTE) ((MASK16_RED_555 & wPixel) >> 7) ;
	rgb.rgbGreen = (BYTE) ((MASK16_GREEN_555 & wPixel) >> 2) ;
	rgb.rgbBlue  = (BYTE) ((MASK16_BLUE_555 & wPixel) << 3) ;
	return rgb ;
}
inline WORD  FCDib::fooCombine16Bit_565 (const RGBQUAD & rgb) {
	WORD		wPixel ;
	wPixel  = ((rgb.rgbRed >> 3) << 11) ;
	wPixel |= ((rgb.rgbGreen >> 2) << 5) ;
	wPixel |= (rgb.rgbBlue >> 3) ;
	return wPixel ;
}
inline WORD  FCDib::fooCombine16Bit_555 (const RGBQUAD & rgb) {
	WORD		wPixel ;
	wPixel  = ((rgb.rgbRed >> 3) << 10) ;
	wPixel |= ((rgb.rgbGreen >> 3) << 5) ;
	wPixel |= (rgb.rgbBlue >> 3) ;
	return wPixel ;
}
inline DWORD  FCDib::fooDibRowBytes (int iWidth, int iColorBit) {
	return ( 4 * ((iWidth * iColorBit + 31) / 32) ) ;
}
inline BYTE  FCDib::fooGetGrayscale (const RGBQUAD & rgb) {
	register UINT32		tol = 30*rgb.rgbRed + 59*rgb.rgbGreen + 11*rgb.rgbBlue ;
	return (tol / 100) ;
}
inline void  FCDib::fooSwapRGB (BYTE * prgb) {
	BYTE	temp = prgb[0] ;
	prgb[0] = prgb[2] ;
	prgb[2] = temp ;
}
inline BYTE  * FCDib::GetMemStart () const {
	return (this->GetHandle() == NULL) ? NULL : m_pByte ;
}
inline void  FCDib::GetPixelColor (int x, int y, RGBQUAD * prgb) const {
	* prgb = this->ParsePixelData (this->GetPixelData (x, y)) ;
}
inline RGBQUAD  FCDib::GetPixelColor (int x, int y) const {
	return this->ParsePixelData (this->GetPixelData (x, y)) ;
}
inline void  FCDib::ConvertTo24Bit () {
	this->__ConvertToTrueColor (24) ;
}
inline void  FCDib::ConvertTo32Bit () {
	this->__ConvertToTrueColor (32) ;
}
inline void  FCDib::GetRedChannel (FCDib * red) {
	this->GetRGBChannel (red, NULL, NULL, NULL) ;
}
inline void  FCDib::GetGreenChannel (FCDib * green) {
	this->GetRGBChannel (NULL, green, NULL, NULL) ;
}
inline void  FCDib::GetBlueChannel (FCDib * blue) {
	this->GetRGBChannel (NULL, NULL, blue, NULL) ;
}
inline void  FCDib::GetAlphaChannel (FCDib * alpha) {
	this->GetRGBChannel (NULL, NULL, NULL, alpha) ;
}
inline void  FCDib::NegatePixel (int x, int y) {
	RGBQUAD		rgb ;
	this->GetPixelColor (x, y, &rgb) ;
	rgb.rgbBlue = ~rgb.rgbBlue ;
	rgb.rgbGreen = ~rgb.rgbGreen ;
	rgb.rgbRed = ~rgb.rgbRed ;
	this->SetPixelColor (x, y, rgb) ;
}
inline void  FCDib::AlphaBlendPixel (int x, int y, RGBQUAD rgb) {
	RGBQUAD		pix ;
	this->GetPixelColor (x, y, &pix) ;
	pix.rgbBlue = max (0, min (0xFF, ((rgb.rgbBlue - pix.rgbBlue) * rgb.rgbReserved) / 0xFF + pix.rgbBlue)) ;
	pix.rgbGreen = max (0, min (0xFF, ((rgb.rgbGreen - pix.rgbGreen) * rgb.rgbReserved) / 0xFF + pix.rgbGreen)) ;
	pix.rgbRed = max (0, min (0xFF, ((rgb.rgbRed - pix.rgbRed) * rgb.rgbReserved) / 0xFF + pix.rgbRed)) ;
	this->SetPixelColor (x, y, pix) ;
}
inline void  FCDib::AND (const FCDib & Dib, int x, int y) {
	this->LogicalOperate (Dib, LOG_OPERATION_AND, x, y) ;
}
inline void  FCDib::OR (const FCDib & Dib, int x, int y) {
	this->LogicalOperate (Dib, LOG_OPERATION_OR, x, y) ;
}
inline void  FCDib::XOR (const FCDib & Dib, int x, int y) {
	this->LogicalOperate (Dib, LOG_OPERATION_XOR, x, y) ;
}
inline bool  FCDib::BoundRect (RECT & rect) {
	if (this->GetHandle () == NULL)
		return false ;
	RECT	img = {0, 0, this->Width(), this->Height()}, src ;
	::CopyRect (&src, &rect) ;
	if (::IntersectRect (&rect, &img, &src) == 0)
	{
		::SetRectEmpty (&rect) ;
		return false ; // û���ཻ����
	}
	return true ;
}
inline void  FCDib::__CopyPalette (const FCDib & DibSrc) {
	if ((this->ColorBits() <= 8) && (this->ColorBits() == DibSrc.ColorBits()))  {
		int			nNum = 1 << this->ColorBits() ;
		RGBQUAD		* pPalette = new RGBQUAD[nNum] ;
		DibSrc.GetColorTable (0, nNum, pPalette) ;
		this->SetColorTable (0, nNum, pPalette) ;
		delete[] pPalette ;
	}
}
inline DWORD  FCDib::__GetPixelData (BYTE * pBits) const {
	if (this->ColorBits() == 24) {
		DWORD		ret = 0 ;
		* (RGBTRIPLE *) &ret = * (RGBTRIPLE *) pBits ;
		return ret ;
	}
	if (this->ColorBits() == 32)
		return * (DWORD *) pBits ;
	if (this->ColorBits() == 8)
		return *pBits ;
	if (this->ColorBits() == 16)
		return * (WORD *) pBits ;
	return 0 ;
}
inline void  FCDib::ResetFinishedPercent () const {
	::InterlockedExchange ((LONG *)&m_FinishedPer, 0) ;
}
inline int  FCDib::GetFinishedPercent () const {
	return m_FinishedPer ;
}
inline void  FCDib::SetFinishedPercent (int nPercent) {
	::InterlockedExchange ((LONG *)&m_FinishedPer, max(0,min(100,nPercent))) ;
}
inline RGBQUAD  FCDib::fooRGBtoRGBQUAD (COLORREF color) {
	RGBQUAD		rgb ;
	rgb.rgbRed = GetRValue (color) ;
	rgb.rgbGreen = GetGValue (color) ;
	rgb.rgbBlue = GetBValue (color) ;
	rgb.rgbReserved = 0 ;
	return rgb ;
}
inline COLORREF  FCDib::fooRGBQUADtoRGB (RGBQUAD rgb) {
	return RGB (rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue) ;
}

#endif