

// �������ƣ��ַ���
// ���뻷����Visual C++ 6.0��EasyX 2011���ݰ�
// �����£�2009-2-22
//

#include <time.h>
#include <conio.h>

void test_chrmatrix()
{
	// ���������������
	srand((unsigned) time(NULL));

	// ��ʼ��ͼ��ģʽ
	initgraph(640, 480);

	int x, y;
	char c;

	setfont(16, 8, "Courier");	// ��������

	while(!kbhit())
	{
		for (int i=0; i<479; i++)
		{
			setcolor(GREEN);
			for (int j=0; j<3; j++)
			{
				x = (rand() % 80) * 8;
				y = (rand() % 20) * 24;
				c = (rand() % 26) + 65;
				outtextxy(x, y, c);
			}

			setcolor(0);
			line(0,i,639,i);

			Sleep(10);
			if (kbhit()) break;
		}
	}

	// �ر�ͼ��ģʽ
	closegraph();
}

