// DibMerger.h: interface for the CDibMerger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIBMERGER_H_INCLUDED_)
#define AFX_DIBMERGER_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDibMerger  
{
public:
	CDibMerger();
	virtual ~CDibMerger();
private:
	int m_iMAXarea;
	//��ͼ�ص���ʼ�ĺ�����λ��
	int left_starX; 
	//��ͼ�ص���ʼ��������λ��
	int left_starY;
    //��ͼ�ص������ĺ�����λ��
	int left_endX;
	//��ͼ�ص�������������λ��
	int left_endY;
	//��ͼ�ص���ʼ�ĺ�����λ��
	int right_starX;
	//��ͼ�ص���ʼ��������λ��
	int right_starY;
	//��ͼ�ص������ĺ�����λ��
	int right_endX;
	//��ͼ�ص�������������λ��
	int right_endY;
};

#endif // !defined(AFX_DIBMERGER_H_INCLUDED_)
