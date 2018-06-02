#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <time.h>

#include "algo.h"
#include "calibration.h"


// ����: ��ֹ������궨.
// ����: ���÷, �� ��.
// ����: 09.07.08.


// �����������, ���������ȡ����ʱ���ظ���ȡ����(�˴����迼�Ƿ���)
struct Combination
{
	int Begin;  // �������
	int End;    // �յ�����
	int IsUsed; // ʹ�ñ��(0: δ�����ѡ��, 1: �������ѡ��)
};

// �����ſ���Ϣ�ĺ�������.

double HorzMeanPos(const uchar *bin, int h, int w, int y);

void CalPosX(double xa, double ya, double xb, double yb, uchar *mark, int h, int w);

void LowestPt(const uchar *mark, int h, int w, const uchar *bin, int top, int bottom, int *loc);

void sobel(const uchar *src, int h, int w, double *rst);

double CalThrd(const double *edge, int h, int w);

void Edge(const uchar *src, int h, int w, uchar *rst);

double Compactness(const uchar *src, int h, int w, int area);

// �궨��������.

bool Intersection(const IPoint &a, const IPoint &b, const IPoint &c, const IPoint &d, FPoint &X);

FPoint LineCtr(const IPoint &a, const IPoint &b);

double Dist2Line(const FPoint &a, const FPoint &b, const IPoint &SamplePt);

int TotalNum(int SampleNum);

void CoupleFill(Combination *Pair, int PtsNum);

double GetConsensusSetCost(const double *Error, int SampleNum, int &C, double TNoiseSquared = 1000.0);

int Ceil(double Value);

int GetIterRansac(double Epsilon, double Q);

double GetQRansac(int N, int N_I, int K);

int CalcAllCross(const IPoint *SamplePts, int LineNum, FPoint *CrossPts);

void EstimateLine(const FPoint &PtA, const FPoint &PtB, double &a, double &b, double &c);

void Ransac(const FPoint *SamplePts, int SamplePtsNum, double Epsilon, double TNoiseSquared, int MaxIter, int MaxNoUpdate, double &RstA, double &RstB, double &RstC, int K = 2);


// ��ֵͼ�е�Ŀ���ڸ����߶�ˮƽ�����ϵ�����ľ�ֵ.
static inline double HorzMeanPos(const uchar *bin, int h, int w, int y)
{
	double sum = 0.0;
	int c = 0;

	for (int i = 0; i < w; ++i)
	{
		if (*(bin + y * w + i) > 0)
		{
			sum += i;
			++c;
		}
	}

	return sum / c;
}

// ����ֱ���ϵ�������a��b, �������yֵ����[0, h)��Ӧ��xֵ���в�����ͼ���ڲ��ĵ�(x, y)�����ֵͼmark��.
static inline void CalPosX(double xa, double ya, double xb, double yb, uchar *mark, int h, int w)
{
	LineSection<double> line(xa, ya, xb, yb);

	double t;

	memset(mark, 0, h * w * sizeof(uchar));

	for (int i = 0; i < h; ++i)
	{
		line.PosX(i, t);

		if (0 < t && t < w)
		{
			*(mark + i * w + int(t)) = 1;
		}
	}
}

/* ��ֵͼ�е�Ŀ���ڴ�ֱûӰ�㷽���ϵ���͵�.

 * mark:   ����CalPosX()�õ��Ķ�ֵͼ��Ŀ���ֵͼ�󽻼��õ�.
 * bin:    Ŀ���ֵͼͷָ��.
 * top:    ��ֵͼbin�����1ֵ���y����.
 * bottom: ��ֵͼbin�����1ֵ���y����.

 */
static inline void LowestPt(const uchar *mark, int h, int w, const uchar *bin, int top, int bottom, int *loc)
{
	int i, j, c, r = 0;
	bool tag = false;

	for (i = 0; i < h; ++i)
	{
		for (j = 0; j < w; ++j)
		{
			if (0 < *(mark + i * w + j) && i > r)
			{
				tag = true;
				r = i;
				c = j;
			}
		}
	}

	if (!tag || (r - top < 0.5 * (bottom - top + 1)))
	{
		*(loc + 0) = int(HorzMeanPos(bin, h, w, bottom) + 0.5);
		*(loc + 1) = bottom;
	}

	else 
	{
		*(loc + 0) = c;
		*(loc + 1) = r;
	}
}

// ˮƽ, ��ֱ������sobel����, ���������������Ӷ�Ӧ���ƽ����.
static inline void sobel(const uchar *src, int h, int w, double *rst)
{
	int i, j;

	uchar *srcExt = new uchar[(h + 2) * (w + 2)];

	if (NULL == srcExt)
	{
		exit(-1);
	}

	memset(srcExt, 0, (h + 2) * (w + 2) * sizeof(uchar));

	for (i = 0; i < h; ++i)
	{
		for (j = 0; j < w; ++j)
		{
			*(srcExt + (i + 1) * (w + 2) + (j + 1)) = *(src + i * w + j);
		}
	}

	for (i = 0; i < h; ++i)
	{
		for (j = 0; j < w; ++j)
		{
			const int T00 = *(srcExt + (i + 0) * (w + 2) + (j + 0));
			const int T01 = *(srcExt + (i + 0) * (w + 2) + (j + 1));
			const int T02 = *(srcExt + (i + 0) * (w + 2) + (j + 2));
			const int T10 = *(srcExt + (i + 1) * (w + 2) + (j + 0));
			const int T12 = *(srcExt + (i + 1) * (w + 2) + (j + 2));
			const int T20 = *(srcExt + (i + 2) * (w + 2) + (j + 0));
			const int T21 = *(srcExt + (i + 2) * (w + 2) + (j + 1));
			const int T22 = *(srcExt + (i + 2) * (w + 2) + (j + 2));

			const double HORZ = (T20 + (T21 << 1) + T22 - T00 - (T01 << 1) - T02) / 8.0;
			const double VERT = (T02 + (T12 << 1) + T22 - T00 - (T10 << 1) - T20) / 8.0;

			*(rst + i * w + j) = HORZ * HORZ + VERT * VERT;
		}
	}

	delete []srcExt; srcExt = NULL;
}

// ˮƽ, ��ֱ������sobel���ӽ��ƽ����ȷ����ֵ�ķ���, ����������ֵ��ƽ��.
static inline double CalThrd(const double *edge, int h, int w)
{
	double sum = 0.0;

	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			sum += *(edge + i * w + j);
		}
	}

	return 4 * sum / (h * w);
}

// ������ͼ��ı�Ե��ֵͼ.
static inline void Edge(const uchar *src, int h, int w, uchar *rst)
{
	double *rstExt = new double[h * w];

	if (NULL == rstExt) exit(-1);

	sobel(src, h, w, rstExt);

	double thrd = CalThrd(rstExt, h, w);

	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			*(rst + i * w + j) = *(rstExt + i * w + j) > thrd ? 1 : 0;
		}
	}

	delete []rstExt; rstExt = NULL;
}

static inline double Compactness(const uchar *src, int h, int w, int area)
{
	uchar *bin = new uchar[h * w];

	if (NULL == bin)
	{
		exit(-1);
	}

	Edge(src, h, w, bin);

	int sum = 0;

	for (int i = 0; i < h * w; ++i)
	{
		if (0 < *(bin + i)) 
		{
			++sum;
		}
	}

	delete []bin; bin = NULL;

	return area / double(sum * sum);
}

// ��ֱ��AB(a, b)��ֱ��CD(c, d)�Ľ���, �޽���ʱ��������false, ���򷵻�true.
inline bool Intersection(const IPoint &a, const IPoint &b, const IPoint &c, const IPoint &d, FPoint &X)
{
	LineSection<int> AB(a.x, a.y, b.x, b.y);
	return AB.Intersection(c.x, c.y, d.x, d.y, X.x, X.y);
}

// ���߶�AB(a, b)���е�
inline FPoint LineCtr(const IPoint &a, const IPoint &b)
{
	FPoint Center;
	Center.x = (a.x + b.x) / 2.0;
	Center.y = (a.y + b.y) / 2.0;
	return Center;
}

// �󵥸������㵽ֱ��(a, b)�Ĵ�ֱ�����ƽ��
inline double Dist2Line(const FPoint &a, const FPoint &b, const IPoint &SamplePt)
{
	LineSection<double> RefLine(a.x, a.y, b.x, b.y);
	return RefLine.Dist2Point2(SamplePt.x, SamplePt.y);
}

// ������������������п��ܳ��ֵ���������ϸ���
inline int TotalNum(int SampleNum)
{
	return SampleNum * (SampleNum - 1) / 2;
}

// ������������������п��ܳ��ֵ�����������(Pair����ߴ� == PtsNum * (PtsNum - 1) / 2)
inline void CoupleFill(Combination *Pair, int PtsNum)
{
	assert(0 < PtsNum && NULL != Pair);
	Combination *pt = Pair;

	for (int i = 0; i < PtsNum; ++i)
	{
		for (int j = i + 1; j < PtsNum; ++j)
		{
			pt->Begin  = i;
			pt->End    = j;
			pt->IsUsed = 0;

			++pt;
		}
	}
}

// ��matlab������RANSAC�е�J, ErrorΪ���������㵽ֱ�ߵľ����ƽ��, ��Ӧ���к���TNoiseSquared == 1000
inline double GetConsensusSetCost(const double *Error, int SampleNum, int &C, double TNoiseSquared)
{
	assert(NULL != Error && 0 < SampleNum);
	double *RHO = new double[SampleNum];

	if (NULL == RHO)
	{
		printf("Error: memory allocation failed!\n");
		exit(-1);
	}

	int i = 0;

	for (C = 0; i < SampleNum; ++i)
	{
		const double TMP_ERROR = *(Error + i);

		if (TMP_ERROR < TNoiseSquared)
		{
			*(RHO + i) = TMP_ERROR;
			++C;
		}
		else 
		{
			*(RHO + i) = TNoiseSquared;
		}
	}

	const double SUM = Sum(RHO, RHO + SampleNum, 0.0);

	delete []RHO; RHO = NULL;

	return (SUM / C);
}

// �󸡵�����ֵ��Ceil.
inline int Ceil(double Value)
{
	const int VALUE_FLOOR = (int)Value;
	return Value - VALUE_FLOOR < 1e-99 ? VALUE_FLOOR : 1 + VALUE_FLOOR;
}

// ��matlab�����е�T_iter.
inline int GetIterRansac(double Epsilon, double Q)
{
	return 1.0 - Q > 1e-12 ? Ceil(log(Epsilon) / log(1 - Q)): 0;
}

// ��matlab������[q num den]�ĺ���.
inline double GetQRansac(int N, int N_I, int K)
{
	assert(K <= N_I);
	if (N == N_I)
	{
		return 1.0;
	}
	else 
	{
		double Q = 1.0;
		int Num = N_I;
		int Den = N;

		for ( ; Den > N - K; )
		{
			Q *= Num / (double)Den;

			--Den;
			--Num;
		}

		return Q;
	}
}

// ��N����ֱƽ���ߵ���������, ���˳��Ϊ: ����, �ŵ�(һ��ֱ��), ����, �ŵ�(һ��ֱ��).
inline int CalcAllCross(const IPoint *SamplePts, int LineNum, FPoint *CrossPts)
{
	assert(NULL != SamplePts && NULL != CrossPts && 0 < LineNum);

	int i, j;
	int c = 0;

	for (i = 0; i < LineNum; ++i)
	{
		for (j = i + 1; j < LineNum; ++j)
		{
			LineSection<int> TmpLine((SamplePts + (i << 1) + 0)->x, 
				              (SamplePts + (i << 1) + 0)->y, 
				              (SamplePts + (i << 1) + 1)->x, 
							  (SamplePts + (i << 1) + 1)->y
							  );

			if (TmpLine.Intersection((SamplePts + (j << 1) + 0)->x, 
				                     (SamplePts + (j << 1) + 0)->y, 
				                     (SamplePts + (j << 1) + 1)->x, 
									 (SamplePts + (j << 1) + 1)->y, 
								     (CrossPts + c)->x, 
									 (CrossPts + c)->y)
									 )
			{
				++c;
			}
		}
	}

	return c;
}

// RANSAC�е�ֱ�߷��̹���
inline void EstimateLine(const FPoint &PtA, const FPoint &PtB, double &a, double &b, double &c)
{
	const double AVG_X = (PtA.x + PtB.x) / 2.0;
	const double AVG_Y = (PtA.y + PtB.y) / 2.0;

	FPoint PtAA, PtBB;

	PtAA.x = PtA.x - AVG_X;
	PtAA.y = PtA.y - AVG_Y;

	PtBB.x = PtB.x - AVG_X;
	PtBB.y = PtB.y - AVG_Y;

	const double A = PtAA.x * PtAA.x + PtBB.x * PtBB.x;
	const double B = PtAA.x * PtAA.y + PtBB.x * PtBB.y;
	const double C = PtAA.y * PtAA.y + PtBB.y * PtBB.y;

	const double PHI = atan2(0.5 * ((A + C) + sqrt((A - C) * (A - C) + 4 * B * B)) - A, B);

	a = -sin(PHI);
	b =  cos(PHI);
	c = -a * AVG_X - b * AVG_Y;
}

const double EPS = 2.220446049250313e-16;

// RANSAC
inline void Ransac(const  FPoint *SamplePts, // ˮƽ���ϵ�����������
				   int    SamplePtsNum,      // ˮƽ���ϵ����������
				   double Epsilon,
				   double TNoiseSquared, 
				   int    MaxIter, 
				   int    MaxNoUpdate, 
				   double &RstA, 
				   double &RstB, 
				   double &RstC, 
				   int    K)
{
	assert(NULL != SamplePts && 0 < SamplePtsNum);
	// ������ʼ��
	double J_Start   = 1e99;
	int N_I_Start    = K;
	int NoUpdate     = 0;
	int T_Iter       = 0;
	int Iter         = 0;
	int N_I          = 0; // �ڵ����
	bool UpdateSet   = false;
	bool UpdateTIter = false;

	// �����ȡ׼������
	const int MAX_PAIR    = TotalNum(SamplePtsNum);
	int CurPair = MAX_PAIR;

	double a, b, c;
	double *Dists         = new double[SamplePtsNum];
	Combination *PairList = new Combination[MAX_PAIR];

	if (NULL == PairList || NULL == Dists)
	{
		printf("Error: memory allocation failed!\n");
		exit(-1);
	}

	CoupleFill(PairList, SamplePtsNum);
	srand(time(NULL));

	// ��ѭ��������ˮƽ��
	while ((CurPair > 0 && Iter <= MaxIter) || (CurPair > 0 && Iter <= T_Iter && NoUpdate <= MaxNoUpdate))
	{
		// ״̬��������
		++Iter;
		UpdateSet = false;
		UpdateTIter = false;
		// �����ȡһ��δ����ȡ������������Ƿ�������Ϊˮƽ�ߵ�����
		int Index = rand() % MAX_PAIR;
		while (1 == (PairList + Index)->IsUsed) Index = rand() % MAX_PAIR;
		(PairList + Index)->IsUsed = 1;
		--CurPair;

		// �������������㵽ֱ��(SamplePts[BEGIN], SamplePts[END])�ľ���ƽ��
		const int BEGIN = (PairList + Index)->Begin;
		const int END   = (PairList + Index)->End;

		EstimateLine(SamplePts[BEGIN], SamplePts[END], a, b, c);

		const double DEN = a * a + b * b;

		for (int i = 0; i < SamplePtsNum; ++i)
		{
			const double TMP_VALUE = (a * SamplePts[i].x + b * SamplePts[i].y + c);
			*(Dists + i) = TMP_VALUE * TMP_VALUE / DEN;
		}

		// ����J, ͬʱͳ���ڵ����, ͬʱ�����ݴ˱仯
		const double J = GetConsensusSetCost(Dists, SamplePtsNum, N_I, TNoiseSquared);

		if ((N_I >= K && J < J_Start) || (J == J_Start && N_I > N_I_Start))
		{
			J_Start = J;
			UpdateSet = true;

			if (N_I > N_I_Start)
			{
				UpdateTIter = true;
			}
		}

		// ��������
		if (UpdateSet)
		{
			N_I_Start = N_I;
			NoUpdate = 0;

			RstA = a;
			RstB = b;
			RstC = c;
		}
		else 
		{
			++NoUpdate;
		}

		if (UpdateTIter)
		{
			const double Q = GetQRansac(SamplePtsNum, N_I_Start, K);

			if (Q > EPS)
			{
				T_Iter = GetIterRansac(Epsilon, Q);
			}
		}

		// �ж��Ƿ��Ѿ��õ���ȷ���
		if (SamplePtsNum == N_I)
		{
			break;
		}
	}

	delete []Dists; Dists = NULL;
	delete []PairList; PairList = NULL;
}

/************************************************************
*                     �궨��������֤����                    *
************************************************************/

// �궨����
int CalibrationX(const IPoint *RefPts,    // �궨�߶ζ˵���������(VerLineNum����ֱ�߶� + HorLineNum��ˮƽ�߶�)
			 	 double       *MatrixP,   // 3 X 4�ľ���ͷָ��
				 double       *HLine,     // 3 X 1�ľ���ͷָ��
			     double       StdHeight,  // ��ֱ������ĸ߶�
				 double       *CamHeight, // ��Ӱ���߶�
				 FPoint       *VertexY,   // ��ֱûӰ������
			     int          VerLineNum, // ��ֱ�߶�����
			     int          HorLineNum, // ˮƽ�߶�����(��ɶ�, ��Ϊż��)
			     int          Row         // ͼ��߶�
			    )
{
	assert(NULL != RefPts);

	const int VerLineNumX = VerLineNum;

	const int VER_LINE_CROSS_NUM = TotalNum(VerLineNum);
	const int HOR_LINE_CROSS_NUM = TotalNum(VerLineNumX) + (HorLineNum >> 1);

	// ���VerLineNum����ֱ�߶ε��е�
	FPoint *VerLineCenter = new FPoint[VerLineNum];

	// ���VerLineNum����ֱ�߶εĽ���
	FPoint *VerLineCross = new FPoint[VER_LINE_CROSS_NUM];

	// ��Ÿ���ˮƽ�ߵĽ���
	FPoint *HorLineCross = new FPoint[HOR_LINE_CROSS_NUM];

	// ���VerLineNum����ֱ�߶�����֮���γɵ�ˮƽ�߶κ�HorLineNum�����е�ˮƽ�߶�
	IPoint *AllHorLine = new IPoint[HOR_LINE_CROSS_NUM << 2];

	if (NULL == VerLineCross || NULL == AllHorLine || NULL == HorLineCross || NULL == VerLineCenter)
	{
		printf("Error: memory allocation failed!\n");
		return -1;
	}

	int i, j, k;

	// ����VerLineNum����ֱ�߶ε��е�
	for (i = 0; i < VerLineNum; ++i)
	{
		*(VerLineCenter + i) = LineCtr(*(RefPts + (i << 1)), *(RefPts + (i << 1) + 1));
	}

	// ����VerLineNum����ֱ�߶ε����н���
	int Num = CalcAllCross(RefPts, VerLineNum, VerLineCross);

	if (Num != VER_LINE_CROSS_NUM)
	{
		return -2;
	}

	// ��VER_LINE_CROSS_NUM����ֱֱ�߽���ѡ����Ѵ�ֱûӰ��
	double MinError = 9e99; // ��С���
	double CurError;        // ��ǰ���
	int MinErrorIndex = -1; // ��С��������

	for (i = 0; i < VER_LINE_CROSS_NUM; ++i)
	{
		// ���������Ľ����y����С��0, ���Ըý���.
		if ((VerLineCross + i)->y < 0.0)
		{
			continue;
		}

		CurError = 0.0;

		for (j = 0; j < VerLineNum; ++j)
		{
			LineSection<double> TmpLine((VerLineCross + i)->x, (VerLineCross + i)->y, (VerLineCenter + j)->x, (VerLineCenter + j)->y);

			CurError += TmpLine.Dist2Point2((RefPts + (j << 1) + 0)->x, (RefPts + (j << 1) + 0)->y);
			CurError += TmpLine.Dist2Point2((RefPts + (j << 1) + 1)->x, (RefPts + (j << 1) + 1)->y);
		}

		if (CurError < MinError)
		{
			MinError = CurError;
			MinErrorIndex = i;
		}
	}

	// ���еĽ��㶼��Ϊy����С��0��������.
	if (MinErrorIndex == -1)
	{
		return -3;
	}

	*VertexY = *(VerLineCross + MinErrorIndex);

	// ���AllHorLine�ռ�
	k = 0;

	for (i = 0; i < VerLineNumX; ++i)
	{
		for (j = i + 1; j < VerLineNumX; ++j)
		{
			// ÿ��ѭ������4����, ���Եõ�һ������
			*(AllHorLine + (k << 1) + 0) = *(RefPts + (i << 1) + 0);
			*(AllHorLine + (k << 1) + 1) = *(RefPts + (j << 1) + 0);
			++k;
			*(AllHorLine + (k << 1) + 0) = *(RefPts + (i << 1) + 1);
			*(AllHorLine + (k << 1) + 1) = *(RefPts + (j << 1) + 1);
			++k;
		}
	}

	memcpy((void*)(AllHorLine + (k << 1)), (void*)(RefPts + (VerLineNum << 1)), HorLineNum * 2 * sizeof(IPoint)); // ԭˮƽ��

	assert((k + HorLineNum) == (HOR_LINE_CROSS_NUM << 1));

	// �������ˮƽ�ߵĽ���
	Num = 0;

	for (i = 0; i < HOR_LINE_CROSS_NUM; ++i)
	{
		LineSection<int> TmpLine((AllHorLine + (i << 2)    )->x, 
			                     (AllHorLine + (i << 2)    )->y, 
						         (AllHorLine + (i << 2) + 1)->x, 
						         (AllHorLine + (i << 2) + 1)->y
						        );

		if (TmpLine.Intersection((AllHorLine + (i << 2) + 2)->x, 
			                     (AllHorLine + (i << 2) + 2)->y, 
			                     (AllHorLine + (i << 2) + 3)->x, 
								 (AllHorLine + (i << 2) + 3)->y, 
							     (HorLineCross + Num)->x, 
								 (HorLineCross + Num)->y)
								 )
		{
			++Num;
		}
	}

	if (HOR_LINE_CROSS_NUM != Num)
	{
		return -4;
	}

	double RstA, RstB, RstC;

	Ransac(HorLineCross, HOR_LINE_CROSS_NUM, 1e-6, 1000.0, 50, 1000, RstA, RstB, RstC);

	HLine[0] = RstA / -RstB;
	HLine[1] = RstC / -RstB;
	HLine[2] = 1.0;

	// ����������ĸ߶�
	double *Hc = new double[VerLineNum];

	if (NULL == Hc)
	{
		printf("Error: memory allocation failed!\n");
		exit(-1);
	}

	LineSection<double> HLineX(0.0, HLine[1], -HLine[1] / HLine[0], 0.0);

	for (i = 0; i < VerLineNumX; ++i)
	{
		FPoint CrossX, CrossY;

		HLineX.Intersection((RefPts + (i << 1) + 0)->x, 
			                (RefPts + (i << 1) + 0)->y, 
			                (RefPts + (i << 1) + 1)->x, 
							(RefPts + (i << 1) + 1)->y, 
							CrossX.x, 
							CrossX.y
							);

		HLineX.Intersection((RefPts + (i << 1) + 0)->x, 
			                (RefPts + (i << 1) + 0)->y, 
			                VertexY->x, 
							VertexY->y,
							CrossY.x, 
							CrossY.y
							);

		const double X = (CrossX.x + CrossY.x) / 2.0;
		const double Y = HLine[0] * X + HLine[1];

		const double CD = sqrt(((RefPts + (i << 1) + 0)->x - X) * ((RefPts + (i << 1) + 0)->x - X) + 
			                   ((RefPts + (i << 1) + 0)->y - Y) * ((RefPts + (i << 1) + 0)->y - Y));

		const double BY = sqrt(((RefPts + (i << 1) + 1)->x - VertexY->x) * ((RefPts + (i << 1) + 1)->x - VertexY->x) + 
			                   ((RefPts + (i << 1) + 1)->y - VertexY->y) * ((RefPts + (i << 1) + 1)->y - VertexY->y));

		const double BD = sqrt(((RefPts + (i << 1) + 1)->x - X) * ((RefPts + (i << 1) + 1)->x - X) + 
			                   ((RefPts + (i << 1) + 1)->y - Y) * ((RefPts + (i << 1) + 1)->y - Y));

		const double CY = sqrt(((RefPts + (i << 1) + 0)->x - VertexY->x) * ((RefPts + (i << 1) + 0)->x - VertexY->x) + 
			                   ((RefPts + (i << 1) + 0)->y - VertexY->y) * ((RefPts + (i << 1) + 0)->y - VertexY->y));

		Hc[i] = StdHeight / (1.0 - (CD * BY) / (BD * CY));
	}

	QuickSort(Hc, Hc + VerLineNum - 1);

	*CamHeight = (1 == (VerLineNum & 0x00000001)) ? *(Hc + VerLineNum / 2) : (*(Hc + VerLineNum / 2) + *(Hc + VerLineNum / 2 - 1)) / 2.0;

	delete []Hc; Hc = NULL;

	// ��������
	FPoint Z, Z1, Y1, P;

	if (0 == HLine[0])
	{
		Z.x = VertexY->x;
		Z.y = HLine[1];
	}
	else 
	{
		const double K = -1.0 / HLine[0];
		const double B = VertexY->y - K * VertexY->x;
		HLineX.Intersection(0.0, B, -B / K, 0.0, Z.x, Z.y);
	}

	const double R = -atan(HLine[0]);

	Z1.x = Z.x * cos(R) - Z.y * sin(R);
	Z1.y = Z.x * sin(R) + Z.y * cos(R);
	Y1.x = VertexY->x * cos(R) - VertexY->y * sin(R);
	Y1.y = VertexY->x * sin(R) + VertexY->y * cos(R);
	P.x = Y1.x;
	P.y = Row / 2.0;

	const double TMP_F = P.y * (Y1.y - P.y) - Z1.y;

	if (0 > TMP_F)
	{
		return -5;
	}

	const double F = sqrt(TMP_F);
	const double BETA = atan((P.y - Z1.y) / F);
	const double AFA = 0.0;

	// ��������(����MatrixP)
	const double U = P.x * cos(-R) - P.y * sin(-R);
	const double V = P.x * sin(-R) + P.y * cos(-R);

	double a1 = sin(AFA), a2 = sin(BETA), a3 = sin(-R);
	double d1 = cos(AFA), d2 = cos(BETA), d3 = cos(-R);

	double MatrixR[] = 
	{
		d1 * d3 + a1 * a2 * a3, -d2 * a3, -a1 * d3 + d1 * a2 * a3, 
	    d1 * a3 - a1 * a2 * d3,  d2 * d3, -a1 * a3 - d1 * a2 * d3, 
	    a1 * d2               , a2      ,                 d1 * d2
	};

	double TT[] = {0.0, *CamHeight, 0.0};

	double N[] = 
	{
		F, 0, U, 
		0, F, V, 
		0, 0, 1
	};

	double T[3];

	MatMul(MatrixR, TT, T, 3, 3, 1);

	double M[] = 
	{
		d1 * d3 + a1 * a2 * a3, -d2 * a3, -a1 * d3 + d1 * a2 * a3, T[0], 
	    d1 * a3 - a1 * a2 * d3,  d2 * d3, -a1 * a3 - d1 * a2 * d3, T[1], 
	    a1 * d2               , a2      ,                 d1 * d2, T[2]
	};

	MatMul(N, M, MatrixP, 3, 3, 4);

	// �ͷŶ�̬������ڴ�ռ�
	delete []AllHorLine;    AllHorLine    = NULL;
	delete []HorLineCross;  HorLineCross  = NULL;
	delete []VerLineCross;  VerLineCross  = NULL;
	delete []VerLineCenter; VerLineCenter = NULL;

	return 0;
}

// �궨��ȷ�Բ���, ��������Ŀ��ͷ, ������, ������ʵ�ʸ߶�.
double CalibrationValidateHeihgt(double heightCam, 
								 const double *hLine, 
								 const FPoint *vexY, 
								 const IPoint *head, 
								 const IPoint *tail)
{
	FPoint crossA, crossB;

	LineSection<double> hLineX(0.0, hLine[1], -hLine[1] / hLine[0], 0.0);

	hLineX.Intersection(head->x, head->y, tail->x, tail->y, crossA.x, crossA.y);

	hLineX.Intersection((double)(head->x), (double)(head->y), vexY->x, vexY->y, crossB.x, crossB.y);

	const double X = (crossA.x + crossB.x) / 2;
	const double Y = hLine[0] * X + hLine[1];

	const double CD = sqrt((head->x - X) * (head->x - X) + (head->y - Y) * (head->y - Y));

	const double BY = sqrt((tail->x - vexY->x) * (tail->x - vexY->x) + (tail->y - vexY->y) * (tail->y - vexY->y));

	const double BD = sqrt((tail->x - X) * (tail->x - X) + (tail->y - Y) * (tail->y - Y));

	const double CY = sqrt((head->x - vexY->x) * (head->x - vexY->x) + (head->y - vexY->y) * (head->y - vexY->y));

	return heightCam * (1 - (CD * BY) / (BD * CY));
}

// �궨��ȷ�Բ���, ��������Ŀ��ʵ�ʸ߶Ⱥͽ�����, ��������ͼ���ϵ�ͷ����location.
void CalibrationValidateLoc(const double *matP, 
							const double *matT, 
							const IPoint *tail, 
							int height, 
							FPoint *location)
{
	const double BX[] = {tail->x, tail->y, 1.0};

	double oo[3], xx[3];

	MatMul(matT, BX, xx, 3, 3, 1);

	xx[0] /= xx[2];
	xx[1] /= xx[2];
	xx[2]  = 1.0;

	const double CC[] = {xx[0], -height, xx[1], 1.0};

	MatMul(matP, CC, oo, 3, 4, 1);

	oo[0] /= oo[2];
	oo[1] /= oo[2];
	oo[2]  = 1.0;

	location->x = oo[0];
	location->y = oo[1];
}

// �궨��Calibration��Ա����.

void Calibration::CalTInv(void)
{
	const double HE[] = 
	{
		*(m_TMat + 0), *(m_TMat + 2), *(m_TMat + 3), 
	    *(m_TMat + 4), *(m_TMat + 6), *(m_TMat + 7), 
	    *(m_TMat + 8), *(m_TMat +10), *(m_TMat +11)
	};

	MAT_INV33(HE, m_TInv);
}

void Calibration::TriDimInfo(const uchar *bin, 
							 int h, 
							 int w, 
							 int area, 
							 const int *rect, 
							 double *targetH, 
							 double *compact, 
							 double *feetPosW, 
							 int *feetPosI) const 
{
	// Ŀ�궥��ƽ������.
	double head[] = {HorzMeanPos(bin, h, w, *(rect + 1)), *(rect + 1)};

	// ���ش�ֱûӰ�㷽����͵ĵ�.
	uchar *mark = new uchar[h * w];

	if (NULL == mark)
	{
		exit(-1);
	}

	CalPosX(*(head + 0), *(head + 1), m_VerPt.x, m_VerPt.y, mark, h, w);

	for (int i = 0; i < h * w; ++i)
	{
		*(mark + i) &= *(bin + i);
	}

	LowestPt(mark, h, w, bin, *(rect + 1), *(rect + 3), feetPosI);

	delete []mark; mark = NULL;

	// ����Ŀ��3Dλ��.
	DimTrans23(m_TInv, feetPosI, feetPosW);

	const IPoint headObj = {int(*(head + 0)), *(rect + 1)};
	const IPoint tailObj = {*(feetPosI + 0), *(feetPosI + 1)};

	*targetH = ValidHeight(&headObj, &tailObj);

	// �����ʵ��.
	*compact = Compactness(bin, h, w, area);
}