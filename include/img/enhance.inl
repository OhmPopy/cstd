
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include "macro.h"
//#include "imglib.h"

/*************************************************************************
* �������ƣ�
*   GradSharp()
* ����:
*   uchar* pBits    - ָ��ԴDIBͼ��ָ��
*   int w       - Դͼ���ȣ���������
*   int h      - Դͼ��߶ȣ���������
*   uchar  bThre  - ��ֵ
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú���������ͼ������ݶ��񻯡�
 ************************************************************************/
static BOOL GradSharp(int h, int w, uchar* pBits, int lLineBytes, uchar bThre)
{
  uchar* pSrc;  // ָ��Դͼ���ָ��
  uchar* lpSrc1;
  uchar* lpSrc2;
  int i, j;    // ѭ������
  uchar bTemp;

  for (i = 0; i < h; i++) {   // ÿ��
    for (j = 0; j < w; j++) {   // ÿ��
      // ָ��DIB��i�У���j�����ص�ָ��
      pSrc = (uchar*) pBits + lLineBytes * (h - 1 - i) + j;
      // ָ��DIB��i+1�У���j�����ص�ָ��
      lpSrc1 = (uchar*) pBits + lLineBytes * (h - 2 - i) + j;
      // ָ��DIB��i�У���j+1�����ص�ָ��
      lpSrc2 = (uchar*) pBits + lLineBytes * (h - 1 - i)
          + j + 1;

      bTemp = abs((*pSrc) - (*lpSrc1)) + abs((*pSrc) - (*lpSrc2));

      // �ж��Ƿ�С����ֵ
      if (bTemp < 255) {
        // �ж��Ƿ������ֵ������С��������Ҷ�ֵ���䡣
        if (bTemp >= bThre) {
          *pSrc = bTemp;
        }
      }
      else {
        *pSrc = 255;
      }
    }
  }

  return TRUE;
}

// ֱ��ͼ���⻯
static BOOL HistogramEqualize(int h, int w, uchar* pBits, int lLineBytes)
{
  // ��ʱ����
  int nTemp;

  // ѭ������
  int i, j;

  // �ۻ�ֱ��ͼ�����Ҷ�ӳ���
  uchar byMap[ 256 ];

  // ֱ��ͼ
  int nCount[ 256 ] = {0};
  int sumcnt[ 256 ] = {0};

  // ��������Ҷ�ֵ�ļ��������õ�ֱ��ͼ
  for (i = 0; i < h; i ++) {
    uchar* pSrc = pBits + lLineBytes * i;
    for (j = 0; j < w; j ++) {
      // ������1
      nCount[ pSrc[j] ] ++;
    }
  }

  sumcnt[0] = nCount[0];

  // �����ۻ�ֱ��ͼ
  for (i = 0; i < 256; i++) {
    nTemp = sumcnt[i] = sumcnt[i-1] + nCount[ i ];
    // �����Ӧ���»Ҷ�ֵ
    byMap[ i ] = (uchar)(nTemp * 255 / h / w);
  }

  // ÿ��
  for (i = 0; i < h; i++) {
    uchar* pSrc = pBits + lLineBytes * i;
    for (j = 0; j < w; j++) {
      // �����µĻҶ�ֵ
      pSrc[j] = byMap[ pSrc[j] ];
    }
  }

  // ����
  return TRUE;
}

/*************************************************************************
 *
* \�������ƣ�
*   GraySegLinTrans()
 *
* \�������:
*   int h, int w, uchar* pBits, int lLineBytes            - ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
*   int  nX1            - �ֶ����ԻҶȱ任��һ���յ��X����
*   int  nY1            - �ֶ����ԻҶȱ任��һ���յ��Y����
*   int  nX2            - �ֶ����ԻҶȱ任�ڶ����յ��X����
*   int  nY2            - �ֶ����ԻҶȱ任�ڶ����յ��Y����
 *
* \����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
* \˵��:
*   �ú���������ͼ����зֶ����ԻҶȱ任����������а����������յ������
 *
 *************************************************************************
 */
static BOOL GraySegLinTrans(int h, int w, uchar* pBits, int lLineBytes, int nX1, int nY1, int nX2, int nY2)
{

  // ָ��Դͼ���ָ��
  uchar* pSrc;

  // ѭ������
  int i, j;

  // �Ҷ�ӳ���
  uchar byMap[ 256 ];

  // ͼ��ÿ�е��ֽ���
  //int   lLineBytes;

  // ����ͼ��ÿ�е��ֽ���
  //lLineBytes = WIDTHBYTES(w* 8);

  // ����Ҷ�ӳ���
  for (i = 0; i <= nX1; i++) {
    // �ж�nX1�Ƿ����0����ֹ��ĸΪ0��
    if (nX1 > 0) {
      // ���Ա任
      byMap[ i ] = (uchar) nY1 * i / nX1;
    }
    else {
      // ֱ�Ӹ�ֵΪ0
      byMap[ i ] = 0;
    }
  }

  for (; i <= nX2; i++) {
    // �ж�nX1�Ƿ����nX2����ֹ��ĸΪ0��
    if (nX2 != nX1) {
      // ���Ա任
      byMap[ i ] = nY1 + (uchar)((nY2 - nY1) * (i - nX1) / (nX2 - nX1));
    }
    else {
      // ֱ�Ӹ�ֵΪnY1
      byMap[ i ] = nY1;
    }
  }

  for (; i < 256; i++) {
    // �ж�nX2�Ƿ����255����ֹ��ĸΪ0��
    if (nX2 != 255) {
      // ���Ա任
      byMap[ i ] = nY2 + (uchar)((255 - nY2) * (i - nX2) / (255 - nX2));
    }
    else {
      // ֱ�Ӹ�ֵΪ255
      byMap[ i ] = 255;
    }
  }

  // ��ͼ�������ֵ���б任
  // ÿ��
  for (i = 0; i < h; i++) {
    // ÿ��
    for (j = 0; j < w; j++) {
      // ָ��DIB��i�У���j�����ص�ָ�� (i*lLineBytes+j)
      pSrc = (uchar*) pBits + (i * lLineBytes + j);

      // �����µĻҶ�ֵ
      *pSrc = byMap[ *pSrc ];
    }
  }

  // ����
  return TRUE;
}

/*************************************************************************
 *
* \�������ƣ�
*   GeneralTemplate()
 *
* \�������:
*   CDib* pDib    - ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
*   int nTempWidth    - ģ��Ŀ��
*   int nTempHeight        - ģ��ĸ߶�
*   int nTempCenX        - ģ�����ĵ�X���꣨�����ģ�壩
*   int nTempCenY        - ģ�����ĵ�Y���꣨�����ģ�壩
*   float* pdbTemp        - ģ�������ָ��
*   float* dbCoef        - ģ���ϵ��
 *
* \����ֵ:
*   BOOL            - �ɹ��򷵻�TRUE�����򷵻�FALSE
 *
* \˵��:
*   �ú�����ָ����ģ���pDibָ���ͼ�����ģ�������ģ��Ķ����˿�ȣ��߶ȣ�
*    ���������ϵ����ģ������ݴ����pdbTemp�С���ͼ�����ģ���������
*    Ȼ�����pDibָ���CDib�����С���Ҫע����ǣ��ú���ֻ�ܴ���8λ��ͼ��
*   ����ָ������ݽ�����
 *
 *************************************************************************
 */
static BOOL GeneralTemplate(int h, int w, uchar* pBits, int lLineBytes, int nTempWidth, int nTempHeight,
    int nTempCenX, int nTempCenY,
    float* pdbTemp, float dbCoef)
{
  // ��ʱ���ͼ�����ݵ�ָ��
  uchar* lpImage;

  // ѭ������
  int i, j, k, l;

  // ָ��Դͼ���ָ��
  uchar* pSrc;

  // ָ��Ҫ���������ָ��
  uchar* lpDst;

  // ������
  float dbResult;

  // ���ͼ�����ݴ洢�ĳߴ�
  int nSizeImage;
  nSizeImage = h * lLineBytes;

  // ����ʱ������ݷ����ڴ�
  lpImage = (uchar*) MALLOC(uchar, nSizeImage);

  // �ж��Ƿ��ڴ����ʧ��
  if (lpImage == NULL) {
    // �����ڴ�ʧ��
    return FALSE;
  }

  // ��ԭʼͼ������ݿ�������ʱ����ڴ���
  memcpy(lpImage, pBits, nSizeImage);



  // ����ģ�����
  // ��(��ȥ��Ե����)
  for (i = nTempCenY; i < h - nTempHeight + nTempCenY + 1; i++) {
    // ��(��ȥ��Ե����)
    for (j = nTempCenX; j < w - nTempWidth + nTempCenX + 1; j++) {
      // ָ����DIB��i�У���j�����ص�ָ��
      lpDst = (uchar*) lpImage + (i * lLineBytes + j);

      dbResult = 0;

      // ����
      for (k = 0; k < nTempHeight; k++) {
        for (l = 0; l < nTempWidth; l++) {
#define GetPixelOffset(i, j) (lLineBytes*(i) + j)
          // ָ��DIB��i - nTempCenY + k�У���j - nTempCenX + l�����ص�ָ��
          pSrc = (uchar*) pBits + GetPixelOffset(i - nTempCenY + k, j - nTempCenX + l);

          // ��������ֵ
          dbResult += (* pSrc) * pdbTemp[ k * nTempWidth + l ];
        }
      }

      // ����ϵ��
      dbResult *= dbCoef;

      // ȡ����ֵ
      dbResult = (float) fabs(dbResult);

      // �ж��Ƿ񳬹�255
      if (dbResult > 255) {
        // ֱ�Ӹ�ֵΪ255
        * lpDst = 255;
      }
      else {
        // ��ֵ
        * lpDst = (uchar)(dbResult + 0.5);
      }

    }
  }

  // ���Ʊ任���ͼ��
  memcpy(pBits, lpImage, nSizeImage);

  // �ͷ��ڴ�
  FREE(lpImage);

  // ����
  return TRUE;
}


/*************************************************************************
 *
* �������ƣ�
*   MedianFilter()
 *
* \�������:
*   CDib* pDib        - ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
*   int nTempWidth        - ģ��Ŀ��
*   int nTempHeight        - ģ��ĸ߶�
*   int nTempCenX        - ģ�����ĵ�X���꣨�����ģ�壩
*   int nTempCenY        - ģ�����ĵ�Y���꣨�����ģ�壩
 *
* \����ֵ:
*   BOOL            - �ɹ��򷵻�TRUE�����򷵻�FALSE
 *
* ˵��:
*   �ú�����ָ����DIBͼ�������ֵ�˲���
 *
 ************************************************************************/
static BOOL MedianFilter1(int h, int w, uchar* pBits, int lLineBytes, int nTempWidth, int nTempHeight,
    int nTempCenX, int nTempCenY)
{
  // ��ʱ���ͼ�����ݵ�ָ��
  uchar* lpImage;

  // ѭ������
  int i, j, k, l;

  // ָ��Դͼ���ָ��
  uchar* pSrc;

  // ָ��Ҫ���������ָ��
  //uchar* lpDst;

  // ���ͼ�����ݴ洢�ĳߴ�
  int nSizeImage = h * lLineBytes;

  // ָ���˲��������ָ��
  uchar* pUnchFltValue;

  // ����ʱ������ݷ����ڴ�
  lpImage = (uchar*) MALLOC(uchar, nSizeImage);

  // �ж��Ƿ��ڴ����ʧ��
  if (lpImage == NULL) {
    // ����
    return FALSE;
  }

  // ��ԭʼͼ������ݿ�������ʱ����ڴ���
  memcpy(lpImage, pBits, nSizeImage);

  // ��ʱ�����ڴ棬�Ա����˲�������
  pUnchFltValue = MALLOC(uchar, nTempHeight * nTempWidth);

  // �ж��Ƿ��ڴ����ʧ��
  if (pUnchFltValue == NULL) {
    // �ͷ��ѷ����ڴ�
    FREE(lpImage);
    // ����
    return FALSE;
  }

  // ��ʼ��ֵ�˲�
  // ��(��ȥ��Ե����)
  for (i = nTempCenY; i < h - nTempHeight + nTempCenY + 1; i++) {
    // ��(��ȥ��Ե����)
    for (j = nTempCenX; j < w - nTempWidth + nTempCenX + 1; j++) {
      // ָ����DIB��i�У���j�����ص�ָ��
      //lpDst = (uchar*) lpImage + (i * lLineBytes + j);
      //lpDst = (uchar* )lpImage + w* (h - 1 - i) + j;

      // ��ȡ�˲�������
      for (k = 0; k < nTempHeight; k++) {
        for (l = 0; l < nTempWidth; l++) {
          // ָ��DIB��i - nTempCenY + k�У���j - nTempCenX + l�����ص�ָ��
          pSrc = (uchar*) pBits + GetPixelOffset(i - nTempCenY + k, j - nTempCenX + l);
          //pSrc = (uchar* )pBits + w* (h - 1 - i + nTempCenY - k) + j - nTempCenX + l;

          // ��������ֵ
          pUnchFltValue[ k * nTempWidth + l ] = *pSrc;
        }
      }

      // ��ȡ��ֵ
      //* lpDst = GetMedianValue(pUnchFltValue, nTempHeight* nTempWidth);
    }
  }

  // ���Ʊ任���ͼ��
  memcpy(pBits, lpImage, nSizeImage);

  // �ͷ��ڴ�
  FREE(lpImage);
  FREE(pUnchFltValue);

  // ����
  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   GetMedianValue()
 *
* ����:
*   uchar* pUnchFltValue    - ָ��Ҫ��ȡ��ֵ������ָ��
*   int  iFilterLen            - ���鳤��
 *
* ����ֵ:
*   uchar            - ����ָ���������ֵ��
 *
* ˵��:
*   �ú�����ð�ݷ���һά����������򣬲���������Ԫ�ص���ֵ��
 *
 ************************************************************************/

static uchar GetMedianValue(uchar* pUnchFltValue, int iFilterLen)
{
  // ѭ������
  int i;
  int j;

  // �м����
  uchar bTemp;

  // ��ð�ݷ��������������
  for (j = 0; j < iFilterLen - 1; j ++) {
    for (i = 0; i < iFilterLen - j - 1; i ++) {
      if (pUnchFltValue[ i ] > pUnchFltValue[ i + 1 ]) {
        // ����
        bTemp = pUnchFltValue[ i ];
        pUnchFltValue[ i ] = pUnchFltValue[ i + 1 ];
        pUnchFltValue[ i + 1 ] = bTemp;
      }
    }
  }

  // ������ֵ
  if ((iFilterLen & 1) > 0) {
    // ������������Ԫ�أ������м�һ��Ԫ��
    bTemp = pUnchFltValue[(iFilterLen + 1) / 2 ];
  }
  else {
    // ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
    bTemp = (pUnchFltValue[ iFilterLen / 2 ] + pUnchFltValue[ iFilterLen / 2 + 1 ]) / 2;
  }

  // ������ֵ
  return bTemp;
}

/*************************************************************************
* \�������ƣ�
*   LinearSharpen()
 *
* \�������:
*   uchar* lpImage        - ָ��ͼ�����ݵ�ָ��
*   int w            - ͼ�����ݿ��
*   int h        - ͼ�����ݸ߶�
 *
* \����ֵ:
*   ��
 *
* \˵��:
*   ������ͼ����ǿ
*   ����������������˹���Ӷ�ͼ�����������
*   ��ԭ��ͼ���ϼ���������˹�����񻯵���Ϣ
 **************************************************************************/
static void LinearSharpen(int h, int w, uchar* pBits, int lLineBytes)
{
  int y; // ����ͼ���������
  int x; // ����ͼ��ĺ�����
  uchar* lpImage = pBits;
  float* pdGrad;

  // ��ʱ����
  float dGrad;

  // ģ��ѭ�����Ʊ���
  int yy;
  int xx;

  //�������������ʾLaplacian��������ֵ
  int nTmp[ 3 ][ 3 ];

  // ����ģ��ϵ��
  static int nWeight[ 3 ][ 3 ];

  pdGrad = MALLOC(float, w * h);

  // ��ʼ��Ϊ0
  memset(pdGrad, 0, w * h * sizeof(float));

  nWeight[ 0 ][ 0 ] = -1;
  nWeight[ 0 ][ 1 ] = -1;
  nWeight[ 0 ][ 2 ] = -1;
  nWeight[ 1 ][ 0 ] = -1;
  nWeight[ 1 ][ 1 ] = 8;
  nWeight[ 1 ][ 2 ] = -1;
  nWeight[ 2 ][ 0 ] = -1;
  nWeight[ 2 ][ 1 ] = -1;
  nWeight[ 2 ][ 2 ] = -1;

  for (y = 1; y < h - 1; y++)
    for (x = 1; x < w - 1; x++) {
      dGrad = 0;
      // Laplacian������Ҫ�ĸ�������ֵ

      // ģ���һ��
      nTmp[ 0 ][ 0 ] = lpImage[(y - 1) * lLineBytes + x - 1 ];
      nTmp[ 0 ][ 1 ] = lpImage[(y - 1) * lLineBytes + x ];
      nTmp[ 0 ][ 2 ] = lpImage[(y - 1) * lLineBytes + x + 1 ];

      // ģ��ڶ���
      nTmp[ 1 ][ 0 ] = lpImage[ y * lLineBytes + x - 1 ];
      nTmp[ 1 ][ 1 ] = lpImage[ y * lLineBytes + x ];
      nTmp[ 1 ][ 2 ] = lpImage[ y * lLineBytes + x + 1 ];

      // ģ�������
      nTmp[ 2 ][ 0 ] = lpImage[(y + 1) * lLineBytes + x - 1 ];
      nTmp[ 2 ][ 1 ] = lpImage[(y + 1) * lLineBytes + x ];
      nTmp[ 2 ][ 2 ] = lpImage[(y + 1) * lLineBytes + x + 1 ];

      // �����ݶ�
      for (yy = 0; yy < 3; yy++)
        for (xx = 0; xx < 3; xx++) {
          dGrad += nTmp[ yy ][ xx ] * nWeight[ yy ][ xx ];
        }

      // �ݶ�ֵд���ڴ�
      *(pdGrad + y * lLineBytes + x) = dGrad;
    }

  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      lpImage[ y * lLineBytes + x ] = (uchar) MAX(0, MIN(255, (lpImage[ y * lLineBytes + x ] + (int) pdGrad[ y * lLineBytes + x ])));
    }
  }

  FREE(pdGrad);
}

/*************************************************************************
* �������ƣ�GrayEqualize(uchar* lpSrcStartBits, int w, int h, int lLineBytes, int   lLineBytes)
* ��������:
     uchar* lpSrcStartBits, ָ��DIB��ʼ���ص�ָ��
     int   w��DIBͼ��Ŀ��
     int   h��DIBͼ��ĸ߶�
     int   lLineBytes��DIBͼ������ֽ�����Ϊ4�ı���
* ��������:BOOL
* ��������:�ú�����������ֱ��ͼ����
 ************************************************************************/
static BOOL GrayEqualize(uchar* lpSrcStartBits, int w, int h, int lLineBytes)
{
  int i;                 //��ѭ������
  int j;                 //��ѭ������
  uchar* lpSrcUnChr;    //ָ�����ص�ָ��

  uchar bGrayMap[ 256 ]; // �Ҷ�ӳ��
  int lGrayNum[ 256 ]; // �Ҷ�ӳ��

  for (i = 0; i < 256; i ++) { // ��0
    lGrayNum[ i ] = 0;
  }

  for (i = 0; i < h; i ++) { // ���Ҷ�ֵ����
    for (j = 0; j < w; j ++) {
      lpSrcUnChr = (uchar*) lpSrcStartBits + lLineBytes * i + j;
      lGrayNum[ *(lpSrcUnChr) ] ++;   // ��1
    }
  }

  for (i = 0; i < 256; i++) { // ����Ҷ�ӳ���
    int varLong;        //��ʱ����
    varLong = 0; // ��ʼΪ0

    for (j = 0; j <= i; j++) {
      varLong += lGrayNum[ j ];
    }

    bGrayMap[ i ] = (uchar)(varLong * 255 / h / w);      // �����Ӧ���»Ҷ�ֵ
  }

  for (i = 0; i < h; i++) { // ��
    for (j = 0; j < w; j++) { // ��
      // ָ��DIB��i�У���j�����ص�ָ��(uchar* )
      lpSrcUnChr = (uchar*) lpSrcStartBits + lLineBytes * (h - 1 - i) + j;
      *lpSrcUnChr = bGrayMap[ *lpSrcUnChr ]; // �����µĻҶ�ֵ
    }
  }

  return TRUE;
}
/*************************************************************************
* \�������ƣ�
*   GrayHistEnhance()
* \�������:
*     uchar* pBits        //ָ��Դͼ�������ָ��
*     int w        //Դͼ��Ŀ��
*     int h        //Դͼ��ĸ߶�
* \����ֵ:
*   BOOL                   //�ɹ��򷵻�TRUE�����򷵻�FALSE
* \˵��:
*   �ú�����ָ����ͼ�����ֱ��ͼ���⻯����
*************************************************************************/
static BOOL GrayHistEnhance(int h, int w, uchar* pBits, int lLineBytes)
{
  uchar* pSrc;                                    //ָ��Դͼ���ָ��
  int nTemp;                                            //��ʱ����
  int j;                                                    //ѭ������
  int i;                                                    //ѭ������
  uchar byMap[ 256 ];                                    //�ۻ�ֱ��ͼ�����Ҷ�ӳ���
  int nCount[ 256 ];                                        //ֱ��ͼ

  for (i = 0; i < 256; i ++) {                                     //���ü���Ϊ0
    nCount[ i ] = 0;                                        //����
  }

  for (i = 0; i < h; i++) {                                 //ÿ��
    for (j = 0; j < w; j++) {                             //ÿ��
      // ָ��DIB��i�У���j�����ص�ָ��
      pSrc = (uchar*) pBits + w * (h - 1 - i) + j;
      nCount[ *(pSrc) ] ++;                                  //������1
    }
  }

  for (i = 0; i < 256; i++) {                                         //�����ۻ�ֱ��ͼ
    nTemp = 0;                                        //��ʼΪ0

    for (j = 0; j <= i; j++) {
      nTemp += nCount[ j ];
    }

    byMap[ i ] = (uchar)(nTemp * 255 / h / w);          //�����Ӧ���»Ҷ�ֵ
    //˵����* 255ʵ���Ͼ��ǽ��µĻҶ�ֵҲת��Ϊ0��255�ķ�Χ��
  }

  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {                             //ÿ��
      //ָ��DIB��i�У���j�����ص�ָ��
      pSrc = (uchar*) pBits + w * (h - 1 - i) + j;
      *pSrc = byMap[ *pSrc ];                            //�����µĻҶ�ֵ
    }
  }

  return TRUE;                                            //����
}

