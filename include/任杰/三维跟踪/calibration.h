#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_


// ����NewTrack.h��Ϊ�˻��IPoint��FPoint�Ķ���.
#include "E:\work\mushroom2009\��ά����\NewTrack.h"

// ������ͼ���ϵĶ�ά����(x, y), �����Ӧ�ĵ����ϵ���ά(x, y)����.
inline void DimTrans23(const double *he, const int *loc2d, double *loc3d)
{
	double tmp[3];
	const int a[] = {*(loc2d + 0), *(loc2d + 1), 1};

	MatMul(he, a, tmp, 3, 3, 1);

	*(loc3d + 0) = *(tmp + 0) / *(tmp + 2);
	*(loc3d + 1) = *(tmp + 1) / *(tmp + 2);
}

// ������ά(x, y)����, �����Ӧ��ͼ���ϵĶ�ά(x, y)����.
inline void DimTrans32(const double *pTMat, const double *loc3d, int *loc2d)
{
	const double HE[] = 
	{
		*(pTMat + 0), *(pTMat + 2), *(pTMat + 3), 
	    *(pTMat + 4), *(pTMat + 6), *(pTMat + 7), 
	    *(pTMat + 8), *(pTMat +10), *(pTMat +11)
	};
	const double a[] = {*(loc3d + 0), *(loc3d + 1), 1};
	double tmp[3];

	MatMul(HE, a, tmp, 3, 3, 1);

	*(loc2d + 0) = int(*(tmp + 0) / *(tmp + 2));
	*(loc2d + 1) = int(*(tmp + 1) / *(tmp + 2));
}

// �궨����.
int CalibrationX(const IPoint *RefPts,
				 double *MatrixP,
				 double *HLine,
			     double StdHeight,
				 double *CamHeight,
				 FPoint *VertexY,
			     int VerLineNum,
			     int HorLineNum,
			     int Row);

// ����Ŀ��ʵ�ʸ߶�.
double CalibrationValidateHeihgt(double heightCam, 
								 const double *hLine, 
								 const FPoint *vexY, 
								 const IPoint *head, 
								 const IPoint *tail);

// ����Ŀ����ͼ���еĶ�������.
void CalibrationValidateLoc(const double *matP, 
							const double *matT, 
							const IPoint *tail, 
							int height, 
							FPoint *location);

// ��ά�궨��.
struct Calibration
{
public:
	// ��ȡ�궨���״̬.
	int GetStatus(void) const
	{
		return m_Status;
	}

	// ��������任����.
	const double* GetTMat(void) const 
	{
		return m_TMat;
	}

	// ����������任����.
	const double* GetITMat(void) const 
	{
		return m_TInv;
	}

	// ���ݵ���������������������������ʵ�ʾ���.
	double RealDist(const IPoint *begin, const IPoint *end) const 
	{
		double realA[2], realB[2];

		const int tmpA[2] = {begin->x, begin->y};
		const int tmpB[2] = {end->x, end->y};

		DimTrans23(m_TInv, tmpA, realA);
		DimTrans23(m_TInv, tmpB, realB);

		return sqrt((realA[0] - realB[0]) * (realA[0] - realB[0]) + (realA[1] - realB[1]) * (realA[1] - realB[1]));
	}

	// ����Ŀ����ͼ���ϵĶ����͵ײ�������������ʵ�ʸ߶�.
	double ValidHeight(const IPoint *head, const IPoint *tail) const 
	{
		return CalibrationValidateHeihgt(m_CamH, m_HLine, &m_VerPt, head, tail);
	}

	// ����Ŀ���ʵ�ʸ߶Ⱥ���ͼ���ϵĵײ��������Ŀ����ͼ���ϵĵĶ�������.
	void ValidPos(FPoint *pos, const IPoint *tail, int height) const 
	{
		CalibrationValidateLoc(m_TMat, m_TInv, tail, height, pos);
	}

	// ��������Ķ�ֵͼ��������߶�, ��ʵ��, �ײ���������, �ײ�ͼ������.
	void TriDimInfo(const uchar *bin, int h, int w, int area, const int *rect, double *targetH, double *compact, double *feetPosW, int *feetPosI) const;

public:
	Calibration(const IPoint *refPt, double refH, int vLineNum, int hLineNum, int h)
	{
		m_Status = CalibrationX(refPt, m_TMat, m_HLine, refH, &m_CamH, &m_VerPt, vLineNum, hLineNum, h);
		CalTInv();
	}

private:
	// ����������任����.
	void CalTInv(void);

private:
	double m_TMat[12]; // ����任����.
	double m_TInv[9];  // ������ת������.
	double m_HLine[3]; // ˮƽ��.
	double m_CamH;     // ������߶�.
	FPoint m_VerPt;    // ��ֱûӰ��.
	int m_Status;      // �궨״̬.
};

#endif