/////////////////////////////////////////////////////////
// �������ƣ���ͼƬת��Ϊ������Ч��
// ���뻷����Visual C++ 6.0 / 2010��EasyX 20130322(beta)
// ��    �ߣ�krissi <zh@easyx.cn>
// ����޸ģ�2013-4-22
//

#include <conio.h>

// ��ͼƬת��Ϊ������Ч��
// ������
//		pimg: ������� IMAGE ����ָ��
//		tilesize: �����˵ĳߴ�
//		startx: �����˵�ƽ����ʼλ�� x ����
//		starty: �����˵�ƽ����ʼλ�� y ����
void Mosaic(IMAGE *pimg, int tilesize, int startx, int starty)
{
	int	width	= pimg->getwidth();		// ͼ��Ŀ�
	int	height	= pimg->getheight();	// ͼ��ĸ�
	int	redsum;			// ��ɫֵ�ĺ�
	int	greensum;		// ��ɫֵ�ĺ�
	int	bluesum;		// ��ɫֵ�ĺ�
	int	count;			// ÿ��С�����ڵ���������
	int	color;			// ÿ�����ص���ɫ
	int x, y, tx, ty;	// ѭ������

	// ��ȡָ���Դ��ָ��
	DWORD* pMem = GetImageBuffer(pimg);

	// ������Ͻǵ�һ�����������
	startx = (startx % tilesize == 0 ? 0 : startx % tilesize - tilesize);
	starty = (starty % tilesize == 0 ? 0 : starty % tilesize - tilesize);

	// ����ÿһ��С����
	for (y = starty; y < height; y += tilesize)
		for (x = startx; x < width; x += tilesize)
		{
			// ����ۼ�ֵ
			redsum = greensum = bluesum = count = 0;

			// ��С����ĺ졢�̡�����ɫֵ�ĺ�
			for (ty = min(y + tilesize, height) - 1; ty >= max(y, 0); ty--)
				for (tx = min(x + tilesize, width) - 1; tx >= max(x, 0); tx--)
				{
					color = pMem[ty * width + tx];
					redsum += GetRValue(color);
					greensum += GetGValue(color);
					bluesum += GetBValue(color);
					count++;
				}

			// ��졢�̡�����ɫ��ƽ��ֵ
 			redsum /= count;
			greensum /= count;
			bluesum /= count;

			// ����С�����ڵ�ÿ������Ϊƽ����ɫֵ
			color = RGB(redsum, greensum, bluesum);
			for (ty = min(y + tilesize, height) - 1; ty >= max(y, 0); ty--)
				for (tx = min(x + tilesize, width) - 1; tx >= max(x, 0); tx--)
					pMem[ty * width + tx] = color;
		}
}

// ������
void test_mosaic()
{
	// ��ʼ����ͼ����
	initgraph(640, 480);
	// ��ȡͼ��
	IMAGE img;
	loadimage(&img, PICNAME);

	// ��ʾԭʼͼ��
	putimage(0, 0, &img);
	
	// �����ִ��
	getch();
#if 0
	// ��ͼƬת��Ϊ������Ч��
	Mosaic(&img, 10, 0, 0);

	// ��ʾ������ͼ��
	putimage(0, 0, &img);
#else
  IMAGE timg;
  for(int i = 2; i < 200; i++)
  {
    // ��ͼƬת��Ϊ������Ч��
    timg = img;
    Mosaic(&timg, i, 320, 240);
    
    // ��ʾ������ͼ��
    putimage(0, 0, &timg);
    
    Sleep(20);
	}
#endif

	// ������رջ�ͼ����
	getch();
	closegraph();
}