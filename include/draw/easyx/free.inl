//1. Ϊ��ʹ������������ȫ��ģ������Ĵ��������˼򻯣�����һ�к����һ�к��Ե��ˡ�ͨ��������������ؿ�������νȫ��ģ������ʵ���Ƕ�ÿ���������ڵļ��������ɫ��ƽ�����������Լ�����ÿ�����Ȩ�أ���
//2. ���ģ������ֻ������ÿ������ϡ��¡����ҹ� 5 ���㡣Ϊ�˻�ò�ͬ��Ч�����������Խ���Χ�˸���һ���������ϡ��¡����ҷ����ϵ�������һ����
//3. �ϸ���˵��ÿ����������ʱ����ʹ�õ���ߺ��ϱߵĵ㣬��ʵ�Ѿ�����ԭ���ĵ��ˣ�����ģ����ĵ㡣����������ģ��������ڵ�������˵���ұߺ��±���ɫҪ��һЩ�������Ҫ��ȷ��ģ�������Խ��� IMAGE ʵ�֣�����Ͳ��ٶ�˵�ˡ�
//4. ��ʱû���ó��õ� Sleep ʵ�֣�����ʹ�õľ�ȷ��ʱ��������¡���ȷ��ʱ��ʵ�֡�

///////////////////////////////////////////////////
// �������ƣ������˶��ĵ�
// ���뻷����Visual C++ 6.0 / 2012��EasyX 2013˪����
// �������ߣ�yangw80 <yw80@qq.com>
// ����޸ģ�2011-5-3
//

#include <conio.h>
#include <math.h>
#include <time.h>
#define AMOUNT 64

// ȫ��ģ������
// ��Ϊ�˼򻯷������ú�����ȥ����Ļ��һ�к����һ�еĴ���
void Blur1(DWORD* pMem)
{
	for(int i = 640; i < 640 * 479; i++)
	{
		pMem[i] = RGB(
			(GetRValue(pMem[i]) + GetRValue(pMem[i - 640]) + GetRValue(pMem[i - 1]) + GetRValue(pMem[i + 1]) + GetRValue(pMem[i + 640])) / 5,
			(GetGValue(pMem[i]) + GetGValue(pMem[i - 640]) + GetGValue(pMem[i - 1]) + GetGValue(pMem[i + 1]) + GetGValue(pMem[i + 640])) / 5,
			(GetBValue(pMem[i]) + GetBValue(pMem[i - 640]) + GetBValue(pMem[i - 1]) + GetBValue(pMem[i + 1]) + GetBValue(pMem[i + 640])) / 5);
	}
}

// ��Ľṹ
struct SPOT
{
	int x, y;
	int targetx, targety;
	int dx, dy;
	COLORREF color;
};

// ��ȷ��ʱ����(���Ծ�ȷ�� 1ms������ ��1ms)
// ��ԭ���� www.easyx.cn ��������ϸ���ͣ�
void HpSleep(int ms)
{
	static clock_t oldclock = clock();		// ��̬��������¼��һ�� tick

	oldclock += ms * CLOCKS_PER_SEC / 1000;	// ���� tick

	if (clock() > oldclock)					// ����Ѿ���ʱ��������ʱ
		oldclock = clock();
	else
		while(clock() < oldclock)			// ��ʱ
			Sleep(1);						// �ͷ� CPU ����Ȩ������ CPU ռ����
}

// ������
void test_free()
{
	// ��ʼ��
	initgraph(640, 480);			// ������ͼ����
	BeginBatchDraw();				// ��������ͼģʽ
	srand((unsigned)time(NULL));	// �����������
	DWORD* pMem = GetImageBuffer();	// ��ȡ�Դ��ַ

	// �������е�
	SPOT spots[AMOUNT];

	// ��ʼ��ÿ����
	for(int i = 0; i < AMOUNT; i++)
	{
		spots[i].x = spots[i].targetx = rand() % 600 + 20;
		spots[i].y = spots[i].targety = rand() % 440 + 20;
		spots[i].dx = rand() % 40 - 20;
		spots[i].dy = (int)sqrt(400 - spots[i].dx * spots[i].dx) * ((rand() % 2) * 2 - 1);
		spots[i].color = HSLtoRGB((float)(rand() % 360), 1.0, 0.5);
	}
	
	while(!kbhit())
	{
		for(int i = 0; i < AMOUNT; i++)
		{
			setcolor(spots[i].color);
			moveto(spots[i].x, spots[i].y);

			spots[i].targetx += spots[i].dx;
			spots[i].targety += spots[i].dy;

			// �ж��Ƿ�Խ�磬�Լ�Խ�紦��
			if (spots[i].targetx <= 0)
			{
				spots[i].dx = rand() % 20;
				spots[i].dy = (int)sqrt(400 - spots[i].dx * spots[i].dx) * ((rand() % 2) * 2 - 1);
			}
			else if (spots[i].targetx >= 639)
			{
				spots[i].dx = - rand() % 20;
				spots[i].dy = (int)sqrt(400 - spots[i].dx * spots[i].dx) * ((rand() % 2) * 2 - 1);
			}
			
			if (spots[i].targety <= 0)
			{
				spots[i].dx = rand() % 40 - 20;
				spots[i].dy = (int)sqrt(400 - spots[i].dx * spots[i].dx);
			}
			else if (spots[i].targety >= 479)
			{
				spots[i].dx = rand() % 40 - 20;
				spots[i].dy = - (int)sqrt(400 - spots[i].dx * spots[i].dx);
			}

			// δԽ��ʱ���� 10% �ĸ��ʸı����з���
			if (rand() % 10 < 1)
			{
				spots[i].dx = rand() % 40 - 20;
				spots[i].dy = (int)sqrt(400 - spots[i].dx * spots[i].dx) * ((rand() % 2) * 2 - 1);
			}

			// �����µ����꣬����
			spots[i].x += (int)((spots[i].targetx - spots[i].x) * 0.1);
			spots[i].y += (int)((spots[i].targety - spots[i].y) * 0.1);
			lineto(spots[i].x, spots[i].y);
		}
		
		// ȫ��ģ������
		Blur1(pMem);
		FlushBatchDraw();

		// ��ʱ
		HpSleep(33);
	}
	
	// ��������˳�
	closegraph();
}