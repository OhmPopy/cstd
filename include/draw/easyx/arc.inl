// �������ƣ�Բ����ɵ�ͼ��
// ���뻷����Visual C++ 6.0��EasyX 2011���ݰ�
// ����޸ģ�2011-3-7
//
#include <conio.h>
#include <math.h>

#define PI	3.1415926536
#define R	200

// ��������
int Round1(double x)
{
	return (int)(x < 0 ? x - 0.5 : x + 0.5);
}

// ������
void test_arc()
{
	// ��������
	int x, y;	// ����
	double a;	// ����

	// ��ͼ������ʼ��
	initgraph(640, 480);	// ��ʼ��ͼ�δ���
	setorigin(320, 240);	// ��������ԭ��
	setcolor(GREEN);		// ���û�ͼ��ɫ

	// ��ͼ
	circle(0, 0, R + 1);
	for (int i=0; i<126; i++)
	{
		if ((i % 21) < 7)
		{
			a = PI * 2 * i / 126;
			x = Round1(cos(a) * R);
			y = Round1(sin(a) * R);
			arc(x, y, PI * 2 / 3 - a, PI * 4 / 3 - a, R, R);
		}
	}

	// ��������˳�
	getch();
	closegraph();
}