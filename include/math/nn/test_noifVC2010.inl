////////////////////////////////////////////////////////////
// noifVC2010.cpp : VC2010���ʹ����ٶȲ���
// Author: zyl910
// Blog: http://www.cnblogs.com/zyl910
// URL: http://www.cnblogs.com/zyl910/archive/2012/03/28/noifopex4.html
// Version: V1.0
// Updata: 2012-02-28
////////////////////////////////////////////////////////////


#include "stdafx.h"

#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>


// ��λ���������ʹ���.������������
#define LIMITSU_FAST(n, bits) ( (n) & -((n) >= 0) | -((n) >= (1<<(bits))) )
#define LIMITSU_SAFE(n, bits) ( (LIMITSU_FAST(n, bits)) & ((1<<(bits)) - 1) )
#define LIMITSU_BYTE(n) ((BYTE)(LIMITSU_FAST(n, 8)))

// ��λ���������ʹ���.�ô�����������������
#define LIMITSW_FAST(n, bits) ( ( (n) | ((signed short)((1<<(bits)) - 1 - (n)) >> 15) ) & ~((signed short)(n) >> 15) )
#define LIMITSW_SAFE(n, bits) ( (LIMITSW_FAST(n, bits)) & ((1<<(bits)) - 1) )
#define LIMITSW_BYTE(n) ((BYTE)(LIMITSW_FAST(n, 8)))

// ���ݹ�ģ
#define DATASIZE	16384	// 128KB / (sizeof(signed short) * 4)

// ������
signed short	bufS[DATASIZE*4];	// Դ��������64λ����ɫ��4ͨ����ÿͨ��16λ��
BYTE	bufD[DATASIZE*4];	// Ŀ�껺������32λ����ɫ��4ͨ����ÿͨ��8λ��

// ����ʱ�ĺ�������
typedef void (*TESTPROC)(BYTE* pbufD, const signed short* pbufS, int cnt);

// ��if��֧�����ʹ���
void f0_if(BYTE* pbufD, const signed short* pbufS, int cnt)
{
	const signed short* pS = pbufS;
	BYTE* pD = pbufD;
	int i;
	for(i=0; i<cnt; ++i)
	{
		// �ֱ��4��ͨ�������ʹ���
		pD[0] = (pS[0]<0) ? 0 : ( (pS[0]>255) ? 255 : (BYTE)pS[0] );
		pD[1] = (pS[1]<0) ? 0 : ( (pS[1]>255) ? 255 : (BYTE)pS[1] );
		pD[2] = (pS[2]<0) ? 0 : ( (pS[2]>255) ? 255 : (BYTE)pS[2] );
		pD[3] = (pS[3]<0) ? 0 : ( (pS[3]>255) ? 255 : (BYTE)pS[3] );
		// next
		pS += 4;
		pD += 4;
	}
}

// ��min��max���ʹ���
void f1_min(BYTE* pbufD, const signed short* pbufS, int cnt)
{
	const signed short* pS = pbufS;
	BYTE* pD = pbufD;
	int i;
	for(i=0; i<cnt; ++i)
	{
		// �ֱ��4��ͨ�������ʹ���
		pD[0] = min(max(0, pS[0]), 255);
		pD[1] = min(max(0, pS[1]), 255);
		pD[2] = min(max(0, pS[2]), 255);
		pD[3] = min(max(0, pS[3]), 255);
		// next
		pS += 4;
		pD += 4;
	}
}

// ��λ���������ʹ���.������������
void f2_neg(BYTE* pbufD, const signed short* pbufS, int cnt)
{
	const signed short* pS = pbufS;
	BYTE* pD = pbufD;
	int i;
	for(i=0; i<cnt; ++i)
	{
		// �ֱ��4��ͨ�������ʹ���
		pD[0] = LIMITSU_BYTE(pS[0]);
		pD[1] = LIMITSU_BYTE(pS[1]);
		pD[2] = LIMITSU_BYTE(pS[2]);
		pD[3] = LIMITSU_BYTE(pS[3]);
		// next
		pS += 4;
		pD += 4;
	}
}

// ��λ���������ʹ���.�ô�����������������
void f3_sar(BYTE* pbufD, const signed short* pbufS, int cnt)
{
	const signed short* pS = pbufS;
	BYTE* pD = pbufD;
	int i;
	for(i=0; i<cnt; ++i)
	{
		// �ֱ��4��ͨ�������ʹ���
		pD[0] = LIMITSW_BYTE(pS[0]);
		pD[1] = LIMITSW_BYTE(pS[1]);
		pD[2] = LIMITSW_BYTE(pS[2]);
		pD[3] = LIMITSW_BYTE(pS[3]);
		// next
		pS += 4;
		pD += 4;
	}
}

// ���в���
void runTest(_TCHAR* szname, TESTPROC proc)
{
	int i,j;
	DWORD	tm0, tm1;	// �洢ʱ��
	for(i=1; i<=3; ++i)	// ��β���
	{
		//tm0 = GetTickCount();
		tm0 = timeGetTime();
		// main
		for(j=1; j<=4000; ++j)	// �ظ����㼸���ӳ�ʱ�䣬�����ʱ��������
		{
			proc(bufD, bufS, DATASIZE);
		}
		// show
		//tm1 = GetTickCount() - tm0;
		tm1 = timeGetTime() - tm0;
		_tprintf(_T("%s[%d]:\t%u\n"), szname, i, tm1);

	}
}

// ��ȡ����λ����������Ϊ����λ�Ĵ��룩
int GetProgramBits()
{
	return sizeof(int*) * 8;
}

// ��ȫ��ȡ����ʵϵͳ��Ϣ
VOID SafeGetNativeSystemInfo(__out LPSYSTEM_INFO lpSystemInfo)
{
	if (NULL==lpSystemInfo)	return;
	typedef VOID (WINAPI *LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
	LPFN_GetNativeSystemInfo fnGetNativeSystemInfo = (LPFN_GetNativeSystemInfo)GetProcAddress( GetModuleHandle(_T("kernel32")), "GetNativeSystemInfo");;
	if (NULL != fnGetNativeSystemInfo)
	{
		fnGetNativeSystemInfo(lpSystemInfo);
	}
	else
	{
		GetSystemInfo(lpSystemInfo);
	}
}

// ��ȡ����ϵͳλ��
int GetSystemBits()
{
	SYSTEM_INFO si;
	SafeGetNativeSystemInfo(&si);
 	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
	{
		return 64;
	}
	return 32;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i;	// ѭ������

	//printf("Hello World!\n");
	const int nBitCode = GetProgramBits();
	const int nBitSys = GetSystemBits();
	_tprintf(_T("== noif:VC2010(%d) on %dbit =="), nBitCode, nBitSys);

	// ��ʼ��
	srand( (unsigned)time( NULL ) );
	for(i=0; i<DATASIZE*4; ++i)
	{
		bufS[i] = (signed short)((rand()&0x1FF) - 128);	// ʹ��ֵ�� [-128, 383] ����
	}

	// ׼����ʼ�����Խ����������ȼ���Ϊʵʱ
	if (argc<=1)
	{
		_tprintf(_T("<Press any key to continue>"));
		_getch();
	}
	_tprintf(_T("\n"));

	// ���в���
	runTest(_T("f0_if"), f0_if);
	runTest(_T("f1_min"), f1_min);
	runTest(_T("f2_neg"), f2_neg);
	runTest(_T("f3_sar"), f3_sar);

	// ����ǰ��ʾ
	if (argc<=1)
	{
		_tprintf(_T("<Press any key to exit>"));
		_getch();
	}
	_tprintf(_T("\n"));

	return 0;
}

