
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

//�Ƕȵ�����ת���ĺ�
#define RADIAN(angle) ((angle)*PI/180.0)

//////////////////////////////////////////////////////////////////////
// ͼ�񼸺α任����
//////////////////////////////////////////////////////////////////////

/*************************************************************************
* �������ƣ�
*   Translation()
* ����:
*   LPBYTE lpDIBBits   - ָ��ԴDIBͼ��ָ��
*   int  lWidth      - Դͼ���ȣ���������
*   int  lHeight     - Դͼ��߶ȣ���������
*   int  lLineBytes  - ͼ��ÿ�е��ֽ���
*   int  lXOffset    - X��ƽ��������������
*   int  lYOffset    - Y��ƽ��������������
* ����ֵ:
*   BOOL               - ƽ�Ƴɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú�������ˮƽ�ƶ�DIBͼ�񡣺�������ı�ͼ��Ĵ�С���Ƴ��Ĳ���ͼ��
* ����ȥ���հײ����ð�ɫ��䡣
 ************************************************************************/
BOOL Translation(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int lXOffset, int lYOffset)
{
  LPBYTE lpSrc;     // ָ��Դͼ���ָ��
  LPBYTE lpDst;     // ָ��Ҫ���������ָ��
  LPBYTE lpNewDIBBits;   // ָ����ͼ���ָ��
  int i, j;        // ��������DIB�е�����
  int i0, j0;      // ������ԴDIB�е�����

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lLineBytes * lHeight);

  for (i = 0; i < lHeight; i++) {   // ÿ��
    for (j = 0; j < lWidth; j++) {   // ÿ��
      // ָ����DIB��i�У���j�����ص�ָ��
      // ע������DIB��ͼ���һ����ʵ���������һ�е�λ�ã����lpDst
      // ֵ����lpNewDIBBits + lLineBytes* i + j������
      // lpNewDIBBits + lLineBytes* (lHeight - 1 - i) + j
      lpDst = lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      // �����������ԴDIB�е�����
      i0 = i - lXOffset;
      j0 = j - lYOffset;

      // �ж��Ƿ���Դͼ��Χ��
      if ((j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight)) {
        // ָ��ԴDIB��i0�У���j0�����ص�ָ��
        // ͬ��Ҫע��DIB���µ��õ�����
        lpSrc = lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;
        *lpDst = *lpSrc;     // ��������
      }
      else {
        // ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪ255
        * ((LPBYTE) lpDst) = 255;
      }
    }
  }

  // ����ƽ�ƺ��ͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

  SAFEFREE(lpNewDIBBits);
  return TRUE;
}

/*************************************************************************
* �������ƣ�Translation(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int lXOffset,
                            int lYOffset, int lLineBytes, int lDstLineBytes)
* ��������:
*   LPBYTE lpDIBBits��ָ��ԴDIB��ʼ���ص�ָ��
*      int lWidth��DIBͼ��Ŀ��
*      int lHeight��DIBͼ��ĸ߶�
*      int lXOffset, X����ƫ����
*      int lYOffset, Y����ƫ����
*      int lLineBytes��DIBͼ������ֽ�����Ϊ4�ı���
*      int lDstLineBytes����ʱDIBͼ������ֽ�����Ϊ4�ı���
* ��������:BOOL
* ��������:�ú�������ƽ��DIBͼ��
 ************************************************************************/
BOOL Translation1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int lXOffset, int lYOffset, int lDstLineBytes)
{
  int i;                 //��ѭ������
  int j;                 //��ѭ������
  LPBYTE lpSrcDIBBits;    //ָ��Դ���ص�ָ��
  LPBYTE lpDstDIBBits;    //ָ����ʱͼ���Ӧ���ص�ָ��
  LPBYTE lpDstStartBits;    //ָ����ʱͼ���Ӧ���ص�ָ��

  lpDstStartBits = MALLOC(BYTE, lWidth * lDstLineBytes);   // ������ʱ�ڴ�

  if (lpDstStartBits == NULL) { // �ж��Ƿ��ڴ����
    return FALSE;  // �����ڴ�ʧ��
  }

  for (i = 0; i < lHeight; i++) {   // ��
    for (j = 0; j < lWidth; j++) {   // ��
      lpDstDIBBits = (LPBYTE) lpDstStartBits + lLineBytes * (lHeight - 1 - i)
          + j; // ָ����DIB��i�У���j�����ص�ָ��

      if ((j - lYOffset >= 0) && (j - lYOffset < lWidth) &&       // ������ԴDIB�е�����j-lXOffset
          (i - lXOffset >= 0) && (i - lXOffset < lHeight)) {    // �ж��Ƿ���Դͼ��Χ��
        lpSrcDIBBits = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 -
            (i - lXOffset)) + (j - lYOffset);      // ָ��ԴDIB��i0�У���j0�����ص�ָ��
        *lpDstDIBBits = *lpSrcDIBBits; // ��������
      }
      else {
        * ((LPBYTE) lpDstDIBBits) = 255;     // Դͼ��û�е����أ���Ϊ255
      }
    }
  }

  memcpy(lpDIBBits, lpDstStartBits, lLineBytes * lHeight);   // ����ͼ��

  // �ͷ��ڴ�
  SAFEFREE(lpDstStartBits);
  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   Mirror()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������
*   int lHeight      - Դͼ��߶ȣ���������
*   BOOL  bDirection   - ����ķ���TRUE��ʾˮƽ����FALSE��ʾ��ֱ����
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú�����������DIBͼ�񡣿���ָ������ķ�ʽ��ˮƽ���Ǵ�ֱ��
 ************************************************************************/
BOOL Mirror(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    BOOL bDirection)
{
  LPBYTE lpSrc;      // ָ��Դͼ���ָ��
  LPBYTE lpDst;      // ָ��Ҫ���������ָ��
  LPBYTE lpBits;      // ָ����ͼ���ָ��
  int i;       // ѭ������
  int j;

  lpBits = MALLOC(BYTE, lLineBytes);   // ��ʱ�����ڴ棬�Ա���һ��ͼ��

  if (lpBits == NULL) {
    return FALSE;
  }

  // �жϾ���ʽ
  if (bDirection) {   // ˮƽ����
    // ���ͼ��ÿ�н��в���
    for (i = 0; i < lHeight; i++) {
      // ���ÿ��ͼ����벿�ֽ��в���
      for (j = 0; j < lWidth / 2; j++) {
        // ָ������i�У���j�����ص�ָ��
        lpSrc = lpDIBBits + lLineBytes * i + j;
        // ָ������i�У�������j�����ص�ָ��
        lpDst = lpDIBBits + lLineBytes * (i + 1) - j;

        // ����һ������
        *lpBits = *lpDst;

        // ��������i�У���j�����ظ��Ƶ�������i�У�������j������
        *lpDst = *lpSrc;
        // ��������i�У�������j�����ظ��Ƶ�������i�У���j������
        *lpSrc = *lpBits;
      }
    }
  }
  else {   // ��ֱ����
    // ����ϰ�ͼ����в���
    for (i = 0; i < lHeight / 2; i++) {
      // ָ������i����������ָ��
      lpSrc = lpDIBBits + lLineBytes * i;
      // ָ���i����������ָ��
      lpDst = lpDIBBits + lLineBytes * (lHeight - i - 1);

      // ����һ�У����ΪlWidth
      memcpy(lpBits, lpDst, lLineBytes);

      // ��������i�����ظ��Ƶ���i��
      memcpy(lpDst, lpSrc, lLineBytes);
      // ����i�����ظ��Ƶ�������i��
      memcpy(lpSrc, lpBits, lLineBytes);
    }
  }

  SAFEFREE(lpBits);
  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   Transpose()
* ����:
*   LPBYTE lpDIB  - ָ��ԴDIB��ָ��
* ����ֵ:
*   BOOL               - ת�óɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú�������ת��DIBͼ�񣬼�ͼ��x��y���껥��������������ı�ͼ��Ĵ�С��
* ����ͼ��ĸ߿�������
 ************************************************************************/
BOOL Transpose(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int lNewLineBytes)
{
  LPBYTE lpSrc;    // ָ��Դ���ص�ָ��
  LPBYTE lpDst;    // ָ��ת��ͼ���Ӧ���ص�ָ��
  LPBYTE lpNewDIBBits;  // ָ��ת��ͼ���ָ��
  int i;
  int j;

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lWidth * lNewLineBytes);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  for (i = 0; i < lHeight; i++) {  // ���ͼ��ÿ�н��в���
    for (j = 0; j < lWidth; j++) {  // ���ÿ��ͼ��ÿ�н��в���
      // ָ��ԴDIB��i�У���j�����ص�ָ��
      lpSrc = lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      // ָ��ת��DIB��j�У���i�����ص�ָ��
      // ע��˴�lWidth��lHeight��ԴDIB�Ŀ�Ⱥ͸߶ȣ�Ӧ�û���
      lpDst = lpNewDIBBits + lNewLineBytes * (lWidth - 1 - j) + i;

      // ��������
      *lpDst = *lpSrc;
    }
  }

  // ����ת�ú��ͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lNewLineBytes);

  SAFEFREE(lpNewDIBBits);
  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   Zoom()
* ����:
*   LPBYTE lpDIB  - ָ��ԴDIB��ָ��
*   Float fXZoomRatio - X�᷽�����ű���
*   Float fYZoomRatio - Y�᷽�����ű���
* ����ֵ:
*   HGLOBAL            - ���ųɹ�������DIB��������򷵻�NULL��
* ˵��:
*   �ú�����������DIBͼ�񣬷���������DIB�ľ����
 ************************************************************************/
int Zoom(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int lNewLineBytes,
    Float fXZoomRatio, Float fYZoomRatio)
{
  int lNewWidth;   // ���ź�ͼ��Ŀ��
  int lNewHeight;   // ���ź�ͼ��ĸ߶�
  LPBYTE lpSrc;    // ָ��Դ���ص�ָ��
  LPBYTE lpDst;    // ָ������ͼ���Ӧ���ص�ָ��
  LPBYTE lpNewDIBBits;

  int i;     // ѭ����������������DIB�е����꣩
  int j;
  int i0;     // ������ԴDIB�е�����
  int j0;

  // �������ź��ͼ��ʵ�ʿ��
  // �˴�ֱ�Ӽ�0.5������ǿ������ת��ʱ���������룬����ֱ�ӽ�ȥС������
  lNewWidth = (int)(lWidth * fXZoomRatio + 0.5);

  // �������ź��ͼ��߶�
  lNewHeight = (int)(lHeight * fYZoomRatio + 0.5);

  // �����ڴ棬�Ա�����DIB
  lpNewDIBBits = MALLOC(BYTE, lNewLineBytes * lNewHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  for (i = 0; i < lNewHeight; i++) {   // ���ͼ��ÿ�н��в���
    for (j = 0; j < lNewWidth; j++) {   // ���ͼ��ÿ�н��в���
      // ָ����DIB��i�У���j�����ص�ָ��
      // ע��˴���Ⱥ͸߶�����DIB�Ŀ�Ⱥ͸߶�
      lpDst = lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;

      // �����������ԴDIB�е�����
      i0 = (int)(i / fYZoomRatio + 0.5);
      j0 = (int)(j / fXZoomRatio + 0.5);

      // �ж��Ƿ���Դͼ��Χ��
      if ((j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight)) {
        // ָ��ԴDIB��i0�У���j0�����ص�ָ��
        lpSrc = lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;
        // ��������
        *lpDst = *lpSrc;
      }
      else {
        // ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪ255
        * ((LPBYTE) lpDst) = 255;
      }
    }
  }

  SAFEFREE(lpNewDIBBits);
  return 0;
}

/*************************************************************************
* �������ƣ�
*   Rotate()
* ����:
*   LPBYTE lpDIB  - ָ��ԴDIB��ָ��
*   int iRotateAngle - ��ת�ĽǶȣ�0-360�ȣ�
* ����ֵ:
*   HGLOBAL            - ��ת�ɹ�������DIB��������򷵻�NULL��
* ˵��:
*   �ú���������ͼ������Ϊ������תDIBͼ�񣬷���������DIB�ľ����
* ���øú������Զ�����ͼ������ʾ���е����ء������в������ڽ���
* ֵ�㷨���в�ֵ��
 ************************************************************************/
int Rotate(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int lNewLineBytes,
    int iRotateAngle)
{
  int lNewWidth;    // ��ת��ͼ��Ŀ��
  int lNewHeight;   // ��ת��ͼ��ĸ߶�
  LPBYTE lpSrc;    // ָ��Դ���ص�ָ��
  LPBYTE lpDst;    // ָ����תͼ���Ӧ���ص�ָ��
  LPBYTE lpNewDIBBits; // ָ����תͼ���ָ��

  int i;     // ѭ����������������DIB�е����꣩
  int j;
  int i0;     // ������ԴDIB�е�����
  int j0;

  Float fRotateAngle;  // ��ת�Ƕȣ����ȣ�
  Float fSina, fCosa;  // ��ת�Ƕȵ����Һ�����
  // Դͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
  Float fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;
  // ��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
  Float fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;
  Float f1, f2;

  // ����ת�ǶȴӶ�ת��������
  fRotateAngle = (Float) RADIAN(iRotateAngle);
  fSina = (Float) sin((Float) fRotateAngle);       // ������ת�Ƕȵ�����
  fCosa = (Float) cos((Float) fRotateAngle);       // ������ת�Ƕȵ�����

  // ����ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
  fSrcX1 = (Float)(- (lWidth - 1) / 2);
  fSrcY1 = (Float)((lHeight - 1) / 2);
  fSrcX2 = (Float)((lWidth - 1) / 2);
  fSrcY2 = (Float)((lHeight - 1) / 2);
  fSrcX3 = (Float)(- (lWidth - 1) / 2);
  fSrcY3 = (Float)(- (lHeight - 1) / 2);
  fSrcX4 = (Float)((lWidth - 1) / 2);
  fSrcY4 = (Float)(- (lHeight - 1) / 2);

  // ������ͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
  fDstX1 = fCosa * fSrcX1 + fSina * fSrcY1;
  fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
  fDstX2 = fCosa * fSrcX2 + fSina * fSrcY2;
  fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
  fDstX3 = fCosa * fSrcX3 + fSina * fSrcY3;
  fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
  fDstX4 = fCosa * fSrcX4 + fSina * fSrcY4;
  fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

  // ������ת���ͼ��ʵ�ʿ��
  lNewWidth = (int)(MAX(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);

  // ������ת���ͼ��߶�
  lNewHeight = (int)(MAX(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

  // �������������������Ժ�ÿ�ζ�������
  f1 = (Float)(-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina
      + 0.5 * (lWidth - 1));
  f2 = (Float)(0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa
      + 0.5 * (lHeight - 1));

  // �����ڴ棬�Ա�����DIB
  lpNewDIBBits = (LPBYTE) MALLOC(BYTE, lNewLineBytes * lNewHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  for (i = 0; i < lNewHeight; i++) {   // ���ͼ��ÿ�н��в���
    for (j = 0; j < lNewWidth; j++) {   // ���ͼ��ÿ�н��в���
      // ָ����DIB��i�У���j�����ص�ָ��
      // ע��˴���Ⱥ͸߶�����DIB�Ŀ�Ⱥ͸߶�
      lpDst = lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;

      // �����������ԴDIB�е�����
      i0 = (int)(-((Float) j) * fSina + ((Float) i) * fCosa + f2 + 0.5);
      j0 = (int)(((Float) j) * fCosa + ((Float) i) * fSina + f1 + 0.5);

      // �ж��Ƿ���Դͼ��Χ��
      if ((j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight)) {
        // ָ��ԴDIB��i0�У���j0�����ص�ָ��
        lpSrc = lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;

        // ��������
        *lpDst = *lpSrc;
      }
      else {
        // ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪ255
        * ((LPBYTE) lpDst) = 255;
      }
    }
  }

  return 0;
}

/*************************************************************************
* �������ƣ�
*   Interpolation()
* ����:
*   LPBYTE lpDIBBits   - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������
*   int lHeight      - Դͼ��߶ȣ���������
*   Float x            - ��ֵԪ�ص�x����
*   Float y            - ��ֵԪ�ص�y����
* ����ֵ:
*   BYTE      - ���ز�ֵ��������
* ˵��:
*   �ú�������˫���Բ�ֵ�㷨����������ֵ�����ڳ���ͼ��Χ�����أ�ֱ�ӷ���255��
 ************************************************************************/
BYTE Interpolation(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    Float x, Float y)
{
  // �ĸ����ٽ����ص�����(i1, j1), (i2, j1), (i1, j2), (i2, j2)
  int i1, i2;
  int j1, j2;

  BYTE f1, f2, f3, f4; // �ĸ����ٽ�����ֵ
  BYTE f12, f34;  // ������ֵ�м�ֵ

  // ����һ��ֵ���������������С�ڸ�ֵʱ��Ϊ������ͬ
  Float EXP;

  EXP = (Float) 0.0001;

  // �����ĸ����ٽ����ص�����
  i1 = (int) x;
  i2 = i1 + 1;
  j1 = (int) y;
  j2 = j1 + 1;

  // ���ݲ�ͬ����ֱ���
  if ((x < 0) || (x > lWidth - 1) || (y < 0) || (y > lHeight - 1)) {
    return 255;  // Ҫ����ĵ㲻��Դͼ��Χ�ڣ�ֱ�ӷ���255��
  }
  else {
    if (fabs(x - lWidth + 1) <= EXP) {
      // Ҫ����ĵ���ͼ���ұ�Ե��
      if (fabs(y - lHeight + 1) <= EXP) {
        // Ҫ����ĵ�������ͼ�������½���һ�����أ�ֱ�ӷ��ظõ�����ֵ
        f1 = *((LPBYTE) lpDIBBits + lLineBytes *
            (lHeight - 1 - j1) + i1);
        return f1;
      }
      else {
        // ��ͼ���ұ�Ե���Ҳ������һ�㣬ֱ��һ�β�ֵ����
        f1 = *((LPBYTE) lpDIBBits + lLineBytes *
            (lHeight - 1 - j1) + i1);
        f3 = *((LPBYTE) lpDIBBits + lLineBytes *
            (lHeight - 1 - j1) + i2);

        // ���ز�ֵ���
        return ((BYTE)(f1 + (y - j1) * (f3 - f1)));
      }
    }
    else if (fabs(y - lHeight + 1) <= EXP) {
      // Ҫ����ĵ���ͼ���±�Ե���Ҳ������һ�㣬ֱ��һ�β�ֵ����
      f1 = *((LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j1) + i1);
      f2 = *((LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j2) + i1);

      // ���ز�ֵ���
      return ((BYTE)(f1 + (x - i1) * (f2 - f1)));
    }
    else {
      // �����ĸ����ٽ�����ֵ
      f1 = *((LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j1) + i1);
      f2 = *((LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j2) + i1);
      f3 = *((LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j1) + i2);
      f4 = *((LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j2) + i2);

      // ��ֵ1
      f12 = (BYTE)(f1 + (x - i1) * (f2 - f1));
      // ��ֵ2
      f34 = (BYTE)(f3 + (x - i1) * (f4 - f3));
      // ��ֵ3
      return ((BYTE)(f12 + (y - j1) * (f34 - f12)));
    }
  }
}

/*************************************************************************
* �������ƣ�
*   Rotate2()
* ����:
*   LPBYTE lpDIB  - ָ��ԴDIB��ָ��
*   int iRotateAngle - ��ת�ĽǶȣ�0-360�ȣ�
* ����ֵ:
*   HGLOBAL            - ��ת�ɹ�������DIB��������򷵻�NULL��
* ˵��:
*   �ú���������ͼ������Ϊ������תDIBͼ�񣬷���������DIB�ľ����
* ���øú������Զ�����ͼ������ʾ���е����ء������в���˫���Բ�
* ֵ�㷨���в�ֵ��
 ************************************************************************/
int Rotate2(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int lNewLineBytes,
    int iRotateAngle)
{
  int lNewWidth;   // ��ת��ͼ��Ŀ��
  int lNewHeight;   // ��ת��ͼ��ĸ߶�
  LPBYTE lpDst;    // ָ����תͼ���Ӧ���ص�ָ��
  LPBYTE lpNewDIBBits; // ָ����תͼ���ָ��

  int i;     // ѭ����������������DIB�е����꣩
  int j;
  Float i0;     // ������ԴDIB�е�����
  Float j0;

  Float fRotateAngle;  // ��ת�Ƕȣ����ȣ�
  Float fSina, fCosa;  // ��ת�Ƕȵ����Һ�����
  // Դͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
  Float fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;
  // ��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
  Float fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;
  Float f1, f2;

  // ����ת�ǶȴӶ�ת��������
  fRotateAngle = (Float) RADIAN(iRotateAngle);

  fSina = (Float) sin((Float) fRotateAngle);       // ������ת�Ƕȵ�����
  fCosa = (Float) cos((Float) fRotateAngle);       // ������ת�Ƕȵ�����

  // ����ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
  fSrcX1 = (Float)(- (lWidth - 1) / 2);
  fSrcY1 = (Float)((lHeight - 1) / 2);
  fSrcX2 = (Float)((lWidth - 1) / 2);
  fSrcY2 = (Float)((lHeight - 1) / 2);
  fSrcX3 = (Float)(- (lWidth - 1) / 2);
  fSrcY3 = (Float)(- (lHeight - 1) / 2);
  fSrcX4 = (Float)((lWidth - 1) / 2);
  fSrcY4 = (Float)(- (lHeight - 1) / 2);

  // ������ͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
  fDstX1 = fCosa * fSrcX1 + fSina * fSrcY1;
  fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
  fDstX2 = fCosa * fSrcX2 + fSina * fSrcY2;
  fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
  fDstX3 = fCosa * fSrcX3 + fSina * fSrcY3;
  fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
  fDstX4 = fCosa * fSrcX4 + fSina * fSrcY4;
  fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

  // ������ת���ͼ��ʵ�ʿ��
  lNewWidth = (int)(MAX(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);

  // ������ת���ͼ��߶�
  lNewHeight = (int)(MAX(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

  f1 = (Float)(-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina
      + 0.5 * (lWidth - 1));
  f2 = (Float)(0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa
      + 0.5 * (lHeight - 1));

  // �����ڴ棬�Ա�����DIB
  lpNewDIBBits = (LPBYTE) MALLOC(BYTE, lNewLineBytes * lNewHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  for (i = 0; i < lNewHeight; i++) {   // ���ͼ��ÿ�н��в���
    for (j = 0; j < lNewWidth; j++) {   // ���ͼ��ÿ�н��в���
      // ָ����DIB��i�У���j�����ص�ָ��
      // ע��˴���Ⱥ͸߶�����DIB�Ŀ�Ⱥ͸߶�
      lpDst = lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;

      // �����������ԴDIB�е�����
      i0 = -((Float) j) * fSina + ((Float) i) * fCosa + f2;
      j0 = ((Float) j) * fCosa + ((Float) i) * fSina + f1;

      // ����˫���Բ�ֵ�㷨����������ֵ
      *lpDst = Interpolation(lHeight, lWidth, lpDIBBits, lNewLineBytes, j0, i0);
    }
  }

  return 0;
}

/*************************************************************************
* �������ƣ�Rotate(LPBYTE lpSrcDib, int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
                   int lLineBytes, WORD palSize, int lDstWidth,
                   int lDstHeight, int lDstLineBytes, Float    fSina, Float fCosa)
* ��������:
*   LPBYTE lpDIBBits   -ָ��ԴDIB����ʼ���ص�ָ��
*   int lWidth         -ԴDIBͼ����
*   int lHeight        -ԴDIBͼ��߶�
*   int lLineBytes     -ԴDIBͼ���ֽڿ�ȣ�4�ı�����
*   int lDstWidth      -Ŀ��ͼ����
*   int lDstHeight     -Ŀ��DIBͼ��߶�
*   int lDstLineBytes  -Ŀ��DIBͼ�����ֽ�����4�ı�����
*   Float fSina        -��ת�ǵ����ң�˵����Ϊ�˱���������ȡ�����ң�������Ϊ����������������
*   Float fCosa        -��ת�ǵ�����
* ��������: LPBYTE
* ��������: ������תDIBͼ��
 ************************************************************************/
BOOL Rotate1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int lDstHeight, int lDstWidth, LPBYTE lpDstDib, int lDstLineBytes,
    Float fSina, Float fCosa)
{
  Float varFloat1;       //�����������1
  Float varFloat2;       //�����������2
  int i, i1;             //��ѭ������
  int j, j1;             //��ѭ������
  LPBYTE lpSrcDIBBits;   //ָ��Դ���ص�ָ��
  LPBYTE lpDstDIBBits;   //ָ����ʱͼ���Ӧ���ص�ָ��
  LPBYTE lpDstStartBits; //ָ����ʱͼ���Ӧ���ص�ָ��

  varFloat1 = (Float)(-0.5 * (lDstWidth - 1) * fCosa - 0.5 * (lDstHeight - 1) * fSina         // �������õ���������������������Ա���Ϊ����ʹ��
      + 0.5 * (lDstWidth - 1));
  varFloat2 = (Float)(0.5 * (lDstWidth - 1) * fSina - 0.5 * (lDstHeight - 1) * fCosa
      + 0.5 * (lDstHeight - 1));
  // ��������ʼλ��, ������ͬFindDIBBits(gCo.lpSrcDib)�����ﳢ��ʹ�������ַ������Ա����ȫ�ֺ����ĵ���
  lpDstStartBits = lpDstDib;

  for (i = 0; i < lDstHeight; i++) {   // �в���
    for (j = 0; j < lDstWidth; j++) {   // �в���
      lpDstDIBBits = (LPBYTE) lpDstStartBits + lDstLineBytes * (lDstHeight - 1 - i) + j;     // ָ����DIB��i�У���j�����ص�ָ��
      i1 = (int)(-((Float) j) * fSina + ((Float) i) * fCosa + varFloat2 + 0.5);              // �����������ԴDIB�е�����
      j1 = (int)(((Float) j) * fCosa + ((Float) i) * fSina + varFloat1 + 0.5);

      if ((j1 >= 0) && (j1 < lWidth) && (i1 >= 0) && (i1 < lHeight)) {           // �ж��Ƿ���Դͼ��
        lpSrcDIBBits = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i1) + j1;     // ָ��ԴDIB��i0�У���j0�����ص�ָ��
        *lpDstDIBBits = *lpSrcDIBBits; // ��������
      }
      else {
        * ((LPBYTE) lpDstDIBBits) = 255;     // Դͼ�в����ڵ����أ���Ϊ255
      }
    }
  }

  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�EigenvalueVector(int rank, Float* pMainCross, Float* pHypoCross,
                  Float* pMatrix, Float Precision, int MaxT)
* ��������:
*   int    rank ������A�Ľ���
*   Float     *pMainCross  ���Գ��������е����Խ�Ԫ�ص�ָ�룬����ʱ���A������ֵ
*   Float  *pHypoCross  ���Գ��������еĴζԽ�Ԫ�ص�ָ��
*     Float  *pMatrix  ���Գƾ���A������������ָ��
*   Float Precision �����ƾ���
*   int MaxT  �� ����������
* ��������:BOOL
* ��������:�ü���ʵ�Գ����Ǿ����ȫ������ֵ�Լ���Ӧ����������
 ************************************************************************/
BOOL EigenvalueVector(int rank, Float* pMainCross, Float* pHypoCross,
    Float* pMatrix, Float Precision, int MaxT)
{
  int i, j, k, m, it, u, v; // ��������
  Float d, f, h, g, p, r, e, s;

  pHypoCross[ rank - 1 ] = 0.0; // ��ʼ��
  d = 0.0;
  f = 0.0;

  for (j = 0; j <= rank - 1; j++) {                              //  �������ȵĿ���
    it = 0;
    h = Precision * (fabs(pMainCross[ j ]) + fabs(pHypoCross[ j ]));

    if (h > d) {
      d = h;
    }

    m = j;

    while ((m <= rank - 1) && (fabs(pHypoCross[ m ]) > d)) {
      m = m + 1;
    }

    if (m != j) {
      // ���������A������ֵ����������
      do {
        // ������������������ʧ��
        if (it == MaxT) {
          return (FALSE);
        }

        it = it + 1;
        g = pMainCross[ j ];
        p = (pMainCross[ j + 1 ] - g) / (2.0 * pHypoCross[ j ]);
        r = sqrt(p * p + 1.0);

        // ���p����0
        if (p >= 0.0) {
          pMainCross[ j ] = pHypoCross[ j ] / (p + r);
        }
        else {
          pMainCross[ j ] = pHypoCross[ j ] / (p - r);
        }

        h = g - pMainCross[ j ];

        //  �������Խ��ߵ�Ԫ��
        for (i = j + 1; i <= rank - 1; i++) {
          pMainCross[ i ] = pMainCross[ i ] - h;
        }

        // ��ֵ
        f = f + h;
        p = pMainCross[ m ];
        e = 1.0;
        s = 0.0;

        for (i = m - 1; i >= j; i--) {
          g = e * pHypoCross[ i ];
          h = e * p;

          //  ���Խ���Ԫ�صľ���ֵ�Ƿ���ڴζԽ���Ԫ�ص�
          if (fabs(p) >= fabs(pHypoCross[ i ])) {
            e = pHypoCross[ i ] / p;
            r = sqrt(e * e + 1.0);
            pHypoCross[ i + 1 ] = s * p * r;
            s = e / r;
            e = 1.0 / r;
          }
          else {
            e = p / pHypoCross[ i ];
            r = sqrt(e * e + 1.0);
            pHypoCross[ i + 1 ] = s * pHypoCross[ i ] * r;
            s = 1.0 / r;
            e = e / r;
          }

          p = e * pMainCross[ i ] - s * g;
          pMainCross[ i + 1 ] = h + s * (e * g + s * pMainCross[ i ]);

          // ���´洢��������
          for (k = 0; k <= rank - 1; k++) {
            u = k * rank + i + 1;
            v = u - 1;
            h = pMatrix[ u ];
            pMatrix[ u ] = s * pMatrix[ v ] + e * h;
            pMatrix[ v ] = e * pMatrix[ v ] - s * h;
          }

        }

        // �����Խ��ߺʹζԽ���Ԫ�����¸�ֵ
        pHypoCross[ j ] = s * p;
        pMainCross[ j ] = e * p;

      }
      while (fabs(pHypoCross[ j ]) > d);
    }

    pMainCross[ j ] = pMainCross[ j ] + f;
  }

  for (i = 0; i <= rank - 1; i++) {   // ����A������ֵ
    k = i;
    p = pMainCross[ i ];

    if (i + 1 <= rank - 1) {   // ��A����ֵ����p
      j = i + 1;

      while ((j <= rank - 1) && (pMainCross[ j ] <= p)) {
        k = j;
        p = pMainCross[ j ];
        j = j + 1;
      }
    }

    if (k != i) {   // �洢A������ֵ����������
      pMainCross[ k ] = pMainCross[ i ];
      pMainCross[ i ] = p;

      for (j = 0; j <= rank - 1; j++) {
        u = j * rank + i;
        v = j * rank + k;
        p = pMatrix[ u ];
        pMatrix[ u ] = pMatrix[ v ];
        pMatrix[ v ] = p;
      }
    }
  }

  return (TRUE);   // ����
}

/*************************************************************************
* �������ƣ�Mirror(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
* ��������:
*   LPBYTE lpDIBBits, ָ��DIB��ʼ���ص�ָ��
*   int   lWidth��DIBͼ��Ŀ��
*   int   lHeight��DIBͼ��ĸ߶�
*   int   lLineBytes��DIBͼ������ֽ�����Ϊ4�ı���
* ��������:BOOL
* ��������:�ú�����������DIBͼ�󣬱�����ֻʵ����ˮƽ���񣬴�ֱ�����ԭ������Ҳ̸���� ������ʵ��
 ************************************************************************/
BOOL Mirror1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  int i;               // ��ѭ������
  int j;               // ��ѭ������
  LPBYTE lpSrcDIBBits; // ָ��Դ���ص�ָ��
  LPBYTE lpDstDIBBits; // ָ����ʱͼ���Ӧ���ص�ָ��
  LPBYTE lpBits;    // ָ���м����ص�ָ�룬������ͼ��ʱ���ṩ��ʱ�������ڴ�ռ�
  lpDstDIBBits = MALLOC(BYTE, lLineBytes);   // ������ʱ�ڴ汣����ͼ��

  if (lpDstDIBBits == NULL) {
    return FALSE;    // �����ڴ�ʧ��
  }

  for (i = 0; i < lHeight; i++) {   // ˮƽ�������ͼ��ÿ�н��в���
    for (j = 0; j < lWidth / 2; j++) {   // ���ÿ��ͼ����벿�ֽ��в���
      lpSrcDIBBits = (LPBYTE) lpDIBBits + lLineBytes * i + j;   // ָ������i�У���j�����ص�ָ��
      lpBits = (LPBYTE) lpDIBBits + lLineBytes * (i + 1) - j;     // ָ������i+1�У�������j�����ص�ָ��
      *lpDstDIBBits = *lpBits; //�����м�����
      *lpBits = *lpSrcDIBBits; // ��������i�У���j�����ظ��Ƶ�������i�У�������j������
      *lpSrcDIBBits = *lpDstDIBBits; // ��������i�У�������j�����ظ��Ƶ�������i�У���j������
    }
  }

  // �ͷ��ڴ�
  SAFEFREE(lpDstDIBBits);
  return TRUE;
}
/*************************************************************************
* �������ƣ�
*   DisK_L(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
* ��������:
*   LPBYTE lpDIBBits, ָ��ԴDIBͼ��ָ��
*   int lWidth, ԴDIBͼ����
*   int lHeight, ԴDIBͼ��߶�
*   int lLineBytes, ԴDIBͼ������ֽ�����Ϊ4�ı���
* ��������:BOOL
* ��������:������ͼ�������ת
 ************************************************************************/
BOOL DisK_L(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  Float Precision = 0.000001; // �涨�����ļ��㾫��
  LPBYTE lpSrcUnChr;    //ָ�����ص�ָ��
  int i, j,  // ѭ������
      lMaxRange,  // �����任��ͼ�������ܷ�Χ
      AverEx, AverEy,  //  Ŀ�������ֵ
      ToaCount; //  Ŀ���ܵ�������
  Float Matr4C[ 2 ][ 2 ],  // ����ֵ��Э�������
        QMatrix[ 2 ][ 2 ],  // ���Э����������������
        MainCross[ 2 ], HypoCross[ 2 ],  //  ���Խ�������ԽǺʹζԽ���Ԫ��
        dTemp; // ��ʱ����
  int lTempI, lTempJ;
  Float* F;

  if (lWidth > lHeight) {   // ����ͼ�󾭹���ת��������Ŀ�Ⱥ͸߶�
    lMaxRange = lWidth;
  }
  else {
    lMaxRange = lHeight;
  }

  AverEx = 0; // ��ʼ��
  AverEy = 0;
  ToaCount = 0;
  Matr4C[ 0 ][ 0 ] = Matr4C[ 0 ][ 1 ] = Matr4C[ 1 ][ 0 ] = Matr4C[ 1 ][ 1 ] = 0.0;

  F = MALLOC(Float, lWidth * lHeight);   // �����ڴ�

  for (i = 0; i < lHeight; i++) {   // ��
    for (j = 0; j < lWidth; j++) {   // ��
      F[ i * lWidth + j ] = 255; // ����ת���������ÿ���㸳��ֵ
      // ָ��λͼi��j�����ص�ָ��
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * i + j;

      // ֵС��255���Ǳ���ɫ��ɫ����������ΪĿ���һ����
      // ����������ֵx��y������ά���ʸ��
      if ((*lpSrcUnChr) < 255) {

        AverEx = AverEx + i; // ����Ŀ�����ص�Y�����X�����ۼ�ֵ
        AverEy = AverEy + j;
        ToaCount++; // Ŀ���ܵ���������һ

        // ���ʸ��Э���������ۼ�ֵ
        Matr4C[ 0 ][ 0 ] = Matr4C[ 0 ][ 0 ] + i * i;
        Matr4C[ 0 ][ 1 ] = Matr4C[ 0 ][ 1 ] + i * j;
        Matr4C[ 1 ][ 0 ] = Matr4C[ 1 ][ 0 ] + j * i;
        Matr4C[ 1 ][ 1 ] = Matr4C[ 1 ][ 1 ] + j * j;
      }
    }
  }

  AverEx = AverEx / ToaCount; // �������ʸ���ľ�ֵ
  AverEy = AverEy / ToaCount;

  Matr4C[ 0 ][ 0 ] = Matr4C[ 0 ][ 0 ] / ToaCount - AverEx * AverEx; //  �������ʸ����Э�������
  Matr4C[ 0 ][ 1 ] = Matr4C[ 0 ][ 1 ] / ToaCount - AverEx * AverEy;
  Matr4C[ 1 ][ 0 ] = Matr4C[ 1 ][ 0 ] / ToaCount - AverEx * AverEy;
  Matr4C[ 1 ][ 1 ] = Matr4C[ 1 ][ 1 ] / ToaCount - AverEy * AverEy;

  ThreeCross(*Matr4C, 2, *QMatrix, MainCross, HypoCross);   // ��Э������������ԽǶԳ���
  EigenvalueVector(2, MainCross, HypoCross, *Matr4C, Precision, 50);   // ��Э������������ֵ������ʸ����

  dTemp = Matr4C[ 0 ][ 1 ]; // ������������ת��������������
  Matr4C[ 0 ][ 1 ] = Matr4C[ 1 ][ 0 ];
  Matr4C[ 1 ][ 0 ] = dTemp;

  for (i = 0; i <= 1; i++) { // ���������������й�һ��
    dTemp = pow(Matr4C[ i ][ 0 ], 2) + pow(Matr4C[ i ][ 1 ], 2);
    dTemp = sqrt(dTemp);
    Matr4C[ i ][ 0 ] = Matr4C[ i ][ 0 ] / dTemp;
    Matr4C[ i ][ 1 ] = Matr4C[ i ][ 1 ] / dTemp;
  }

  // ���Ҿ���ɢK-L�任����������ԭ����ϵ�е�����
  for (i = -lMaxRange + 1; i < lMaxRange; i++) {
    for (j = -lMaxRange + 1; j < lMaxRange; j++) {
      //  ��������ֵӳ�䵽�ɵ�����ϵ
      int Cx = (int)(i * Matr4C[ 0 ][ 0 ] - j * Matr4C[ 0 ][ 1 ]) + AverEx;
      int Cy = (int)(-i * Matr4C[ 1 ][ 0 ] + j * Matr4C[ 1 ][ 1 ]) + AverEy;

      //  ӳ��ֵ�Ƿ�����Դͼ��
      if (Cx >= 0 && Cx < lHeight && Cy >= 0 && Cy < lWidth) {
        lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * Cx + Cy;

        // ӳ��ֵ�Ƿ�����ԭ����Ŀ��
        if (*(lpSrcUnChr) < 255) {
          //  ��������ϵԭ��ƽ�Ƶ����ģ��Ա���ʾ
          lTempI = (int)(lHeight / 2) + j;
          lTempJ = (int)(lWidth / 2) + i;

          // ������ܹ�������ʾ����ֵ�����飬���д洢
          if (lTempI >= 0 && lTempI < lHeight && lTempJ >= 0 && lTempJ < lWidth) {
            F[ lTempJ + (lTempI) * lWidth ] = *(lpSrcUnChr);
          }
        }
      }
    }
  }

  for (i = 0; i < lMaxRange; i++) {   // ��
    for (j = 0; j < lMaxRange; j++) {   // ��
      dTemp = F[ i * lMaxRange + j ]; // ��ɢK-L�任�������ֵ
      // ָ��λͼi��j�����ص�ָ��
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      * (lpSrcUnChr) = (BYTE)(dTemp);        // ����Դͼ��
    }
  }

  SAFEFREE(F);   // �ͷ��ڴ�
  return TRUE; // ����
}
