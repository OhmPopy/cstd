////////////////////////////////////////////////////////////
// noifCheck.c : �����֤����λ��������֧
// Author: zyl910
// Blog: http://www.cnblogs.com/zyl910
// URL: http://www.cnblogs.com/zyl910/archive/2012/03/12/noifopex1.html
// Version: V1.0
// Updata: 2012-03-12
////////////////////////////////////////////////////////////

#include <Windows.h>
#include <stdio.h>

// ��λ���������ʹ���.������������
#define LIMITSU_FAST(n, bits) ( (n) & -((n) >= 0) | -((n) >= (1<<(bits))) )
#define LIMITSU_SAFE(n, bits) ( (LIMITSU_FAST(n, bits)) & ((1<<(bits)) - 1) )
#define LIMITSU_BYTE(n) ((BYTE)(LIMITSU_FAST(n, 8)))

// ��λ���������ʹ���.�ô�����������������
//#define LIMITSW_FAST(n, bits) ( (n) & ~((signed short)(n) >> 15) | ((signed short)((1<<(bits)) - 1 - (n)) >> 15) )
#define LIMITSW_FAST(n, bits) ( ( (n) | ((signed short)((1<<(bits)) - 1 - (n)) >> 15) ) & ~((signed short)(n) >> 15) )
#define LIMITSW_SAFE(n, bits) ( (LIMITSW_FAST(n, bits)) & ((1<<(bits)) - 1) )
#define LIMITSW_BYTE(n) ((BYTE)(LIMITSW_FAST(n, 8)))

signed short	buf[0x10000];	// ����ֵ���������У���������������Ż�

int main(int argc, char* argv[])
{
	int i;	// ѭ��������32λ��
	signed short n;	// ��ǰ��ֵ
	signed short m;	// ��ʱ����
	BYTE	by0;	// ��if��֧�����ʹ���
	BYTE	by1;	// ��λ���������ʹ���.������������
	BYTE	by2;	// ��λ���������ʹ���.�ô�����������������

	//printf("Hello World!\n");
	printf("== noifCheck ==\n");

	// ��ʼ��buf
	for(i=0; i<0x10000; ++i)
	{
		buf[i] = (signed short)(i - 0x8000);
		//printf("%d\n", buf[i]);
	}

	// ��� ��<0������
	printf("[Test: less0]\n");
	for(i=0; i<0x8100; ++i)	// [-32768, 255]
	//for(i=0x7FFE; i<=0x8002; ++i)	// [-2, 2]
	{
		// ������ֵ
		n = buf[i];

		// ��if��֧�����ʹ���
		m = n;
		if (m < 0) m = 0;
		by0 = (BYTE)m;

		// ��λ���������ʹ���.������������
		by1 = (BYTE)(n & -(n >= 0));
		if (by1 != by0)	printf("[Error] 1.1 neg: [%d] %d!=%d\n", n, by0, by1);	// ��֤

		// ��λ���������ʹ���.�ô�����������������
		by2 = (BYTE)(n & ~((signed short)n >> 15));
		if (by2 != by0)	printf("[Error] 1.2 sar: [%d] %d!=%d\n", n, by0, by2);	// ��֤
	}

	// ��� ��>255������
	printf("[Test: great255]\n");
	for(i=0x8000; i<0x10000; ++i)	// [0, 32767]
	//for(i=0x80FE; i<=0x8102; ++i)	// [254, 258]
	{
		// ������ֵ
		n = buf[i];

		// ��if��֧�����ʹ���
		m = n;
		if (m > 255) m = 255;
		by0 = (BYTE)m;

		// ��λ���������ʹ���.������������
		by1 = (BYTE)(n | -(n >= 256) );
		if (by1 != by0)	printf("[Error] 2.1 neg: [%d] %d!=%d\n", n, by0, by1);	// ��֤

		// ��λ���������ʹ���.�ô�����������������
		by2 = (BYTE)(n | ((signed short)(255-n) >> 15));
		if (by2 != by0)	printf("[Error] 2.2 sar: [%d] %d!=%d\n", n, by0, by2);	// ��֤
	}

	// ��� ���ʹ���
	printf("[Test: saturation]\n");
	for(i=0; i<0x10000; ++i)	// [-32768, 32767]
	//for(i=0x7FFE; i<=0x8102; ++i)	// [-2, 258]
	{
		// ������ֵ
		n = buf[i];

		// ��if��֧�����ʹ���
		m = n;
		if (m < 0) m = 0;
		else if (m > 255) m = 255;
		by0 = (BYTE)m;

		// ��λ���������ʹ���.������������
		by1 = LIMITSU_BYTE(n);
		if (by1 != by0)	printf("[Error] 3.1 neg: [%d] %d!=%d\n", n, by0, by1);	// ��֤

		// ��λ���������ʹ���.������������
		by2 = LIMITSW_BYTE(n);
		if (by2 != by0)	printf("[Error] 3.2 sar: [%d] %d!=%d\n", n, by0, by2);	// ��֤
	}

	return 0;
}

