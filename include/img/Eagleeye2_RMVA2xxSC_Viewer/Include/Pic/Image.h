//////////////////////////////////////////////////////////////////
//																//
//		��; : ͼ���ļ�������� Jpg,Gif,Bmp,Pcx,Tga,Png,Tif		//
//		���� : [Foolish] / 2001-4-27							//
//		���� : 2002-11-9										//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef	 __FOO_PICIMAGE_H__
#define	 __FOO_PICIMAGE_H__
#include "Effect.h"
#include "..\Compress\Lzw.h"
extern "C"
{
	#include "..\..\Lib\jpeg\jpeglib.h"
	#include "..\..\Lib\jpeg\jerror.h"
	#include "..\..\Lib\tiff\tiffio.h"
} ;
#pragma once

//===================================================================
//	ͼ���ļ�����
//===================================================================
/********************************************************************/
/*		����Save, �ļ������򸲸�									*/
/********************************************************************/
class FCImage : public FCDibEffect
{
public :
	FCImage () ;
	virtual ~FCImage () ;
	void	operator= (const FCImage & pic) ;
	void	operator= (const FCDibEffect & pic) ;
	void	operator= (const FCDib & pic) ;
	void	GetImageInfo (PCTSTR szFileName, FPICINFO * PicInfo) ;
	void	GetGifInfo (GIFINFO * gifinfo) ;
	int		GetImageType (PCTSTR szFileName) ; // �����ļ���׺���ж��ļ�����

	/************************************************************/
	/*					�Զ�����								*/
	/************************************************************/
	BOOL	Load (PCTSTR szFileName) ;
	BOOL	Save (PCTSTR szFileName) ;

	/************************************************************/
	/*				BMPͼ���ļ�ģ��								*/
	/*		֧  �� : 1, 4, 8, 16, 24, 32λɫͼ��				*/
	/*		��֧�� : RLE��ʽ��λͼ								*/
	/************************************************************/
	BOOL	LoadBmp (PCTSTR szFileName) ;
	BOOL	SaveBmp (PCTSTR szFileName) ;

	/************************************************************/
	/*				PCXͼ���ļ�ģ��								*/
	/*		֧  �� : 1, 4, 8, 24λɫͼ��						*/
	/*		��֧�� : 4 plane ��16λɫλͼ						*/
	/************************************************************/
	BOOL	LoadPcx (BYTE * pStart, int iFileSize) ; // ���ڴ�
	BOOL	LoadPcx (PCTSTR resName, PCTSTR resType) ; // ����Դ�ж�ȡ
	BOOL	LoadPcx (PCTSTR szFileName) ;
	BOOL	SavePcx (PCTSTR szFileName) ;

	/************************************************************/
	/*				TGAͼ���ļ�ģ��								*/
	/*		֧  �� : 8, 16, 24, 32λɫͼ��						*/
	/*		��֧�� : 15λɫͼ��, 15,16λɫ��ɫ��ͼ��,			*/
	/*				 ���Ҿ����TGA								*/
	/************************************************************/
	BOOL	LoadTga (BYTE * pStart) ; // ���ڴ�
	BOOL	LoadTga (PCTSTR resName, PCTSTR resType) ; // ����Դ�ж�ȡ
	BOOL	LoadTga (PCTSTR szFileName) ;
	BOOL	SaveTga (PCTSTR szFileName, bool bEncode = true) ; // �Ƿ�ʹ��RLEѹ��

	/************************************************************/
	/*				JPEGͼ���ļ�ģ��							*/
	/************************************************************/
	BOOL	LoadJpeg (BYTE * pStart, int iFileSize) ; // ���ڴ�
	BOOL	LoadJpeg (PCTSTR resName, PCTSTR resType) ; // ����Դ�ж�ȡ
	BOOL	LoadJpeg (PCTSTR szFileName) ;
	BOOL	SaveJpeg (PCTSTR szFileName, DWORD dwQuality = 85) ; // ֻ��8(gray), 24λɫ��Ч

	/************************************************************/
	/*				TIFFͼ���ļ�ģ��							*/
	/*		��֧�� : 16λɫDIB��Save							*/
	/************************************************************/
	BOOL	LoadTif (BYTE * pStart, int iFileSize) ; // ���ڴ�
	BOOL	LoadTif (PCTSTR resName, PCTSTR resType) ; // ����Դ�ж�ȡ
	BOOL	LoadTif (PCTSTR szFileName) ;
	BOOL	SaveTif (PCTSTR szFileName, int nEncodeMode = COMPRESSION_JPEG) ;

	/************************************************************/
	/*				PNGͼ���ļ�ģ��								*/
	/*	 ��ģ��ʹ�� libpng version 1.2.1 (December 12, 2001)	*/	
	/*		Copyright (c) 1998-2001 Glenn Randers-Pehrson		*/
	/************************************************************/
	BOOL	LoadPng (BYTE * pStart) ; // ���ڴ�
	BOOL	LoadPng (PCTSTR resName, PCTSTR resType) ; // ����Դ�ж�ȡ
	BOOL	LoadPng (PCTSTR szFileName) ;
	BOOL	SavePng (PCTSTR szFileName, bool bInterlace = true) ;

	/************************************************************/
	/*				GIFͼ���ļ�ģ��								*/
	/*		֧  �� : 1--8λɫ����, ����洢GIF����				*/
	/*				 1, 4, 8λɫ����GIF����						*/
	/*		��֧�� :											*/
	/************************************************************/
	BOOL	LoadGif (PCTSTR resName, PCTSTR resType) ; // ����Դ�ж�ȡ��һ֡
	BOOL	LoadGif (PCTSTR szFileName) ; // �����һ֡
	BOOL	LoadGifFrame (int iNumber) ; // iNumber = [1..wTotalFrame]
	int		LoadGifNextFrame () ; // ������һ֡, �����ȵ�����LoadGif (0-ʧ��, 1-�ɹ�, 2-���һ֡)

	//	�ѵ�ǰDIB��Ϊgif�ļ�, bGolPalΪ�Ƿ�ʹ�ô�֡��ɫ����Ϊȫ�ֵ�ɫ��
	BOOL	SaveGif (PCTSTR szFileName, bool bGolPal = true) ;
	//	�ѵ�ǰDIB���뵽gif�ļ������, bGolPalΪ�Ƿ�ʹ��ȫ�ֵ�ɫ����Ϊ��֡�ĵ�ɫ��
	//	dwDelayΪ��֡����ʱ������, dwTransIndexΪ͸��ɫ,-1��ʾ��͸��ɫ
	BOOL	AddGifFrame (PCTSTR szFileName, bool bGolPal = false,
						 DWORD dwDelay = 100, DWORD dwTransIndex = -1) ;
//	BOOL	InsertGifFrame (PCTSTR szFileName, const FCDib & dib) ;

private :
	GIFINFO		* m_pGifInfo ;
private :
	bool	__fooGifScanFile (BYTE * pStart, int iFileSize) ; // ɨ���ȡGif�ļ���Ϣ, ���m_pGifInfo
	void	__fooGifInitInfo (int iFileSize) ; // ��ʼ��m_pGifInfo
	void	__fooGifFreeInfo () ; // �ͷ�m_pGifInfo��Դ
	void	__fooGifDecode (BYTE * InBuffer) ; // ����, ��Ϊ��SEH����������������ܲ���
	DWORD	__fooGifEncode (BYTE * InBuffer) ; // ����, ��Ϊ��SEH����������������ܲ���
	BYTE  *	__fooImageReadFile (PCTSTR szFileName, HANDLE * hFile, HANDLE * hMap) ; // ӳ���ļ�
	BYTE  *	__fooImageSaveFile (PCTSTR szFileName, HANDLE * hFile, HANDLE * hMap, int iMaxSize) ; // ӳ���ļ�
	BYTE  *	__fooGifOpenFileForAddFrame (PCTSTR szFileName, HANDLE * hFile, HANDLE * hMap, int iMaxSize) ; // ӳ���ļ�
	void	__fooImageUnmapFile (BYTE * pBase, HANDLE hMap, HANDLE hFile) ; // ����ӳ��
// JPEG helper class in order to load JPEG from memory
private :
class FCJpegStream : public jpeg_source_mgr
{
public:
	FCJpegStream (BYTE * pStart, int iFileSize)
	{
		next_input_byte = NULL ; bytes_in_buffer = 0 ; // base class member
		m_pBase = pStart ; m_iFileSize = iFileSize ;
		m_END[0] = (JOCTET) 0xFF ; m_END[1] = (JOCTET) JPEG_EOI ;
		fill_input_buffer = FillInputBuffer ;
		skip_input_data = SkipInputData ;
		resync_to_restart = jpeg_resync_to_restart ; // use default method
		init_source = InitSource ; term_source = TermSource ; // none use
	}
	static boolean FillInputBuffer (j_decompress_ptr cinfo)
	{
		FCJpegStream	* pSrc = (FCJpegStream *)cinfo->src ;
		pSrc->next_input_byte = (pSrc->m_iFileSize > 0) ? pSrc->m_pBase : pSrc->m_END ;
		pSrc->bytes_in_buffer = (pSrc->m_iFileSize > 0) ? pSrc->m_iFileSize : 2 ; // fake EOI marker
		return TRUE ;
	}
	static void SkipInputData (j_decompress_ptr cinfo, long num_bytes)
	{
		FCJpegStream	* pSrc = (FCJpegStream *)cinfo->src ;
		if (num_bytes > 0)
			if (pSrc->bytes_in_buffer > num_bytes)
			{
				pSrc->next_input_byte += num_bytes ;
				pSrc->bytes_in_buffer -= num_bytes ;
			}
			else
			{
				pSrc->next_input_byte = pSrc->m_END ;
				pSrc->bytes_in_buffer = 2 ;
			}
	}
	static void TermSource (j_decompress_ptr cinfo) { }
	static void InitSource(j_decompress_ptr cinfo) { }
public :
	BYTE		* m_pBase ;
	int			m_iFileSize ;
	BYTE		m_END[2] ;
} ;
} ;

//=============================================================================
//	Implement
//=============================================================================
inline FCImage::FCImage () {
	m_pGifInfo = NULL ;
}
inline FCImage::~FCImage () {
	this->__fooGifFreeInfo () ;
}
inline void  FCImage::operator= (const FCImage & pic) {
	FCDib::operator = (*(FCDib *)&pic) ;
}
inline void  FCImage::operator= (const FCDibEffect & pic) {
	FCDib::operator = (*(FCDib *)&pic) ;
}
inline void  FCImage::operator= (const FCDib & pic) {
	FCDib::operator = (*(FCDib *)&pic) ;
}
inline void  FCImage::__fooGifDecode (BYTE * InBuffer) {
	FCLzw	lzw ;
	lzw.LZW_GIF_Decode (InBuffer, this->GetBits (0), this->Width(), this->Height(), (m_pGifInfo->byInterlace != 0)) ;
}
inline DWORD  FCImage::__fooGifEncode (BYTE * OutBuffer) {
	FCLzw	lzw ;
	return lzw.LZW_GIF_Encode (this->GetBits (0), OutBuffer, this->Width(), this->Height(), this->ColorBits()) ;
}
inline void  FCImage::GetGifInfo (GIFINFO * gifinfo) {
	if ((gifinfo != NULL) && (m_pGifInfo != NULL))
		CopyMemory (gifinfo, m_pGifInfo, sizeof(GIFINFO)) ;
}
inline void  FCImage::__fooGifInitInfo (int iFileSize) {
	if (m_pGifInfo == NULL) // ��һ�ζ�ȡGIF
	{
		m_pGifInfo = new GIFINFO ;
		//	����֡����, ���֧��2048֡ͼ (StdDefine.h)
		m_pGifInfo->pFrameIndexArray = new DWORD [GIF_MAX_FRAME] ;
	}
	else
		delete[] m_pGifInfo->pStart ;
	m_pGifInfo->pStart = new BYTE [iFileSize] ;
	ZeroMemory (m_pGifInfo, 16) ;
	ZeroMemory (m_pGifInfo->pFrameIndexArray, sizeof(DWORD) * GIF_MAX_FRAME) ;
}
inline void  FCImage::__fooGifFreeInfo () {
	if (m_pGifInfo != NULL) // �Ѿ�Ϊ��ȡGIF������m_pGifInfo
	{
		delete[] m_pGifInfo->pFrameIndexArray ;
		delete[] m_pGifInfo->pStart ;
		delete m_pGifInfo ;
		m_pGifInfo = NULL ;
	}
}
inline BYTE *  FCImage::__fooImageReadFile (PCTSTR szFileName, HANDLE * hFile, HANDLE * hMap) {
	*hFile = ::CreateFile (szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL) ;
	if (*hFile == INVALID_HANDLE_VALUE)
		return NULL ;
	*hMap = ::CreateFileMapping (*hFile, NULL, PAGE_READONLY, 0, 0, NULL) ;
	if (*hMap == NULL)
		return NULL ;
	return (BYTE *) ::MapViewOfFile (*hMap, FILE_MAP_READ, 0, 0, 0) ;
}
inline BYTE *  FCImage::__fooImageSaveFile (PCTSTR szFileName, HANDLE * hFile, HANDLE * hMap, int iMaxSize) {
	*hFile = ::CreateFile (szFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL) ;
	if (*hFile == INVALID_HANDLE_VALUE)
		return NULL ;
	*hMap = ::CreateFileMapping (*hFile, NULL, PAGE_READWRITE, 0, iMaxSize, NULL) ;
	if (*hMap == NULL)
		return NULL ;
	return (BYTE *) ::MapViewOfFile (*hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0) ;
}
inline BYTE *  FCImage::__fooGifOpenFileForAddFrame (PCTSTR szFileName, HANDLE * hFile, HANDLE * hMap, int iMaxSize) {
	//	����AddGifFrame������
	*hFile = ::CreateFile (szFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL) ;
	if (*hFile == INVALID_HANDLE_VALUE)
		return NULL ;
	*hMap = ::CreateFileMapping (*hFile, NULL, PAGE_READWRITE, 0, iMaxSize, NULL) ;
	if (*hMap == NULL)
		return NULL ;
	return (BYTE *) ::MapViewOfFile (*hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0) ;
}
inline void  FCImage::__fooImageUnmapFile (BYTE * pBase, HANDLE hMap, HANDLE hFile) {
	if (pBase != NULL)	::UnmapViewOfFile (pBase) ;
	if (hMap != NULL)	::CloseHandle (hMap) ;
	if (hFile != INVALID_HANDLE_VALUE)	::CloseHandle (hFile) ;
}

#endif