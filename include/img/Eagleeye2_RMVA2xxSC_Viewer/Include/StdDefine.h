#ifndef		__FOO_STDDEFINE_H__
#define		__FOO_STDDEFINE_H__
#include <windows.h>
#pragma once

//=============================================================================
//	����
//=============================================================================
const double	LIB_PI = 3.1415926536;
//=============================================================================
//	������������
//=============================================================================
typedef		signed char			INT8;
typedef		unsigned char		UINT8;
typedef		signed short		INT16;
typedef		unsigned short		UINT16;
typedef		signed int			INT32;
typedef		unsigned int		UINT32;
#ifndef PCTSTR
typedef		LPCTSTR				PCTSTR;
#endif
//=============================================================================
//	�߼�����
//=============================================================================
#define		LOG_OPERATION_AND			0x00000001
#define		LOG_OPERATION_OR			0x00000002
#define		LOG_OPERATION_XOR			0x00000004
#define		LOG_OPERATION_ADD			0x00000008
#define		LOG_OPERATION_SUB			0x00000010
#define		LOG_OPERATION_MUL			0x00000020
#define		LOG_OPERATION_DIV			0x00000040
//=============================================================================
//	ƽ��8������
//=============================================================================
#define		DIRECT_UPPER_LEFT			0x00000001
#define		DIRECT_UPPER				0x00000002
#define		DIRECT_UPPER_RIGHT			0x00000004
#define		DIRECT_LEFT					0x00000008
#define		DIRECT_RIGHT				0x00000010
#define		DIRECT_LOWER_LEFT			0x00000020
#define		DIRECT_LOWER				0x00000040
#define		DIRECT_LOWER_RIGHT			0x00000080
//=============================================================================
//	16, 32 λɫ������
//=============================================================================
#define		MASK16_RED_565				0x0000F800
#define		MASK16_GREEN_565			0x000007E0
#define		MASK16_BLUE_565				0x0000001F
#define		MASK16_RED_555				0x00007C00
#define		MASK16_GREEN_555			0x000003E0
#define		MASK16_BLUE_555				0x0000001F
#define		MASK32_ALPHA				0xFF000000
#define		MASK32_RED					0x00FF0000
#define		MASK32_GREEN				0x0000FF00
#define		MASK32_BLUE					0x000000FF
//=============================================================================
//	RGBAͨ��
//=============================================================================
#define		CHANNEL_RED					0x00000001
#define		CHANNEL_GREEN				0x00000002
#define		CHANNEL_BLUE				0x00000004
#define		CHANNEL_ALPHA				0x00000008
//#define		CHANNEL_BITS				0x00000008 // ÿChannelλ��
//=============================================================================
//	ͼ���ļ���ʽ
//=============================================================================
#define		IMAGE_UNKNOW				0x00000000
#define		IMAGE_BMP					0x00000001
#define		IMAGE_PCX					0x00000002
#define		IMAGE_JPG					0x00000004
#define		IMAGE_GIF					0x00000008
#define		IMAGE_TGA					0x00000010
#define		IMAGE_TIF					0x00000020
#define		IMAGE_PNG					0x00000040
#define		IMAGE_ICO					0x00000080
#define		IMAGE_WMF					0x00000100

#define		GIF_MAX_FRAME				0x00000800 // ���֧��2048֡��GIF
//=============================================================================
// Foolish Simple Demo Windows Classes
//=============================================================================
#define		WND_DEFAULT_CLASS		TEXT("Foolish_Class_Template")
#define		WND_PROPERTY_this		TEXT("this_address")
#define		INVALID_FILE_ATTRIBUTES	0xFFFFFFFF

#define		DLG_RESIZE_MOVE			0x00000001
#define		DLG_RESIZE_STRETCH		0x00000002

//=============================================================================
//	FCBitmap::DrawEx(...) �� ShowParam ����
//=============================================================================
typedef struct tagSHOWEFFECT
{
	DWORD	dwMethod;		// ����
	DWORD	dwSubMethod;	// ���巽��
	DWORD	dwInterval;	// ����� (΢/��) ����
	DWORD	dwBlockLen;	// �ӷ�������
} SHOWEFFECT, * PSHOWEFFECT;

//	SCAN���� : dwInterval��Ϊ���<΢>����
#define		SHOWDIB_SCAN			0x00000004 
#define			SHOWDIB_SCAN_TODOWN		0x0001 
#define			SHOWDIB_SCAN_TOUP		0x0002
#define			SHOWDIB_SCAN_TOLEFT		0x0004
#define			SHOWDIB_SCAN_TORIGHT	0x0008
#define			SHOWDIB_SCAN_HORIZON	0x0010
#define			SHOWDIB_SCAN_VERTICAL	0x0020
#define			SHOWDIB_SCAN_VINTERLACE	0x0040
#define			SHOWDIB_SCAN_HINTERLACE	0x0080

//	MOVE���� : dwInterval��Ϊ���<΢>����
//			   ���ڽϴ��ͼƬ�����
#define		SHOWDIB_MOVE			0x00000008
#define			SHOWDIB_MOVE_TODOWN		0x0001
#define			SHOWDIB_MOVE_TOUP		0x0002
#define			SHOWDIB_MOVE_TOLEFT		0x0004
#define			SHOWDIB_MOVE_TORIGHT	0x0008
#define			SHOWDIB_MOVE_HORIZON	0x0010
#define			SHOWDIB_MOVE_VERTICAL	0x0020

//	��Ҷ������ : dwInterval��Ϊ���<��>����
//				 dwBlockLen��Ϊ��ļ��
#define		SHOWDIB_HSHUTTER		0x00000010
#define		SHOWDIB_VSHUTTER		0x00000020

//	��������ʾ���� : dwInterval��Ϊ���<΢>����
//					 dwBlockLen��ΪС�����α߳�
#define		SHOWDIB_MOSAIC			0x00000040

//	���ַ��� : dwInterval��Ϊ���<��>����
//			   dwBlockLen��Ϊ���
#define		SHOWDIB_FADE			0x00000080
#define		SHOWDIB_FADE_SCAN		0x00000100

//=============================================================================
//	FCDibEffect::Algorithm () ����
//=============================================================================
#define		EFFECT_SMOOTH_1				0x00000001	// ƽ��
#define		EFFECT_SMOOTH_2				0x00000002
#define		EFFECT_SMOOTH_3				0x00000004
#define		EFFECT_SHARP_1				0x00000008	// ��
#define		EFFECT_SHARP_2				0x00000010
#define		EFFECT_SHARP_3				0x00000020
#define		EFFECT_ENHANCE_VERT			0x00000040	// ��Ե��ǿ
#define		EFFECT_ENHANCE_HORZ			0x00000080
#define		EFFECT_ENHANCE_NORTH		0x00000100
#define		EFFECT_ENHANCE_SOUTH		0x00000200
#define		EFFECT_ENHANCE_WEST			0x00000400
#define		EFFECT_ENHANCE_EAST			0x00000800
#define		EFFECT_ENHANCE_NORTHEAST	0x00001000
#define		EFFECT_ENHANCE_SOUTHWEST	0x00002000
#define		EFFECT_ENHANCE_NORTHWEST	0x00004000
#define		EFFECT_ENHANCE_SOUTHEAST	0x00008000
#define		EFFECT_ENHANCE_VERTHORZ		0x00010000
#define		EFFECT_ENHANCE_LAPLACE1		0x00020000	// Laplace��Ե��ǿ
#define		EFFECT_ENHANCE_LAPLACE2		0x00040000
#define		EFFECT_ENHANCE_LAPLACE3		0x00080000
#define		EFFECT_ENHANCE_LAPLACE4		0x00100000

typedef struct tagFPICINFO
{
	DWORD		dwImageType;		// �ļ�����, ���� : IMAGE_XXX
	DWORD		dwWidth;			// ��
	DWORD		dwHeight;			// ��
	DWORD		dwLoadTime;		// ��ȡʱ��(ms)
	WORD		wBitCount;			// Gif��Ϊ��ʵ��ɫλ�� [1..8]
	WORD		wTotalFrame	;		// �ļ�������֡��
	WORD		wDelayTime;		// ��֡����ʱ������
	BYTE		byInterlace;		// �Ƿ��Խ���ʽ�洢
	BYTE		byTransFlag;		// �Ƿ���͸��ɫ
	RGBQUAD		rgbTransparency;	// ͸��ɫ����ֵ, ����ɫ����ʾ����Ļ��
} FPICINFO, * PFPICINFO; // ͼ���ļ���Ϣ

typedef struct tagVOLUMEINFO // ������Ϣ
{
	TCHAR	 Driver[32]; 			
	TCHAR	 FileType[32];			// �ļ�ϵͳ���� (such as "FAT32", "FAT16", "NTFS", "CDFS")
	TCHAR 	 VolumeName[32];		// ���̾��
	DWORD 	 DiskType;				// �������� ( ��::GetDriveType )
	DWORD	 dwVolumeSerialNumber;	// �������к�
} VOLUMEINFO, * PVOLUMEINFO;

typedef struct tagDISPLAYMODE
{
	DWORD	dwBits;
	DWORD	dwWidth;
	DWORD	dwHeight;
	DWORD	dwFrequency;
} DISPLAYMODE, * PDISPLAYMODE; // ��Ļ��Ϣ

//=============================================================================

#endif