#ifndef _IMGLIB_H_
#define _IMGLIB_H_

//#include <windows.h>
#include <stdio.h>
#include "xtypes.h"

#if 1

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
//typedef int BOOL;
typedef BYTE* LPBYTE;
typedef double Float;
//typedef struct tagPOINT {  int x, y; } POINT;
#endif

//���ڸ�������
#if 1
typedef struct tagCOMPLEX {
  Float re;
  Float im;
}
COMPLEX;

// CplexSet (RE, IM, C);
#define CREAL(C) ((C).re)
#define CIMAG(C) ((C).im)
#define CplexSet(RE, IM, C) {(C).re =(RE);(C).im =(IM); }
#define CplexAdd(C1, C2, C) { CREAL(C)=CREAL(C1)+CREAL(C2); CIMAG(C)=CIMAG(C1)+CIMAG(C2); }
#define CplexSub(C1, C2, C) { CREAL(C)=CREAL(C1)-CREAL(C2); CIMAG(C)=CIMAG(C1)-CIMAG(C2); }
#define CplexMul(C1, C2, C) { CREAL(C)=CREAL(C1)*CREAL(C2)-CIMAG(C1)*CIMAG(C2); CIMAG(C)=CREAL(C1)*CIMAG(C2)+CREAL(C2)*CIMAG(C1); }

#else
#include <complex>
typedef std::complex<Float> COMPLEX;
#define CREAL(C) ((C).CREAL())
#define CIMAG(C) ((C).CIMAG())
#define CplexSet(RE, IM, C) {(C) = COMPLEX(RE, IM); }
#define CplexAdd(C1, C2, C) {(C) =(C1) +(C2); }
#define CplexSub(C1, C2, C) {(C) =(C1) -(C2); }
#define CplexMul(C1, C2, C) {(C) =(C1)*(C2); }

#endif

//----------------------------------------------------------------------
// ͼ��ָ�
// A[h��w] ����ͼ��
// B[h��w] ����ͼ��
// K[kh��kw] �˲���
// kx,ky    �˲���ԭ������
// border_type �߽���䷽ʽ, ȡ1
int filter2d(int h, int w, const real* A, int al, int ai, real* B, int bl,
    int kh, int kw, const real* K, int kl, int kx, int ky, int border_type);
int filter2d_old(int h, int w, const real* A, int al, int ai, real* B, int bl,
    int kh, int kw, const real* K, int kl, int kx, int ky, int border_type);

//----------------------------------------------------------------------
// ͼ��ָ�
void RegionSegFixThreshold(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nThreshold);
void Roberts(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pdGrad);
void Laplacian1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pdGrad);
void EdgeTrack(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, LPBYTE pUnEdgeTrack);
void RegionGrow(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, LPBYTE pUnRegion, int nThreshold);
void RegionSegAdaptive(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
void Sobel1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pdGrad);
void Prewitt1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pdGrad);

//-------------------------------------------------------------------------------------
// ͼ���Ե���������㺯��
BOOL Robert(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL Sobel(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL Prewitt(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL Kirsch(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL Gauss(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL Hough(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL Fill(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL Fill2(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL Contour(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL Trace(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);

// canny ����
void MakeGauss(Float sigma, Float** pdKernel, int* pnWindowSize);
void EstimateThreshold(int* pnMag, int lWidth, int lHeight, int* pnThdHigh, int* pnThdLow,
    LPBYTE pUnchEdge, Float dRatioHigh, Float dRationLow);

int shen_castan(int h, int w, const unsigned char* src, int al, unsigned char* edges, int bl, float b);

void cvCanny(int height, int width, const unsigned char* src, int src_step, int src_cn,
    unsigned char* dst, int dst_step, int dst_cn, double low_thresh, double high_thresh);

void Canny(int nHeight, int nWidth, const unsigned char* pUnchImage, unsigned char* pUnchEdge,
    double sigma, double dRatioLow, double dRatioHigh);

BOOL DWT_1D(Float* pDbSrc, int nMaxLevel, int nDWTSteps, int nInv, int nStep, int nSupp);
BOOL DWTStep_1D(Float* pDbSrc, int nCurLevel, int nInv, int nStep, int nSupp);
BOOL DWT_2D(Float* pDbSrc, int nMaxWLevel, int nMaxHLevel, int nDWTSteps, int nInv, int nStep, int nSupp);
BOOL DWTStep_2D(Float* pDbSrc, int nCurWLevel, int nCurHLevel, int nMaxWLevel, int nMaxHLevel, int nInv, int nStep, int nSupp);
BOOL ImageDWT(LPBYTE lpImage, int nMaxWLevel, int nMaxHLevel, int nDWTSteps, int nInv, int nStep, int nSupp);
BOOL DWTStep(int nHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nInv);
int Log2(int n);
BYTE FloatToByte(Float f);
char FloatToChar(Float f);


//----------------------------------------------------------------------
// ͼ����ǿ����
BOOL GraySegLinTrans(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nX1, int nY1,
    int nX2, int nY2);      // �ֶ����Ա任
BOOL HistogramEqualize(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);        // ֱ��ͼ���⻯
BOOL GeneralTemplate(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nTempWidth, int nTempHeight,
    int nTempCenX, int nTempCenY, Float* pdbTemp, Float dbCoef);  // ͨ��ģ�����
BOOL MedianFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nTempWidth, int nTempHeight,
    int nTempCenX, int nTempCenY);    // ��ֵ�˲�
BYTE GetMedianValue(LPBYTE pUnchFltValue, int iFilterLen);   // ��ȡ��ֵ
BOOL ReplaceDIBColorTable(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, LPBYTE pColorsTable);     // �滻DIB��ɫ��

BOOL LowPassFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nRadius1, int nRadius2);   // ��ͨ�˲�
BOOL HighPassFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nRadius1, int nRadius2);   // ��ͨ�˲�
BOOL ButterWorthLowPass1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nRadius);   //  ButterWorth��ͨ�˲�
BOOL ButterWorthHighPass1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nRadius);   //  ButterWorth��ͨ�˲�
void LinearSharpen(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);     //  ��������ǿ

void gauss_filter(int h, int w, const unsigned char* A, int al, int ai, unsigned char* B, int bl, int bi, int win);

void bilateral_filter(int h, int w, const unsigned char* A, int al, int ai, unsigned char* B, int bl, int bi, int win, int delta);

//--------------------------------------------------------------------------
// ͼ���������ԭ��
BOOL HoleNumber(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
int Moment(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL BaryCenterMoment(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int* pBarycenterX, int* pBarycenterY, int* pBarycenterMoment);
BOOL FreameWork(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL ChessBoarddisreStore(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL StreetDis(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL OutLine(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);

//-------------------------------------------------------------------------------------
// ͼ�������任����
void FFT_1D(COMPLEX* pCTData, COMPLEX* pCFData, int nLevel);
void IFFT_1D(COMPLEX* pCFData, COMPLEX* pCTData, int nLevel);
void IFFT_2D(COMPLEX* pCFData, COMPLEX* pCTData, int lWidth, int lHeight);
void IDFT_2D(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    Float* pTrRstRpart, Float* pTrRstIpart);
void FFT(COMPLEX* TD, COMPLEX* FD, int r);
void IFFT(COMPLEX* FD, COMPLEX* TD, int r);
void Dct1D(const Float* f, Float* F, int power);
void iDct1D(const Float* F, Float* f, int power);
void WALSH(const Float* f, Float* F, int r);
void IWALSH(const Float* F, Float* f, int r);

BOOL Fourier(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL Dct(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL Walsh(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL Hotelling(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
void Dft2D(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pTrRstRpart, Float* pTrRstIpart);
void Fft2D(COMPLEX* pCTData, int lWidth, int lHeight, COMPLEX* pCFData);

BOOL ThreeCross(Float* Matrix, int Rank, Float* QMatrix, Float* MainCross, Float* HypoCross);
BOOL BSTQ(int Rank, Float* MainCross, Float* HypoCross, Float* Matrix, Float Eqs, int MaxT);

BOOL Fourn(Float* data, unsigned int nn[], int ndim, int isign);
BOOL BlurDIB(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL Restore(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL NoiseBlur(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL Wiener(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL RandomNoise(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL SaltNoise(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);

//------------------------------------------------------------------------------------
// ͼ��ԭ�ĺ���ԭ��
BOOL NoRestriction(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL InverseFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL NoiseDegeneration(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL WinnerFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL MotionDegeneration(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL MotionRestore(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);

//-------------------------------------------------------------------------------------
// ͼ����뺯��ԭ��
BOOL BitPlane(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, BYTE bBitNum);
BOOL LimbPatternBayer(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL DitherFloydSteinberg(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);

//-------------------------------------------------------------------------------------
// ͼ��������⺯��ԭ��
BOOL Threshold(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL AddMinus(LPBYTE lpDIBBits, LPBYTE lpDIBBitsBK, int lWidth, int lHeight, int lLineBytes, BOOL bAddMinus);
BOOL Hproject(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL Vproject(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);
BOOL TemplateMatch1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int lTemplateHeight, int lTemplateWidth,
    LPBYTE lpTemplateDIBBits, int lTemplateLineBytes);

BOOL TemplateMatch(int lHeight, int lWidth, LPBYTE lpDIBBits1,
    int TemplateHeight, int TemplateWidth, LPBYTE lpDIBBits2);

//-------------------------------------------------------------------------------------
// ͼ��ģ��任����
BOOL Template(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int iTempH, int iTempW, int iTempMX, int iTempMY, Float* fpArray, Float fCoef);
BYTE GetMedianNum(LPBYTE bArray, int iFilterLen);
BOOL GradSharp(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, BYTE bThre);
BOOL ReplaceColorPal(LPBYTE lpDIB, LPBYTE bpColorsTable);

//-------------------------------------------------------------------------------------
// ͼ�񼸺α任����
BOOL Translation1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int lXOffset, int lYOffset, int lDstLineBytes);
BOOL Translation(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int lXOffset, int lYOffset);
BOOL Mirror(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, BOOL bDirection);
BOOL Transpose(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int lNewLineBytes);
int Zoom(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int lNewLineBytes,
    Float fXZoomRatio, Float fYZoomRatio);
int Rotate(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int lNewLineBytes,
    int iRotateAngle);
int Rotate2(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int lNewLineBytes,
    int iRotateAngle);
BYTE Interpolation(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    Float x, Float y);

//-------------------------------------------------------------------------------------
// ͼ������㺯��
BOOL LinerTrans(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float fA, Float fB);
BOOL ThresholdTrans(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, BYTE bThre);
BOOL WindowTrans(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, BYTE bLow, BYTE bUp);
BOOL GrayStretch(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2);
BOOL InteEqualize(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);

//-------------------------------------------------------------------------------------
//  ͼ����̬ѧ�任����
BOOL ErosionBlock(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, BOOL bHori, int structure[ 3 ][ 3 ]);
BOOL DilationBlock(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, BOOL bHori, int structure[ 3 ][ 3 ]);
BOOL OpenBlock(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, BOOL bHori, int structure[ 3 ][ 3 ]);
BOOL CloseBlock(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, BOOL bHori, int structure[ 3 ][ 3 ]);

BOOL ErosionLine(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int type, int num);
BOOL DilationLine(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int type , int num);
BOOL ThiningLine(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes);

//-------------------------------------------------------------------------------------
//  ͼ��ǵ��⺯��

#endif // _IMGLIB_H_
