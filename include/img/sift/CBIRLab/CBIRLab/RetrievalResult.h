#pragma once
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <vector>

using namespace std;

class RegionQuad
{
public:
	//�ı��ε���ʱ���ĸ������꣬�ֱ��Ӧ���ϣ����£����£�����
	CvPoint points[4];
	CvPoint rectTopLeft, rectBottomRight;

public:
	void transformToRect(int width, int height);
};

class CRetrievalResult
{
public:
	CRetrievalResult(void);
	~CRetrievalResult(void);
	bool initialize(CString path);

public:
	//ͼ��·��
	CString imagePath;
	//�����������
	int regionCount;
	//ͼ�����򼯺�
	vector<RegionQuad> vpRegions;
};
