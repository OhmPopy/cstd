
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

#define print_err(A, B)  printf(A)

//////////////////////////////////////////////////////////////////////
// ͼ���Ե���������㺯��
//////////////////////////////////////////////////////////////////////

/*************************************************************************
* �������ƣ�
*   Robert()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL               - ��Ե���ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú�����Robert��Ե������Ӷ�ͼ����б�Ե������㡣
* Ҫ��Ŀ��ͼ��Ϊ�Ҷ�ͼ��
 ************************************************************************/
BOOL Robert(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;   // ָ��Դͼ���ָ��
  LPBYTE lpDst;   // ָ�򻺴�ͼ���ָ��
  LPBYTE lpNewDIBBits; // ָ�򻺴�DIBͼ���ָ��
  int i, j;    //ѭ������
  Float result;   //����ֵ
  BYTE pixel[ 4 ];

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  //ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
  for (j = lHeight - 1; j > 0; j--) {
    for (i = 0; i < lWidth - 1; i++) {
      //����ʹ��2��2��ģ�壬Ϊ��ֹԽ�磬���Բ��������±ߺ����ұߵ���������

      // ָ��Դͼ���j�У���i�����ص�ָ��
      lpSrc = lpDIBBits + lWidth * j + i;
      // ָ��Ŀ��ͼ���j�У���i�����ص�ָ��
      lpDst = lpNewDIBBits + lWidth * j + i;

      //ȡ�õ�ǰָ�봦2*2���������ֵ��ע��Ҫת��ΪBYTE��
      pixel[ 0 ] = (BYTE) * lpSrc;
      pixel[ 1 ] = (BYTE) * (lpSrc + 1);
      pixel[ 2 ] = (BYTE) * (lpSrc - lWidth);
      pixel[ 3 ] = (BYTE) * (lpSrc - lWidth + 1);

      //����Ŀ��ͼ���еĵ�ǰ��
      result = sqrt((pixel[ 0 ] - pixel[ 3 ]) * (pixel[ 0 ] - pixel[ 3 ]) + \
          (pixel[ 1 ] - pixel[ 2 ]) * (pixel[ 1 ] - pixel[ 2 ]));
      *lpDst = (BYTE) result;
    }
  }

  // ���Ƹ�ʴ���ͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits);

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   Sobel()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL               - ��Ե���ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú�����Sobel��Ե������Ӷ�ͼ����б�Ե������㡣
* Ҫ��Ŀ��ͼ��Ϊ�Ҷ�ͼ��
 ************************************************************************/
BOOL Sobel(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpDst1;    // ָ�򻺴�ͼ���ָ��
  LPBYTE lpDst2;
  LPBYTE lpNewDIBBits1;  // ָ�򻺴�DIBͼ���ָ��
  LPBYTE lpNewDIBBits2;

  int i, j;      // ѭ������
  int iTempH;    // ģ��߶�
  int iTempW;    // ģ����
  Float fTempC;  // ģ��ϵ��
  int iTempMX;   // ģ������Ԫ��X����
  int iTempMY;   // ģ������Ԫ��Y����
  Float aTemplate[ 9 ];   // ģ������

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits1 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits1 == NULL) {
    return FALSE;
  }

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits2 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits2 == NULL) {
    return FALSE;
  }

  // ����Դͼ�񵽻���ͼ����
  lpDst1 = lpNewDIBBits1;
  memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
  lpDst2 = lpNewDIBBits2;
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // ����Sobelģ�����
  iTempW = 3;
  iTempH = 3;
  fTempC = 1.0;
  iTempMX = 1;
  iTempMY = 1;
  aTemplate[ 0 ] = -1.0;
  aTemplate[ 1 ] = -2.0;
  aTemplate[ 2 ] = -1.0;
  aTemplate[ 3 ] = 0.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = 0.0;
  aTemplate[ 6 ] = 1.0;
  aTemplate[ 7 ] = 2.0;
  aTemplate[ 8 ] = 1.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits1, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  // ����Sobelģ�����
  aTemplate[ 0 ] = -1.0;
  aTemplate[ 1 ] = 0.0;
  aTemplate[ 2 ] = 1.0;
  aTemplate[ 3 ] = -2.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = 2.0;
  aTemplate[ 6 ] = -1.0;
  aTemplate[ 7 ] = 0.0;
  aTemplate[ 8 ] = 1.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //����������ͼ������ֵ
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // ���ƾ���ģ��������ͼ��Դͼ��
  memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits1);
  SAFEFREE(lpNewDIBBits2);

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   Prewitt()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL               - ��Ե���ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú�����Prewitt��Ե������Ӷ�ͼ����б�Ե������㡣
* Ҫ��Ŀ��ͼ��Ϊ�Ҷ�ͼ��
 ************************************************************************/
BOOL Prewitt(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpDst1;    // ָ�򻺴�ͼ���ָ��
  LPBYTE lpDst2;
  LPBYTE lpNewDIBBits1;  // ָ�򻺴�DIBͼ���ָ��
  LPBYTE lpNewDIBBits2;

  int i, j;     // ѭ������
  int iTempH;    // ģ��߶�
  int iTempW;    // ģ����
  Float fTempC;    // ģ��ϵ��
  int iTempMX;   // ģ������Ԫ��X����
  int iTempMY;   // ģ������Ԫ��Y����
  Float aTemplate[ 9 ];   // ģ������

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits1 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits1 == NULL) {
    return FALSE;
  }

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits2 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits2 == NULL) {
    return FALSE;
  }

  // ����Դͼ�񵽻���ͼ����
  lpDst1 = lpNewDIBBits1;
  memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
  lpDst2 = lpNewDIBBits2;
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);


  // ����Prewittģ�����
  iTempW = 3;
  iTempH = 3;
  fTempC = 1.0;
  iTempMX = 1;
  iTempMY = 1;
  aTemplate[ 0 ] = -1.0;
  aTemplate[ 1 ] = -1.0;
  aTemplate[ 2 ] = -1.0;
  aTemplate[ 3 ] = 0.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = 0.0;
  aTemplate[ 6 ] = 1.0;
  aTemplate[ 7 ] = 1.0;
  aTemplate[ 8 ] = 1.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits1, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  // ����Prewittģ�����
  aTemplate[ 0 ] = 1.0;
  aTemplate[ 1 ] = 0.0;
  aTemplate[ 2 ] = -1.0;
  aTemplate[ 3 ] = 1.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = -1.0;
  aTemplate[ 6 ] = 1.0;
  aTemplate[ 7 ] = 0.0;
  aTemplate[ 8 ] = -1.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //����������ͼ������ֵ
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // ���ƾ���ģ��������ͼ��Դͼ��
  memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits1);
  SAFEFREE(lpNewDIBBits2);

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   Kirsch()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL               - ��Ե���ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú�����kirsch��Ե������Ӷ�ͼ����б�Ե������㡣
* Ҫ��Ŀ��ͼ��Ϊ�Ҷ�ͼ��
 ************************************************************************/
BOOL Kirsch(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpDst1;   // ָ�򻺴�ͼ���ָ��
  LPBYTE lpDst2;
  LPBYTE lpNewDIBBits1; // ָ�򻺴�DIBͼ���ָ��
  LPBYTE lpNewDIBBits2;

  int i, j;    // ѭ������
  int iTempH;   // ģ��߶�
  int iTempW;   // ģ����
  Float fTempC;   // ģ��ϵ��
  int iTempMX;  // ģ������Ԫ��X����
  int iTempMY;  // ģ������Ԫ��Y����
  Float aTemplate[ 9 ];  // ģ������

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits1 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits1 == NULL) {
    return FALSE;
  }

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits2 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits2 == NULL) {
    return FALSE;
  }

  // ����Դͼ�񵽻���ͼ����
  lpDst1 = lpNewDIBBits1;
  memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
  lpDst2 = lpNewDIBBits2;
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // ����Kirschģ��1����
  iTempW = 3;
  iTempH = 3;
  fTempC = 1.0;
  iTempMX = 1;
  iTempMY = 1;
  aTemplate[ 0 ] = 5.0;
  aTemplate[ 1 ] = 5.0;
  aTemplate[ 2 ] = 5.0;
  aTemplate[ 3 ] = -3.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = -3.0;
  aTemplate[ 6 ] = -3.0;
  aTemplate[ 7 ] = -3.0;
  aTemplate[ 8 ] = -3.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits1, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  // ����Kirschģ��2����
  aTemplate[ 0 ] = -3.0;
  aTemplate[ 1 ] = 5.0;
  aTemplate[ 2 ] = 5.0;
  aTemplate[ 3 ] = -3.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = 5.0;
  aTemplate[ 6 ] = -3.0;
  aTemplate[ 7 ] = -3.0;
  aTemplate[ 8 ] = -3.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //����������ͼ������ֵ
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // ����Դͼ�񵽻���ͼ����
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // ����Kirschģ��3����
  aTemplate[ 0 ] = -3.0;
  aTemplate[ 1 ] = -3.0;
  aTemplate[ 2 ] = 5.0;
  aTemplate[ 3 ] = -3.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = 5.0;
  aTemplate[ 6 ] = -3.0;
  aTemplate[ 7 ] = -3.0;
  aTemplate[ 8 ] = 5.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //����������ͼ������ֵ
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // ����Դͼ�񵽻���ͼ����
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // ����Kirschģ��4����
  aTemplate[ 0 ] = -3.0;
  aTemplate[ 1 ] = -3.0;
  aTemplate[ 2 ] = -3.0;
  aTemplate[ 3 ] = -3.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = 5.0;
  aTemplate[ 6 ] = -3.0;
  aTemplate[ 7 ] = 5.0;
  aTemplate[ 8 ] = 5.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //����������ͼ������ֵ
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // ����Դͼ�񵽻���ͼ����
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // ����Kirschģ��5����
  aTemplate[ 0 ] = -3.0;
  aTemplate[ 1 ] = -3.0;
  aTemplate[ 2 ] = -3.0;
  aTemplate[ 3 ] = -3.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = -3.0;
  aTemplate[ 6 ] = 5.0;
  aTemplate[ 7 ] = 5.0;
  aTemplate[ 8 ] = 5.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  // ����Դͼ�񵽻���ͼ����
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  //����������ͼ������ֵ
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // ����Դͼ�񵽻���ͼ����
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // ����Kirschģ��6����
  aTemplate[ 0 ] = -3.0;
  aTemplate[ 1 ] = -3.0;
  aTemplate[ 2 ] = -3.0;
  aTemplate[ 3 ] = 5.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = -3.0;
  aTemplate[ 6 ] = 5.0;
  aTemplate[ 7 ] = 5.0;
  aTemplate[ 8 ] = -3.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //����������ͼ������ֵ
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // ����Դͼ�񵽻���ͼ����
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // ����Kirschģ��7����
  aTemplate[ 0 ] = 5.0;
  aTemplate[ 1 ] = -3.0;
  aTemplate[ 2 ] = -3.0;
  aTemplate[ 3 ] = 5.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = -3.0;
  aTemplate[ 6 ] = 5.0;
  aTemplate[ 7 ] = -3.0;
  aTemplate[ 8 ] = -3.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //����������ͼ������ֵ
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // ����Դͼ�񵽻���ͼ����
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // ����Kirschģ��8����
  aTemplate[ 0 ] = 5.0;
  aTemplate[ 1 ] = 5.0;
  aTemplate[ 2 ] = -3.0;
  aTemplate[ 3 ] = 5.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = -3.0;
  aTemplate[ 6 ] = -3.0;
  aTemplate[ 7 ] = -3.0;
  aTemplate[ 8 ] = -3.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //����������ͼ������ֵ
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // ���ƾ���ģ��������ͼ��Դͼ��
  memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits1);
  SAFEFREE(lpNewDIBBits2);

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*  Gauss()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL               - ��Ե���ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú����ø�˹������˹��Ե������Ӷ�ͼ����б�Ե������㡣
* Ҫ��Ŀ��ͼ��Ϊ�Ҷ�ͼ��
 ************************************************************************/
BOOL Gauss(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpDst1;    // ָ�򻺴�ͼ���ָ��
  LPBYTE lpDst2;
  LPBYTE lpNewDIBBits1;  // ָ�򻺴�DIBͼ���ָ��
  LPBYTE lpNewDIBBits2;

  int iTempH;    // ģ��߶�
  int iTempW;    // ģ����
  Float fTempC;    // ģ��ϵ��
  int iTempMX;   // ģ������Ԫ��X����
  int iTempMY;   // ģ������Ԫ��Y����
  Float aTemplate[ 25 ];  //ģ������

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits1 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits1 == NULL) {
    return FALSE;
  }

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits2 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits2 == NULL) {
    return FALSE;
  }

  // ����Դͼ�񵽻���ͼ����
  lpDst1 = lpNewDIBBits1;
  memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
  lpDst2 = lpNewDIBBits2;
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // ����Gaussģ�����
  iTempW = 5;
  iTempH = 5;
  fTempC = 1.0;
  iTempMX = 3;
  iTempMY = 3;
  aTemplate[ 0 ] = -2.0;
  aTemplate[ 1 ] = -4.0;
  aTemplate[ 2 ] = -4.0;
  aTemplate[ 3 ] = -4.0;
  aTemplate[ 4 ] = -2.0;
  aTemplate[ 5 ] = -4.0;
  aTemplate[ 6 ] = 0.0;
  aTemplate[ 7 ] = 8.0;
  aTemplate[ 8 ] = 0.0;
  aTemplate[ 9 ] = -4.0;
  aTemplate[ 10 ] = -4.0;
  aTemplate[ 11 ] = 8.0;
  aTemplate[ 12 ] = 24.0;
  aTemplate[ 13 ] = 8.0;
  aTemplate[ 14 ] = -4.0;
  aTemplate[ 15 ] = -4.0;
  aTemplate[ 16 ] = 0.0;
  aTemplate[ 17 ] = 8.0;
  aTemplate[ 18 ] = 0.0;
  aTemplate[ 19 ] = -4.0;
  aTemplate[ 20 ] = -2.0;
  aTemplate[ 21 ] = -4.0;
  aTemplate[ 22 ] = -4.0;
  aTemplate[ 23 ] = -4.0;
  aTemplate[ 24 ] = -2.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits1, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  // ���ƾ���ģ��������ͼ��Դͼ��
  memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits1);
  SAFEFREE(lpNewDIBBits2);

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   Hough()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
*   int lLineBytes   - ͼ��ÿ�е��ֽ���
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶԼ��ͼ���е�ƽ��ֱ�ߡ����ͼ����������ƽ�е�ֱ�ߣ�
* ��������ƽ��ֱ����ȡ������
* Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL Hough(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;    // ָ��Դͼ���ָ��
  LPBYTE lpDst;    // ָ�򻺴�ͼ���ָ��
  LPBYTE lpTrans;   // ָ��任���ָ��
  LPBYTE lpNewDIBBits;  // ָ�򻺴�DIBͼ���ָ��
  LPBYTE lpTransArea;  // ָ��任���ָ��

  int iMaxDist;   // �任��ĳߴ�
  int iMaxAngleNumber;
  int iDist;    // �任�������
  int iAngleNumber;
  int i, j;    // ѭ������
  BYTE pixel;  // ����ֵ
  struct {
    int Value;
    int Dist;
    int AngleNumber;
  }
  MaxValue1, MaxValue2; // �洢�任���е��������ֵ

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  //����任��ĳߴ� ������
  iMaxDist = (int) sqrt(lWidth * lWidth + lHeight * lHeight);
  //�Ƕȴ�0��180��ÿ��2��
  iMaxAngleNumber = 90;

  //Ϊ�任������ڴ�
  lpTransArea = MALLOC(BYTE, lWidth * lHeight * sizeof(int));

  if (lpTransArea == NULL) {
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
  lpTrans = lpTransArea;
  memset(lpTrans, 0, lWidth * lHeight * sizeof(int));

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = lpDIBBits + lLineBytes * j + i;

      //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixel = (BYTE) * lpSrc;

      //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
      if (pixel != 255 && *lpSrc != 0) {
        return FALSE;
      }

      //����Ǻڵ㣬���ڱ任��Ķ�Ӧ�����ϼ�1
      if (pixel == 0) {
        //ע�ⲽ����2��
        for (iAngleNumber = 0; iAngleNumber < iMaxAngleNumber; iAngleNumber++) {
          iDist = (int) fabs(i * cos(iAngleNumber * 2 * PI / 180.0) + \
              j * sin(iAngleNumber * 2 * PI / 180.0));

          //�任��Ķ�Ӧ���ϼ�1
          *(lpTransArea + iDist * iMaxAngleNumber + iAngleNumber) = \
              *(lpTransArea + iDist * iMaxAngleNumber + iAngleNumber) + 1;
        }
      }
    }
  }

  //�ҵ��任���е��������ֵ��
  MaxValue1.Value = 0;
  MaxValue2.Value = 0;

  //�ҵ���һ�����ֵ��
  for (iDist = 0; iDist < iMaxDist; iDist++) {
    for (iAngleNumber = 0; iAngleNumber < iMaxAngleNumber; iAngleNumber++) {
      if ((int) * (lpTransArea + iDist * iMaxAngleNumber + iAngleNumber) > MaxValue1.Value) {
        MaxValue1.Value = (int) * (lpTransArea + iDist * iMaxAngleNumber
            + iAngleNumber);
        MaxValue1.Dist = iDist;
        MaxValue1.AngleNumber = iAngleNumber;
      }
    }
  }

  //����һ�����ֵ�㸽������
  for (iDist = -9; iDist < 10; iDist++) {
    for (iAngleNumber = -1; iAngleNumber < 2; iAngleNumber++) {
      if (iDist + MaxValue1.Dist >= 0 && iDist + MaxValue1.Dist < iMaxDist \
          && iAngleNumber + MaxValue1.AngleNumber >= 0 && iAngleNumber
          + MaxValue1.AngleNumber <= iMaxAngleNumber) {
        *(lpTransArea + (iDist + MaxValue1.Dist) * iMaxAngleNumber + \
            (iAngleNumber + MaxValue1.AngleNumber)) = 0;
      }
    }
  }

  //�ҵ��ڶ������ֵ��
  for (iDist = 0; iDist < iMaxDist; iDist++) {
    for (iAngleNumber = 0; iAngleNumber < iMaxAngleNumber; iAngleNumber++) {
      if ((int) * (lpTransArea + iDist * iMaxAngleNumber + iAngleNumber) > MaxValue2.Value) {
        MaxValue2.Value = (int) * (lpTransArea + iDist * iMaxAngleNumber
            + iAngleNumber);
        MaxValue2.Dist = iDist;
        MaxValue2.AngleNumber = iAngleNumber;
      }
    }
  }

  //�ж���ֱ���Ƿ�ƽ��
  if (abs(MaxValue1.AngleNumber - MaxValue2.AngleNumber) <= 2) {
    //��ֱ��ƽ�У��ڻ���ͼ�����ػ�������ֱ��
    for (j = 0; j < lHeight; j++) {
      for (i = 0; i < lWidth; i++) {
        // ָ�򻺴�ͼ������j�У���i�����ص�ָ��
        lpDst = lpNewDIBBits + lLineBytes * j + i;

        //����õ���ĳһ��ƽ��ֱ���ϣ����ڻ���ͼ���Ͻ��õ㸳Ϊ��

        //�ڵ�һ��ֱ����
        iDist = (int) fabs(i * cos(MaxValue1.AngleNumber * 2 * PI / 180.0) + \
            j * sin(MaxValue1.AngleNumber * 2 * PI / 180.0));

        if (iDist == MaxValue1.Dist) {
          *lpDst = (BYTE) 0;
        }

        //�ڵڶ���ֱ����
        iDist = (int) fabs(i * cos(MaxValue2.AngleNumber * 2 * PI / 180.0) + \
            j * sin(MaxValue2.AngleNumber * 2 * PI / 180.0));

        if (iDist == MaxValue2.Dist) {
          *lpDst = (BYTE) 0;
        }
      }
    }
  }

  // ���Ƹ�ʴ���ͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits);
  SAFEFREE(lpTransArea);

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   Fill2()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL               - �������ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶ�ͼ���������������㡣
* Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL Fill2(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;   // ָ��Դͼ���ָ��
  int i;     // ѭ������
  BYTE pixel; // ����ֵ
  int xl, xr;    // ���ұ߽�����λ��
  BOOL bFilll, bFillr;  // �Ƿ���������߽�
  IPOINT Seeds[ 10 ];   // ���Ӷ�ջ��ָ��
  int StackPoint;
  int iCurrentPixelx, iCurrentPixely; // ��ǰ����λ��
  int iBufferPixelx, iBufferPixely;

  Seeds[ 1 ].y = lHeight / 2;  // ��ʼ������
  Seeds[ 1 ].x = lWidth / 2;
  StackPoint = 1;

  while (StackPoint != 0) {
    //ȡ������
    iCurrentPixelx = Seeds[ StackPoint ].x;
    iCurrentPixely = Seeds[ StackPoint ].y;
    StackPoint--;
    //  if(Seed2.Height== 75)
    //  {
    //   return TRUE;
    //   i++;
    //  }
    bFilll = TRUE;
    bFillr = TRUE;
    //����������ڵ��У������������ص�λ��
    iBufferPixelx = iCurrentPixelx;
    iBufferPixely = iCurrentPixely;

    //���������
    while (bFilll) {
      lpSrc = lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx;
      //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixel = (BYTE) * lpSrc;

      //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }

      //�����߽�
      if (pixel == 0) {
        bFilll = FALSE;
        xl = iCurrentPixelx + 1;
      }
      else {
        *lpSrc = (BYTE) 0;
        iCurrentPixelx--;

        //��ֹԽ��
        if (iCurrentPixelx < 0) {
          bFilll = FALSE;
          iCurrentPixelx = 0;
          xl = 0;
        }
      }
    }

    //��������䣬ȡ���������ص�λ��
    iCurrentPixelx = iBufferPixelx + 1;

    if (iCurrentPixelx > lWidth) {
      bFillr = FALSE;
      iCurrentPixelx = lWidth;
      xr = lWidth;
    }

    iCurrentPixely = iBufferPixely;

    while (bFillr) {
      lpSrc = lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx;
      //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixel = (BYTE) * lpSrc;

      //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }

      //�����߽�
      if (pixel == 0) {
        bFillr = FALSE;
        xr = iCurrentPixelx - 1;
      }
      else {
        *lpSrc = (BYTE) 0;
        iCurrentPixelx++;

        //��ֹԽ��
        if (iCurrentPixelx > lWidth) {
          bFillr = FALSE;
          iCurrentPixelx = lWidth;
          xr = lWidth;
        }
      }
    }

    //�ϡ�������ɨ�����Ƿ�ȫΪ�߽����ػ�������
    //�ȿ������ɨ����
    iCurrentPixely--;

    if (iCurrentPixely < 0) {
      iCurrentPixely = 0;
    }

    for (i = xr; i >= xl; i--) {
      lpSrc = lpDIBBits + lWidth * iCurrentPixely + i;
      //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixel = (BYTE) * lpSrc;

      //��δ�������أ����µ�����ѹ���ջ
      if (pixel == 255) {
        StackPoint++;
        Seeds[ StackPoint ].y = iCurrentPixely;
        Seeds[ StackPoint ].x = i;
        break;
      }
    }

    //�ٿ������ɨ����
    iCurrentPixely += 2;

    if (iCurrentPixely > lHeight) {
      iCurrentPixely = lHeight;
    }

    for (i = xr; i >= xl; i--) {
      lpSrc = lpDIBBits + lWidth * iCurrentPixely + i;
      //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixel = (BYTE) * lpSrc;

      //��δ�������أ����µ�����ѹ���ջ
      if (pixel == 255) {
        StackPoint++;
        Seeds[ StackPoint ].y = iCurrentPixely;
        Seeds[ StackPoint ].x = i;
        break;
      }
    }
  }

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   Fill()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL               - �������ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶ�ͼ���������������㡣
* Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL Fill(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;    // ָ��Դͼ���ָ��
  BYTE pixel;  // ����ֵ
  IPOINT* Seeds;    // ���Ӷ�ջ��ָ��
  int StackPoint;
  int iCurrentPixelx, iCurrentPixely; // ��ǰ����λ��

  Seeds = MALLOC(IPOINT, lWidth * lHeight);   // ��ʼ������
  Seeds[ 1 ].y = lHeight / 2;
  Seeds[ 1 ].x = lWidth / 2;
  StackPoint = 1;

  while (StackPoint != 0) {
    //ȡ������
    iCurrentPixelx = Seeds[ StackPoint ].x;
    iCurrentPixely = Seeds[ StackPoint ].y;
    StackPoint--;

    lpSrc = lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx;
    //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
    pixel = (BYTE) * lpSrc;

    //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
    if (pixel != 255 && pixel != 0) {
      return FALSE;
    }

    //����ǰ��Ϳ��
    *lpSrc = (BYTE) 0;

    //�ж�����ĵ㣬���Ϊ�ף���ѹ���ջ��ע���ֹԽ��
    if (iCurrentPixelx > 0) {
      lpSrc = lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx - 1;
      //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixel = (BYTE) * lpSrc;

      if (pixel == 255) {
        StackPoint++;
        Seeds[ StackPoint ].y = iCurrentPixely;
        Seeds[ StackPoint ].x = iCurrentPixelx - 1;
      }

      //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }
    }

    //�ж�����ĵ㣬���Ϊ�ף���ѹ���ջ��ע���ֹԽ��
    if (iCurrentPixely < lHeight - 1) {
      lpSrc = lpDIBBits + lWidth * (iCurrentPixely + 1) + iCurrentPixelx;
      //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixel = (BYTE) * lpSrc;

      if (pixel == 255) {
        StackPoint++;
        Seeds[ StackPoint ].y = iCurrentPixely + 1;
        Seeds[ StackPoint ].x = iCurrentPixelx;
      }

      //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }
    }

    //�ж�����ĵ㣬���Ϊ�ף���ѹ���ջ��ע���ֹԽ��
    if (iCurrentPixelx < lWidth - 1) {
      lpSrc = lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx + 1;
      //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixel = (BYTE) * lpSrc;

      if (pixel == 255) {
        StackPoint++;
        Seeds[ StackPoint ].y = iCurrentPixely;
        Seeds[ StackPoint ].x = iCurrentPixelx + 1;
      }

      //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }
    }

    //�ж�����ĵ㣬���Ϊ�ף���ѹ���ջ��ע���ֹԽ��
    if (iCurrentPixely > 0) {
      lpSrc = lpDIBBits + lWidth * (iCurrentPixely - 1) + iCurrentPixelx;
      //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixel = (BYTE) * lpSrc;

      if (pixel == 255) {
        StackPoint++;
        Seeds[ StackPoint ].y = iCurrentPixely - 1;
        Seeds[ StackPoint ].x = iCurrentPixelx;
      }

      //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }
    }
  }

  SAFEFREE(Seeds);

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   Contour()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶ�ͼ�����������ȡ���㡣
* Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL Contour(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;    // ָ��Դͼ���ָ��
  LPBYTE lpDst;    // ָ�򻺴�ͼ���ָ��
  LPBYTE lpNewDIBBits;  // ָ�򻺴�DIBͼ���ָ��
  int i, j;     //ѭ������
  BYTE n, e, s, w, ne, se, nw, sw;
  BYTE pixel;  //����ֵ

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  for (j = 1; j < lHeight - 1; j++) {
    for (i = 1; i < lWidth - 1; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = lpDIBBits + lWidth * j + i;
      // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
      lpDst = lpNewDIBBits + lWidth * j + i;
      //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixel = (BYTE) * lpSrc;

      //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
      //   if(pixel != 255 && pixel != 0)
      //    return FALSE;
      if (pixel == 0) {
        *lpDst = (BYTE) 0;
        nw = (BYTE) * (lpSrc + lWidth - 1);
        n = (BYTE) * (lpSrc + lWidth);
        ne = (BYTE) * (lpSrc + lWidth + 1);
        w = (BYTE) * (lpSrc - 1);
        e = (BYTE) * (lpSrc + 1);
        sw = (BYTE) * (lpSrc - lWidth - 1);
        s = (BYTE) * (lpSrc - lWidth);
        se = (BYTE) * (lpSrc - lWidth + 1);

        //������ڵİ˸��㶼�Ǻڵ�
        if (nw + n + ne + w + e + sw + s + se == 0) {
          *lpDst = (BYTE) 255;
        }
      }
    }
  }

  // ���Ƹ�ʴ���ͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits);

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   Trace()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
*   int lLineBytes   - ͼ��ÿ�е��ֽ���
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶ�ͼ����������������㡣
* Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL Trace(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;    // ָ��Դͼ���ָ��
  LPBYTE lpDst;    // ָ�򻺴�ͼ���ָ��
  LPBYTE lpNewDIBBits;  // ָ�򻺴�DIBͼ���ָ��
  int i, j;     // ѭ������
  BYTE pixel;  // ����ֵ
  BOOL bFindStartPoint;  // �Ƿ��ҵ���ʼ�㼰�ص���ʼ��
  BOOL bFindPoint;   // �Ƿ�ɨ�赽һ���߽��
  IPOINT StartPoint, CurrentPoint; // ��ʼ�߽���뵱ǰ�߽��

  //�˸��������ʼɨ�跽��
  int Direction[ 8 ][ 2 ] = {{ -1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, { -1, -1}, { -1, 0}};
  int BeginDirect;

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lLineBytes * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lLineBytes * lHeight);

  //���ҵ������Ϸ��ı߽��
  bFindStartPoint = FALSE;

  for (j = 0; j < lHeight && !bFindStartPoint; j++) {
    for (i = 0; i < lWidth && !bFindStartPoint; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = lpDIBBits + lLineBytes * j + i;
      //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixel = (BYTE) * lpSrc;

      if (pixel == 0) {
        bFindStartPoint = TRUE;
        StartPoint.y = j;
        StartPoint.x = i;

        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = lpNewDIBBits + lLineBytes * j + i;
        *lpDst = (BYTE) 0;
      }
    }
  }

  //������ʼ���������·�������ʼɨ�������Ϸ���
  BeginDirect = 0;
  //���ٱ߽�
  bFindStartPoint = FALSE;
  //�ӳ�ʼ�㿪ʼɨ��
  CurrentPoint.y = StartPoint.y;
  CurrentPoint.x = StartPoint.x;

  while (!bFindStartPoint) {
    bFindPoint = FALSE;

    while (!bFindPoint) {
      //��ɨ�跽��鿴һ������
      lpSrc = lpDIBBits + lLineBytes * (CurrentPoint.y + Direction[ BeginDirect ][ 1 ]) +
          (CurrentPoint.x + Direction[ BeginDirect ][ 0 ]);
      pixel = (BYTE) * lpSrc;

      if (pixel == 0) {
        bFindPoint = TRUE;
        CurrentPoint.y = CurrentPoint.y + Direction[ BeginDirect ][ 1 ];
        CurrentPoint.x = CurrentPoint.x + Direction[ BeginDirect ][ 0 ];

        if (CurrentPoint.y == StartPoint.y && CurrentPoint.x
            == StartPoint.x) {
          bFindStartPoint = TRUE;
        }

        lpDst = lpNewDIBBits + lLineBytes * CurrentPoint.y
            + CurrentPoint.x;
        *lpDst = (BYTE) 0;
        //ɨ��ķ�����ʱ����ת����
        BeginDirect--;

        if (BeginDirect == -1) {
          BeginDirect = 7;
        }

        BeginDirect--;

        if (BeginDirect == -1) {
          BeginDirect = 7;
        }
      }
      else {
        //ɨ�跽��˳ʱ����תһ��
        BeginDirect++;

        if (BeginDirect == 8) {
          BeginDirect = 0;
        }
      }
    }
  }

  // ���Ƹ�ʴ���ͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits);

  return TRUE;
}


/*************************************************************************
* �������ƣ�
*   EdgeDetection()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
*     int mode            - ��������
* ����ֵ:
*   BOOL               - ��Ե���ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
* �ú�����Sobel��Isotropic Sobel��LOG��Ե������Ӷ�ͼ����б�Ե������㡣
* Ҫ��Ŀ��ͼ��Ϊ�Ҷ�ͼ��
 ************************************************************************/
BOOL EdgeDetection(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int mode)
{
  int nTempWidth;        //ģ��Ŀ��
  int nTempHeight;     // ģ��ĸ߶�
  int nTempCenX;        //ģ�����ĵ�X���꣨�����ģ�壩
  int nTempCenY;        //ģ�����ĵ�Y���꣨�����ģ�壩
  Float* Tempdata;    //ģ�������ָ��
  Float TempCoef;    //ģ��ϵ��
  //ѭ������
  int i;
  int j;

  if (mode == 0) {         //Sobel����
    // ָ�򻺴�ͼ���ָ��
    LPBYTE lpDst1;
    LPBYTE lpDst2;
    // ָ�����ͼ���ָ��
    LPBYTE lpImage1;
    LPBYTE lpImage2;

    // ��ʱ�����ڴ棬�Ա�����ͼ��
    lpImage1 = (LPBYTE) MALLOC(char, lWidth * lHeight);
    lpImage2 = (LPBYTE) MALLOC(char, lWidth * lHeight);

    // �ж��Ƿ��ڴ����ʧ��
    if (lpImage1 == NULL) {
      // �����ڴ�ʧ��
      return FALSE;
    }

    // �ж��Ƿ��ڴ����ʧ��
    if (lpImage2 == NULL) {
      // �����ڴ�ʧ��
      return FALSE;
    }

    // ��ʼ��ͼ��Ϊԭʼͼ��
    memcpy(lpImage1, lpDIBBits, lWidth * lHeight);
    memcpy(lpImage2, lpDIBBits, lWidth * lHeight);
    lpDst1 = (LPBYTE) lpImage1;
    lpDst2 = (LPBYTE) lpImage2;
    //Sobel����ˮƽ���ģ�����
    nTempWidth = 3;
    nTempHeight = 3;
    nTempCenX = 1;
    nTempCenY = 1;
    TempCoef = 1;
    Tempdata = MALLOC(Float, 9);   //Sobel����ģ��Ϊ3X3
    //ˮƽ��ģ�����
    Tempdata[ 0 ] = -1;
    Tempdata[ 1 ] = -2;
    Tempdata[ 2 ] = -1;
    Tempdata[ 3 ] = 0;
    Tempdata[ 4 ] = 0;
    Tempdata[ 5 ] = 0;
    Tempdata[ 6 ] = 1;
    Tempdata[ 7 ] = 2;
    Tempdata[ 8 ] = 1;

    if (!Template(lHeight, lWidth, lpImage1, lLineBytes, nTempWidth, nTempHeight,
        nTempCenX, nTempCenY, Tempdata, TempCoef)) {
      return FALSE;
    }

    //��ֱ����ģ�����
    Tempdata[ 0 ] = -1;
    Tempdata[ 1 ] = 0;
    Tempdata[ 2 ] = 1;
    Tempdata[ 3 ] = -2;
    Tempdata[ 4 ] = 0;
    Tempdata[ 5 ] = 2;
    Tempdata[ 6 ] = -1;
    Tempdata[ 7 ] = 0;
    Tempdata[ 8 ] = 1;

    if (!Template(lHeight, lWidth, lpImage2, lLineBytes, nTempWidth, nTempHeight,
        nTempCenX, nTempCenY, Tempdata, TempCoef)) {
      return FALSE;
    }

    SAFEFREE(Tempdata);

    //����������ͼ������ֵ
    for (j = 0; j < lHeight; j++) {
      for (i = 0; i < lWidth; i++) {

        // ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��
        lpDst1 = (LPBYTE) lpImage1 + lWidth * j + i;

        // ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��
        lpDst2 = (LPBYTE) lpImage2 + lWidth * j + i;

        if (*lpDst2 > *lpDst1) {
          *lpDst1 = *lpDst2;
        }

      }
    }

    //���ƾ���ģ��������ͼ��Դͼ��
    memcpy(lpDIBBits, lpImage1, lWidth * lHeight);
    return TRUE;
  }

  if (mode == 1) {   //Isotropic Sobel����
    // ָ�򻺴�ͼ���ָ��
    LPBYTE lpDst1;
    LPBYTE lpDst2;
    // ָ�����ͼ���ָ��
    LPBYTE lpImage1;
    LPBYTE lpImage2;

    // ��ʱ�����ڴ棬�Ա�����ͼ��
    lpImage1 = (LPBYTE) MALLOC(char, lWidth * lHeight);
    lpImage2 = (LPBYTE) MALLOC(char, lWidth * lHeight);

    // �ж��Ƿ��ڴ����ʧ��
    if (lpImage1 == NULL) {
      // �����ڴ�ʧ��
      return FALSE;
    }

    // �ж��Ƿ��ڴ����ʧ��
    if (lpImage2 == NULL) {
      // �����ڴ�ʧ��
      return FALSE;
    }

    // ��ʼ��ͼ��Ϊԭʼͼ��
    memcpy(lpImage1, lpDIBBits, lWidth * lHeight);
    memcpy(lpImage2, lpDIBBits, lWidth * lHeight);
    lpDst1 = (LPBYTE) lpImage1;
    lpDst2 = (LPBYTE) lpImage2;
    //Sobel����ˮƽ���ģ�����
    nTempWidth = 3;
    nTempHeight = 3;
    nTempCenX = 1;
    nTempCenY = 1;
    TempCoef = 1;
    Tempdata = MALLOC(Float, 9);   //Sobel����ģ��Ϊ3X3
    //ˮƽ��ģ�����
    Tempdata[ 0 ] = -1;
    Tempdata[ 1 ] = -sqrt(2);
    Tempdata[ 2 ] = -1;
    Tempdata[ 3 ] = 0;
    Tempdata[ 4 ] = 0;
    Tempdata[ 5 ] = 0;
    Tempdata[ 6 ] = 1;
    Tempdata[ 7 ] = sqrt(2);
    Tempdata[ 8 ] = 1;

    if (!Template(lHeight, lWidth, lpImage1, lLineBytes, nTempWidth, nTempHeight,
        nTempCenX, nTempCenY, Tempdata, TempCoef)) {
      return FALSE;
    }

    //��ֱ����ģ�����
    Tempdata[ 0 ] = -1;
    Tempdata[ 1 ] = 0;
    Tempdata[ 2 ] = 1;
    Tempdata[ 3 ] = -sqrt(2);
    Tempdata[ 4 ] = 0;
    Tempdata[ 5 ] = sqrt(2);
    Tempdata[ 6 ] = -1;
    Tempdata[ 7 ] = 0;
    Tempdata[ 8 ] = 1;

    if (!Template(lHeight, lWidth, lpImage2, lLineBytes, nTempWidth, nTempHeight,
        nTempCenX, nTempCenY, Tempdata, TempCoef)) {
      return FALSE;
    }

    SAFEFREE(Tempdata);

    //����������ͼ������ֵ
    for (j = 0; j < lHeight; j++) {
      for (i = 0; i < lWidth; i++) {

        // ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��
        lpDst1 = (LPBYTE) lpImage1 + lWidth * j + i;

        // ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��
        lpDst2 = (LPBYTE) lpImage2 + lWidth * j + i;

        if (*lpDst2 > *lpDst1) {
          *lpDst1 = *lpDst2;
        }

      }
    }

    //���ƾ���ģ��������ͼ��Դͼ��
    memcpy(lpDIBBits, lpImage1, lWidth * lHeight);
    return TRUE;
  }

  if (mode == 2) {         //LOG����
    nTempWidth = 5;
    nTempHeight = 5;
    nTempCenX = 2;
    nTempCenY = 2;
    TempCoef = 1;
    Tempdata = MALLOC(Float, 25);
    Tempdata[ 0 ] = -2;
    Tempdata[ 1 ] = -4;
    Tempdata[ 2 ] = -4;
    Tempdata[ 3 ] = -4;
    Tempdata[ 4 ] = -2;
    Tempdata[ 5 ] = -4;
    Tempdata[ 6 ] = 0;
    Tempdata[ 7 ] = 8;
    Tempdata[ 8 ] = 0;
    Tempdata[ 9 ] = -4;
    Tempdata[ 10 ] = -4;
    Tempdata[ 11 ] = 8;
    Tempdata[ 12 ] = 24;
    Tempdata[ 13 ] = 8;
    Tempdata[ 14 ] = -4;
    Tempdata[ 15 ] = -4;
    Tempdata[ 16 ] = 0;
    Tempdata[ 17 ] = 8;
    Tempdata[ 18 ] = 0;
    Tempdata[ 19 ] = -4;
    Tempdata[ 20 ] = -2;
    Tempdata[ 21 ] = -4;
    Tempdata[ 22 ] = -4;
    Tempdata[ 23 ] = -4;
    Tempdata[ 24 ] = -2;

    if (!Template(lHeight, lWidth, lpDIBBits, lLineBytes, nTempWidth, nTempHeight,
        nTempCenX, nTempCenY, Tempdata, TempCoef)) {
      return FALSE;
    }

    SAFEFREE(Tempdata);
    return TRUE;
  }

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   Outline()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
* �ú������ڶ�ͼ�����������ȡ���㡣
* Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/

BOOL Outline(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;
  // ָ��Ҫ���������ָ��
  LPBYTE lpDst;
  // ָ����ͼ���ָ��
  LPBYTE lpImage;
  // ѭ������
  int i;
  int j;
  //��Ŵ������ذ���������ػҶ�
  BYTE n, e, s, w, ne, se, nw, sw;
  //����ֵ
  BYTE pixeltemp;
  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // �ж��Ƿ��ڴ����ʧ��
  if (lpImage == NULL) {
    // �����ڴ�ʧ��
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  lpDst = (LPBYTE) lpImage;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  //���ͼ���Ƕ�ֵͼ��ת��Ϊ��ֵͼ��
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {

      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

      //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixeltemp = (BYTE) * lpSrc;

      //Ŀ��ͼ���лҶ�ֵ����120������Ϊ255����������Ϊ0
      if (pixeltemp >= 120) {
        *lpSrc = (BYTE) 255;
      }
      else {
        *lpSrc = (BYTE) 0;
      }
    }
  }

  for (j = 1; j < lHeight - 1; j++) {
    for (i = 1; i < lWidth - 1; i++) {
      //Ϊ��ֹԽ�磬��ȣ��߶��������Ҹ�����һ��
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

      // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
      lpDst = (LPBYTE) lpImage + lWidth * j + i;

      //�Ǻڵ��������
      if (*(lpSrc) == 0) {
        //Ŀ��ͼ����Ӧ������Ҳ��Ӧ��Ϊ�ڵ�
        *(lpDst) = 0;
        w = (BYTE) * (lpSrc - 1);      //���ڵ�
        e = (BYTE) * (lpSrc + 1);      //���ڵ�
        nw = (BYTE) * (lpSrc + lWidth - 1);     //�����ڵ�
        n = (BYTE) * (lpSrc + lWidth);     //���ڵ�
        ne = (BYTE) * (lpSrc + lWidth + 1);     //�����ڵ�
        sw = (BYTE) * (lpSrc - lWidth - 1);     //�����ڵ�
        s = (BYTE) * (lpSrc - lWidth);     //���ڵ�
        se = (BYTE) * (lpSrc - lWidth + 1);     //�����ڵ�

        //������ڵİ˸��㶼�Ǻڵ�
        if (nw + n + ne + w + e + sw + s + se == 0) {
          //���ͼ�иúڵ�ɾ��
          *(lpDst) = (BYTE) 255;
        }
      }
    }
  }

  // ����������ȡ���ͼ��
  memcpy(lpDIBBits, lpImage, lWidth * lHeight);
  // �ͷ��ڴ�
  SAFEFREE(lpImage);
  // ����
  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   ContourTrace()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶ�ͼ����������������㡣
* Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ�񣬷����Զ�ת����
 ************************************************************************/

BOOL ContourTrace(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{

  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;
  // ָ��Ҫ���������ָ��
  LPBYTE lpDst;
  // ָ����ͼ���ָ��
  LPBYTE lpImage;
  // ѭ������
  int i;
  int j;
  //����ֵ
  BYTE pixeltemp;
  //�Ƿ��ҵ���ʼ�㼰�ص���ʼ��
  BOOL IsStartPoint;
  //�Ƿ�ɨ�赽һ���߽��
  BOOL Pointfind;
  //��ʼ�߽���뵱ǰ�߽��
  IPOINT StartPoint;
  IPOINT CurrentPoint;
  //�˸��������ʼɨ�跽��
  int Direction[ 8 ][ 2 ] = {{ -1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, { -1, -1}, { -1, 0}};
  int BeginDirect;

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // �ж��Ƿ��ڴ����ʧ��
  if (lpImage == NULL) {
    // �����ڴ�ʧ��
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  lpDst = (LPBYTE) lpImage;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  //���ͼ���Ƕ�ֵͼ��ת��Ϊ��ֵͼ��
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {

      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

      //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
      pixeltemp = (BYTE) * lpSrc;

      //Ŀ��ͼ���лҶ�ֵ����120������Ϊ255����������Ϊ0
      if (pixeltemp >= 120) {
        *lpSrc = (BYTE) 255;
      }
      else {
        *lpSrc = (BYTE) 0;
      }
    }
  }

  //���ҵ�ͼ�������·��ı߽�㣬ע�������·�
  IsStartPoint = FALSE;

  for (j = 1; j < lHeight && !IsStartPoint; j++) {
    for (i = 1; i < lWidth && !IsStartPoint; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

      //ȡ�õ�ǰָ�봦������ֵ
      pixeltemp = (BYTE) * lpSrc;

      //���Ϊ�ڵ㣬��Ϊ��ʼ��
      if (pixeltemp == 0) {
        IsStartPoint = TRUE;
        StartPoint.y = j;        //��ʼ��Ĵ�ֱλ��
        StartPoint.x = i;        //��ʼ���ˮƽλ��

        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = (LPBYTE) lpImage + lWidth * j + i;
        *lpDst = (BYTE) 0;          //Ŀ�������Ϊ�ڵ�
      }
    }
  }

  //������ʼ���������·�������ʼɨ�������Ϸ���
  BeginDirect = 0;
  //���ٱ߽�
  IsStartPoint = FALSE;
  //�ӳ�ʼ�㿪ʼɨ��
  CurrentPoint.y = StartPoint.y;
  CurrentPoint.x = StartPoint.x;

  while (!IsStartPoint) {
    Pointfind = FALSE;

    while (!Pointfind) {
      //��ɨ�跽��鿴��һ������
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (CurrentPoint.y + Direction[ BeginDirect ][ 1 ])
          + (CurrentPoint.x + Direction[ BeginDirect ][ 0 ]);
      //�����õ������ֵ
      pixeltemp = (BYTE) * lpSrc;

      //����õ�Ϊ�ڵ�
      if (pixeltemp == 0) {
        //�ҵ��˵㣬����Ϊ��
        Pointfind = TRUE;
        //��¼��ǰ���λ��
        CurrentPoint.y = CurrentPoint.y + Direction[ BeginDirect ][ 1 ];
        CurrentPoint.x = CurrentPoint.x + Direction[ BeginDirect ][ 0 ];

        //�����ǰ�������ʼ��
        if (CurrentPoint.y == StartPoint.y && CurrentPoint.x == StartPoint.x) {
          //��ʼ���������Ϊ�棬�˳�ѭ��
          IsStartPoint = TRUE;
        }

        //��¼��ǰ�㣬����Ϊ�ڵ�
        lpDst = (LPBYTE) lpImage + lWidth * CurrentPoint.y + CurrentPoint.x;
        *lpDst = (BYTE) 0;
        //ɨ��ķ�����ʱ����ת����
        BeginDirect--; //ѡ��һ��

        if (BeginDirect == -1) {   //�������Ϊ��1�����Ƿ���7
          BeginDirect = 7;
        }

        BeginDirect--;

        if (BeginDirect == -1) {     //�������Ϊ��1�����Ƿ���7
          BeginDirect = 7;
        }
      }
      //ɨ�跽�����һ����Ϊ�׵㣬��ô������Ҫ�ı�ɨ�跽�򣬼���Ѱ��
      else {
        //ɨ�跽��˳ʱ����תһ��
        BeginDirect++;

        if (BeginDirect == 8) {
          BeginDirect = 0;    //�������Ϊ8�����Ƿ���0
        }

      }

    }
  }

  // �����������ٺ��ͼ��
  memcpy(lpDIBBits, lpImage, lWidth * lHeight);
  // �ͷ��ڴ�
  SAFEFREE(lpImage);
  // ����
  return TRUE;
}
//ǿ��һ�㣬������ֻ���������ٵļ���ʾ����������ͼ������·����ҵ��ĵ�һ���ڵ�
//������Ϊ��ʼ�㣬�Դ˵�Ϊ�������б߽�ɨ�裬���ֻ�ܸ���һ���������������ж������
//������Ҫ����ʱ����Ҫ���Ѿ����ٹ�������ȥ���������������̡�



#define TEMPLATE_SMOOTH_BOX    1
#define TEMPLATE_SMOOTH_GAUSS  2
#define TEMPLATE_SHARPEN_LAPLACIAN 3
#define TEMPLATE_SOBEL_H 4
#define TEMPLATE_SOBEL_V 5
#define TEMPLATE_ISOSOBEL_H 6
#define TEMPLATE_ISOSOBEL_V 7

//template array
static const Float Template_Smooth_Box[ 9 ] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const Float Template_Smooth_Gauss[ 9 ] = {
  1.0f, 2.0f, 1.0f, 2.0f, 4.0f, 2.0f, 1.0f, 2.0f, 1.0f
};
static const Float Template_Sharpen_Laplacian[ 9 ] = {
  -1.0f, -1.0f, -1.0f, -1.0f, 9.0f, -1.0f, -1.0f, -1.0f, -1.0f
};
static const Float Template_HSobel[ 9 ] = {
  -1.0f, 0.0f, 1.0f, -2.0f, 0.0f, 2.0f, -1.0f, 0.0f, 1.0f
};
static const Float Template_VSobel[ 9 ] = {
  -1.0f, -2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 1.0f
};
static const Float Template_HIsoSobel[ 9 ] = {
  -1.0f, 0.0f, 1.0f, -1.4142f, 0.0f, 1.4142f, -1.0f, 0.0f, 1.0f
};
static const Float Template_VIsoSobel[ 9 ] = {
  -1.0f, -1.4142f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.4142f, 1.0f
};
static const Float Template_Log[ 25 ] = { //
  -2.0f, -4.0f, -4.0f, -4.0f, -2.0f,
  -4.0f, 0.0f, 8.0f, 0.0f, -4.0f,
  -4.0f, 8.0f, 24.0f, 8.0f, -4.0f,
  -4.0f, 0.0f, 8.0f, 0.0f, -4.0f,
  -2.0f, -4.0f, -4.0f, -4.0f, -2.0f
};
////////////////////////////////////////////////////////////////
BOOL TemplateOperation(LPBYTE lpImgData, int lWidth, int lHeight, int lLineBytes,
    LPBYTE lpTempImgData, int TemplateType)
{
  DWORD BufSize = lHeight * lLineBytes;
  LPBYTE lpPtr;
  LPBYTE lpTempPtr;
  int x, y;
  Float coef;
  Float CoefArray[ 9 ];
  Float TempNum;

  switch (TemplateType) {
  case TEMPLATE_SMOOTH_BOX:
    coef = (Float)(1.0 / 9.0);
    memcpy(CoefArray, Template_Smooth_Box, 9 * sizeof(Float));
    break;

  case TEMPLATE_SMOOTH_GAUSS:
    coef = (Float)(1.0 / 16.0);
    memcpy(CoefArray, Template_Smooth_Gauss, 9 * sizeof(Float));
    break;

  case TEMPLATE_SHARPEN_LAPLACIAN:
    coef = (Float) 1.0;
    memcpy(CoefArray, Template_Sharpen_Laplacian, 9 * sizeof(Float));
    break;

  case TEMPLATE_SOBEL_H:
    coef = (Float) 1.0;
    memcpy(CoefArray, Template_HSobel, 9 * sizeof(Float));
    break;

  case TEMPLATE_SOBEL_V:
    coef = (Float) 1.0;
    memcpy(CoefArray, Template_VSobel, 9 * sizeof(Float));
    break;

  case TEMPLATE_ISOSOBEL_H:
    coef = (Float) 1.0;
    memcpy(CoefArray, Template_HIsoSobel, 9 * sizeof(Float));
    break;

  case TEMPLATE_ISOSOBEL_V:
    coef = (Float) 1.0;
    memcpy(CoefArray, Template_VIsoSobel, 9 * sizeof(Float));
    break;
  }

  lpPtr = (LPBYTE) lpImgData;
  lpTempPtr = (LPBYTE) lpTempImgData;

  memcpy(lpTempPtr, lpPtr, BufSize);

  for (y = 1; y < lHeight - 1; y++) {
    for (x = 1; x < lWidth - 1; x++) {
      lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - y * lLineBytes) + x;
      lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - y * lLineBytes) + x;
      TempNum = (Float)((BYTE) * (lpPtr + lLineBytes - 1)) * CoefArray[ 0 ];
      TempNum += (Float)((BYTE) * (lpPtr + lLineBytes)) * CoefArray[ 1 ];
      TempNum += (Float)((BYTE) * (lpPtr + lLineBytes + 1)) * CoefArray[ 2 ];
      TempNum += (Float)((BYTE) * (lpPtr - 1)) * CoefArray[ 3 ];
      TempNum += (Float)((BYTE) * lpPtr) * CoefArray[ 4 ];
      TempNum += (Float)((BYTE) * (lpPtr + 1)) * CoefArray[ 5 ];
      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes - 1)) * CoefArray[ 6 ];
      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes)) * CoefArray[ 7 ];
      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes + 1)) * CoefArray[ 8 ];
      TempNum *= coef;

      if (TempNum > 255.0) {
        * lpTempPtr = (BYTE) 255;
      }
      else if (TempNum < 0.0) {
        * lpTempPtr = (BYTE) fabs(TempNum);
      }
      else {
        *lpTempPtr = (BYTE) TempNum;
      }
    }
  }

  return TRUE;
}

////////////////////////////////////////////////////////////////
BOOL Outline1(LPBYTE lpImgData, int lWidth, int lHeight, int lLineBytes, LPBYTE lpTempImgData)
{
  DWORD BufSize = lHeight * lLineBytes;
  LPBYTE lpPtr;
  LPBYTE lpTempPtr;
  int x, y, num;
  int nw, n, ne, w, e, sw, s, se;

  memcpy(lpTempImgData, lpImgData, BufSize);

  for (y = 1; y < lHeight - 1; y++) {
    lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - y * lLineBytes);
    lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - y * lLineBytes);

    for (x = 1; x < lWidth - 1; x++) {
      if (*(lpPtr + x) == 0) {
        nw = (BYTE) * (lpPtr + x + lLineBytes - 1);
        n = (BYTE) * (lpPtr + x + lLineBytes);
        ne = (BYTE) * (lpPtr + x + lLineBytes + 1);
        w = (BYTE) * (lpPtr + x - 1);
        e = (BYTE) * (lpPtr + x + 1);
        sw = (BYTE) * (lpPtr + x - lLineBytes - 1);
        s = (BYTE) * (lpPtr + x - lLineBytes);
        se = (BYTE) * (lpPtr + x - lLineBytes + 1);
        num = nw + n + ne + w + e + sw + s + se;

        if (num == 0) {
          * (lpTempPtr + x) = (BYTE) 255;
        }
      }
    }
  }

  return TRUE;
}

////////////////////////////////////////////////////////////////
BOOL Hough1(LPBYTE lpImgData, int lWidth, int lHeight, int lLineBytes)
{
  typedef struct {
    int topx;
    int topy;
    int botx;
    int boty;
  }
  MYLINE;
  DWORD BufSize = lHeight * lLineBytes;
  int x, y, i, maxd;
  int k, Dist, Alpha, *lpDistAlpha;
  LPBYTE lpPtr;
  MYLINE* lpMyLine, *TempLine, MaxdLine;
  //static LOGPEN rlp={PS_SOLID, 1, 1, RGB(255, 0, 0)};

  Dist = (int)(sqrt((Float) lWidth * lWidth + (Float) lHeight * lHeight) + 0.5);
  Alpha = 180 / 2; //0 degree to 178 degree , step is 2 degrees

  if ((lpDistAlpha = MALLOC(int, Dist * Alpha)) == NULL) {
    print_err("Error alloc memory!", "Error Message");
    return FALSE;
  }

  if ((lpMyLine = MALLOC(MYLINE, Dist * Alpha)) == NULL) {
    SAFEFREE(lpDistAlpha);
    return FALSE;
  }

  for (i = 0; i < (int) Dist * Alpha; i++) {
    TempLine = (MYLINE*)(lpMyLine + i);
    (*TempLine).boty = 32767;
  }

  for (y = 0; y < lHeight; y++) {
    lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - y * lLineBytes);

    for (x = 0; x < lWidth; x++) {
      if (*(lpPtr++) == 0) {
        for (k = 0; k < 180; k += 2) {
          i = (int) fabs((x * cos(k * PI / 180.0) + y * sin(k * PI / 180.0)));
          *(lpDistAlpha + i * Alpha + k / 2) = *(lpDistAlpha + i * Alpha + k / 2) + 1;
          TempLine = (MYLINE*)(lpMyLine + i * Alpha + k / 2);

          if (y > (*TempLine).topy) {
            (*TempLine).topx = x;
            (*TempLine).topy = y;
          }

          if (y < (*TempLine).boty) {
            (*TempLine).botx = x;
            (*TempLine).boty = y;
          }
        }
      }
    }
  }

  maxd = 0;

  for (i = 0; i < (int) Dist * Alpha; i++) {
    TempLine = (MYLINE*)(lpMyLine + i);
    k = *(lpDistAlpha + i);

    if (k > maxd) {
      maxd = k;
      MaxdLine.topx = (*TempLine).topx;
      MaxdLine.topy = (*TempLine).topy;
      MaxdLine.botx = (*TempLine).botx;
      MaxdLine.boty = (*TempLine).boty;
    }
  }

  SAFEFREE(lpDistAlpha);
  SAFEFREE(lpMyLine);
  return TRUE;
}
////////////////////////////////////////////////////////////////
BOOL LapOfGauss(LPBYTE lpImgData, int lWidth, int lHeight, int lLineBytes, LPBYTE lpTempImgData)
{
  LPBYTE lpPtr;
  LPBYTE lpTempPtr;
  int x, y, BufSize = lHeight * lLineBytes;
  Float coef;
  Float TempNum;

  coef = (Float)(1.0);

  lpPtr = (LPBYTE) lpImgData;
  lpTempPtr = (LPBYTE) lpTempImgData;

  memcpy(lpTempPtr, lpPtr, BufSize);

  for (y = 2; y < lHeight - 2; y++) {
    for (x = 2; x < lWidth - 2; x++) {
      lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - y * lLineBytes) + x;
      lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - y * lLineBytes) + x;
      TempNum = (Float)((BYTE) * (lpPtr + 2 * lLineBytes - 2)) * Template_Log[ 0 ];
      TempNum += (Float)((BYTE) * (lpPtr + 2 * lLineBytes - 1)) * Template_Log[ 1 ];
      TempNum += (Float)((BYTE) * (lpPtr + 2 * lLineBytes)) * Template_Log[ 2 ];
      TempNum += (Float)((BYTE) * (lpPtr + 2 * lLineBytes + 1)) * Template_Log[ 3 ];
      TempNum += (Float)((BYTE) * (lpPtr + 2 * lLineBytes + 2)) * Template_Log[ 4 ];

      TempNum += (Float)((BYTE) * (lpPtr + lLineBytes - 2)) * Template_Log[ 5 ];
      TempNum += (Float)((BYTE) * (lpPtr + lLineBytes - 1)) * Template_Log[ 6 ];
      TempNum += (Float)((BYTE) * (lpPtr + lLineBytes)) * Template_Log[ 7 ];
      TempNum += (Float)((BYTE) * (lpPtr + lLineBytes + 1)) * Template_Log[ 8 ];
      TempNum += (Float)((BYTE) * (lpPtr + lLineBytes + 2)) * Template_Log[ 9 ];

      TempNum += (Float)((BYTE) * (lpPtr - 2)) * Template_Log[ 10 ];
      TempNum += (Float)((BYTE) * (lpPtr - 1)) * Template_Log[ 11 ];
      TempNum += (Float)((BYTE) * (lpPtr)) * Template_Log[ 12 ];
      TempNum += (Float)((BYTE) * (lpPtr + 1)) * Template_Log[ 13 ];
      TempNum += (Float)((BYTE) * (lpPtr + 2)) * Template_Log[ 14 ];

      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes - 2)) * Template_Log[ 15 ];
      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes - 1)) * Template_Log[ 16 ];
      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes)) * Template_Log[ 17 ];
      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes + 1)) * Template_Log[ 18 ];
      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes + 2)) * Template_Log[ 19 ];

      TempNum += (Float)((BYTE) * (lpPtr - 2 * lLineBytes - 2)) * Template_Log[ 20 ];
      TempNum += (Float)((BYTE) * (lpPtr - 2 * lLineBytes - 1)) * Template_Log[ 21 ];
      TempNum += (Float)((BYTE) * (lpPtr - 2 * lLineBytes)) * Template_Log[ 22 ];
      TempNum += (Float)((BYTE) * (lpPtr - 2 * lLineBytes + 1)) * Template_Log[ 23 ];
      TempNum += (Float)((BYTE) * (lpPtr - 2 * lLineBytes + 2)) * Template_Log[ 24 ];

      TempNum *= coef;

      if (TempNum > 255.0) {
        * lpTempPtr = (BYTE) 255;
      }
      else if (TempNum < 0.0) {
        * lpTempPtr = (BYTE) fabs(TempNum);
      }
      else {
        *lpTempPtr = (BYTE) TempNum;
      }
    }
  }

  return TRUE;
}

////////////////////////////////////////////////////////////////
BOOL IsContourP(int lLineBytes, LPBYTE lpPtr)
{
  int num, n, w, e, s;

  n = (BYTE) * (lpPtr + lLineBytes);
  w = (BYTE) * (lpPtr - 1);
  e = (BYTE) * (lpPtr + 1);
  s = (BYTE) * (lpPtr - lLineBytes);
  num = n + w + e + s;

  if (num == 0) {
    return FALSE;
  }

  return TRUE;
}

////////////////////////////////////////////////////////////////
BOOL ContourTemp(LPBYTE lpImgData, int lWidth, int lHeight, int lLineBytes, LPBYTE lpTempImgData)
{
  LPBYTE lpPtr;
  LPBYTE lpTempPtr;
  int x, y, BufSize = lHeight * lLineBytes;
  IPOINT StartP, CurP;
  BOOL found;
  int i;
  int direct[ 8 ][ 2 ] = {{1, 0}, {1, -1}, {0, -1}, { -1, -1}, { -1, 0}, { -1, 1}, {0, 1}, {1, 1}};

  memset(lpTempImgData, (BYTE) 255, BufSize);
  //memcpy(lpTempImgData, lpImgData, OffBits);

  found = FALSE;

  for (y = 0; y < lHeight && !found; y++) {
    lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - y * lLineBytes);

    for (x = 0; x < lWidth && !found; x++)
      if (*(lpPtr++) == 0) {
        found = TRUE;
      }
  }

  if (found) {
    StartP.x = x - 1;
    StartP.y = y - 1;
    lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - StartP.y * lLineBytes) + StartP.x;
    *lpTempPtr = (BYTE) 0;
    CurP.x = StartP.x + 1;
    CurP.y = StartP.y;
    lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - CurP.y * lLineBytes) + CurP.x;

    if (*lpPtr != 0) {
      CurP.x = StartP.x + 1;
      CurP.y = StartP.y + 1;
      lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - CurP.y * lLineBytes) + CurP.x;

      if (*lpPtr != 0) {
        CurP.x = StartP.x;
        CurP.y = StartP.y + 1;
      }
      else {
        CurP.x = StartP.x - 1;
        CurP.y = StartP.y + 1;
      }
    }

    while (!((CurP.x == StartP.x) && (CurP.y == StartP.y))) {
      lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - CurP.y * lLineBytes) + CurP.x;
      *lpTempPtr = (BYTE) 0;

      for (i = 0; i < 8; i++) {
        x = CurP.x + direct[ i ][ 0 ];
        y = CurP.y + direct[ i ][ 1 ];
        lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - y * lLineBytes) + x;
        lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - y * lLineBytes) + x;

        if (((*lpPtr == 0) && (*lpTempPtr != 0)) || ((x == StartP.x) && (y == StartP.y)))
          if (IsContourP(lLineBytes, lpPtr)) {
            CurP.x = x;
            CurP.y = y;
            break;
          }
      }
    }
  }

  return TRUE;
}

//owner defined stack
typedef struct MYSTACK {
  int ElementsNum;
  int ptr;
  IPOINT* lpMyStack;
}
MYSTACK;

////////////////////////////////////////////////////////////////
BOOL InitStack(MYSTACK SeedFillStack, int StackLen)
{
  SeedFillStack.ElementsNum = StackLen;

  if ((SeedFillStack.lpMyStack = MALLOC(IPOINT, SeedFillStack.ElementsNum)) == NULL) {
    print_err("Error alloc memory!", "ErrorMessage");
    return FALSE;
  }

  memset(SeedFillStack.lpMyStack, 0, SeedFillStack.ElementsNum * sizeof(IPOINT));
  SeedFillStack.ptr = 0;
  return TRUE;
}
////////////////////////////////////////////////////////////////
void DeInitStack(MYSTACK SeedFillStack)
{
  SAFEFREE(SeedFillStack.lpMyStack);
  SeedFillStack.ElementsNum = 0;
  SeedFillStack.ptr = 0;
}
////////////////////////////////////////////////////////////////
BOOL MyPush(MYSTACK SeedFillStack, IPOINT p)
{
  IPOINT* TempPtr;

  if (SeedFillStack.ptr >= SeedFillStack.ElementsNum) {
    return FALSE;
  }

  TempPtr = (IPOINT*)(SeedFillStack.lpMyStack + SeedFillStack.ptr++);
  (*TempPtr).x = p.x;
  (*TempPtr).y = p.y;
  return TRUE;
}
////////////////////////////////////////////////////////////////
IPOINT MyPop(MYSTACK SeedFillStack)
{
  IPOINT InvalidP;
  InvalidP.x = -1;
  InvalidP.y = -1;

  if (SeedFillStack.ptr <= 0) {
    return InvalidP;
  }

  SeedFillStack.ptr--;
  return *(SeedFillStack.lpMyStack + SeedFillStack.ptr);
}
////////////////////////////////////////////////////////////////
#define IsStackEmpty(SeedFillStack) (((SeedFillStack).ptr==0) ? TRUE : FALSE)

////////////////////////////////////////////////////////////////
BOOL SeedFill(LPBYTE lpImgData, int lWidth, int lHeight, int lLineBytes, LPBYTE lpTempImgData, IPOINT SeedPoint)
{
  DWORD BufSize = lHeight * lLineBytes;
  IPOINT CurP, NeighborP;
  LPBYTE lpTempPtr, lpTempPtr1;
  MYSTACK SeedFillStack = {0};

  //copy image data
  memcpy(lpTempImgData, lpImgData, BufSize);

  if (!InitStack(SeedFillStack, (int) lHeight * lWidth)) {
    return FALSE;
  }

  lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - SeedPoint.y * lLineBytes) + SeedPoint.x;

  if (*lpTempPtr == 0) {
    DeInitStack(SeedFillStack);
    return FALSE;
  }

  MyPush(SeedFillStack, SeedPoint);

  while (!IsStackEmpty(SeedFillStack)) {
    CurP = MyPop(SeedFillStack);
    lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - CurP.y * lLineBytes) + CurP.x;
    *lpTempPtr = (BYTE) 0;

    //left neighbour
    if (CurP.x > 0) {
      NeighborP.x = CurP.x - 1;
      NeighborP.y = CurP.y;
      lpTempPtr1 = lpTempPtr - 1;

      if (*lpTempPtr1 != 0) {
        MyPush(SeedFillStack, NeighborP);
      }
    }

    //up neighbour
    if (CurP.y > 0) {
      NeighborP.x = CurP.x;
      NeighborP.y = CurP.y - 1;
      lpTempPtr1 = lpTempPtr + lLineBytes;

      if (*lpTempPtr1 != 0) {
        MyPush(SeedFillStack, NeighborP);
      }
    }

    //right neighbour
    if (CurP.x < lWidth - 1) {
      NeighborP.x = CurP.x + 1;
      NeighborP.y = CurP.y;
      lpTempPtr1 = lpTempPtr + 1;

      if (*lpTempPtr1 != 0) {
        MyPush(SeedFillStack, NeighborP);
      }
    }

    //down neighbour
    if (CurP.y < lHeight - 1) {
      NeighborP.x = CurP.x;
      NeighborP.y = CurP.y + 1;
      lpTempPtr1 = lpTempPtr - lLineBytes;

      if (*lpTempPtr1 != 0) {
        MyPush(SeedFillStack, NeighborP);
      }
    }
  }

  DeInitStack(SeedFillStack);

  return TRUE;
}
