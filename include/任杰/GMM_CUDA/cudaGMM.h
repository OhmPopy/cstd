#ifndef _CUDA_GMM_H_
#define _CUDA_GMM_H_

// ���Ͷ���.
typedef unsigned char uchar;

// GMM��ǰ������.
struct GMM;

// ��������.

// ����GMM�����.
GMM* CreateGMM(int Height, int Width, int Step);

// ����GMM�����.
void DestroyGMM(struct GMM **pGMM);

// ����GMM����ģ��.
int SetupBGModel(struct GMM *pGMM, const uchar *src, int num);

// ǰ�����.
int ForegroundDetect(GMM *pGMM, const uchar *src);

// ���ؼ������ֵͼ.
const int* GetBinResult(GMM *pGMM);

#endif