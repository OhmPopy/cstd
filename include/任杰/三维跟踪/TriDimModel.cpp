#include "algo.h"
#include "TriDimModel.h"
#include "..\��ά�궨\calibration.h"



// �������3dģ��.
void SamplePtsOnCircle(double *x, double *y, int num, double r);

void CalCoordinate(const double *pTInv, int m, int n, const double *stdX, const double *stdZ, double *x, double *z, int row = 3, int col = 51);

void TransDim(int index, int h, const double *pTransMat, int *xz3D, int stdHeightLow, int stdHeightMid, int stdHeightHigh, const double *x, const double *z, int row = 3, int col = 51);


// ��Բ��(0, 0), �뾶Ϊr��Բ������ʱ����ȳ�ȡ(num + 1, ��һ���������һ�����غ�)����, ��ȡ�ĵ�һ��������Ϊ(r, 0).
static inline void SamplePtsOnCircle(double *x, double *y, int num, double r)
{
	const double PI = 3.141592653589793;

	assert(NULL != x && NULL != y && num > 0 && r > 0);

	const double ANGLE_STEP = 2 * PI / num;

	// �׵���β���غ�.
	*(x + 0) = *(x + num) = r;
	*(y + 0) = *(y + num) = 0;

	double *px = x + 1;
	double *py = y + 1;

	for (int c = 1; c < num; ++c)
	{
		*px = r * cos(ANGLE_STEP * c);
		*py = r * sin(ANGLE_STEP * c);

		++px;
		++py;
	}
}

// ��������������м���X, Z��������. ����pTransMat��ά��Ϊ(3 * 4), stdX, stdZ, x, y������ͬ��ά��row * col(��ǰʹ��3 * 51).
static inline void CalCoordinate(const double *pTInv, int m, int n, const double *stdX, const double *stdY, double *x, double *y, int row, int col)
{
	assert(NULL != pTInv && NULL != stdX && NULL != stdY && NULL != x && NULL != y && 0 < row && 0 < col);

	const int F[] = {n, m};

	double t[2];

	DimTrans23(pTInv, F, t);

	for (int i = 0; i < row * col; ++i)
	{
		*(x + i) = *(stdX + i) + *(t + 0);
		*(y + i) = *(stdY + i) + *(t + 1);
	}
}

// ��ģ��ĳ��(x, y)���任����ת�������άģ������(x, y). pTransMat��ά��Ϊ(3 * 4), x, uά��ͬΪsize(��ǰʹ��3 * 51)
static inline void TransDim(int index, int h, const double *pTransMat, int *xy3D, int stdHeightLow, int stdHeightMid, int stdHeightHigh, const double *x, const double *y, int row, int col)
{
	assert(-1 < index && -1 < h && NULL != pTransMat && NULL != xy3D && NULL != x && NULL != y && 0 < row && 0 < col);

	double tmpRst[3];

	const int DIMENSION = h < stdHeightLow ? 0 : (h < stdHeightMid ? 1 : 2);
	const double WORLD_SAMPLE[] = {*(x + DIMENSION * col + index), -h, *(y + DIMENSION * col + index), 1};

	MatMul(pTransMat, WORLD_SAMPLE, tmpRst, 3, 4, 1);

	*(xy3D + 0) = int(tmpRst[0] / tmpRst[2]);
	*(xy3D + 1) = int(tmpRst[1] / tmpRst[2]);
}

// ����ֿ�����������������ģ��3D�任���ƽ���ֵͼ��ʾ.
void CreateModel(uchar *binRst,           // �����ֵͼ����(m * n����ֵͼ)ͷָ��.
				 int h,                   // ͼ��߶�.
				 int w,                   // ͼ����.
				 int m,                   // �߶��ϵ��ӿ����.
				 int n,                   // ����ϵ��ӿ����.
				 const double *pTransMat, // 3D�任����.
				 const double *pTInv,     // 3D��任����.
				 int SampOnCircle,        // ��Բ��ƽ����������ĸ���.
				 int rLow,                // ��Բ���뾶.
				 int rMid,                // ����Բ���뾶.
				 int rHigh,               // ͷԲ���뾶.
				 int stdHeightLow,        // �Ȳ��߶�.
				 int stdHeightMid,        // ����߶�.
				 int stdHeightHigh)       // ͷ���߶�.
{
	assert(NULL != binRst && NULL != pTransMat);

	// ��һ������������һ���غ�(+1), Բ��������2�������(*2), ������ͷ3��Բ��(*3).
	const int SAMP_NUM = (SampOnCircle + 1);

	double *x = new double[SAMP_NUM * 3];
	double *y = new double[SAMP_NUM * 3];
	double *p = new double[SAMP_NUM * 3];
	double *q = new double[SAMP_NUM * 3];

	if (NULL == x || NULL == y || NULL == p || NULL == q) exit(-1);

	SamplePtsOnCircle(x + SAMP_NUM * 0, y + SAMP_NUM * 0, SampOnCircle, rLow);  // �Ȳ�Բ��������.
	SamplePtsOnCircle(x + SAMP_NUM * 1, y + SAMP_NUM * 1, SampOnCircle, rMid);  // ����Բ��������.
	SamplePtsOnCircle(x + SAMP_NUM * 2, y + SAMP_NUM * 2, SampOnCircle, rHigh); // ͷ��Բ��������.

	memset(binRst, 0, h * w * m * n * sizeof(uchar));

	int loc[2]; // ��ʱ���ת��Ϊ3Dģʽ֮���(x, y)����.

	for (int r = 0; r < m; ++r)
	{
		const int CUR_ROW_CTR = (h / m) / 2 + (h / m) * r;

		for (int c = 0; c < n; ++c)
		{
			const int CUR_COL_CTR = (w / n) / 2 + (w / n) * c;

			CalCoordinate(pTInv, CUR_ROW_CTR, CUR_COL_CTR, x, y, p, q, 3, SAMP_NUM);

			for (int i = 0; i < SampOnCircle + 1; ++i)
			{
				for (int j = 0; j < stdHeightHigh + 1; ++j)
				{
					TransDim(i, j, pTransMat, loc, stdHeightLow, stdHeightMid, stdHeightHigh, p, q, 3, SAMP_NUM);

					if (-1 < *(loc + 0) && *(loc + 0) < w && -1 < *(loc + 1) && *(loc + 1) < h) // �ڶ�ֵͼ��߽��ڲ�.
					{
						*(binRst + *(loc + 1) * w + *(loc + 0)) = 255;
					}
				}
			}

			binRst += (h * w); // ָ����һ����ֵͼ����ָ��.
		}
	}

	// �ڴ��ͷ�.
	delete []p; p = NULL;
	delete []q; q = NULL;
	delete []x; x = NULL;
	delete []y; y = NULL;
}

// ��������ӿ��е�����ģ�͵ĳߴ�.
void TriDimModel::CalSize()
{
	const uchar *bin = m_Model3D;
	int         *sum = m_ModelSize;

	memset(m_ModelSize, 0, m_HSubNum * m_WSubNum * sizeof(int));

	for (int i = 0; i < m_HSubNum * m_WSubNum; ++i)
	{
		for (int j = 0; j < m_Row * m_Col; ++j)
		{
			if (0 < *(bin + j))
			{
				++(*sum);
			}
		}

		++sum;
		bin += m_Row * m_Col;
	}
}