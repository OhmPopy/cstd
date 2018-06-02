
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"


/**************************************************************************
*  �����任API�����⣺
*  THREECROSS()   - ��ʵ�Գƾ��������ԽǾ���
*  BSTQ()         - �����ԽǶԳƾ��������ֵ����������
*  FFT_1D()       - ����һά����Ҷ�任
*  IFFT_1D()      - ����һά����Ҷ���任
*  IFFT_2D()      - ���ٶ�ά����Ҷ���任
*  DCT()          - ��ɢ���ұ任
*  IDCT()         - ��ɢ���ҷ��任
*  WALSH()        - �ֶ�ʲ��������任
*  IWALSH()       - �ֶ�ʲ�������귴�任
*  Fft2D()       - ͼ��Ķ�ά��ɢ����Ҷ���ٱ任
*  DFT_2D()       - ͼ��Ķ�ά��ɢ����Ҷ�任
*  HOTELLING()    - ͼ��Ļ����ֱ任
*  Dct()          - ͼ�����ɢ���ұ任
*  Walsh()        - ͼ����ֶ�ʲ��������任
 *
 *************************************************************************/

// ������
#define PI (3.14159265358979323846264338327950288419716939937511)

//////////////////////////////////////////////////////////////////////
//  ͼ�������任����
//////////////////////////////////////////////////////////////////////

/*************************************************************************
 *
 * \�������ƣ�
 *   FFT_1D()
 *
 * \�������:
 *   COMPLEX* pCTData    - ָ��ʱ�����ݵ�ָ�룬�������Ҫ�任������
 *   COMPLEX* pCFData    - ָ��Ƶ�����ݵ�ָ�룬����ľ����任������
 *   nLevel                        ������Ҷ�任�����㷨�ļ�����2��������
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   һά���ٸ���Ҷ�任��
 *
 **************************************************************************/
void FFT_1D(COMPLEX* pCTData, COMPLEX* pCFData, int nLevel)
{
  // ѭ�����Ʊ���
  int i, j, k, nInter;
  // ĳһ���ĳ���
  int nBtFlyLen = 0;
  // ����Ҷ�任����
  int nCount = (int) pow(2, nLevel);

  // �任ϵ���ĽǶ� ��2* PI* i / nCount
  Float dAngle;

  // �任��Ҫ�Ĺ����ռ�
  COMPLEX* pCWork1, *pCWork2, *pCW, *pCTmp;

  // �����ڴ棬�洢����Ҷ�仯��Ҫ��ϵ����
  pCW = MALLOC(COMPLEX, nCount / 2);

  // ���㸵��Ҷ�任��ϵ��
  for (i = 0; i < nCount / 2; i++) {
    dAngle = -2 * PI * i / nCount;
    CplexSet(cos(dAngle), sin(dAngle), pCW[ i ]);
  }

  // ���乤���ռ�
  pCWork1 = MALLOC(COMPLEX, nCount);
  pCWork2 = MALLOC(COMPLEX, nCount);

  // ��ʼ����д������
  memcpy(pCWork1, pCTData, sizeof(COMPLEX) * nCount);

  nInter = 0;

  // �����㷨���п��ٸ���Ҷ�任
  for (k = 0; k < nLevel; k++) {
    for (j = 0; j < (int) pow(2, k); j++) {
      //���㳤��
      nBtFlyLen = (int) pow(2, (nLevel - k));

      //�������ţ���Ȩ����
      for (i = 0; i < nBtFlyLen / 2; i++) {
        COMPLEX c;
        nInter = j * nBtFlyLen;
        CplexAdd(pCWork1[ i + nInter ], pCWork1[ i + nInter + nBtFlyLen / 2 ], pCWork2[ i + nInter ]);
        CplexSub(pCWork1[ i + nInter ], pCWork1[ i + nInter + nBtFlyLen / 2 ], c);
        CplexMul(c, pCW[(int)(i * pow(2, k)) ], pCWork2[ i + nInter + nBtFlyLen / 2 ]);
      }
    }

    // ���� pCWork1��pCWork2������
    pCTmp = pCWork1;
    pCWork1 = pCWork2;
    pCWork2 = pCTmp;
  }

  // ��������
  for (j = 0; j < nCount; j++) {
    nInter = 0;

    for (i = 0; i < nLevel; i++) {
      if (j & (1 << i)) {
        nInter += 1 << (nLevel - i - 1);
      }
    }

    pCFData[ j ] = pCWork1[ nInter ];
  }

  // �ͷ��ڴ�ռ�
  SAFEFREE(pCW);
  SAFEFREE(pCWork1);
  SAFEFREE(pCWork2);
}

/*************************************************************************
* \�������ƣ�
*    IFFT_1D()
 *
* \�������:
*   COMPLEX* pCTData    - ָ��ʱ�����ݵ�ָ�룬�������Ҫ���任������
*   COMPLEX* pCFData    - ָ��Ƶ�����ݵ�ָ�룬����ľ������任������
*   nLevel                        ������Ҷ�任�����㷨�ļ�����2��������
 *
* \����ֵ:
*   ��
 *
* \˵��:
*   һά���ٸ���Ҷ���任��
 *************************************************************************/
void IFFT_1D(COMPLEX* pCFData, COMPLEX* pCTData, int nLevel)
{

  // ѭ�����Ʊ���
  int i;

  // ����Ҷ���任����
  int nCount;
  // �任��Ҫ�Ĺ����ռ�
  COMPLEX* pCWork;

  // ���㸵��Ҷ�任����
  nCount = (int) pow(2, nLevel);


  // ���乤���ռ�
  pCWork = MALLOC(COMPLEX, nCount);

  // ����Ҫ���任������д�빤���ռ�pCWork
  memcpy(pCWork, pCFData, sizeof(COMPLEX) * nCount);

  // Ϊ�����ø���Ҷ���任, ���԰Ѹ���ҶƵ�������ȡ����
  // Ȼ��ֱ���������任�����������Ǹ���Ҷ���任����Ĺ���
  for (i = 0; i < nCount; i++) {
    CplexSet(CREAL(pCWork[ i ]), -CIMAG(pCWork[ i ]), pCWork[ i ]);
  }

  // ���ÿ��ٸ���Ҷ�任ʵ�ַ��任������洢��pCTData��
  FFT_1D(pCWork, pCTData, nLevel);

  // ��ʱ���Ĺ��������ս��
  // ���ݸ���Ҷ�任ԭ�����������ķ�����õĽ����ʵ�ʵ�ʱ������
  // ���һ��ϵ��nCount
  for (i = 0; i < nCount; i++) {
    CplexSet(CREAL(pCTData[ i ]) / nCount, -CIMAG(pCTData[ i ]) / nCount, pCTData[ i ]);
  }

  // �ͷ��ڴ�
  SAFEFREE(pCWork);
}

/*************************************************************************
 *
* \�������ƣ�
*   Fft2D()
 *
* \�������:
*   COMPLEX* pCTData    - ͼ������
*   int   lWidth                - ���ݿ��
*   int   lHeight                - ���ݸ߶�
*   COMPLEX* pCFData    - ����Ҷ�任��Ľ��
 *
* \����ֵ:
*   ��
 *
* \˵��:
*   ��ά����Ҷ�任��
 *
 ************************************************************************
 */
void Fft2D(COMPLEX* pCTData, int lWidth, int lHeight, COMPLEX* pCFData)
{

  // ѭ�����Ʊ���
  int x;
  int y;

  // ��ʱ����
  Float dTmpOne, dTmpTwo;

  // ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ���2���������ݣ�
  // ͼ��Ŀ�Ⱥ͸߶Ȳ�һ��Ϊ2����������
  int nTransWidth, nTransHeight;

  // x��y�����У������ϵĵ�������
  int nXLev, nYLev;

  // ������и���Ҷ�任�Ŀ�� ��2���������ݣ�
  dTmpOne = log(lWidth) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransWidth = (int) dTmpTwo;

  // ������и���Ҷ�任�ĸ߶� ��2���������ݣ�
  dTmpOne = log(lHeight) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransHeight = (int) dTmpTwo;


  // ����x��y�����У������ϵĵ�������
  nXLev = (int)(log(nTransWidth) / log(2) + 0.5);
  nYLev = (int)(log(nTransHeight) / log(2) + 0.5);

  for (y = 0; y < nTransHeight; y++) {
    // x������п��ٸ���Ҷ�任
    FFT_1D(&pCTData[ nTransWidth * y ], &pCFData[ nTransWidth * y ], nXLev);
  }

  // pCFData��Ŀǰ�洢��pCTData�����б任�Ľ��
  // Ϊ��ֱ������FFT_1D����Ҫ��pCFData�Ķ�ά����ת�ã���һ������FFT_1D����
  // ����Ҷ�б任��ʵ�����൱�ڶ��н��и���Ҷ�任��
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      pCTData[ nTransHeight * x + y ] = pCFData[ nTransWidth * y + x ];
    }
  }

  for (x = 0; x < nTransWidth; x++) {
    // ��x������п��ٸ���Ҷ�任��ʵ�����൱�ڶ�ԭ����ͼ�����ݽ����з����
    // ����Ҷ�任
    FFT_1D(&pCTData[ x * nTransHeight ], &pCFData[ x * nTransHeight ], nYLev);
  }

  // pCFData��Ŀǰ�洢��pCTData������ά����Ҷ�任�Ľ��������Ϊ�˷����з���
  // �ĸ���Ҷ�任�����������ת�ã����ڰѽ��ת�û���
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      pCTData[ nTransWidth * y + x ] = pCFData[ nTransHeight * x + y ];
    }
  }

  memcpy(pCTData, pCFData, sizeof(COMPLEX) * nTransHeight * nTransWidth);
}

/*************************************************************************
 *
* \�������ƣ�
*   IFFT_2D()
 *
* \�������:
*   COMPLEX* pCFData - Ƶ������
*   COMPLEX* pCTData - ʱ������
*   int   lWidth    - ͼ�����ݿ��
*   int   lHeight    - ͼ�����ݸ߶�
 *
* \����ֵ:
*   ��
 *
* \˵��:
*   ��ά����Ҷ���任��
 *
 ************************************************************************
 */
void IFFT_2D(COMPLEX* pCFData, COMPLEX* pCTData, int lWidth, int lHeight)
{
  // ѭ�����Ʊ���
  int x;
  int y;

  // ��ʱ����
  Float dTmpOne;
  Float dTmpTwo;

  // ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ���2���������ݣ�
  // ͼ��Ŀ�Ⱥ͸߶Ȳ�һ��Ϊ2����������
  int nTransWidth;
  int nTransHeight;

  // ���乤����Ҫ���ڴ�ռ�
  COMPLEX* pCWork, *pCTmp;

  // ������и���Ҷ�任�Ŀ�� ��2���������ݣ�
  dTmpOne = log(lWidth) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransWidth = (int) dTmpTwo;

  // ������и���Ҷ�任�ĸ߶� ��2���������ݣ�
  dTmpOne = log(lHeight) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransHeight = (int) dTmpTwo;

  pCWork = MALLOC(COMPLEX, nTransWidth * nTransHeight);

  // Ϊ�����ø���Ҷ���任, ���԰Ѹ���ҶƵ�������ȡ����
  // Ȼ��ֱ���������任�����������Ǹ���Ҷ���任����Ĺ���
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      pCTmp = &pCFData[ nTransWidth * y + x ];
      CplexSet(CREAL(*pCTmp) , -CIMAG(*pCTmp), pCWork[ nTransWidth * y + x ]);
    }
  }

  // ���ø���Ҷ���任
  Fft2D(pCWork, lWidth, lHeight, pCTData);

  // ��ʱ���Ĺ��������ս��
  // ���ݸ���Ҷ�任ԭ�����������ķ�����õĽ����ʵ�ʵ�ʱ������
  // ���һ��ϵ��
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      pCTmp = &pCTData[ nTransWidth * y + x ];
      CplexSet(CREAL(*pCTmp) / (nTransWidth * nTransHeight),
          -CIMAG(*pCTmp) / (nTransWidth * nTransHeight),
          pCTData[ nTransWidth * y + x ]);
    }
  }

  SAFEFREE(pCWork);
}

/*************************************************************************
* �������ƣ�FastFourierTran(COMPLEX* pTd, COMPLEX* pFd, int power)
* ��������:
*   COMPLEX* pTd, ָ��ʱ�������ָ��
*   COMPLEX* pFd, ָ��Ƶ�������ָ��
*   int            power, 2������������������
* ��������:void
 ��������:����ʵ�ֿ��ٸ���Ҷ�任
************************************************************************/
void FastFourierTran(COMPLEX* pTd, COMPLEX* pFd, int power)
{
  int i; //��ѭ������
  int j; //��ѭ������

  int dotCount; // ����Ҷ�任����
  int k; // ѭ������
  int bfsize, p; // �м����
  Float angle; // �Ƕ�
  COMPLEX* pWn, *temReg1, *temReg2, *temReg;

  dotCount = 1 << power; // ���㸶��Ҷ�任����
  pWn = MALLOC(COMPLEX, sizeof(COMPLEX) * dotCount / 2);     // ������������洢��
  temReg1 = MALLOC(COMPLEX, sizeof(COMPLEX) * dotCount);
  temReg2 = MALLOC(COMPLEX, sizeof(COMPLEX) * dotCount);

  for (i = 0; i < dotCount / 2; i++) {   // �����Ȩϵ��
    angle = -i * PI * 2 / dotCount;
    CplexSet(cos(angle), sin(angle), pWn[ i ]);
  }

  memcpy(temReg1, pTd, sizeof(COMPLEX) * dotCount);     // ��ʱ���д��temReg1

  for (k = 0; k < power; k++) {   // ���õ����㷨���п��ٸ���Ҷ�任
    for (j = 0; j < 1 << k; j++) {
      bfsize = 1 << (power - k);

      for (i = 0; i < bfsize / 2; i++) {
        COMPLEX c;
        p = j * bfsize;
        CplexAdd(temReg1[ i + p ], temReg1[ i + p + bfsize / 2 ], temReg2[ i + p ]);
        CplexSub(temReg1[ i + p ], temReg1[ i + p + bfsize / 2 ], c);
        CplexMul(c, pWn[ i * (1 << k) ], temReg2[ i + p + bfsize / 2 ]);
      }
    }

    temReg = temReg1;
    temReg1 = temReg2;
    temReg2 = temReg;
  }

  for (j = 0; j < dotCount; j++) { // ��������
    p = 0;

    for (i = 0; i < power; i++) {
      if (j & (1 << i)) {
        p += 1 << (power - i - 1);
      }
    }

    pFd[ j ] = temReg1[ p ];
  }

  SAFEFREE(pWn);   // �ͷ��ڴ�
  SAFEFREE(temReg1);
  SAFEFREE(temReg2);
}

/*************************************************************************
 *
* �������ƣ�InverFastFourierTran(COMPLEX* pFd, COMPLEX* pTd, int power)
* ��������:
*   COMPLEX * pFd��ָ��Ƶ��ֵ��ָ��
*   COMPLEX* pTd��ָ��ʱ��ֵ��ָ��
*   int power    ��2������
 *��������:void
 *��������:����ʵ�ֿ��ٸ���Ҷ���任
 ************************************************************************/
void InverFastFourierTran(COMPLEX* pFd, COMPLEX* pTd, int power)
{
  int dotCount;            // ����Ҷ�任����
  int i;                // ѭ������
  COMPLEX* pX;
  dotCount = 1 << power;        // ���㸶��Ҷ�任����
  pX = MALLOC(COMPLEX, sizeof(COMPLEX) * dotCount);     // ������������洢��
  memcpy(pX, pFd, sizeof(COMPLEX) * dotCount);     // ��Ƶ���д��X

  for (i = 0; i < dotCount; i++) {   // ����
    CplexSet(CREAL(pX[ i ]), -CIMAG(pX[ i ]), pX[ i ]);
  }

  FastFourierTran(pX, pTd, power);   // ���ÿ��ٸ���Ҷ�任

  for (i = 0; i < dotCount; i++) {   // ��ʱ���Ĺ���
    CplexSet(CREAL(pTd[ i ]) / dotCount, -CIMAG(pTd[ i ]) / dotCount, pTd[ i ]);
  }

  SAFEFREE(pX);                  // �ͷ��ڴ�
}

/*************************************************************************
* �������ƣ�Fourier(COMPLEX* pTd, int lWidth, int lHeight, COMPLEX* pFd)
* ��������:
*   COMPLEX* pTd��ָ��ʱ��ֵ��ָ��
*   int   lWidth��ͼ����
*   int   lHeight��ͼ��߶�
*   COMPLEX* pFd    ��ָ��Ƶ��ֵ��ָ��
 *
* ��������:void
* ��������:��ά���ٸ���Ҷ�任
 *************************************************************************/
void Fourier1(COMPLEX* pTd, int lWidth, int lHeight, COMPLEX* pFd)
{
  // ѭ�����Ʊ���
  int i, j;
  // ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ���2���������ݣ�
  // ͼ��Ŀ�Ⱥ͸߶Ȳ�һ��Ϊ2����������
  int wid = 1;
  int hei = 1;
  int widpor = 0, heiPor = 0; //2������

  while (wid * 2 <= lWidth) {   // ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
    wid *= 2;
    widpor++;
  }

  while (hei * 2 <= lHeight) {
    hei *= 2;
    heiPor++;
  }

  for (i = 0; i < hei; i++) {
    // x������п��ٸ���Ҷ�任
    FastFourierTran(&pTd[ wid * i ], &pFd[ wid * i ], widpor);
  }

  // pFd��Ŀǰ�洢��pTd�����б任�Ľ��
  // Ϊ��ֱ������FastFourierTran����Ҫ��pFd�Ķ�ά����ת�ã���һ������FastFourierTran����
  // ����Ҷ�б任��ʵ�����൱�ڶ��н��и���Ҷ�任��
  for (i = 0; i < hei; i++) {
    for (j = 0; j < wid; j++) {
      pTd[ hei * j + i ] = pFd[ wid * i + j ];
    }
  }

  for (j = 0; j < wid; j++) {
    // ��x������п��ٸ���Ҷ�任��ʵ�����൱�ڶ�ԭ����ͼ�����ݽ����з����
    // ����Ҷ�任
    FastFourierTran(&pTd[ j * hei ], &pFd[ j * hei ], heiPor);
  }

  // pFd��Ŀǰ�洢��pTd������ά����Ҷ�任�Ľ��������Ϊ�˷����з���
  // �ĸ���Ҷ�任�����������ת�ã����ڰѽ��ת�û���
  for (i = 0; i < hei; i++) {
    for (j = 0; j < wid; j++) {
      pTd[ wid * i + j ] = pFd[ hei * j + i ];
    }
  }

  memcpy(pTd, pFd, sizeof(COMPLEX) * hei * wid);
}

/*************************************************************************
* �������ƣ�
*   Fourier()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������
*   int lHeight      - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL              - �ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú���������ͼ����и���Ҷ�任��, int lLineBytes
 ************************************************************************/
BOOL Fourier(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc; // ָ��Դͼ���ָ��
  Float dTemp;
  int i;    // ѭ������
  int j;
  int w;    // ���и���Ҷ�任�Ŀ�ȣ�2�������η���
  int h;    // ���и���Ҷ�任�ĸ߶ȣ�2�������η���
  int wp;
  int hp;
  COMPLEX* TD, *FD;

  // ����ֵ
  w = 1;
  h = 1;
  wp = 0;
  hp = 0;

  // ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
  while (w * 2 <= lWidth) {
    w *= 2;
    wp++;
  }

  while (h * 2 <= lHeight) {
    h *= 2;
    hp++;
  }

  TD = MALLOC(COMPLEX, w * h);
  FD = MALLOC(COMPLEX, w * h);

  for (i = 0; i < h; i++) {    // ��
    for (j = 0; j < w; j++) {   // ��
      // ָ��DIB��i�У���j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      // ��ʱ��ֵ
      CplexSet(*(lpSrc), 0, TD[ j + w * i ]);
    }
  }

  for (i = 0; i < h; i++) {
    // ��y������п��ٸ���Ҷ�任
    FFT(&TD[ w * i ], &FD[ w * i ], wp);
  }

  // ����任���
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      TD[ i + h * j ] = FD[ j + w * i ];
    }
  }

  for (i = 0; i < w; i++) {
    // ��x������п��ٸ���Ҷ�任
    FFT(&TD[ i * h ], &FD[ i * h ], hp);
  }

  for (i = 0; i < h; i++) {    // ��
    for (j = 0; j < w; j++) {   // ��
      // ����Ƶ��
      dTemp = sqrt(CREAL(FD[ j * h + i ]) * CREAL(FD[ j * h + i ]) +
          CIMAG(FD[ j * h + i ]) * CIMAG(FD[ j * h + i ])) / 100;

      if (dTemp > 255) {
        dTemp = 255;
      }

      // ָ��DIB��(i<h/2 ? i+h/2 : i-h/2)�У���(j<w/2 ? j+w/2 : j-w/2)��
      // ���ص�ָ�룬�˴���ֱ��ȡi��j����Ϊ�˽��任���ԭ���Ƶ�����
      // lpSrc = (LPBYTE )lpDIBBits + lLineBytes* (lHeight-1-i) + j;
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes *
          (lHeight - 1 - (i < h / 2 ? i + h / 2 : i - h / 2)) + (j < w / 2 ? j + w / 2 : j - w / 2);

      // ����Դͼ��
      * (lpSrc) = (BYTE)(dTemp);
    }
  }

  SAFEFREE(TD);
  SAFEFREE(FD);
  return TRUE;
}

/*************************************************************************
* �������ƣ�InverseFourier(COMPLEX* pFd, COMPLEX* pTd, int lWidth, int lHeight)
* ��������:
*   COMPLEX* pTd��ָ��ʱ��ֵ��ָ��
*   COMPLEX* pFd    ��ָ��Ƶ��ֵ��ָ��
*   int   lWidth��ͼ����
*   int   lHeight��ͼ��߶�
* ��������:void
* ��������:��ά���ٸ���Ҷ���任
 *************************************************************************/
void InverseFourier(COMPLEX* pFd, COMPLEX* pTd, int lWidth, int lHeight)
{
  // ѭ�����Ʊ���
  int i, j;
  // ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ���2���������ݣ�
  // ͼ��Ŀ�Ⱥ͸߶Ȳ�һ��Ϊ2����������
  int wid = 1, hei = 1, widpor = 0, heiPor = 0; //2������

  // ���乤����Ҫ���ڴ�ռ�
  COMPLEX* pCWork, *pCTmp;

  while (wid * 2 <= lWidth) {   // ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
    wid *= 2;
    widpor++;
  }

  while (hei * 2 <= lHeight) {
    hei *= 2;
    heiPor++;
  }

  pCWork = MALLOC(COMPLEX, sizeof(COMPLEX) * wid * hei);

  // Ϊ�����ø���Ҷ���任, ���԰Ѹ���ҶƵ�������ȡ����
  // Ȼ��ֱ���������任�����������Ǹ���Ҷ���任����Ĺ���
  for (i = 0; i < hei; i++) {
    for (j = 0; j < wid; j++) {
      pCTmp = &pFd[ wid * i + j ];
      CplexSet(CREAL(*pCTmp), -CIMAG(*pCTmp), pCWork[ wid * i + j ]);
      //CplexSet( pCTmp->CREAL() , -pCTmp->CIMAG() );
    }
  }

  // ���ø���Ҷ���任
  Fourier1(pCWork, lWidth, lHeight, pTd);

  // ��ʱ���Ĺ��������ս��
  // ���ݸ���Ҷ�任ԭ�����������ķ�����õĽ����ʵ�ʵ�ʱ������
  // ���һ��ϵ��
  for (i = 0; i < hei; i++) {
    for (j = 0; j < wid; j++) {
      pCTmp = &pTd[ wid * i + j ];
      CplexSet(CREAL(*pCTmp) / (wid * hei), -CIMAG(*pCTmp) / (wid * hei), pTd[ wid * i + j ]);
    }
  }

  SAFEFREE(pCWork);
}

/*************************************************************************
* �������ƣ�DisFCosTran(Float* pTd, Float* pFd, int power)
* ��������:
*   Float* pTd, ָ��ʱ��ֵ��ָ��
*   Float* pFd, ָ��Ƶ��ֵ��ָ��
*   int   power, 2������
 *
* ��������:void
* ��������: ����ʵ�ֿ�����ɢ���ұ任
 ************************************************************************/
void DisFCosTran(Float* pTd, Float* pFd, int power)
{
  int i;                 //��ѭ������
  int dotCount; // ��ɢ���ұ任����
  Float dTemp; // ��ʱ����
  COMPLEX* temReg;
  dotCount = 1 << power; // ������ɢ���ұ任����
  temReg = MALLOC(COMPLEX, sizeof(COMPLEX) * dotCount * 2);     // �����ڴ�
  memset(temReg, 0, sizeof(COMPLEX) * dotCount * 2);     // ��Ϊ0

  // ��ʱ���д������temReg
  for (i = 0; i < dotCount; i++) {
    CplexSet(pTd[ i ], 0, temReg[ i ]);
  }

  FastFourierTran(temReg, temReg, power + 1);   // ���ÿ��ٸ���Ҷ�任
  dTemp = 1 / sqrt(dotCount);   // ����ϵ��
  pFd[ 0 ] = CREAL(temReg[ 0 ]) * dTemp;   // ��pFd[0]
  dTemp *= sqrt(2);

  for (i = 1; i < dotCount; i++) {   // ��pFd[u]
    pFd[ i ] = (CREAL(temReg[ i ]) * cos(i * PI / (dotCount * 2)) + CIMAG(temReg[ i ]) * sin(i * PI / (dotCount * 2))) * dTemp;
  }

  SAFEFREE(temReg);   // �ͷ��ڴ�
}

/*************************************************************************
* �������ƣ�CosTran(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
* ��������:
*   LPBYTE lpDIBBits, ָ��DIB��ʼ���ص�ָ��
*   int lWidth, DIB�Ŀ��
*   int lHeight, int lLineBytes, DIB�ĸ߶�
*     int lLineBytes, DIB�����ֽ�����Ϊ4�ı���
* ��������:BOOL
* ��������: ������ͼ�������ɢ���ұ任
************************************************************************/
BOOL CosTran(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrcUnChr;    //ָ�����ص�ָ��
  int i;                 //��ѭ������
  int j;                 //��ѭ������
  int wid = 1, hei = 1; // ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ���ʼ��Ϊ1
  Float dTemp; // �м����
  int widpor = 0, heiPor = 0; //2������
  Float* pTd, *pFd;

  // ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
  while (wid * 2 <= lWidth) {
    wid *= 2;
    widpor++;
  }

  while (hei * 2 <= lHeight) {
    hei *= 2;
    heiPor++;
  }

  pTd = MALLOC(Float, wid * hei);   // �����ڴ�
  pFd = MALLOC(Float, wid * hei);

  for (i = 0; i < hei; i++) {   // ��
    for (j = 0; j < wid; j++) {   // ��
      // ָ��DIB��i�У���j�����ص�ָ��
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      pTd[ j + i * wid ] = *(lpSrcUnChr);   // ��ʱ��ֵ
    }
  }

  for (i = 0; i < hei; i++) {
    DisFCosTran(&pTd[ wid * i ], &pFd[ wid * i ], widpor);   // ��y���������ɢ���ұ任
  }

  for (i = 0; i < hei; i++) {   // ���������
    for (j = 0; j < wid; j++) {
      pTd[ j * hei + i ] = pFd[ j + wid * i ];
    }
  }

  for (j = 0; j < wid; j++) {
    DisFCosTran(&pTd[ j * hei ], &pFd[ j * hei ], heiPor);   // ��x���������ɢ���ұ任
  }

  for (i = 0; i < hei; i++) {   // ��
    for (j = 0; j < wid; j++) {   // ��
      dTemp = fabs(pFd[ j * hei + i ]);   // ����Ƶ��

      // �Ƿ񳬹�255, �����ģ�ֱ������Ϊ255
      if (dTemp > 255) {
        dTemp = 255;
      }

      // ָ��DIB��y�У���x�����ص�ָ��
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      * (lpSrcUnChr) = (BYTE)(dTemp);        // ����Դͼ��
    }
  }

  SAFEFREE(pTd);   // �ͷ��ڴ�
  SAFEFREE(pFd);
  return TRUE;
}

/*************************************************************************
* �������ƣ�Walshei_Har(Float* pTd, Float* pFd, int power)
* ��������:
*   Float* pTd, ָ��ʱ��ֵ��ָ��
*   Float* pFd, ָ��Ƶ��ֵ��ָ��
*   int   power, 2������
 *
* ��������:void
* ��������: ����ʵ�ֿ����ֶ�ʲ-������任
************************************************************************/
void Walshei_Har(Float* pTd, Float* pFd, int power)
{
  int dotCount; // �ֶ�ʲ-������任����
  int i, j, k; // ѭ������
  int bfsize, p; // �м����
  Float* temReg1, *temReg2, *temReg;
  dotCount = 1 << power; // ��������ֶ�ʲ�任����
  temReg1 = MALLOC(Float, dotCount);   // �����������������
  temReg2 = MALLOC(Float, dotCount);
  memcpy(temReg1, pTd, sizeof(Float) * dotCount);     // ��ʱ���д������temReg1

  for (k = 0; k < power; k++) {   // ��������
    for (j = 0; j < 1 << k; j++) {
      bfsize = 1 << (power - k);

      for (i = 0; i < bfsize / 2; i++) {
        p = j * bfsize;
        temReg2[ i + p ] = temReg1[ i + p ] + temReg1[ i + p + bfsize / 2 ];
        temReg2[ i + p + bfsize / 2 ] = temReg1[ i + p ] - temReg1[ i + p + bfsize / 2 ];
      }
    }

    temReg = temReg1; // ����temReg1��temReg2
    temReg1 = temReg2;
    temReg2 = temReg;
  }

  for (j = 0; j < dotCount; j++) {   // ����ϵ��
    p = 0;

    for (i = 0; i < power; i++) {
      if (j & (1 << i)) {
        p += 1 << (power - i - 1);
      }
    }

    pFd[ j ] = temReg1[ p ] / dotCount;
  }

  SAFEFREE(temReg1);   // �ͷ��ڴ�
  SAFEFREE(temReg2);
}

/*************************************************************************
* �������ƣ�Walsh_HarTran(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
* ��������:
*   LPBYTE lpDIBBits, ָ��ԴDIBͼ��ָ��
*   int lWidth, ԴDIBͼ����
*   int lHeight, ԴDIBͼ��߶�
*   int lLineBytes, ԴDIBͼ������ֽ�����Ϊ4�ı���
* ��������:BOOL
* ��������:������ͼ������ֶ�ʲ-������任
 ************************************************************************/
BOOL Walsh_HarTran(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrcUnChr;    //ָ�����ص�ָ��
  int i;                 //��ѭ������
  int j;                 //��ѭ������

  int wid = 1, hei = 1; // ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ���ʼ��Ϊ1
  Float dTemp; // �м����
  int widpor = 0, heiPor = 0; //2������
  Float* pTd, *pFd;

  while (wid * 2 <= lWidth) { // ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
    wid *= 2;
    widpor++;
  }

  while (hei * 2 <= lHeight) {
    hei *= 2;
    heiPor++;
  }

  pTd = MALLOC(Float, wid * hei);   // �����ڴ�
  pFd = MALLOC(Float, wid * hei);

  for (i = 0; i < hei; i++) {   // ��
    for (j = 0; j < wid; j++) {   // ��
      // ָ��DIB��i�У���j�����ص�ָ��
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      pTd[ j + i * wid ] = *(lpSrcUnChr);   // ��ʱ��ֵ
    }
  }

  for (i = 0; i < hei; i++) {
    Walshei_Har(pTd + wid * i, pFd + wid * i, widpor);   // ��y��������ֶ�ʲ-������任
  }

  for (i = 0; i < hei; i++) {   // ���������
    for (j = 0; j < wid; j++) {
      pTd[ j * hei + i ] = pFd[ j + wid * i ];
    }
  }

  for (j = 0; j < wid; j++) {
    Walshei_Har(pTd + j * hei, pFd + j * hei, heiPor);   // ��x��������ֶ�ʲ-������任
  }

  for (i = 0; i < hei; i++) {   // ��
    for (j = 0; j < wid; j++) {   // ��
      dTemp = fabs(pFd[ j * hei + i ] * 1000);   // ����Ƶ��

      if (dTemp > 255) { // ���ڳ���255�ģ�ֱ������Ϊ255
        dTemp = 255;
      }

      // ָ��DIB��i�У���j�����ص�ָ��
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      * (lpSrcUnChr) = (BYTE)(dTemp);        // ����Դͼ��
    }
  }

  SAFEFREE(pTd);   //�ͷ��ڴ�
  SAFEFREE(pFd);
  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   FFT()
* ����:
*   COMPLEX* TD - ָ��ʱ�������ָ��
*   COMPLEX* FD - ָ��Ƶ�������ָ��
*   r      ��2������������������
* ����ֵ:
*   �ޡ�
* ˵��:
*   �ú�������ʵ�ֿ��ٸ���Ҷ�任��
 ************************************************************************/
void FFT(COMPLEX* TD, COMPLEX* FD, int r)
{
  int count;    // ����Ҷ�任����
  int i, j, k;    // ѭ������
  int bfsize, p;
  Float angle;    // �Ƕ�
  COMPLEX* W, *X1, *X2, *X;

  count = 1 << r;    // ���㸶��Ҷ�任����

  // ������������洢��
  W = MALLOC(COMPLEX, count / 2);
  X1 = MALLOC(COMPLEX, count);
  X2 = MALLOC(COMPLEX, count);

  // �����Ȩϵ��
  for (i = 0; i < count / 2; i++) {
    angle = -i * PI * 2 / count;
    CplexSet(cos(angle), sin(angle), W[ i ]);
  }

  // ��ʱ���д��X1
  memcpy(X1, TD, sizeof(COMPLEX) * count);

  // ���õ����㷨���п��ٸ���Ҷ�任
  for (k = 0; k < r; k++) {
    for (j = 0; j < 1 << k; j++) {
      bfsize = 1 << (r - k);

      for (i = 0; i < bfsize / 2; i++) {
        COMPLEX c;
        p = j * bfsize;
        CplexAdd(X1[ i + p ], X1[ i + p + bfsize / 2 ], X2[ i + p ]);
        CplexSub(X1[ i + p ], X1[ i + p + bfsize / 2 ], c);
        CplexMul(c, W[ i * (1 << k) ], X2[ i + p + bfsize / 2 ]);
      }
    }

    X = X1;
    X1 = X2;
    X2 = X;
  }

  // ��������
  for (j = 0; j < count; j++) {
    p = 0;

    for (i = 0; i < r; i++) {
      if (j & (1 << i)) {
        p += 1 << (r - i - 1);
      }
    }

    FD[ j ] = X1[ p ];
  }

  SAFEFREE(W);
  SAFEFREE(X1);
  SAFEFREE(X2);
}

/*************************************************************************
* �������ƣ�
*   IFFT()
* ����:
*   COMPLEX* FD - ָ��Ƶ��ֵ��ָ��
*   COMPLEX* TD - ָ��ʱ��ֵ��ָ��
*   r      ��2������
* ����ֵ:
*   �ޡ�
* ˵��:
*   �ú�������ʵ�ֿ��ٸ���Ҷ���任��
 ************************************************************************/
void IFFT(COMPLEX* FD, COMPLEX* TD, int r)
{
  int count;     // ����Ҷ�任����
  int i;      // ѭ������
  COMPLEX* X;

  count = 1 << r;     // ���㸶��Ҷ�任����
  X = MALLOC(COMPLEX, count);   // ������������洢��
  memcpy(X, FD, sizeof(COMPLEX) * count);     // ��Ƶ���д��X

  // ����
  for (i = 0; i < count; i++) {
    CplexSet(CREAL(X[ i ]), -CIMAG(X[ i ]), X[ i ]);
  }

  FFT(X, TD, r);       // ���ÿ��ٸ���Ҷ�任

  // ��ʱ���Ĺ���
  for (i = 0; i < count; i++) {
    CplexSet(CREAL(TD[ i ]) / count, -CIMAG(TD[ i ]) / count, TD[ i ]);
  }

  SAFEFREE(X);
}

/*************************************************************************
* �������ƣ�
*   Dct1D()
* ����:
*   Float* f    - ָ��ʱ��ֵ��ָ��
*   Float* F    - ָ��Ƶ��ֵ��ָ��
*   r      ��2������
* ����ֵ:
*   �ޡ�
* ˵��:
*   �ú�������ʵ�ֿ�����ɢ���ұ任���ú�������2N��Ŀ��ٸ���Ҷ�任
* ��ʵ����ɢ���ұ任��
 ************************************************************************/
void Dct1D(const Float* f, Float* F, int r)
{
  int count;   // ��ɢ���ұ任����
  int i;    // ѭ������
  Float dTemp;
  COMPLEX* X;

  count = 1 << r;   // ������ɢ���ұ任����

  X = MALLOC(COMPLEX, count * 2);
  memset(X, 0, sizeof(COMPLEX) * count * 2);     // ����ֵΪ0

  // ��ʱ���д������X
  for (i = 0; i < count; i++) {
    CplexSet(f[ i ], 0, X[ i ]);
  }

  FFT(X, X, r + 1);      // ���ÿ��ٸ���Ҷ�任
  dTemp = 1 / sqrt(count);    // ����ϵ��
  F[ 0 ] = CREAL(X[ 0 ]) * dTemp;   // ��F[0]
  dTemp *= sqrt(2);

  // ��F[u]
  for (i = 1; i < count; i++) {
    F[ i ] = (CREAL(X[ i ]) * cos(i * PI / (count * 2)) + CIMAG(X[ i ]) *
        sin(i * PI / (count * 2))) * dTemp;
  }

  SAFEFREE(X);
}

/*************************************************************************
* �������ƣ�
*   iDct1D()
* ����:
*   Float* F    - ָ��Ƶ��ֵ��ָ��
*   Float* f    - ָ��ʱ��ֵ��ָ��
*   r      ��2������
* ����ֵ:
*   �ޡ�
* ˵��:
*   �ú�������ʵ�ֿ�����ɢ���ҷ��任���ú���Ҳ����2N��Ŀ��ٸ���Ҷ�任
* ��ʵ����ɢ���ҷ��任��
 ************************************************************************/
void iDct1D(const Float* F, Float* f, int r)
{
  int count;   // ��ɢ���ҷ��任����
  int i;    // ѭ������
  Float dTemp, d0;
  COMPLEX* X;

  count = 1 << r;   // ������ɢ���ұ任����

  X = MALLOC(COMPLEX, count * 2);
  memset(X, 0, sizeof(COMPLEX) * count * 2);     // ����ֵΪ0

  // ��Ƶ��任���д������X
  for (i = 0; i < count; i++) {
    CplexSet(F[ i ] * cos(i * PI / (count * 2)), F[ i ] *
        sin(i * PI / (count * 2)), X[ i ]);
  }

  IFFT(X, X, r + 1);    // ���ÿ��ٸ���Ҷ���任

  // ����ϵ��
  dTemp = sqrt(2.0 / count);
  d0 = (sqrt(1.0 / count) - dTemp) * F[ 0 ];

  for (i = 0; i < count; i++) {
    f[ i ] = d0 + CREAL(X[ i ]) * dTemp * 2 * count;
  }

  SAFEFREE(X);
}

/*************************************************************************
* �������ƣ�
*   Dct()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������
*   int lHeight      - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL              - �ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú���������ͼ�������ɢ���ұ任��
 ************************************************************************/
BOOL Dct(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc; // ָ��Դͼ���ָ��
  int i, j;    // ѭ������
  int w;    // ���и���Ҷ�任�Ŀ�ȣ�2�������η���
  int h;    // ���и���Ҷ�任�ĸ߶ȣ�2�������η���
  Float dTemp, *f, *F;
  int wp;
  int hp;

  // ����ֵ
  w = 1;
  h = 1;
  wp = 0;
  hp = 0;

  // ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
  while (w * 2 <= lWidth) {
    w *= 2;
    wp++;
  }

  while (h * 2 <= lHeight) {
    h *= 2;
    hp++;
  }

  f = MALLOC(Float, w * h);
  F = MALLOC(Float, w * h);

  for (i = 0; i < h; i++) {   // ��
    for (j = 0; j < w; j++) {   // ��
      // ָ��DIB��i�У���j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (i) + j;

      // ��ʱ��ֵ
      f[ j + i * w ] = *(lpSrc);
    }
  }

  for (i = 0; i < h; i++) {
    // ��y���������ɢ���ұ任
    Dct1D(&f[ w * i ], &F[ w * i ], wp);
  }

  // ���������
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      f[ j * h + i ] = F[ j + w * i ];
    }
  }

  for (j = 0; j < w; j++) {
    // ��x���������ɢ���ұ任
    Dct1D(&f[ j * h ], &F[ j * h ], hp);
  }

  for (i = 0; i < h; i++) {   // ��
    for (j = 0; j < w; j++) {   // ��
      dTemp = fabs(F[ j * h + i ]);   // ����Ƶ��

      // �ж��Ƿ񳬹�255
      if (dTemp > 255) {
        dTemp = 255;
      }

      // ָ��DIB��y�У���x�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (i) + j;

      // ����Դͼ��
      * (lpSrc) = (BYTE)(dTemp);
    }
  }

  SAFEFREE(f);
  SAFEFREE(F);
  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   WALSH()
* ����:
*   Float* f    - ָ��ʱ��ֵ��ָ��
*   Float* F    - ָ��Ƶ��ֵ��ָ��
*   r      ��2������
* ����ֵ:
*   �ޡ�
* ˵��:
*   �ú�������ʵ�ֿ����ֶ�ʲ-������任��
 ************************************************************************/
void WALSH(const Float* f, Float* F, int r)
{
  int count;    // �ֶ�ʲ-������任����
  int i, j, k;    // ѭ������
  int bfsize, p;
  Float* X1, *X2, *X;

  count = 1 << r;    // ��������ֶ�ʲ�任����
  X1 = MALLOC(Float, count);    // �����������������
  X2 = MALLOC(Float, count);    // �����������������

  memcpy(X1, f, sizeof(Float) * count);     // ��ʱ���д������X1

  // ��������
  for (k = 0; k < r; k++) {
    for (j = 0; j < 1 << k; j++) {
      bfsize = 1 << (r - k);

      for (i = 0; i < bfsize / 2; i++) {
        p = j * bfsize;
        X2[ i + p ] = X1[ i + p ] + X1[ i + p + bfsize / 2 ];
        X2[ i + p + bfsize / 2 ] = X1[ i + p ] - X1[ i + p + bfsize / 2 ];
      }
    }

    // ����X1��X2
    X = X1;
    X1 = X2;
    X2 = X;
  }

  // ����ϵ��
  for (j = 0; j < count; j++) {
    p = 0;

    for (i = 0; i < r; i++) {
      if (j & (1 << i)) {
        p += 1 << (r - i - 1);
      }
    }

    F[ j ] = X1[ p ] / count;
  }

  SAFEFREE(X1);
  SAFEFREE(X2);
}

/*************************************************************************
* �������ƣ�
*   IWALSH()
* ����:
*   Float* F    - ָ��Ƶ��ֵ��ָ��
*   Float* f    - ָ��ʱ��ֵ��ָ��
*   r      ��2������
* ����ֵ:
*   �ޡ�
* ˵��:
*   �ú�������ʵ�ֿ����ֶ�ʲ-�����귴�任��
 ************************************************************************/
void IWALSH(const Float* F, Float* f, int r)
{
  int count;    // �任����
  int i;     // ѭ������

  count = 1 << r;    // ����任����
  WALSH(F, f, r);      // ���ÿ����ֶ�ʲ��������任���з��任

  for (i = 0; i < count; i++) {   // ����ϵ��
    f[ i ] *= count;
  }
}

/*************************************************************************
* �������ƣ�
*   Walsh()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int  lWidth      - Դͼ���ȣ���������
*   int  lHeight     - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL              - �ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú���������ͼ������ֶ�ʲ-������任���������ȶ�ͼ��ÿ�н���һά
* �ֶ�ʲ��������任��Ȼ��Ա任�����ÿ�н���һά�ֶ�ʲ��������任��
 ************************************************************************/
BOOL Walsh(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;  // ָ��Դͼ���ָ��
  int i, j;    // ѭ������
  int w;     // ���и���Ҷ�任�Ŀ�ȣ�2�������η���
  int h;     // ���и���Ҷ�任�ĸ߶ȣ�2�������η���
  Float dTemp, *f, *F;
  int wp;
  int hp;

  // ����ֵ
  w = 1;
  h = 1;
  wp = 0;
  hp = 0;

  // ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
  while (w * 2 <= lWidth) {
    w *= 2;
    wp++;
  }

  while (h * 2 <= lHeight) {
    h *= 2;
    hp++;
  }

  f = MALLOC(Float, w * h);
  F = MALLOC(Float, w * h);

  for (i = 0; i < h; i++) {   // ��
    for (j = 0; j < w; j++) {   // ��
      // ָ��DIB��i�У���j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (i) + j;

      // ��ʱ��ֵ
      f[ j + i * w ] = *(lpSrc);
    }
  }

  for (i = 0; i < h; i++) {
    // ��y��������ֶ�ʲ-������任
    WALSH(f + w * i, F + w * i, wp);
  }

  // ���������
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      f[ j * h + i ] = F[ j + w * i ];
    }
  }

  for (j = 0; j < w; j++) {
    // ��x��������ֶ�ʲ-������任
    WALSH(f + j * h, F + j * h, hp);
  }

  for (i = 0; i < h; i++) {   // ��
    for (j = 0; j < w; j++) {   // ��
      dTemp = fabs(F[ j * h + i ] * 1000);   // ����Ƶ��

      if (dTemp > 255) {
        dTemp = 255;
      }

      // ָ��DIB��i�У���j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (i) + j;

      // ����Դͼ��
      * (lpSrc) = (BYTE)(dTemp);
    }
  }

  SAFEFREE(f);
  SAFEFREE(F);

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   Walsh1()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������
*   int lHeight      - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL              - �ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
*   �ú���������ͼ������ֶ�ʲ-������任�������治ͬ���ǣ��˴��ǽ���ά
* ����ת����һ����������Ȼ��Ը�����������һ��һά�ֶ�ʲ-������任��
 ************************************************************************/
BOOL Walsh1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;  // ָ��Դͼ���ָ��
  int i, j;    // ѭ������
  int w;     // ���и���Ҷ�任�Ŀ�ȣ�2�������η���
  int h;     // ���и���Ҷ�任�ĸ߶ȣ�2�������η���
  Float dTemp, *f, *F;
  int wp;
  int hp;

  // ����ֵ
  w = 1;
  h = 1;
  wp = 0;
  hp = 0;

  // ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
  while (w * 2 <= lWidth) {
    w *= 2;
    wp++;
  }

  while (h * 2 <= lHeight) {
    h *= 2;
    hp++;
  }

  f = MALLOC(Float, w * h);
  F = MALLOC(Float, w * h);

  for (i = 0; i < w; i++) {   // ��
    for (j = 0; j < h; j++) {   // ��
      // ָ��DIB��j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;

      // ��ʱ��ֵ
      f[ j + i * w ] = *(lpSrc);
    }
  }

  // ���ÿ����ֶ�ʲ��������任
  WALSH(f, F, wp + hp);

  for (i = 0; i < w; i++) {   // ��
    for (j = 0; j < h; j++) {   // ��
      dTemp = fabs(F[ i * w + j ] * 1000);   // ����Ƶ��

      if (dTemp > 255) {
        dTemp = 255;
      }

      // ָ��DIB��j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;

      // ����Դͼ��
      * (lpSrc) = (BYTE)(dTemp);
    }
  }

  SAFEFREE(f);
  SAFEFREE(F);

  return TRUE;
}

/*************************************************************************
 *
* \�������ƣ�
*   Fft2D()
 *
* \�������:
*   COMPLEX* pCTData - ͼ������
*   int   lWidth    - ���ݿ��
*   int   lHeight    - ���ݸ߶�
*   COMPLEX* pCFData - ����Ҷ�任��Ľ��
 *
* \����ֵ:
*   ��
 *
* \˵��:
*   ��ά����Ҷ�任��
 *
 ************************************************************************
 */
void FFT_2D1(COMPLEX* pCTData, int lWidth, int lHeight, COMPLEX* pCFData)
{
  // ѭ�����Ʊ���
  int x;
  int y;

  // ��ʱ����
  Float dTmpOne;
  Float dTmpTwo;

  // ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ���2���������ݣ�
  // ͼ��Ŀ�Ⱥ͸߶Ȳ�һ��Ϊ2����������
  int nTransWidth;
  int nTransHeight;

  // x��y�����У������ϵĵ�������
  int nXLev;
  int nYLev;

  // ������и���Ҷ�任�Ŀ�� ��2���������ݣ�
  dTmpOne = log(lWidth) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransWidth = (int) dTmpTwo;

  // ������и���Ҷ�任�ĸ߶� ��2���������ݣ�
  dTmpOne = log(lHeight) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransHeight = (int) dTmpTwo;

  // ����x��y�����У������ϵĵ�������
  nXLev = (int)(log(nTransWidth) / log(2) + 0.5);
  nYLev = (int)(log(nTransHeight) / log(2) + 0.5);

  for (y = 0; y < nTransHeight; y++) {
    // x������п��ٸ���Ҷ�任
    FFT_1D(&pCTData[ nTransWidth * y ], &pCFData[ nTransWidth * y ], nXLev);
  }

  // pCFData��Ŀǰ�洢��pCTData�����б任�Ľ��
  // Ϊ��ֱ������FFT_1D����Ҫ��pCFData�Ķ�ά����ת�ã���һ������FFT_1D����
  // ����Ҷ�б任��ʵ�����൱�ڶ��н��и���Ҷ�任��
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      pCTData[ nTransHeight * x + y ] = pCFData[ nTransWidth * y + x ];
    }
  }

  for (x = 0; x < nTransWidth; x++) {
    // ��x������п��ٸ���Ҷ�任��ʵ�����൱�ڶ�ԭ����ͼ�����ݽ����з����
    // ����Ҷ�任
    FFT_1D(&pCTData[ x * nTransHeight ], &pCFData[ x * nTransHeight ], nYLev);
  }

  // pCFData��Ŀǰ�洢��pCTData������ά����Ҷ�任�Ľ��������Ϊ�˷����з���
  // �ĸ���Ҷ�任�����������ת�ã����ڰѽ��ת�û���
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      pCTData[ nTransWidth * y + x ] = pCFData[ nTransHeight * x + y ];
    }
  }

  memcpy(pCTData, pCFData, sizeof(COMPLEX) * nTransHeight * nTransWidth);
}

/*************************************************************************
 * �������ƣ�
 *   ThreeCross()
 *
 * ����:
 *   Float  *Matrix     - ������ž���A
 *   int    Rank        - ����A�Ľ���
 *   Float  *QMatrix    - ����householder�任�ľ���Q
 *   Float  *MainCross  - �Գ��������е����Խ�Ԫ��
 *   Float  *HypoCross  - �Գ��������еĴζԽ�Ԫ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�����householder�任��n��ʵ�Գƾ���Ϊ�Գ�������
 *
 ************************************************************************/
BOOL ThreeCross(Float* Matrix, int Rank, Float* QMatrix, Float* MainCross, Float* HypoCross)
{
  //   ѭ������
  int i, j, k, u;
  Float h, f, g, h2;

  // �Ծ���QMatrix��ֵ
  for (i = 0; i <= Rank - 1; i++)
    for (j = 0; j <= Rank - 1; j++) {
      u = i * Rank + j;
      QMatrix[ u ] = Matrix[ u ];
    }

  for (i = Rank - 1; i >= 1; i--) {
    h = 0.0;

    if (i > 1)
      for (k = 0; k <= i - 1; k++) {
        u = i * Rank + k;
        h = h + QMatrix[ u ] * QMatrix[ u ];
      }

    // ���һ��ȫ��Ϊ��
    if (h + 1.0 == 1.0) {
      HypoCross[ i ] = 0.0;

      if (i == 1) {
        HypoCross[ i ] = QMatrix[ i * Rank + i - 1 ];
      }

      MainCross[ i ] = 0.0;
    }

    // �������householder�任�������������ֵ
    else {
      // ��ζԽ�Ԫ�ص�ֵ
      HypoCross[ i ] = sqrt(h);

      // �ж�i��i-1��Ԫ���ǲ��Ǵ�����
      u = i * Rank + i - 1;

      if (QMatrix[ u ] > 0.0) {
        HypoCross[ i ] = -HypoCross[ i ];
      }

      h = h - QMatrix[ u ] * HypoCross[ i ];
      QMatrix[ u ] = QMatrix[ u ] - HypoCross[ i ];
      f = 0.0;

      // householder�任
      for (j = 0; j <= i - 1; j++) {
        QMatrix[ j * Rank + i ] = QMatrix[ i * Rank + j ] / h;
        g = 0.0;

        for (k = 0; k <= j; k++) {
          g = g + QMatrix[ j * Rank + k ] * QMatrix[ i * Rank + k ];
        }

        if (j + 1 <= i - 1)
          for (k = j + 1; k <= i - 1; k++) {
            g = g + QMatrix[ k * Rank + j ] * QMatrix[ i * Rank + k ];
          }

        HypoCross[ j ] = g / h;
        f = f + g * QMatrix[ j * Rank + i ];
      }

      h2 = f / (h + h);

      // �����������ֵ
      for (j = 0; j <= i - 1; j++) {
        f = QMatrix[ i * Rank + j ];
        g = HypoCross[ j ] - h2 * f;
        HypoCross[ j ] = g;

        for (k = 0; k <= j; k++) {
          u = j * Rank + k;
          QMatrix[ u ] = QMatrix[ u ] - f * HypoCross[ k ] - g * QMatrix[ i * Rank + k ];
        }
      }

      MainCross[ i ] = h;
    }
  }

  // ����ֵ
  for (i = 0; i <= Rank - 2; i++) {
    HypoCross[ i ] = HypoCross[ i + 1 ];
  }

  HypoCross[ Rank - 1 ] = 0.0;
  MainCross[ 0 ] = 0.0;

  for (i = 0; i <= Rank - 1; i++) {
    // ���Խ�Ԫ�صļ���
    if ((MainCross[ i ] != 0.0) && (i - 1 >= 0))
      for (j = 0; j <= i - 1; j++) {
        g = 0.0;

        for (k = 0; k <= i - 1; k++) {
          g = g + QMatrix[ i * Rank + k ] * QMatrix[ k * Rank + j ];
        }

        for (k = 0; k <= i - 1; k++) {
          u = k * Rank + j;
          QMatrix[ u ] = QMatrix[ u ] - g * QMatrix[ k * Rank + i ];
        }
      }

    // �����Խ��ߵ�Ԫ�ؽ��д洢���Ա㷵��
    u = i * Rank + i;
    MainCross[ i ] = QMatrix[ u ];
    QMatrix[ u ] = 1.0;

    // �����Խ������е�Ԫ�ظ���ֵ
    if (i - 1 >= 0)
      for (j = 0; j <= i - 1; j++) {
        QMatrix[ i * Rank + j ] = 0.0;
        QMatrix[ j * Rank + i ] = 0.0;
      }
  }

  // ����ֵ
  return (TRUE);
}


/*************************************************************************
* �������ƣ�
*   BSTQ()
 *
* ����:
*   int    Rank        - ����A�Ľ���
*   Float  *MainCross  - �Գ��������е����Խ�Ԫ�أ�����ʱ���A������ֵ
*   Float  *HypoCross  - �Գ��������еĴζԽ�Ԫ��
*   Float  *Matrix     - ���ضԳƾ���A����������
*   Float Eps          - ���ƾ���
*   int MaxT            - ����������
 *
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
*   �ú����ñ���QR��������ʵ�Գ����Ǿ����ȫ������ֵ�Լ���Ӧ����������
 ************************************************************************/
BOOL BSTQ(int Rank, Float* MainCross, Float* HypoCross,
    Float* Matrix, Float Eps, int MaxT)
{
  // �����Ķ���
  int i, j, k, m, it, u, v;
  Float d, f, h, g, p, r, e, s;

  // ����ֵ
  HypoCross[ Rank - 1 ] = 0.0;
  d = 0.0;
  f = 0.0;

  for (j = 0; j <= Rank - 1; j++) {
    //  �������ȵĿ���
    it = 0;
    h = Eps * (fabs(MainCross[ j ]) + fabs(HypoCross[ j ]));

    if (h > d) {
      d = h;
    }

    m = j;

    while ((m <= Rank - 1) && (fabs(HypoCross[ m ]) > d)) {
      m = m + 1;
    }

    if (m != j) {
      // ���е�������þ���A������ֵ����������
      do {
        // �����������������ص���ʧ��
        if (it == MaxT) {
          return (FALSE);
        }

        it = it + 1;
        g = MainCross[ j ];
        p = (MainCross[ j + 1 ] - g) / (2.0 * HypoCross[ j ]);
        r = sqrt(p * p + 1.0);

        // ���p����0
        if (p >= 0.0) {
          MainCross[ j ] = HypoCross[ j ] / (p + r);
        }
        else {
          MainCross[ j ] = HypoCross[ j ] / (p - r);
        }

        h = g - MainCross[ j ];

        //  �������Խ��ߵ�Ԫ��
        for (i = j + 1; i <= Rank - 1; i++) {
          MainCross[ i ] = MainCross[ i ] - h;
        }

        // ��ֵ
        f = f + h;
        p = MainCross[ m ];
        e = 1.0;
        s = 0.0;

        for (i = m - 1; i >= j; i--) {
          g = e * HypoCross[ i ];
          h = e * p;

          //  ���Խ���Ԫ�صľ���ֵ�Ƿ���ڴζԽ���Ԫ�ص�
          if (fabs(p) >= fabs(HypoCross[ i ])) {
            e = HypoCross[ i ] / p;
            r = sqrt(e * e + 1.0);
            HypoCross[ i + 1 ] = s * p * r;
            s = e / r;
            e = 1.0 / r;
          }
          else {
            e = p / HypoCross[ i ];
            r = sqrt(e * e + 1.0);
            HypoCross[ i + 1 ] = s * HypoCross[ i ] * r;
            s = 1.0 / r;
            e = e / r;
          }

          p = e * MainCross[ i ] - s * g;
          MainCross[ i + 1 ] = h + s * (e * g + s * MainCross[ i ]);

          // ���´洢��������
          for (k = 0; k <= Rank - 1; k++) {
            u = k * Rank + i + 1;
            v = u - 1;
            h = Matrix[ u ];
            Matrix[ u ] = s * Matrix[ v ] + e * h;
            Matrix[ v ] = e * Matrix[ v ] - s * h;
          }

        }

        // �����Խ��ߺʹζԽ���Ԫ�����¸�ֵ
        HypoCross[ j ] = s * p;
        MainCross[ j ] = e * p;

      }
      while (fabs(HypoCross[ j ]) > d);
    }

    MainCross[ j ] = MainCross[ j ] + f;
  }

  // ����A������ֵ
  for (i = 0; i <= Rank - 1; i++) {
    k = i;
    p = MainCross[ i ];

    // ��A����ֵ����p
    if (i + 1 <= Rank - 1) {
      j = i + 1;

      while ((j <= Rank - 1) && (MainCross[ j ] <= p)) {
        k = j;
        p = MainCross[ j ];
        j = j + 1;
      }
    }

    // �洢A������ֵ����������
    if (k != i) {
      MainCross[ k ] = MainCross[ i ];
      MainCross[ i ] = p;

      for (j = 0; j <= Rank - 1; j++) {
        u = j * Rank + i;
        v = j * Rank + k;
        p = Matrix[ u ];
        Matrix[ u ] = Matrix[ v ];
        Matrix[ v ] = p;
      }
    }
  }

  // ����ֵ
  return (TRUE);
}

/*************************************************************************
 *
* \�������ƣ�
*   IDFT_2D()
 *
* \�������:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes    - ָ��CDib���ָ�룬����ͼ������
*   Float* pTrRstRpart  - ָ����Ҷϵ��ʵ����ָ��
*   Float* pTrRstIpart  - ָ����Ҷϵ���鲿��ָ��
 *
* \����ֵ:
*   ��
 *
* \˵��:
*   ��ά����Ҷ���任��
 *
 *************************************************************************
 */

void IDFT_2D(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pTrRstRpart, Float* pTrRstIpart)
{
  //����ͼ���������
  int y;

  //����ͼ��ĺ�����
  int x;

  //Ƶ��ĺ�����
  int m;

  //Ƶ���������
  int n;

  Float fCosTable, fSinTable;
  Float fTmpPxValue, fRpartValue, fIpartValue;

  fCosTable = 0;
  fSinTable = 0;
  fTmpPxValue = 0;
  fRpartValue = 0;
  fIpartValue = 0;

  for (y = 0; y < lHeight; y++)
    for (x = 0; x < lWidth; x++) {
      fTmpPxValue = 0;

      for (n = 0; n < lHeight; n++)
        for (m = 0; m < lWidth; m++) {
          fCosTable = cos(2 * PI * (((Float) m * x) / lWidth + ((Float) n * y) / lHeight));
          fSinTable = sin(2 * PI * (((Float) m * x) / lWidth + ((Float) n * y) / lHeight));
          fRpartValue = *(pTrRstRpart + n * lHeight + m);
          fIpartValue = *(pTrRstIpart + n * lHeight + m);

          fTmpPxValue += fCosTable * fRpartValue - fSinTable * fIpartValue;
        }

      fTmpPxValue = fTmpPxValue / (lHeight * lWidth);
      *(lpDIBBits + lLineBytes * y + x) = (BYTE) fTmpPxValue;
    }
}

/*************************************************************************
* �������ƣ�
*   DIBOHOTELLING()
 *
* ����:
*   CDib  *pDib        - ָ��CDib���ָ��
 *
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
*   �ú����û����ֱ任����ͼ�������ת��
 ************************************************************************/
BOOL Hotelling(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // �涨�����ļ��㾫��
  Float Eps = 0.000001;

  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  // ѭ������
  int i;
  int j;

  // �����任��ͼ�������ܷ�Χ
  int lMaxRange;

  //  ��������ľ�ֵ
  int AverEx;
  int AverEy;

  //  �����ܵ�������
  int ToaCount;

  // ����ֵ��Э�������
  Float Matr4C[ 2 ][ 2 ];

  // ���Э����������������
  Float QMatrix[ 2 ][ 2 ];

  //  ���Խ�������ԽǺʹζԽ���Ԫ��
  Float MainCross[ 2 ];
  Float HypoCross[ 2 ];

  // �м����
  Float dTemp, *F;
  int lTempI;
  int lTempJ;

  // ����ͼ�󾭹���ת��������Ŀ�Ⱥ͸߶�
  if (lWidth > lHeight) {
    lMaxRange = lWidth;
  }
  else {
    lMaxRange = lHeight;
  }

  // ����ֵ
  AverEx = 0;
  AverEy = 0;
  ToaCount = 0;
  Matr4C[ 0 ][ 0 ] = Matr4C[ 0 ][ 1 ] = Matr4C[ 1 ][ 0 ] = Matr4C[ 1 ][ 1 ] = 0.0;

  // �����ڴ�
  F = MALLOC(Float, lWidth * lHeight);


  for (i = 0; i < lHeight; i++) {   // ��
    for (j = 0; j < lWidth; j++) {   // ��
      // ����ת���������ÿ���㸳��ֵ���Ҷ�ֵ255��Ӧ��ʾ��ɫ��
      F[ i * lWidth + j ] = 255;

      // ָ��λͼi��j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * i + j;

      // ֵС��255���Ǳ���ɫ��ɫ����������Ϊ�����һ����
      // ����������ֵx��y������ά���ʸ��
      if ((*lpSrc) < 255) {
        // �����������ص�Y�����X�����ۼ�ֵ
        AverEx = AverEx + i;
        AverEy = AverEy + j;

        // �����ܵ���������һ
        ToaCount++;

        // ���ʸ��Э���������ۼ�ֵ
        Matr4C[ 0 ][ 0 ] = Matr4C[ 0 ][ 0 ] + i * i;
        Matr4C[ 0 ][ 1 ] = Matr4C[ 0 ][ 1 ] + i * j;
        Matr4C[ 1 ][ 0 ] = Matr4C[ 1 ][ 0 ] + j * i;
        Matr4C[ 1 ][ 1 ] = Matr4C[ 1 ][ 1 ] + j * j;
      }
    }
  }


  // �������ʸ���ľ�ֵ
  AverEx = AverEx / ToaCount;
  AverEy = AverEy / ToaCount;

  //  �������ʸ����Э�������
  Matr4C[ 0 ][ 0 ] = Matr4C[ 0 ][ 0 ] / ToaCount - AverEx * AverEx;
  Matr4C[ 0 ][ 1 ] = Matr4C[ 0 ][ 1 ] / ToaCount - AverEx * AverEy;
  Matr4C[ 1 ][ 0 ] = Matr4C[ 1 ][ 0 ] / ToaCount - AverEx * AverEy;
  Matr4C[ 1 ][ 1 ] = Matr4C[ 1 ][ 1 ] / ToaCount - AverEy * AverEy;

  // ��Э������������ԽǶԳ���
  ThreeCross(*Matr4C, 2, *QMatrix, MainCross, HypoCross);

  // ��Э������������ֵ������ʸ����
  BSTQ(2, MainCross, HypoCross, *Matr4C, Eps, 50);

  // ������������ת��������������
  dTemp = Matr4C[ 0 ][ 1 ];
  Matr4C[ 0 ][ 1 ] = Matr4C[ 1 ][ 0 ];
  Matr4C[ 1 ][ 0 ] = dTemp;

  // ���������������й�һ��
  for (i = 0; i <= 1; i++) {
    dTemp = pow(Matr4C[ i ][ 0 ], 2) + pow(Matr4C[ i ][ 1 ], 2);
    dTemp = sqrt(dTemp);
    Matr4C[ i ][ 0 ] = Matr4C[ i ][ 0 ] / dTemp;
    Matr4C[ i ][ 1 ] = Matr4C[ i ][ 1 ] / dTemp;
  }

  // ���Ҿ������ֱ任����������ԭ����ϵ�е�����
  for (i = -lMaxRange + 1; i < lMaxRange; i++) {
    for (j = -lMaxRange + 1; j < lMaxRange; j++) {
      //  ��������ֵӳ�䵽�ɵ�����ϵ
      int Cx = (int)(i * Matr4C[ 0 ][ 0 ] - j * Matr4C[ 0 ][ 1 ]) + AverEx;
      int Cy = (int)(-i * Matr4C[ 1 ][ 0 ] + j * Matr4C[ 1 ][ 1 ]) + AverEy;

      //  ӳ��ֵ�Ƿ�����Դͼ��
      if (Cx >= 0 && Cx < lHeight && Cy >= 0 && Cy < lWidth) {
        lpSrc = (LPBYTE) lpDIBBits + lLineBytes * Cx + Cy;

        // ӳ��ֵ�Ƿ�����ԭ��������
        if (*(lpSrc) < 255) {
          //  ��������ϵԭ��ƽ�Ƶ����ģ��Ա���ʾ
          lTempI = (int)(lHeight / 2) + j;
          lTempJ = (int)(lWidth / 2) + i;

          // ������ܹ�������ʾ����ֵ�����飬���д洢
          if (lTempI >= 0 && lTempI < lHeight && lTempJ >= 0 && lTempJ < lWidth) {
            F[ lTempJ + (lTempI) * lWidth ] = *(lpSrc);
          }
        }
      }
    }
  }

  // ��
  for (i = 0; i < lMaxRange; i++) {
    // ��
    for (j = 0; j < lMaxRange; j++) {
      // �����ֱ任�������ֵ
      dTemp = F[ i * lMaxRange + j ];

      // ָ��λͼi��j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      // ����Դͼ��
      * (lpSrc) = (BYTE)(dTemp);
    }
  }

  // �ͷ��ڴ�
  SAFEFREE(F);

  // ����
  return TRUE;
}

/*************************************************************************
* �������ƣ�InverseFilter (int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
* ��������:
*   LPBYTE lpDIBBits, ָ��DIB��ʼ���ص�ָ��
*   int lWidth, DIB�Ŀ��
*   int lHeight, int lLineBytes, DIB�ĸ߶�
*     int lLineBytes, DIB�����ֽ�����Ϊ4�ı���
* ��������:BOOL
* ��������: ������ͼ��������˲���ԭ
************************************************************************/
BOOL InverseFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrcUnChr;    //ָ�����ص�ָ��
  int i, j; //ѭ������
  int wid = 1; // ʵ�ʽ��и���Ҷ�任�Ŀ�Ⱥ͸߶�
  int hei = 1;
  int widpor = 0;
  int heiPor = 0;
  Float tempre, tempim, a, b, c, d; //��ʱ����
  COMPLEX* pTdSrc, *pTh; //�����洢Դͼ��ͱ任�˵�ʱ������
  COMPLEX* pFdSrc, *pFh; //�����洢Դͼ��ͱ任�˵�Ƶ������
  Float MaxNum; //ͼ���һ������

  // ��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2�������η�
  while (wid * 2 <= lLineBytes) {
    wid = wid * 2;
    widpor++;
  }

  while (hei * 2 <= lHeight) {
    hei = hei * 2;
    heiPor++;
  }

  //�����˻�ͼ��ĳ��Ϳ����Ϊ2��������
  if (wid != (int) lLineBytes) {
    return FALSE;
  }

  if (hei != (int) lHeight) {
    return FALSE;
  }

  pTdSrc = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);     // Ϊʱ���Ƶ����������ռ�
  pTh = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);
  pFdSrc = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);
  pFh = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);

  // ���˻�ͼ�����ݴ���ʱ������
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * j + i;   // ָ���˻�ͼ������j�У���i�����ص�ָ��

      CplexSet((Float) * lpSrcUnChr, 0.0, pTdSrc[ lLineBytes * j + i ]);
      CplexSet(0.0, 0.0, pFdSrc[ lLineBytes * j + i ]);

      if (i < 5 && j < 5) {
        CplexSet(0.04, 0.0, pTh[ lLineBytes * j + i ]);
      }
      else {
        CplexSet(0.0, 0.0, pTh[ lLineBytes * j + i ]);
      }

      CplexSet(0.0, 0.0, pFh[ lLineBytes * j + i ]);
    }
  }

  Fourier1(pTdSrc, lLineBytes, lHeight, pFdSrc);   //���˻�ͼ�����FFT
  Fourier1(pTh, lLineBytes, lHeight, pFh);   //�Ա任��ͼ�����FFT

  for (i = 0; i < lHeight * lLineBytes; i++) { //Ƶ�����
    a = CREAL(pFdSrc[ i ]);
    b = CIMAG(pFdSrc[ i ]);
    c = CREAL(pFh[ i ]);
    d = CIMAG(pFh[ i ]);

    //���Ƶ��ֵ̫С�����迼��
    if (c * c + d * d > 1e-3) {
      tempre = (a * c + b * d) / (c * c + d * d);
      tempim = (b * c - a * d) / (c * c + d * d);
    }

    CplexSet(tempre, tempim, pFdSrc[ i ]);

  }

  InverseFourier(pFdSrc, pTdSrc, lLineBytes, lHeight);   //�Ը�ԭͼ����з�FFT
  MaxNum = 300; //ȷ����һ������

  for (j = 0; j < lHeight; j++) { //ת��Ϊ��ԭͼ��
    for (i = 0; i < lLineBytes; i++) {
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * j + i;   // ָ��ԭͼ������j�У���i�����ص�ָ��
      *lpSrcUnChr = (BYTE)(CREAL(pTdSrc[(lLineBytes) * j + i ]) * 255.0 / MaxNum);
    }
  }

  SAFEFREE(pTdSrc);   //�ͷŴ洢�ռ�
  SAFEFREE(pTh);
  SAFEFREE(pFdSrc);
  SAFEFREE(pFh);
  return TRUE; // ����
}

/*************************************************************************
* �������ƣ�WinnerFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
* ��������:
*   LPBYTE lpDIBBits, ָ��DIB��ʼ���ص�ָ��
*   int lWidth, DIB�Ŀ��
*   int lHeight, int lLineBytes, DIB�ĸ߶�
*     int lLineBytes, DIB�����ֽ�����Ϊ4�ı���
* ��������:BOOL
* ��������: ������ͼ�����ά���˲���ԭ
************************************************************************/
BOOL WinnerFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrcUnChr;    //ָ�����ص�ָ��
  int i; //ѭ������
  int j;
  Float temp, tempre, tempim,  //��ʱ����
        a, b, c, d, tempnorm;
  int wid = 1; // ʵ�ʽ��и���Ҷ�任�Ŀ�Ⱥ͸߶�
  int hei = 1;

  int widpor = 0;
  int heiPor = 0;
  COMPLEX* pTdSrc, *pTh; //�����洢Դͼ��ͱ任�˵�ʱ������
  COMPLEX* pFdSrc, *pFh; //�����洢Դͼ��ͱ任�˵�Ƶ������
  Float* pCFFilter;

  // ��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2�������η�
  while (wid * 2 <= lLineBytes) {
    wid = wid * 2;
    widpor++;
  }

  while (hei * 2 <= lHeight) {
    hei = hei * 2;
    heiPor++;
  }

  //�����˻�ͼ��ĳ��Ϳ����Ϊ2��������
  if (wid != (int) lLineBytes) {
    return FALSE;
  }

  if (hei != (int) lHeight) {
    return FALSE;
  }

  pTdSrc = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);     // Ϊʱ���Ƶ����������ռ�
  pTh = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);
  pFdSrc = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);
  pFh = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);

  pCFFilter = MALLOC(Float, lHeight * lLineBytes);   // �˲�����Ȩϵ��

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * j + i;   // ָ���˻�ͼ������j�У���i�����ص�ָ��
      // ������ֵ�洢��ʱ��������
      CplexSet((Float) * lpSrcUnChr, 0.0, pTdSrc[ lLineBytes * j + i ]);
      // Ƶ����ֵ
      CplexSet(0.0, 0.0, pFdSrc[ lLineBytes * j + i ]);

      if (i < 5 && j < 5) {   // �˻�ϵͳʱ��ά���˲���Ȩϵ����ֵ
        CplexSet(0.04, 0.0, pTh[ lLineBytes * j + i ]);
        pCFFilter[ lLineBytes * j + i ] = 0.5;
      }
      else {
        CplexSet(0.0, 0.0, pTh[ lLineBytes * j + i ]);
        pCFFilter[ lLineBytes * j + i ] = 0.05;
      }

      // Ƶ����ֵ
      CplexSet(0.0, 0.0, pFh[ lLineBytes * j + i ]);
    }
  }

  Fourier1(pTdSrc, lLineBytes, lHeight, pFdSrc);   //���˻�ͼ�����FFT
  Fourier1(pTh, lLineBytes, lHeight, pFh);         //�Ա任��ͼ�����FFT

  for (i = 0; i < lHeight * lLineBytes; i++) {  // ����M
    a = CREAL(pFdSrc[ i ]);   // ��ֵ
    b = CIMAG(pFdSrc[ i ]);
    c = CREAL(pFh[ i ]);
    d = CIMAG(pFh[ i ]);
    tempnorm = c * c + d * d; // ��ģ
    temp = (tempnorm) / (tempnorm + pCFFilter[ i ]);     // |H(u, v)|*|H(u, v)|/(|H(u, v)|*|H(u, v)|+a)
    {
      tempre = (a * c + b * d) / (c * c + d * d);
      tempim = (b * c - a * d) / (c * c + d * d);
      // ���f(u, v)
      CplexSet(temp * tempre, temp * tempim, pFdSrc[ i ]);
    }
  }

  InverseFourier(pFdSrc, pTdSrc, lLineBytes, lHeight);   //�Ը�ԭͼ����з�FFT

  for (j = 0; j < lHeight; j++) { //ת��Ϊ��ԭͼ��
    for (i = 0; i < lLineBytes; i++) {
      // ָ��ԭͼ������j�У���i�����ص�ָ��
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * j + i;
      a = CREAL(pTdSrc[(lLineBytes) * j + i ]);
      b = CIMAG(pTdSrc[(lLineBytes) * j + i ]);
      tempnorm = a * a + b * b;
      tempnorm = sqrt(tempnorm) + 40;

      if (tempnorm > 255) {
        tempnorm = 255.0;
      }

      if (tempnorm < 0) {
        tempnorm = 0;
      }

      *lpSrcUnChr = (BYTE)(tempnorm);
    }
  }

  SAFEFREE(pTdSrc);   //�ͷŴ洢�ռ�
  SAFEFREE(pTh);
  SAFEFREE(pFdSrc);
  SAFEFREE(pFh);
  SAFEFREE(pCFFilter);
  return TRUE; // ����
}

/*************************************************************************
* \�������ƣ�
*   LowPassFilter()
* \�������:
*   LPBYTE lpDIBBits     - ָ����Ҫ�˲���ͼ������ָ��
*   int lWidth      - ���ݿ��
*   int lHeight      - ���ݸ߶�
*   int nRadius1      - �����ͨ�˲��ĺ����˲��뾶
*  int nRadius2      - �����ͨ�˲��������˲��뾶
* \����ֵ:
*    BOOL       // �ɹ�����TRUE�����򷵻�FALSE��
* \˵��:
*  lpDIBBits ��ָ����Ҫ�˲���ͼ������ָ�롣
*  ������ͨ�˲���������Ȼ�洢��lpDIBBits ���С�
**************************************************************************/
BOOL LowPassFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nRadius1, int nRadius2)
{
  LPBYTE lpSrc;       // ָ��Դͼ���ָ��
  int x, y;          // ѭ�����Ʊ���
  Float dTmpOne;        //�����ʱ����
  Float dTmpTwo;        //�����ʱ����
  int nTransWidth;        // ����Ҷ�任�Ŀ�ȣ�2���������ݣ�
  int nTransHeight;        // ����Ҷ�任�ĸ߶ȣ�2���������ݣ�
  Float unchValue;        // ����ͼ������ػҶȵ���ʱ����
  Float dReal;         // ����Ҷ�任��ʵ��
  Float dImag;         // ����Ҷ�任���鲿
  COMPLEX* pCTData;      // ָ��ʱ�����ݵ�ָ��
  COMPLEX* pCFData;      // ָ��Ƶ�����ݵ�ָ��
  // ������и���Ҷ�任�ĵ��������� ��2���������ݣ�
  dTmpOne = log(lWidth) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransWidth = (int) dTmpTwo;
  // ������и���Ҷ�任�ĵ��������� ��2���������ݣ�
  dTmpOne = log(lHeight) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransHeight = (int) dTmpTwo;

  // ��ͨ�˲��İ뾶���ܳ���Ƶ������뾶
  if (nRadius1 > nTransWidth || nRadius2 > nTransHeight) {
    return (FALSE);          // ����FALSE
  }

  pCTData = MALLOC(COMPLEX, nTransWidth * nTransHeight);   // �����ڴ�
  pCFData = MALLOC(COMPLEX, nTransWidth * nTransHeight);   // �����ڴ�

  // ͼ�����ݵĿ�͸߲�һ����2���������ݣ�����pCTData��һ����������Ҫ��0
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      CplexSet(0, 0, pCTData[ y * nTransWidth + x ]);    // ����
    }
  }

  //��ͼ�����ݴ���pCTData
  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      // ָ��DIB��y�У���x�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      unchValue = *lpSrc;
      CplexSet(unchValue, 0, pCTData[ y * nTransWidth + x ]);
    }
  }

  Fft2D(pCTData, lWidth, lHeight, pCFData);       // ����Ҷ���任

  for (x = 0; x < nTransWidth; x++) {       //��ʼʵʩ����ĵ�ͨ�˲�
    for (y = nRadius2; y < nTransHeight - nRadius2; y++) {
      // ���������д���nRadius2�ĸ�Ƶ��������Ϊ0
      CplexSet(0, 0, pCFData[ x * nTransHeight + y ]);
    }
  }

  for (x = nRadius1; x < nTransWidth - nRadius1; x++) {
    for (y = 0; y < nTransHeight; y++) {
      // �Ѻ������д���nRadius1�ĸ�Ƶ��������Ϊ0
      CplexSet(0, 0, pCFData[ x * nTransHeight + y ]);
    }
  }

  IFFT_2D(pCFData, pCTData, lHeight, lWidth);       // ������ͨ�˲���ͼ����з��任

  for (y = 0; y < lHeight; y++) {         // ���任�����ݴ���lpDIBBits
    for (x = 0; x < lWidth; x++) {
      //��Ҫ�����źŵ����������Լ�ʵ�����õ�ͼ�������ǻҶ�ֵ����ԭʼ����
      dReal = CREAL(pCTData[ y * nTransWidth + x ]);     // ʵ��
      dImag = CIMAG(pCTData[ y * nTransWidth + x ]);    // �鲿
      unchValue = dReal;
      // ָ��DIB��y�У���x�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      *lpSrc = (BYTE) unchValue;
    }
  }

  SAFEFREE(pCTData);            // �ͷ��ڴ�
  SAFEFREE(pCFData);            // �ͷ��ڴ�
  pCTData = NULL;
  pCFData = NULL;
  return (TRUE);            //���ؽ��
}


/*************************************************************************
* \�������ƣ�
*   ButterWorthLowPass()
* \�������:
*  LPBYTE lpDIBBits     - ָ����Ҫ�˲���ͼ������ָ��
*   int lWidth      - ���ݿ��
*   int lHeight      - ���ݸ߶�
*   int nRadius      - ButterWorth��ͨ�˲���"�빦��"��
* \����ֵ:
*    BOOL       // �ɹ�����TRUE�����򷵻�FALSE��
* \˵��:
*  pDIBBits ��ָ����Ҫ�˲���ͼ������ָ�롣
*  ����ButterWorth��ͨ�˲���������Ȼ�洢��lpDIBBits ���С�
 **************************************************************************/
BOOL ButterWorthLowPass1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nRadius)
{
  LPBYTE lpSrc;       // ָ��Դͼ���ָ��
  int y;          // ѭ�����Ʊ���
  int x;          // ѭ�����Ʊ���
  Float dTmpOne;        //�����ʱ����
  Float dTmpTwo;        //�����ʱ����
  Float H;          // ButterWorth �˲�ϵ��
  int nTransWidth;        // ����Ҷ�任�Ŀ�ȣ�2���������ݣ�
  int nTransHeight;        // ����Ҷ�任�ĸ߶ȣ�2���������ݣ�
  Float dReal;         // ����Ҷ�任��ʵ��
  Float dImag;         // ����Ҷ�任���鲿
  Float unchValue;        // ����ͼ������ػҶȵ���ʱ����
  COMPLEX* pCTData;      // ָ��ʱ�����ݵ�ָ��
  COMPLEX* pCFData;      // ָ��Ƶ�����ݵ�ָ��
  // ������и���Ҷ�任�ĵ��������� ��2���������ݣ�
  dTmpOne = log(lWidth) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransWidth = (int) dTmpTwo;
  // ������и���Ҷ�任�ĵ��������� ��2���������ݣ�
  dTmpOne = log(lHeight) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransHeight = (int) dTmpTwo;
  pCTData = MALLOC(COMPLEX, nTransWidth * nTransHeight);   // �����ڴ�
  pCFData = MALLOC(COMPLEX, nTransWidth * nTransHeight);   // �����ڴ�

  // ͼ�����ݵĿ�͸߲�һ����2���������ݣ�����pCTData��һ����������Ҫ��0
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      CplexSet(0, 0, pCTData[ y * nTransWidth + x ]);   // ����
    }
  }

  for (y = 0; y < lHeight; y++) {   // ��ͼ�����ݴ���pCTData
    for (x = 0; x < lWidth; x++) {
      // ָ��DIB��y�У���x�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      unchValue = *lpSrc;
      CplexSet(unchValue, 0, pCTData[ y * nTransWidth + x ]);
    }
  }

  Fft2D(pCTData, lWidth, lHeight, pCFData);     // ����Ҷ���任

  for (y = 0; y < nTransHeight; y++) {   // ��ʼʵʩButterWorth��ͨ�˲�
    for (x = 0; x < nTransWidth; x++) {
      H = (Float)(y * y + x * x);
      H = H / (nRadius * nRadius);
      H = 1 / (1 + H);         // ��Hֵ
      CplexSet(CREAL(pCFData[ y * nTransWidth + x ]) * H,
          CIMAG(pCFData[ y * nTransWidth + x ]) * H, pCFData[ y * nTransWidth + x ]);
    }
  }

  // ����ButterWorth��ͨ�˲���ͼ����з��任
  IFFT_2D(pCFData, pCTData, lWidth, lHeight);

  for (y = 0; y < lHeight; y++) {        // ���任�����ݴ���lpDIBBits
    for (x = 0; x < lWidth; x++) {
      dReal = CREAL(pCTData[ y * nTransWidth + x ]);
      dImag = CIMAG(pCTData[ y * nTransWidth + x ]);
      unchValue = MAX(0, MIN(255, sqrt(dReal * dReal + dImag * dImag)));
      // ָ��DIB��y�У���x�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      *lpSrc = (BYTE) unchValue;
    }
  }

  SAFEFREE(pCTData);           // �ͷ��ڴ�
  SAFEFREE(pCFData);           // �ͷ��ڴ�
  return (TRUE);           //���ؽ��
}

/*************************************************************************
* \�������ƣ�
*   HighPassFilter()
* \�������:
*   LPBYTE lpDIBBits     - ָ����Ҫ�˲���ͼ������ָ��
*   int lWidth      - ���ݿ��
*   int lHeight      - ���ݸ߶�
*   int nRadius1      - �����ͨ�˲��ĺ����˲��뾶
*  int nRadius2      - �����ͨ�˲��������˲��뾶
* \����ֵ:
*    BOOL       // �ɹ�����TRUE�����򷵻�FALSE��
* \˵��:
*  lpDIBBits ��ָ����Ҫ�˲���ͼ������ָ�롣
*  ������ͨ�˲���������Ȼ�洢��lpDIBBits ���С�
**************************************************************************/
BOOL HighPassFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nRadius1, int nRadius2)
{
  LPBYTE lpSrc;       // ָ��Դͼ���ָ��
  int y;          // ѭ�����Ʊ���
  int x;          // ѭ�����Ʊ���
  Float dTmpOne;        //�����ʱ����
  Float dTmpTwo;        //�����ʱ����
  int nTransWidth;        // ����Ҷ�任�Ŀ�ȣ�2���������ݣ�
  int nTransHeight;        // ����Ҷ�任�ĸ߶ȣ�2���������ݣ�
  Float unchValue;        // ����ͼ������ػҶȵ���ʱ����
  COMPLEX* pCTData;      // ָ��ʱ�����ݵ�ָ��
  COMPLEX* pCFData;      // ָ��Ƶ�����ݵ�ָ��
  Float dReal;         // ����Ҷ�任��ʵ��
  Float dImag;         // ����Ҷ�任���鲿
  // ������и���Ҷ�任�ĵ��������� ��2���������ݣ�
  dTmpOne = log(lWidth) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransWidth = (int) dTmpTwo;
  // ������и���Ҷ�任�ĵ��������� ��2���������ݣ�
  dTmpOne = log(lHeight) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransHeight = (int) dTmpTwo;

  // �˲��İ뾶���ܳ���Ƶ������뾶
  if (nRadius1 > nTransWidth - 1 || nRadius2 > nTransHeight - 1) {
    return (FALSE);          // ����FALSE
  }

  pCTData = MALLOC(COMPLEX, nTransWidth * nTransHeight);   // �����ڴ�
  pCFData = MALLOC(COMPLEX, nTransWidth * nTransHeight);   // �����ڴ�

  // ͼ�����ݵĿ�͸߲�һ����2���������ݣ�����pCTData��һ����������Ҫ��0
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      CplexSet(0, 0, pCTData[ y * nTransWidth + x ]);   // ����
    }
  }

  for (y = 0; y < lHeight; y++) {   // ��ͼ�����ݴ���pCTData
    for (x = 0; x < lWidth; x++) {
      // ָ��DIB��y�У���x�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      unchValue = *lpSrc;
      CplexSet(unchValue, 0, pCTData[ y * nTransWidth + x ]);
    }
  }

  Fft2D(pCTData, lWidth, lHeight, pCFData);      // ����Ҷ���任

  // ���濪ʼ�˲���������С��nRadius�ĵ�Ƶ��������Ϊ0
  // ���õķ�������ŷʽ���룬�����������
  //  || (u, v)-(0, 0) || = max(|u|, |v|)
  for (x = 0; x < nTransWidth; x++) {   // ��ʼʵʩ����ĸ�ͨ�˲�
    for (y = 0; y < nRadius2; y++) {
      //����������С��nRadius2�ĵ�Ƶ��������Ϊ0
      CplexSet(0, 0, pCFData[ x * nTransHeight + y ]);
    }

    for (y = nTransHeight - 1 - nRadius2; y < nTransHeight; y++) {
      CplexSet(0, 0, pCFData[ x * nTransHeight + y ]);
    }
  }

  for (x = 0; x < nRadius1; x++) {
    //�Ѻ�������С��nRadius1�ĵ�Ƶ��������Ϊ0
    for (y = 0; y < nTransHeight; y++) {
      CplexSet(0, 0, pCFData[ x * nTransHeight + y ]);
    }
  }

  for (x = nTransWidth - nRadius1; x < nTransWidth; x++) {
    for (y = 0; y < nTransHeight; y++) {
      CplexSet(0, 0, pCFData[ x * nTransHeight + y ]);
    }
  }

  IFFT_2D(pCFData, pCTData, lHeight, lWidth);     // ������ͨ�˲���ͼ����з��任

  // ���任�����ݴ���lpDIBBits
  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      // ��Ϊ��ͨ�˲�����ѵ�Ƶ�ɷ�ȥ��������ͼ��������Ƚϰ���Ϊ�˽���������
      // �ھ�����ǿ���ͼ�����һ��������ʹͼ�������������ϸ��������ִ����൱��
      // ������һЩ��Ƶ��������Ƶ��(0, 0)���������������һ����������ͨЧ���ڹۿ�
      // �ϾͲ��Ǻܷ��㡣
      // ��Ҫ�����źŵ����������Լ�ʵ�����õ�ͼ�������ǻҶ�ֵ����ԭʼ����
      dReal = CREAL(pCTData[ y * nTransWidth + x ]);
      dImag = CIMAG(pCTData[ y * nTransWidth + x ]);
      unchValue = dReal;
      // ָ��DIB��y�У���x�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      *lpSrc = (BYTE) unchValue;
    }
  }

  SAFEFREE(pCTData);            // �ͷ��ڴ�
  SAFEFREE(pCFData);            // �ͷ��ڴ�
  return (TRUE);            // ���ؽ��
}

/*************************************************************************
* \�������ƣ�
*   ButterWorthHighPass()
* \�������:
*  LPBYTE lpDIBBits     - ָ����Ҫ�˲���ͼ������ָ��
*   int lWidth       - ���ݿ��
*   int lHeight      - ���ݸ߶�
*   int nRadius            - ButterWorth��ͨ�˲���"�빦��"��
* \����ֵ:
*    BOOL       // �ɹ�����TRUE�����򷵻�FALSE��
* \˵��:
*   lpImage ��ָ����Ҫ��ǿ������ָ�롣ע�⣬���ָ��ָ���������������
*     CDibָ����������� ��ΪCDibָ�����������ÿһ����DWORD����ġ�
*   ����ButterWorth��ͨ�˲������ݴ洢��lpImage���С�
 **************************************************************************/
BOOL ButterWorthHighPass1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nRadius)
{
  LPBYTE lpSrc;       // ָ��Դͼ���ָ��
  int x, y;           // ѭ�����Ʊ���
  Float dTmpOne, dTmpTwo; //�����ʱ����
  Float H;                // ButterWorth �˲�ϵ��
  int nTransWidth;         // ����Ҷ�任�Ŀ�ȣ�2���������ݣ�
  int nTransHeight;        // ����Ҷ�任�ĸ߶ȣ�2���������ݣ�
  Float dReal;            // ����Ҷ�任��ʵ��
  Float dImag;            // ����Ҷ�任���鲿
  Float unchValue;        // ����ͼ������ػҶȵ���ʱ����
  COMPLEX* pCTData;       // ָ��ʱ�����ݵ�ָ��
  COMPLEX* pCFData;       // ָ��Ƶ�����ݵ�ָ��
  // ������и���Ҷ�任�ĵ���������2���������ݣ�
  dTmpOne = log(lWidth) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransWidth = (int) dTmpTwo;
  // ������и���Ҷ�任�ĵ��������� ��2���������ݣ�
  dTmpOne = log(lHeight) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransHeight = (int) dTmpTwo;
  pCTData = MALLOC(COMPLEX, nTransWidth * nTransHeight);      // �����ڴ�
  pCFData = MALLOC(COMPLEX, nTransWidth * nTransHeight);      // �����ڴ�

  // ͼ�����ݵĿ�͸߲�һ����2���������ݣ�����pCTData��һ����������Ҫ��0
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      CplexSet(0, 0, pCTData[ y * nTransWidth + x ]);      // ����
    }
  }

  for (y = 0; y < lHeight; y++) {                         // ��ͼ�����ݴ���pCTData
    for (x = 0; x < lWidth; x++) {
      // ָ��DIB��y�У���x�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      unchValue = *lpSrc;
      CplexSet(unchValue, 0, pCTData[ y * nTransWidth + x ]);
    }
  }

  Fft2D(pCTData, lWidth, lHeight, pCFData);          // ����Ҷ���任

  for (y = 0; y < nTransHeight; y++) {                     // ���濪ʼʵʩButterWorth��ͨ�˲�
    for (x = 0; x < nTransWidth; x++) {
      H = (Float)(y * y + x * x);
      H = (nRadius * nRadius) / H;
      H = 1 / (1 + H);                          // ��Hֵ
      CplexSet(H * (CREAL(pCFData[ y * nTransWidth + x ])),
          H * (CIMAG(pCFData[ y * nTransWidth + x ])),
          pCFData[ y * nTransWidth + x ]);
    }
  }

  // ����ButterWorth��ͨ�˲���ͼ����з��任
  IFFT_2D(pCFData, pCTData, lWidth, lHeight);

  // ���任�����ݴ���lpDIBBits
  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      dReal = CREAL(pCTData[ y * nTransWidth + x ]);
      dImag = CIMAG(pCTData[ y * nTransWidth + x ]);
      unchValue = MAX(0, MIN(255, sqrt(dReal * dReal + dImag * dImag) + 100));
      // ָ��DIB��y�У���x�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      *lpSrc = (BYTE) unchValue;
    }
  }

  SAFEFREE(pCTData);                                      // �ͷ��ڴ�
  SAFEFREE(pCFData);                                      // �ͷ��ڴ�
  return (TRUE);                                      // ���ؽ��
}

