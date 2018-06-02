#ifndef _GMMODEL_H_
#define _GMMODEL_H_

#include <stdlib.h>

// ��˹������ģ��
struct GMM {
public:
	// ������ģ����, ��������ģ��ʱÿ֡ͼƬ����һ�θú���.
	void UpdateModel(const unsigned char* src, int step, int num);

	// ��������ģȨ��, ��ֵ, �������������ı��ĵ�.
	void Export2File(const char* wf, const char* mf, const char* vf) const;

	// ǰ�����ͺ��ڴ���, ���ؼ��������.
	int Detect(const unsigned char* src, int step, const unsigned char *pmask = NULL);

	// ��ǰ����ͼ��.
	const double* Background(void) const {return b;}

	// ��ͨ������.
	const unsigned int* BlobRst(void) const {return blob;}

	// �����߶�.
	int SceneH(void) const {return row;}

	// �������.
	int SceneW(void) const {return col;}

public:

	GMM(double Afa  = 0.005, 
		double Thd  = 0.4, 
		double VirV = 5.0, 
		double VirW = 0.05, 

		double shadowp0 = 0.95, 
		double shadowp1 = 0.3, 
		double shadowp2 = 0.3, 
		double shadowp3 = 0.1, 

		int R   = 120, 
		int C   = 160, 
		int Num = 3, 
		int MinPts = 20, 

		bool IsRectLabel   = true, 
		bool IsRectShadowX = true, 
		bool IsRectShadowY = false, 
		bool IsRectShape   = true
		): 
	afa  (Afa), 
	thrd (Thd), 
	vir_v(VirV), 
	vir_w(VirW), 

	row (R), 
	col (C), 
	kind(Num), 
	min_pts(MinPts), 

	s(new int[row * col]),
	w(new double[row * col * kind]), 
	m(new double[row * col * kind * 3]), 
	v(new double[row * col * kind]), 
	b(new double[row * col * 3]), 

	blob(new unsigned int[col * row]), 
	label(new unsigned char[row * col]), 

	rlabel  (IsRectLabel), 
	rshadowx(IsRectShadowX), 
	rshadowy(IsRectShadowY), 
	rshape  (IsRectShape)
	{
		*(shadowp + 0) = shadowp0;
		*(shadowp + 1) = shadowp1;
		*(shadowp + 2) = shadowp2;
		*(shadowp + 3) = shadowp3;

		for (int i = 0; i < row * col * kind; ++i) 
		{
			*(w + i) = 0.0;
			*(v + i) = 0.0;
		}

		for (i = 0; i < row * col * kind * 3; ++i) 
		{
			*(m + i) = 0.0;
		}

		for (i = 0; i < row * col * 3; ++i) 
		{
			*(b + i) = 0.0;
		}

		for (i = 0; i < row * col; ++i) 
		{
			*(blob + i) = 0;
		}

		SetS();
	}

	~GMM() 
	{
		delete []label; label = NULL;
		delete [] blob; blob  = NULL;

		delete []b; b = NULL;
		delete []v; w = NULL;
		delete []m; m = NULL;
		delete []w; v = NULL;
		delete []s; s = NULL;
	}


private:
	double afa;
	double thrd;
	double vir_v;
	double vir_w;
	double shadowp[4];

	int row;
	int col;
	int kind; 

	unsigned int min_pts;

	int *s;
	double *w, *m, *v, *b;

	unsigned int *blob;
	unsigned char *label;

	bool rlabel, rshadowx, rshadowy, rshape;

private:
	// ���ÿ������캯���͸�ֵ�����.
	GMM(const GMM&);
	const GMM& operator = (const GMM&);

	// ��ʼ������㵱ǰ���е��������ÿһ�����½�������ģ�Ͷ�������ô˺���.
	void SetS(void) 
	{
		for (int i = 0; i < row * col; ++i) 
		{
			*(s + i) = 1; 
		}
	}

	// ���������ܱ����ĺ���, ÿ�μ��ǰ�������.
	void GetIb(void);

	// ǰ�����.
	void DetectForeGround(const unsigned char* pt, int step, const unsigned char *pmask = NULL);

	// �ն������.
	void RectifyLabel(void);

	// ����ͻ��������Ӱ�������.
	void Rectify(const unsigned char *pt, int step);

	// ��ʴ, ��������.
	void RectifyShape(void);

	// ������ͨ�������(������ſ����).
	int RectLabel(void);

};

#endif