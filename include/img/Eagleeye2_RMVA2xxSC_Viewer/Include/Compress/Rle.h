//////////////////////////////////////////////////////////////////
//																//
//		��; : RLE ѹ���㷨	(PCX, TGAͼ��)						//
//		���� : [Foolish] / 2000-9-6								//
//		���� : 2002-11-29										//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_RLE_COMPRESS_H__
#define		__FOO_RLE_COMPRESS_H__
#include <windows.h>
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

//====================================================================
//	RLE - ѹ���㷨
//====================================================================
//////////////////////////////////////////////////////////////////////
//	��  �� ��RLEѹ / ��ѹ�� ( PCX ͼ�� )							//
//	��  �� ��														//
//	����ֵ ��д��OutBuffer�е��ֽ���								//
//	˵  �� ��ѹ��ʱOutBuffer��������InBuffer��С					//
//////////////////////////////////////////////////////////////////////
int  RLE_PCX_Encode (BYTE * InBuffer, int nInSize, BYTE * OutBuffer) ;
int  RLE_PCX_Decode (BYTE * InBuffer, int nInSize, BYTE * OutBuffer) ;
//////////////////////////////////////////////////////////////////////
//	��  �� ��RLEѹ / ��ѹ�� ( PCX, TGA ͼ�� )						//
//																	//
//	��  �� ��iColorBit Ϊ������ɫλ��								//
//					PCXΪ ��1, 4, 8, 24								//
//					TGAΪ ��8, 16, 24, 32							//
//			 iNumPixel Ϊд��OutBuffer�е����ظ���					//
//																	//
//	����ֵ ��EncodeLine ���ر���� OutBuffer ��ָ��					//
//			 DecodeLine ���ؽ���� InBuffer ��ָ��					//
//																	//
//	˵  �� ��PCXͼ�� ��1,4,8λɫλͼ���ֽڴ���24λɫ�ֳ�3ͨ������	//
//			 TGAͼ�� ��֧��8, 16, 24, 32ɫ, ��֧��15λɫ			//
//			 ѹ��ʱOutBuffer��������InBuffer��С					//
//////////////////////////////////////////////////////////////////////
BYTE * RLE_TGA_EncodeLine (BYTE * InBuffer, int iColorBit,
						   int iNumPixel,  // iNumPixel��Ϊͼ���
						   BYTE * OutBuffer) ;
BYTE * RLE_TGA_DecodeLine (BYTE * InBuffer, int iColorBit,
						   int iNumPixel,  // iNumPixel��Ϊͼ���
						   BYTE * OutBuffer) ;

//===================================================================
#ifdef __cplusplus
}
#endif
#endif