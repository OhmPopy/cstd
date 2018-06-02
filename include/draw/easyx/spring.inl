///////////////////////////////////////////////////
// �������ƣ���ѧ������õ��ɹ���һ��С����
// ���뻷����Visual C++ 6.0 / 2010��EasyX 20120404(beta)
// ԭ �� Ʒ��������ǰ�ղص�һ�� javascript �ű���Դ��ַ����
// ��ֲ���ߣ�yangw80 <yw80@qq.com>
// ����޸ģ�2012-4-27
//

#include <math.h>
#include <time.h>
#undef WIDTH
#undef HEIGHT
const	int		WIDTH		= 640;		// ���ڿ�
const	int		HEIGHT		= 480;		// ���ڸ�
const	int		BOXCOUNT	= 10;		// ���������
const	int		BOXSIZE		= 10;		// ����ı߳�
const	double	DELTAT		= 0.01;		// ʱ������
const	double	SEGLEN		= 10;		// һ�����ɵľ�ֹ����
const	double	SPRINGK		= 10;		// ���ɵĵ���ϵ��
const	double	MASS		= 1;		// ����
const	double	GRAVITY		= 50;		// �������ٶ�
const	double	RESISTANCE	= 10;		// ����������ϵ��������=�ٶ�*����ϵ����
const	double	STOPVEL		= 0.1;		// �ٶȵ��ٽ�ֵ�����ڴ�ֵ�����ԣ�
const	double	STOPACC		= 0.1;		// �������ٶȵ��ٽ�ֵ�����ڴ�ֵ�����ԣ�
const	double	BOUNCE		= 0.75;		// �߿�ĵ����������ٶ� = ԭ�ٶ� * BOUNCE��



//////////////////////////////
// ��������
//
int round(double x)
{
	return (int)(x < 0 ? x - 0.5 : x + 0.5);
}



//////////////////////////////
// ��������
//
class FPOINT
{
public:
	double X, Y;

	FPOINT(double x, double y)
	{
		X = x;
		Y = y;
	}
};



//////////////////////////////
// �������
//
class BOX
{
private:
	double oldx, oldy;

public:
	double X, Y;
	double dx, dy;

	// ���캯��
	BOX()
	{
		oldx = X = dx = 0;
		oldy = Y = dy = 0;
	}

	// ���÷����Ƿ���Լ�
	void setvisible(bool visible)
	{
		if (visible)
			rectangle(round(X), round(Y), round(X) + BOXSIZE - 1, round(Y) + BOXSIZE - 1);
	}

	// ���Ʒ���
	void draw()
	{
		rectangle(round(oldx), round(oldy), round(oldx) + BOXSIZE - 1, round(oldy) + BOXSIZE - 1);
		rectangle(round(X),	   round(Y),	round(X) + BOXSIZE - 1,    round(Y) + BOXSIZE - 1);
		oldx = X;
		oldy = Y;
	}
};



//////////////////////////////
// ��������
//
BOX g_boxes[BOXCOUNT];



//////////////////////////////
// ���� g_boxes[i] �� g_boxes[j] ֮�������
//
void springForce(int i, int j, FPOINT* spring)
{
	double dx = (g_boxes[i].X - g_boxes[j].X);
	double dy = (g_boxes[i].Y - g_boxes[j].Y);
	double len = sqrt(dx * dx + dy * dy);

	if (len > SEGLEN)
	{
		double springF = SPRINGK * (len - SEGLEN);
		spring->X += (dx / len) * springF;
		spring->Y += (dy / len) * springF;
	}
}



//////////////////////////////
// ��������
//
void Animate()
{
	// g_boxes[0] ��ʾ���λ�ã������Ʒ���
		
	for (int i = 1 ; i < BOXCOUNT; i++ )
	{
		FPOINT spring(0, 0);

		// ����ÿ��������ǰ�󷽿������
		springForce(i-1, i, &spring);
		if (i < (BOXCOUNT - 1))
			springForce(i+1, i, &spring);

		// ��������
		FPOINT resist(-g_boxes[i].dx * RESISTANCE, -g_boxes[i].dy * RESISTANCE);

		// �����µļ��ٶ�
		FPOINT accel((spring.X + resist.X) / MASS, (spring.Y + resist.Y) / MASS + GRAVITY);

		// �����µ��ٶ�
		g_boxes[i].dx += (DELTAT * accel.X);
		g_boxes[i].dy += (DELTAT * accel.Y);

		// �ӽ���ֹʱʹ�䲻���˶�
		if (fabs(g_boxes[i].dx) < STOPVEL && fabs(g_boxes[i].dy) < STOPVEL &&
			fabs(accel.X) < STOPACC && fabs(accel.Y) < STOPACC)
		{
			g_boxes[i].dx = 0;
			g_boxes[i].dy = 0;
		}

		// �����ƶ�������λ��
		g_boxes[i].X += g_boxes[i].dx;
		g_boxes[i].Y += g_boxes[i].dy;

		// ǽ�ڵķ������컨�岻������
		if (g_boxes[i].X > WIDTH  - BOXSIZE)
		{	g_boxes[i].X =  WIDTH  - BOXSIZE;	if (g_boxes[i].dx > 0)	g_boxes[i].dx = BOUNCE * -g_boxes[i].dx; }
		if (g_boxes[i].Y > HEIGHT - BOXSIZE)
		{	g_boxes[i].Y =  HEIGHT - BOXSIZE;	if (g_boxes[i].dy > 0)	g_boxes[i].dy = BOUNCE * -g_boxes[i].dy; }
		if (g_boxes[i].X < 0)
		{	g_boxes[i].X = 0;					if (g_boxes[i].dx < 0)	g_boxes[i].dx = BOUNCE * -g_boxes[i].dx; }

		// ����λ�û�������
		g_boxes[i].draw();
	}
}



//////////////////////////////
// ��ʱ��
//
void MySleep(int t)
{
	static clock_t oldclock = clock();	// ��̬��������¼��һ�� tick

	t *= CLOCKS_PER_SEC / 1000;			// ������ת��Ϊ tick
	oldclock += t;						// ���� tick

	while(clock() < oldclock)			// ��ʱ
		Sleep(1);						// �ͷ� CPU ����Ȩ������ CPU ռ����
}



//////////////////////////////
// ������
//
void test_spring()
{
	// ��ʼ����ͼ����
	initgraph(WIDTH, HEIGHT);			// ���û�ͼ���ڿ��
	setcolor(GREEN);					// ���û�ͼ��ɫ
	setwritemode(R2_XORPEN);			// ��������ͼģʽ
	BeginBatchDraw();					// ��ʼ������ͼģʽ

	// ��ʼ�����з���
	for (int i = 0; i < BOXCOUNT; i++)
	{
		g_boxes[i].setvisible(i != 0);	// ���˵� 0 �����飬������Ϊ�ɼ�
		g_boxes[i].draw();
	}


	// ������ѭ��

	MOUSEMSG m;							// ���������Ϣ�ı���

	while(true)
	{
		// ���������Ϣ
		while (MouseHit())
		{
			m = GetMouseMsg();			// ��ȡһ�������Ϣ

			switch(m.uMsg)
			{
				case WM_MOUSEMOVE:
					g_boxes[0].X = m.x;	// g_boxes[0] ��ʾ���λ�ã������ƣ�
					g_boxes[0].Y = m.y;
					break;

				case WM_RBUTTONUP:		// ������Ҽ��˳�����
					return;
			}
		}

		// ����һ֡��������ʾ
		Animate();
		FlushBatchDraw();

		// ��ʱ 20 ����
		MySleep(20);
	}
}