//////////////////////////////////////////////////////////////////////
// GrayShow.h: interface for the CGrayShow class.
// ����������Ҷȹ��־����㷨ʵ��
// 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAYSHOW_H_INCLUDED_)
#define AFX_GRAYSHOW_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dib.h"

class CGrayShow  
{
public:
	void ComputeMatrix(BYTE** LocalImage, int LocalImageWidth);
	void ComputeFeature(double& FeatureEnergy, double& FeatureEntropy, double& FeatureInertiaQuadrature, double& FeatureCorrelation, double& FeatureLocalCalm, int** pMatrix, int dim);
	//����

	int FilterWindowWidth;	//���������Ĵ�С��ͨ����ͼ�񻮷ֳ����ɸ���������
	long ImageHeight;	//ԭͼ��ĸ߶�
	long ImageWidth;		//ԭͼ��Ŀ��
	int distance;	//���룬���ݲ�ͬ��Ӧ��ѡȡ	
	int GrayLayerNum;	//�Ҷȷּ�
	int** PMatrixRD;	//45�ȷ����ϵĻҶȹ��־���
	int** PMatrixLD;	//135�ȷ����ϵĻҶȹ��־���
	int** PMatrixV;		//90�ȷ����ϵĻҶȹ��־���
	int** PMatrixH;		//0�ȷ����ϵĻҶȹ��־���
	BYTE** ImageArray;	//ԭʼ��ͼ���������飭�Ҷ�

	RGBQUAD** m_tOriPixelArray;//ԭʼλͼ����
	RGBQUAD** m_tResPixelArray;//�ο�λͼ����

	CDib *m_dib;

	//����
	BOOL IsEmpty;
	void LoadImage(CString &FilePathName);
	CGrayShow();
	virtual ~CGrayShow();

};

#endif // !defined(AFX_GRAYSHOW_H_INCLUDED_)
