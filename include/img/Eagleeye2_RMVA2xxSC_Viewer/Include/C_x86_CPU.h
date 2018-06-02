//////////////////////////////////////////////////////////////////
//																//
//		��; : x86-CPU����										//
//		���� : [Foolish] / 2001-3-15							//
//		���� : 2002-4-21										//
//		��ҳ : http://crazybit.topcities.com/					//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_X86CPU_H__
#define		__FOO_X86CPU_H__
#include <windows.h>
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	/************************************************************/
	/*				 ����CPU����Ƶ								*/
	/*		��2000��, CPU��Ƶ����ֱ�Ӵ�Registry�л��			*/
	/*	 H_L_M\HARDWARE\DESCRIPTION\System\CentralProcessor\0	*/
	/************************************************************/
	void *  fooStartCountCpuSpeed () ;
	DWORD  fooEndCountCpuSpeed (void * pRet) ; // pRet Ϊ��һ��������ֵ

	/************************************************************/
	/*				 ��ȷ��ʱ									*/
	/*		 ��ƵΪ~MHz, �ӳ�ʱ��Ϊ<΢>����						*/
	/************************************************************/
	void  fooDelay (int iFrequency, DWORD dwDelayTime) ; // dwDelayTime����
	void  fooDelayCount (DWORD dwDelayCount) ;

	/************************************************************/
	/*				 ��ñ���CPU����Ŀ							*/
	/************************************************************/
	DWORD  fooGetCpuNumber () ;

	/************************************************************/
	/*		��pData��ʼ���ֽ������iAdd, ��������[0,255]		*/
	/*			iAdd �� [-255,255] �У�����Ϊ��ֵ				*/
	/*					����ͼ������ʱʹ��						*/
	/************************************************************/
	void  fooMMX_ADD (BYTE * pData, int iLength, int iAdd) ;

//===================================================================
__forceinline void  fooDelayCount (DWORD dwDelayCount) {
	ULARGE_INTEGER		uStart, uFlag ;
	__asm
	{
		rdtsc
		mov		uStart.LowPart, eax		// ��32λ
		mov		uStart.HighPart, edx	// ��32λ
	}
	uFlag.QuadPart = dwDelayCount + uStart.QuadPart ;
	do {
		__asm
		{
			rdtsc
			mov		uStart.LowPart, eax		// ��32λ
			mov		uStart.HighPart, edx	// ��32λ
		}
	} while (uStart.QuadPart < uFlag.QuadPart) ;
}
__forceinline void  fooDelay (int iFreq, DWORD dwDelayTime) {
	ULARGE_INTEGER		uStart, uFlag ;
	__asm
	{
		rdtsc
		mov		uStart.LowPart, eax		// ��32λ
		mov		uStart.HighPart, edx	// ��32λ
	}
	uFlag.QuadPart = dwDelayTime * iFreq + uStart.QuadPart - 5 ;
	do {
		__asm
		{
			rdtsc
			mov		uStart.LowPart, eax		// ��32λ
			mov		uStart.HighPart, edx	// ��32λ
		}
	} while (uStart.QuadPart < uFlag.QuadPart) ;
}
//===================================================================
inline void  fooMMX_ADD (BYTE * pData, int iLength, int iAdd) {
	if (iLength <= 8) 
	{
		for ( ; iLength > 0 ; iLength--)
			*pData++ = max (min (*pData + iAdd, 255), 0) ;
		return ;
	}
	BYTE   byMM7[8] ;
	memset (byMM7, (iAdd > 0) ? iAdd : (-iAdd), 8) ;

	// ʹpData 8Byte����
	DWORD	temp = 8 - ((DWORD)pData % 8) ;
	iLength -= temp ;
	for ( ; temp > 0 ; temp--)
		*pData++ = max (min (*pData + iAdd, 255), 0) ;

	__asm
	{
		mov		eax, iLength			// eax���ʣ���ֽ���
		mov		esi, dword ptr pData	// esi�������
		lea		edi, byMM7
		movq	mm7, [edi]
		cmp		iAdd, 0
		jng		foo_sub
foo_add :
		cmp		eax, 32
		jb		not_enough
		movq	mm0, [esi + 0]
		movq	mm1, [esi + 8]
		movq	mm2, [esi + 16]
		movq	mm3, [esi + 24]
		paddusb	mm0, mm7
		paddusb	mm1, mm7
		paddusb	mm2, mm7
		paddusb	mm3, mm7
		movq	[esi + 0], mm0
		movq	[esi + 8], mm1
		movq	[esi + 16], mm2
		movq	[esi + 24], mm3
		sub		eax, 32
		add		esi, 32
		jmp		foo_add
foo_sub :
		cmp		eax, 32
		jb		not_enough
		movq	mm0, [esi + 0]
		movq	mm1, [esi + 8]
		movq	mm2, [esi + 16]
		movq	mm3, [esi + 24]
		psubusb	mm0, mm7
		psubusb	mm1, mm7
		psubusb	mm2, mm7
		psubusb	mm3, mm7
		movq	[esi + 0], mm0
		movq	[esi + 8], mm1
		movq	[esi + 16], mm2
		movq	[esi + 24], mm3
		sub		eax, 32
		add		esi, 32
		jmp		foo_sub
not_enough :
		mov		dword ptr temp, eax
		mov		dword ptr pData, esi
		emms
	}
	for ( ; temp > 0 ; temp--)
		*pData++ = max (min (*pData + iAdd, 255), 0) ;
}

#ifdef __cplusplus
}
#endif
#endif