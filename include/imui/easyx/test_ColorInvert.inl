/////////////////////////////////////////////////////////
// �������ƣ���ƬЧ��
// ���뻷����Visual C++ 6.0 / 2010��EasyX 20130114(beta)
// ��    �ߣ�krissi <zh@easyx.cn>
// ����޸ģ�2013-1-20
//
#include <graphics.h>
#include <conio.h>

// ��ƬЧ��
void ColorInvert(IMAGE *pimg)
{	
	// ��ȡָ���Դ��ָ��
	DWORD* pMem = GetImageBuffer(pimg);

	// ֱ�Ӷ��Դ渳ֵ
	for(int i = pimg->getwidth() * pimg->getheight() - 1; i >= 0; i--)
		pMem[i] = (~pMem[i]) & 0x00FFFFFF;
}

// ������
void main()
{
	// ��ʼ����ͼ����
	initgraph(640, 480);

	// ��ȡͼ��
	IMAGE img;
	loadimage(&img, _T("c:\\test.jpg"));

	// ��ʾԭʼͼ��
	putimage(0, 0, &img);
	
	// �����ִ��
	getch();

	// ��ƬЧ��
	ColorInvert(&img);

	// ��ʾ������ͼ��
	putimage(0, 0, &img);

	// ������رջ�ͼ����
	getch();
	closegraph();
}