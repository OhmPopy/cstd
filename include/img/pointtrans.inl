
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

//////////////////////////////////////////////////////////////////////
// ͼ������㺯��
//////////////////////////////////////////////////////////////////////

/*************************************************************************
* \�������ƣ�
*   GrayLinTrans()
* \�������:
*     LPBYTE lpDIBBits        //ָ��Դͼ�������ָ��
*     int lWidth        //Դͼ��Ŀ��
*     int lHeight        // Դͼ��ĸ߶�
*   int  oralow            // ԭʼͼ��ĻҶ�ֵ�������򣭵�ֵ
*   int  orahig            // ԭʼͼ��ĻҶ�ֵ�������򣭸�ֵ
*   int  newlow            // �任��ͼ��ҶȵĶ�̬��Χ����ֵ
*   int  newhig            // �任��ͼ��ҶȵĶ�̬��Χ����ֵ
* \����ֵ:
*   BOOL               // �ɹ�����TRUE�����򷵻�FALSE��
* \˵��:
*   �ú���������ͼ��ָ���ĻҶȷֲ����лҶȱ任��Ҫ����ʵ�ֻҶȷֲ�����չ
*************************************************************************/
BOOL GrayLinTrans(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int oralow, int orahig, int newlow, int newhig)
{
  LPBYTE lpSrc;                        // ָ��Դͼ���ָ��
  int i, j;                            // ѭ������
  BYTE byMap[ 256 ];                  // ����Ҷ�ӳ���

  for (i = 0; i <= oralow; i++) {         //���Ҷȼ�С��Ҫ��ǿ�ĻҶȼ�ʱ
    if (newlow > 0) {   //�ж��»Ҷȼ��Ƿ����0
      byMap[ i ] = newlow;               //������ֵ
    }
    else {
      byMap[ i ] = 0;                   //ֱ�Ӹ�ֵΪ0
    }
  }

  for (; i <= orahig; i++) {
    // �ж�orahig�Ƿ����oralow(��ֹ��ĸΪ0)
    if (orahig != oralow) {
      //���Ա任
      byMap[ i ] = newlow + (BYTE)((newhig - newlow) * (i - oralow) / (orahig - oralow));
    }
    else {
      byMap[ i ] = newlow;    //ֱ�Ӹ�ֵΪnewlow
    }
  }

  for (; i < newhig; i++) {
    if (newhig <= 255) {         //�ж�d�Ƿ����255
      byMap[ i ] = newhig;    //ֱ�Ӹ�ֵΪnewhig
    }
    else {
      byMap[ i ] = 255;        //ֱ�Ӹ�ֵΪ255
    }
  }

  for (i = 0; i < lHeight; i++) {   //��ͼ���ÿ������ֵ���б任
    for (j = 0; j < lWidth; j++) {   //ÿ��
      // ָ��DIB��i�У���j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - i) + j;
      *lpSrc = byMap[ *lpSrc ];   //���µĻҶ����ԭ�еĻҶ�
    }
  }

  return TRUE;     //����
}


/*************************************************************************
* �������ƣ�
*   LinerTrans()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������
*   int lHeight      - Դͼ��߶ȣ���������
*   Float fA      - ���Ա任��б��
*   Float fB           - ���Ա任�Ľؾ�
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú���������ͼ����лҶȵ����Ա任������
 ************************************************************************/
BOOL LinerTrans(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    Float fA, Float fB)
{
  LPBYTE lpSrc;  // ָ��Դͼ���ָ��
  int i;     // ѭ������
  int j;
  Float fTemp;    // �м����

  for (i = 0; i < lHeight; i++) {    // ÿ��
    for (j = 0; j < lWidth; j++) {    // ÿ��
      // ָ��DIB��i�У���j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      fTemp = fA * (*lpSrc) + fB;    // ���Ա任

      if (fTemp > 255) {     // �ж��Ƿ񳬳���Χ
        *lpSrc = 255;
      }
      else if (fTemp < 0) {
        *lpSrc = 0;
      }
      else {
        *lpSrc = (BYTE)(fTemp + 0.5);      // ��������
      }
    }
  }

  return TRUE;
}

/*************************************************************************
 * �������ƣ�
 *   Threshold()
 *
 * ����:
 *   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   int lWidth       - Դͼ���ȣ���������
 *   int lHeight      - Դͼ��߶ȣ���������
 *
 * ����ֵ:
 *   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ�������ֵ�ָ����㡣
 ************************************************************************/

BOOL Threshold(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;       // ָ��Դͼ���ָ��
  LPBYTE lpDst;       // ָ�򻺴�ͼ���ָ��
  LPBYTE lpNewDIBBits; // ָ�򻺴�DIBͼ���ָ��
  int i, j; //ѭ������
  BYTE pixel; //����ֵ
  int lHistogram[ 256 ]; //ֱ��ͼ����
  int iIterationTimes; //��������

  //��ֵ�����Ҷ�ֵ����С�Ҷ�ֵ�����������ƽ���Ҷ�ֵ
  BYTE iThreshold, iNewThreshold, iMaxGrayValue, iMinGrayValue, iMean1GrayValue, iMean2GrayValue;

  //���ڼ�������Ҷ�ƽ��ֵ���м����
  int lP1, lP2, lS1, lS2;

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    // �����ڴ�ʧ��
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  lpDst = (LPBYTE) lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  for (i = 0; i < 256; i++) {
    lHistogram[ i ] = 0;
  }

  //���ֱ��ͼ
  iMaxGrayValue = 0;
  iMinGrayValue = 255;

  for (i = 0; i < lWidth; i++) {
    for (j = 0; j < lHeight; j++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      lHistogram[ pixel ] ++;

      //�޸������С�Ҷ�ֵ
      if (iMinGrayValue > pixel) {
        iMinGrayValue = pixel;
      }

      if (iMaxGrayValue < pixel) {
        iMaxGrayValue = pixel;
      }
    }
  }

  //�����������ֵ
  iNewThreshold = (iMinGrayValue + iMaxGrayValue) / 2;
  iThreshold = 0;

  for (iIterationTimes = 0; iThreshold != iNewThreshold && iIterationTimes < 100; iIterationTimes ++) {
    iThreshold = iNewThreshold;
    lP1 = 0;
    lP2 = 0;
    lS1 = 0;
    lS2 = 0;

    //����������ĻҶ�ƽ��ֵ
    for (i = iMinGrayValue; i < iThreshold; i++) {
      lP1 += lHistogram[ i ] * i;
      lS1 += lHistogram[ i ];
    }

    iMean1GrayValue = (BYTE)(lP1 / lS1);

    for (i = iThreshold + 1; i < iMaxGrayValue; i++) {
      lP2 += lHistogram[ i ] * i;
      lS2 += lHistogram[ i ];
    }

    iMean2GrayValue = (BYTE)(lP2 / lS2);
    iNewThreshold = (iMean1GrayValue + iMean2GrayValue) / 2;
  }

  //������ֵ��ͼ���ֵ��
  for (i = 0; i < lWidth; i++) {
    for (j = 0; j < lHeight; j++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
      lpDst = (LPBYTE) lpNewDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      if (pixel <= iThreshold) {
        *lpDst = (BYTE) 0;
      }
      else {
        *lpDst = (BYTE) 255;
      }
    }
  }

  // ����ͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  // �ͷ��ڴ�
  SAFEFREE(lpNewDIBBits);

  // ����
  return TRUE;
}

/*************************************************************************
 * �������ƣ�
 *   ThresholdTrans()
 * ����:
 *   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   int lWidth       - Դͼ���ȣ���������
 *   int lHeight      - Դͼ��߶ȣ���������
 *   BYTE  bThre     - ��ֵ
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 * ˵��:
 *   �ú���������ͼ�������ֵ�任�����ڻҶ�ֵС����ֵ������ֱ������
 * �Ҷ�ֵΪ0���Ҷ�ֵ������ֵ������ֱ������Ϊ255��
 ************************************************************************/
BOOL ThresholdTrans(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, BYTE bThre)
{
  LPBYTE lpSrc;    // ָ��Դͼ���ָ��
  int i;       // ѭ������
  int j;

  for (i = 0; i < lHeight; i++) {   // ÿ��
    for (j = 0; j < lWidth; j++) {   // ÿ��
      // ָ��DIB��i�У���j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      if ((*lpSrc) < bThre) {     // �ж��Ƿ�С����ֵ
        *lpSrc = 0;
      }
      else {
        *lpSrc = 255;
      }
    }
  }

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   WindowTrans()
* ����:
*   LPBYTE lpDIBBits   - ָ��ԴDIBͼ��ָ��
*   int lWidth         - Դͼ���ȣ���������
*   int lHeight        - Դͼ��߶ȣ���������
*   BYTE  bLow         - ��������
*   BYTE  bUp          - ��������
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú���������ͼ����д��ڱ任��ֻ���ڴ��ڷ�Χ�ڵĻҶȱ��ֲ��䣬
* С�����޵�����ֱ�����ûҶ�ֵΪ0���������޵�����ֱ�����ûҶ�ֵΪ255��
 ************************************************************************/
BOOL WindowTrans(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    BYTE bLow, BYTE bUp)
{
  LPBYTE lpSrc;    // ָ��Դͼ���ָ��
  int i;       // ѭ������
  int j;

  for (i = 0; i < lHeight; i++) {   // ÿ��
    for (j = 0; j < lWidth; j++) {   // ÿ��
      // ָ��DIB��i�У���j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      if ((*lpSrc) < bLow) {     // �ж��Ƿ񳬳���Χ
        *lpSrc = 0;
      }
      else if ((*lpSrc) > bUp) {
        *lpSrc = 255;
      }
    }
  }

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   GrayStretch()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������
*   int lHeight      - Դͼ��߶ȣ���������
*   BYTE bX1   - �Ҷ������һ�����X����
*   BYTE bY1   - �Ҷ������һ�����Y����
*   BYTE bX2   - �Ҷ�����ڶ������X����
*   BYTE bY2   - �Ҷ�����ڶ������Y����
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú���������ͼ����лҶ����졣
 ************************************************************************/
BOOL GrayStretch(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2)
{
  LPBYTE lpSrc;    // ָ��Դͼ���ָ��
  int i;       // ѭ������
  int j;
  BYTE bMap[ 256 ];     // �Ҷ�ӳ���

  // ����Ҷ�ӳ���
  for (i = 0; i <= bX1; i++) {
    if (bX1 > 0) {      // �ж�bX1�Ƿ����0����ֹ��ĸΪ0��
      bMap[ i ] = (BYTE) bY1 * i / bX1;
    }
    else {
      bMap[ i ] = 0;
    }
  }

  for (; i <= bX2; i++) {
    if (bX2 != bX1) {      // �ж�bX1�Ƿ����bX2����ֹ��ĸΪ0��
      bMap[ i ] = bY1 + (BYTE)((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
    }
    else {
      bMap[ i ] = bY1;
    }
  }

  for (; i < 256; i++) {
    if (bX2 != 255) {      // �ж�bX2�Ƿ����255����ֹ��ĸΪ0��
      bMap[ i ] = bY2 + (BYTE)((255 - bY2) * (i - bX2) / (255 - bX2));
    }
    else {
      bMap[ i ] = 255;
    }
  }

  for (i = 0; i < lHeight; i++) {   // ÿ��
    for (j = 0; j < lWidth; j++) {   // ÿ��
      // ָ��DIB��i�У���j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      // �����µĻҶ�ֵ
      *lpSrc = bMap[ *lpSrc ];
    }
  }

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   InteEqualize()
* ����:
*   LPBYTE lpDIBBits   - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������
*   int lHeight      - Դͼ��߶ȣ���������
*   int lLineBytes   - ͼ��ÿ�е��ֽ���
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú���������ͼ�����ֱ��ͼ���⡣
 ************************************************************************/
BOOL InteEqualize(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;    // ָ��Դͼ���ָ��
  int lTemp;      // ��ʱ����
  int i;       // ѭ������
  int j;
  BYTE bMap[ 256 ];     // �Ҷ�ӳ���
  int lCount[ 256 ];    // �Ҷ�ӳ���

  for (i = 0; i < 256; i ++) {   // ���ü���Ϊ0
    lCount[ i ] = 0;
  }

  // ��������Ҷ�ֵ�ļ���
  for (i = 0; i < lHeight; i ++) {
    for (j = 0; j < lWidth; j ++) {
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * i + j;
      lCount[ *(lpSrc) ] ++;
    }
  }

  // ����Ҷ�ӳ���
  for (i = 0; i < 256; i++) {
    lTemp = 0;

    for (j = 0; j <= i; j++) {
      lTemp += lCount[ j ];
    }

    // �����Ӧ���»Ҷ�ֵ
    bMap[ i ] = (BYTE)(lTemp * 255 / lHeight / lWidth);
  }

  for (i = 0; i < lHeight; i++) {    // ÿ��
    for (j = 0; j < lWidth; j++) {    // ÿ��
      // ָ��DIB��i�У���j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      // �����µĻҶ�ֵ
      *lpSrc = bMap[ *lpSrc ];
    }
  }

  return TRUE;
}

/*************************************************************************
* �������ƣ�LineTranslation(LPBYTE lpSrcStartBits, int lWidth, int lHeight, int lLineBytes, Float fSlope, Float fIntercept, int lLineBytes)
* ��������:
*   LPBYTE lpSrcStartBits��ָ��ԴDIBͼ����ʼ���ص�ָ��
*   int lWidth��DIBͼ����
*   int lHeight��DIBͼ��߶�
*   Float fSlope��б��
*   Float fIntercept���ؾ�
*   intlLineBytes��DIBͼ������ֽ�����Ϊ4�ı���
* ��������:BOOL
* ��������:�ú���������ͼ����лҶȵ����Ա任����
 ************************************************************************/
BOOL LineTranslation(LPBYTE lpSrcStartBits, int lWidth, int lHeight, int lLineBytes, Float fSlope, Float fIntercept)
{
  int i;                 //��ѭ������
  int j;                 //��ѭ������
  Float varFloat;        //�����������
  LPBYTE lpSrcUnChr;    //ָ�����ص�ָ��

  for (i = 0; i < lHeight; i++) { // ��
    for (j = 0; j < lWidth; j++) { // ��
      lpSrcUnChr = (LPBYTE) lpSrcStartBits + lLineBytes * (lHeight - 1 - i) + j;     // ָ��DIB��i�У���j�����ص�ָ��
      varFloat = fSlope * (*lpSrcUnChr) + fIntercept;   // ���Ա任

      if (varFloat > 255) { // ������Χֱ�Ӹ�ֵΪ255
        *lpSrcUnChr = 255;
      }
      else if (varFloat < 0) {
        *lpSrcUnChr = 0; // ֱ�Ӹ�ֵΪ0
      }
      else {
        *lpSrcUnChr = (BYTE)(varFloat + 0.5);      // ����
      }
    }
  }

  return TRUE;
}

/*************************************************************************
* �������ƣ�LogTranslation(LPBYTE lpSrcStartBits, int lWidth, int lHeight, int lLineBytes)
* ��������:
     LPBYTE lpSrcStartBits, ָ��DIB��ʼ���ص�ָ��
     int   lWidth��DIBͼ��Ŀ��
     int   lHeight��DIBͼ��ĸ߶�
     int   lLineBytes��DIBͼ������ֽ�����Ϊ4�ı���
* ��������:BOOL
* ��������:�ú���������ͼ����ж����任
 ************************************************************************/

BOOL LogTranslation(LPBYTE lpSrcStartBits, int lWidth, int lHeight, int lLineBytes)
{
  int i;                 //��ѭ������
  int j;                 //��ѭ������
  LPBYTE lpSrcUnChr;    //ָ�����ص�ָ��

  for (i = 0; i < lHeight; i++) { // ��
    for (j = 0; j < lWidth; j++) { // ��
      lpSrcUnChr = (LPBYTE) lpSrcStartBits + lLineBytes * (lHeight - 1 - i) + j;     // ָ��DIB��i�У���j�����ص�ָ��
      *lpSrcUnChr = 50 * (BYTE) log((*lpSrcUnChr + 1));
    }
  }

  return TRUE;
}
