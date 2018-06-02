#ifndef		__FOO_IMAGEFORMAT_H__
#define		__FOO_IMAGEFORMAT_H__
#include <windows.h>
#pragma once

//=============================================================================
//	PCX ͼ���ļ�ͷ
//=============================================================================
#include <pshpack1.h>
typedef struct tagPCXHEAD
{
	BYTE	signal ;		// 00 - always 0x0A
	BYTE	version ;		// 01 - version number (0,2,3,4,5)
	BYTE	encoding ;		// 02 - always 1 (RLE)
	BYTE	bit_per_pixel ;	// 03 - ÿƽ������bit�� (1 or 4 or 8)
	WORD	xmin ;			// 04 
	WORD	ymin ;			// 06   ���ĸ���ָ��
	WORD	xmax ;			// 08     ͼ���С
	WORD	ymax ;			// 0A  
	WORD	Xresolution ;	// 0C - ��ĻX������
	WORD	Yresolution ;	// 0E - ��ĻY������
	BYTE	palette[48] ;	// 10 - 1 or 4 Bits ͼ���ɫ��
	BYTE	reserved ;		// 40 - Set 0
	BYTE	color_planes ;	// 41 - ɫ��ƽ����Ŀ (1 or 3 or 4)
	WORD	byte_per_line ;	// 42 - ÿ���ֽڿ�� (ÿɫ��plane)
	WORD	palette_type ;	// 44 - always 1
	BYTE	filler[58] ;	// 46 - Set 0
} PCXHEAD, * PPCXHEAD ; // 128 - Bytes
#include <poppack.h>

//=============================================================================
//	TGA ͼ���ļ�ͷ ( ��BMP�ļ�һ�������ش����½Ǵ洢 )
//=============================================================================
#include <pshpack1.h>
typedef struct tagTGAHEAD
{
	BYTE	byID_Length ;	// ͼ��ʶ����Ϣ��С
	BYTE	byPalType ;		// 00 : �޵�ɫ��	01 : �е�ɫ��
	BYTE	byImageType ;	// 01-UC+PAL, 02-UC+NP, 09-RLE+PAL, 0x0A-RLE+NP
	WORD	wPalFirstNdx ;	// ��ɫ����ʼ����
	WORD	wPalLength ;	// ��ɫ�峤��
	BYTE	byPalBits ;		// ��ɫ����ÿһ��ɫ��ռλ��
	WORD	wLeft ;			// �������Ļ���½�X����
	WORD	wBottom ;		// �������Ļ���½�Y����
	WORD	wWidth ;		// ��
	WORD	wHeight ;		// ��
	BYTE	byColorBits ;	// ɫ��λ��
	struct 
	{
		BYTE	AlphaBits : 4 ;	// ÿ����Alpha Channelλ��
		BYTE	HorMirror : 1 ;	// Ϊ1��ʾͼ���������ҵߵ��洢
		BYTE	VerMirror : 1 ;	// Ϊ1��ʾͼ���������µߵ��洢
		BYTE	Reserved : 2 ;
	} Descriptor ;
} TGAHEAD, * PTGAHEAD ; // 18 - Bytes
#include <poppack.h>

//=============================================================================
//	GIF �ļ�ͷ
//=============================================================================
#include <pshpack1.h>
typedef struct tagGIFHEAD
{
	BYTE	bySignature[3] ;	// �̶�Ϊ "GIF"
	BYTE	byVersion[3]   ;	// "89a" �� "87a"
	WORD	wWidth  ;			// ��Ļ��� 
	WORD	wHeight ;			// ��Ļ�߶�
	struct
	{
		BYTE	PalBits	  : 3 ;	// ȷ����ɫ���С : 3 * (1 << (PalBits + 1))
		BYTE	SortFlag  : 1 ;	// ��ɫ���е���ɫ�Ƿ�ʹ��������
		BYTE	ColorRes  : 3 ;	// �ɼ���ֱ���   : 3 * (1 << (ColorRes + 1))
		BYTE	GlobalPal : 1 ;	// ȫ�ֵ�ɫ��, Ϊ 1 ʱPalBits��������
	} GlobalFlag ;
	BYTE	byBackground ;		// ͼ��С���߼���Ļʱ, �ô���ɫ���δ�����ǵ�����
	BYTE	byAspect ;			// �߼���Ļ��߱�
} GIFHEAD, * PGIFHEAD ;  // 13 - Bytes
#include <poppack.h>

/************************************************************/
/*		ȫ�ֵ�ɫ�����ļ��е�ƫ��������13					*/
/************************************************************/
typedef struct tagGIFINFO
{
	BYTE	byVersion[4]   ;		// "89a" �� "87a"
	WORD	wTotalFrame	;			// ��֡��
	WORD	wCurrentFrame ;			// ��ǰ֡�� [0..wTotalFrame]
	WORD	wGolPalNum ;			// ȫ�ֵ�ɫ����ɫ����, Ϊ0����
	WORD	wDelayTime ;			// ��֡����ʱ������
	BYTE	byInterlace ;			// �Ƿ��Խ���ʽ�洢
	BYTE	byTransFlag ;			// �Ƿ���͸��ɫ
	BYTE	byTransparencyIndex ;	// ͸��ɫ����ֵ, ����ɫ����ʾ����Ļ��
	BYTE	byBitCount ;			// Ϊ��ʵColorBit��
	DWORD	* pFrameIndexArray ;	// ָ֡��, ���ļ��е�ƫ����, ��λ��0x2C��ĵ�һ���ֽ�
	BYTE	* pStart ;				// ����������ʼָ��
}  GIFINFO, * PGIFINFO ;

//===================================================================

#endif