#ifndef _3D_TRACK_H_
#define _3D_TRACK_H_

#include "algo.h"
#include "3dKalman.h"
#include "TriDimModel.h"
#include "..\��ά�궨\calibration.h"


// �궨��ǰ������.
struct Calibration;


// �켣״̬ö����.
typedef enum 
{
	DELETE = 0, // ������ɾ��.
	EMERGE,     // �������.
	NORMAL,     // ����״̬.
	COMBINE,    // �ϲ�״̬.
	SPLIT,      // ����״̬.
	FADING,     // ��ʧ״̬.
}
STATUS;

// Ŀ�����.
typedef enum
{
	PERSON  = 1, 
	CROWD   = 2, 
	VEHICLE = 4, 
	UNKNOW  = 8, 
}
CLASS;

// �ſ���.
class Block
{
public:
	int cy;
	int cx;

	int l;
	int t;
	int r;
	int b;

	Block(int y = 0, int x = 0, int left = 0, int top = 0, int right = 0, int bottom = 0): 
		cy(y), // ����y����.
		cx(x), // ����x����.
		l(left), 
		t(top), 
		r(right), 
		b(bottom)
	{
	}
};

// �켣��.
class TrackTarget
{
public:
	// ���캯��.
	TrackTarget(double length, 
		        double compact, 
				const double *wloc, 
				const int *iloc, 
		        int cy, 
		        int cx, 
		        int left, 
		        int top, 
				int right, 
				int bottom, 
				int size, 
				int name, 
				int h, 
				int w):
	    direction(0.0), 
	    speed(0.0), 
	    len(length), 
		cmt(compact), 
	    rect(cy, cx, left, top, right, bottom), 
		pred(*(wloc + 0), *(wloc + 1)), 
		real(new uchar[h * w * 3]), 
		area(size), 
		s(EMERGE), 
		attri(UNKNOW), 
		score(1), 
		label(name)
	{
		if (NULL == real)
		{
			exit(-1);
		}

		*(ftw + 0) = *(wloc + 0);
		*(ftw + 1) = *(wloc + 1);

		*(fti + 0) = *(iloc + 0);
		*(fti + 1) = *(iloc + 1);

		memset(real, 0, h * w * 3 * sizeof(uchar));
	}

	~TrackTarget()
	{
		delete []real; real = NULL;
	}

public: 
	double direction; // Ŀ��ͼ���ϵ��ƶ��Ƕ�.
	double speed;  // Ŀ��ʵ������.
	double len;    // Ŀ��ʵ�ʸ߶�.
	double cmt;    // Ŀ���ʵ��.
	double ftw[2]; // Ŀ��ײ�����������.
	int fti[2];    // Ŀ��ײ���ͼ������.

	kalman pred; // Ŀ���ٶȺ�λ��Ԥ����Ϣ.

	Block  rect; // Ŀ��2d��Ϣ.
	int    area; // Ŀ�����.
	uchar *real; // Ŀ����ɫ��Ϣ.

	STATUS s; // Ŀ��״̬.
	CLASS attri; // Ŀ�����.

	int score; // Ŀ��״̬�÷�.
	int label; // Ŀ���ʶ.

private:
	// ��ֹʹ�ÿ������캯���͸�ֵ�����.
	const TrackTarget& operator = (const TrackTarget&);
	TrackTarget(const TrackTarget&);
};

// ��ά������.
struct TriDimTracker
{
private:
	// �������سߴ�.
	int m_SceneH;
	int m_SceneW;

	// �����켣��, ��ǰ�켣��, ��һ���¹켣���������ID.
	int m_MaxObjNum;
	int m_CurObjNum;
	int m_CurIDNum;

	// �켣ָ������.
	TrackTarget **m_TargetList;

private:
	// �������й켣�͵�ǰ�����֮��Ĺ�ϵ����.
	void AnalyzeDistMat(const int *map, 
						const uchar *relation, 
						int num, 
						int *norm, 
						int *emerge, 
						int *fade, 
						int *combine, 
						int *split);

	// ����켣��ʧ�����.
	void ProcFade(const int *map, const int *fade, const Calibration *pCbr);

	// �����³��ֵ�Ŀ��.
	int ProcEmerge(const uchar *src, 
				   const int *emerge, 
				   uchar **bin, 
				   const int *rect, 
				   const double *feet, 
				   const int *area, 
				   const int *center, 
				   const int *feeti, 
				   const double *len, 
				   const double *compact, 
				   int num);

	// �������켣��Ӧ������ǰ����ſ��������.
	void ProcOne2One(const uchar *src, 
		             const int *map, 
					 const int *norm, 
					 uchar **bin, 
					 const int *rect, 
					 const double *feet, 
					 const int *area, 
					 const int *center, 
					 const int *feeti, 
					 const double *len, 
					 const double *compact, 
					 const Calibration *pCbr, 
					 int num);

	// ����Ŀ����ѵ����.
	int ProcSplit(const uchar *src, 
		          const int *map, 
		 		  const int *split, 
				  uchar **bin, 
				  const int *rect, 
				  const double *feet, 
				  const int *area, 
				  const int *center, 
				  const int *feeti, 
				  const double *len, 
				  const double *compact, 
				  const Calibration *pCbr, 
				  int num);

	// ������Ŀ��ϲ������.
	void ProcMerge(const uchar *src, 
		           const int *map, 
		 		   const int *combine, 
				   uchar **bin, 
				   const int *rect, 
				   const double *feet, 
				   const int *area, 
				   const int *center, 
				   const int *feeti, 
				   const double *len, 
				   const double *compact, 
				   const Calibration *pCbr, 
				   int num);

	// ���ݵ�ǰ������ſ������й켣���ص���ϵ���ɾ������.
	void DistMatrix(const int *map, uchar **binList, int num, uchar *relation);

	// ��������������������ڴ�.
	void AnalyzeMem(int **norm, int **emerge, int **fade, int **combine, int **split, int num);

	// ��ɫ�ſ���Ϣ�ƶ�.
	void ColorMove(TrackTarget *&pt, int offsetX, int offsetY);

	// �켣�����ж�.
	void TraceClassify(const TriDimModel *pTDM);

	// ��ʼ���켣��Ϣ.
	void InitTrace(int loc, 
		           const int *center, 
				   const int *rect, 
				   const double *feet, 
				   int area, 
				   const int *feeti, 
				   double len, 
				   double compact);

	// ����һ���µ�ID.
	int NewID(void)
	{
		return m_CurIDNum++;
	}

	// �ӹ켣ָ�������л�ȡһ����ָ������.
	int NullPtr(void)
	{
		for (int i = 0; i < m_MaxObjNum; ++i)
		{
			if (NULL == *(m_TargetList + i))
			{
				return i;
			}
		}

		return -1;
	}

	// ����ָ������ӳ���ϵ.
	void TargetMap(int *map)
	{
		assert(NULL != map);

		for (int i = 0, j = 0; i < m_MaxObjNum && j < m_CurObjNum; ++i)
		{
			if (NULL != *(m_TargetList + i))
			{
				*(map + j++) = i;
			}
		}
	}

	// ������ֵͼ�󽻼����ص����.
	int BinImgInter(const uchar *binX, const uchar *binY, int h, int w)
	{
		assert(NULL != binX && NULL != binY && h > 0 && w > 0);

		int sum = 0;

		for (int i = 0; i < h * w; ++i)
		{
			if (0 < *(binX + i) && 0 < *(binY + i)) 
			{
				++sum;
			}
		}

		return sum;
	}

	// ��ȡ��ֵͼ��Ӧλ�õ�RGB��Ϣ.
	void ColorMsg(const uchar *src, const uchar *bin, int h, int w, uchar *rgb)
	{
		assert(NULL != src && NULL != bin && NULL != rgb && h > 0 && w > 0);

		memset(rgb, 0, h * w * 3 * sizeof(uchar));

		for (int i = 0; i < h; ++i)
		{
			for (int j = 0; j < w; ++j)
			{
				if (0 < *(bin + i * w + j))
				{
					*(rgb + i * w * 3 + j * 3 + 0) = *(src + i * w * 3 + j * 3 + 0);
					*(rgb + i * w * 3 + j * 3 + 1) = *(src + i * w * 3 + j * 3 + 1);
					*(rgb + i * w * 3 + j * 3 + 2) = *(src + i * w * 3 + j * 3 + 2);
				}
			}
		}
	}

	// ��Ŀ��RGBͼ���ƶ����µ�λ��.
	void Move(const uchar *src, int h, int w, int offsetX, int offsetY, uchar *dst)
	{
		assert(NULL != src && NULL != dst && h > 0 && w > 0);

		memset(dst, 0, h * w * 3 * sizeof(uchar));

		for (int i = 0; i < h; ++i)
		{
			for (int j = 0; j < w; ++j)
			{
				if (0 < *(src + i * w * 3 + j * 3 + 0) || 
					0 < *(src + i * w * 3 + j * 3 + 1) || 
					0 < *(src + i * w * 3 + j * 3 + 2))
				{
					const int M = i + offsetY;
					const int N = j + offsetX;

					if (-1 < M && M < h && -1 < N && N < w)
					{
						*(dst + M * w * 3 + N * 3 + 0) = *(src + i * w * 3 + j * 3 + 0);
						*(dst + M * w * 3 + N * 3 + 1) = *(src + i * w * 3 + j * 3 + 1);
						*(dst + M * w * 3 + N * 3 + 2) = *(src + i * w * 3 + j * 3 + 2);
					}
				}
			}
		}
	}

	// ����Ŀ���ɫ��Ϣ���ɶ�ֵͼ��Ϣ.
	void Real2Bin(const uchar *real, uchar *bin, int h, int w)
	{
		assert(NULL != real && NULL != bin && h > 0 && w > 0);

		memset(bin, 0, h * w * sizeof(uchar));

		for (int i = 0; i < h * w; ++i)
		{
			if (0 < *(real + i * 3 + 0) || 
				0 < *(real + i * 3 + 1) || 
				0 < *(real + i * 3 + 2))
			{
				*(bin + i) = 1;
			}
		}
	}

	// �ͷŷ����������������ڴ�.
	void FreeAnalyzeMem(int **norm, int **emerge, int **fade, int **combine, int **split)
	{
		assert(NULL != norm && NULL != emerge && NULL != fade && NULL != combine && NULL != split);
		assert(NULL != *norm && NULL != *emerge && NULL != *fade && NULL != *combine && NULL != *split);

		delete []*norm;    *norm    = NULL;
		delete []*emerge;  *emerge  = NULL;
		delete []*fade;    *fade    = NULL;
		delete []*combine; *combine = NULL;
		delete []*split;   *split   = NULL;
	}

	// �����ڳ�����Ե�Ĺ켣.
	int TraceOnBorder(const TrackTarget *&pt, int thrd)
	{
		assert(NULL != pt);

		if (pt->rect.cx > m_SceneW - (thrd + 1) || 
			pt->rect.cy > m_SceneH - (thrd + 1) || 
			pt->rect.cx < thrd || 
			pt->rect.cy < thrd)
		{
			return 1;
		}

		else 
		{
			return 0;
		}
	}

	// ɾ������״̬ΪDELETE�Ĺ켣.
	int DelInvalid(void)
	{
		int num = 0;

		for (int i = 0; i < m_MaxObjNum; ++i)
		{
			TrackTarget *&pt = *(m_TargetList + i);

			if (NULL != pt && DELETE == pt->s)
			{
				delete pt; pt = NULL;
				--num;
			}
		}

		return num;
	}

	// ������һ֡����������͵�ǰ֡������������Ŀ���ڳ��������ϵ��ƶ�����.
	double MoveDirection(int preX, int preY, int curX, int curY);

public:
	TriDimTracker(int h, 
		          int w, 
				  int maxObjNum):
		m_SceneH(h), 
		m_SceneW(w), 
		m_MaxObjNum(maxObjNum), 
		m_CurObjNum(0), 
		m_CurIDNum(0), 
		m_TargetList(new TrackTarget*[maxObjNum])
	{
		if (NULL == m_TargetList)
		{
			exit(-1);
		}

		for (int i = 0; i < maxObjNum; ++i)
		{
			*(m_TargetList + i) = NULL;
		}
	}

	~TriDimTracker()
	{
		for (int i = 0; i < m_MaxObjNum; ++i)
		{
			if (NULL != *(m_TargetList + i))
			{
				delete *(m_TargetList + i);
				*(m_TargetList + i) = NULL;
			}
		}

		delete []m_TargetList; m_TargetList = NULL;
	}

public:
	// ����.
	void MultiTrack(const uchar *src, const unsigned int *block, int num, const Calibration *pCbr, const TriDimModel *pTDM);

	// ��ȡ���켣����.
	int GetMaxObjNum(void) const 
	{
		return m_MaxObjNum;
	}

	// ��ȡ��ǰ�켣����.
	int GetCurObjNum(void) const 
	{
		return m_CurObjNum;
	}

	// ��ȡ��i���켣ָ���ָ��״��, ָ�շ���0, ���򷵻�1.
	int GetTracePtr(int i) const 
	{
		return NULL == *(m_TargetList + i) ? 0 : 1;
	}

	// ��ȡ��i���켣ָ��ָ��Ĺ켣��ID.
	int GetIdentity(int i) const 
	{
		return (*(m_TargetList + i))->label;
	}

	// ��ȡ��i���켣ָ��ָ��Ĺ켣��״̬.
	STATUS GetStatus(int i) const 
	{
		return (*(m_TargetList + i))->s;
	}

	// ��ȡ��i���켣ָ��ָ��Ĺ켣�����, ������Ϣ.
	const Block& GetGeoMsg(int i) const 
	{
		return (*(m_TargetList + i))->rect;
	}

	// ��ȡ��i���켣ָ��ָ��Ĺ켣�����.
	CLASS GetKind(int i) const 
	{
		return (*(m_TargetList + i))->attri;
	}

	// ��ȡ��i���켣ָ��ָ��Ĺ켣�ķ���.
	double GetDirection(int i) const 
	{
		return (*(m_TargetList + i))->direction;
	}

	// ��ȡ��i���켣ָ��ָ��Ĺ켣���ſ����.
	int GetArea(int i) const 
	{
		return (*(m_TargetList + i))->area;
	}

	// ��ȡ��i���켣ָ��ָ��Ĺ켣����ʵ����.
	double GetSpeed(int i) const 
	{
		return (*(m_TargetList + i))->speed;
	}

	// ��ȡ��i���켣ָ��ָ��Ĺ켣����ʵ�߶�.
	double GetRealHeight(int i) const 
	{
		return (*(m_TargetList + i))->len;
	}
};

#endif