// �������ƣ���Բ��϶��ɵĳ���ͼ��
// ���뻷����Visual C++ 6.0/2010��EasyX 2011���ݰ�
// �����д��1998-2-21��by yw80@qq.com��QuickBasic �汾��
// ����޸ģ�2011-3-23��by yw80@qq.com
//
#include <conio.h>
#include <math.h>

#define	PI	3.1415926536

// ��������
int round1(double x)
{
	return (int)(x < 0 ? x - 0.5 : x + 0.5);
}

// ������
void test_pixel()
{
	// ��ʼ��
	initgraph(640, 480);		// ������ͼ����
	setorigin(320, 240);		// ����ԭ��Ϊ��Ļ����
	setaspectratio(1, -1);		// ���� y ������Ϊ��

	double r = 58;
	double csin = sin(2 * PI / 200);
	double ccos = cos(2 * PI / 200);
	for(int j = 0; j < 100; j++, r -= 0.9)
	{
		double tsin = sin(2 * PI * j / 100);
		double tcos = cos(2 * PI * j / 100);
		double x = 0;
		double y = r;
		for(int i = 0; i < 200; i++)
		{
			double temp = x;
			x = x * ccos + y * csin;
			y = y * ccos - temp * csin;
			int scrx = round1(x * tcos + y * 4 * tsin);
			int scry = round1(y * 4 * tcos - x * tsin);
			putpixel(scrx, scry, GREEN);
		}

		Sleep(20);
	}

	getch();
	closegraph();
}