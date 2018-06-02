// ��Ե���

#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "filters.h"
//#include "macro.h"



#define _IABS(a) (((a) ^ ((a) < 0 ? -1 : 0)) - ((a) < 0 ? -1 : 0))

#ifndef MALLOC
#define MALLOC(TYPE, size)  (TYPE*)pmalloc(sizeof(TYPE) * (size_t)(size))
#endif

/*************************************************************************
* \�������:
*   double sigma               - ��˹�����ı�׼��
*   double **pdKernel          - ָ���˹���������ָ��
*   int *pnWindowSize          - ���ݵĳ���
*
* \����ֵ:
*   ��
*
* \˵��:
*   ���������������һ��һά�ĸ�˹�������������ݣ������ϸ�˹���ݵĳ���Ӧ
*   �������޳��ģ�����Ϊ�˼���ļ򵥺��ٶȣ�ʵ�ʵĸ�˹����ֻ�������޳���
*   pnWindowSize�������ݳ���
*
*************************************************************************
*/
void MakeGauss(double sigma, double** pdKernel, int* pnWindowSize)
{
  // ѭ�����Ʊ���
  int i;

  // ��������ĵ�
  int nCenter;

  // �����ĳһ�㵽���ĵ�ľ���
  double dDis;

  // �м����
  double dValue;
  double dSum;
  dSum = 0;

  // ���鳤�ȣ����ݸ����۵�֪ʶ��ѡȡ[-3*sigma, 3*sigma]���ڵ����ݡ�
  // ��Щ���ݻḲ�Ǿ��󲿷ֵ��˲�ϵ��
  *pnWindowSize = 1 + 2 * (int) ceil(3 * sigma);

  // ����
  nCenter = (*pnWindowSize) / 2;

  // �����ڴ�
  *pdKernel = MALLOC(double, *pnWindowSize);

  for (i = 0; i < (*pnWindowSize); i++) {
    dDis = (double)(i - nCenter);
    dValue = exp((-1. / 2.) * dDis * dDis / (sigma * sigma)) / (sqrt(2 * PI) * sigma);
    (*pdKernel) [ i ] = dValue;
    dSum += dValue;
  }

  // ��һ��
  for (i = 0; i < (*pnWindowSize); i++) {
    (*pdKernel) [ i ] /= dSum;
  }
}

/*************************************************************************
 * \�������:
 *   unsigned char* pUnchImg      - ָ��ͼ�����ݵ�ָ��
 *   int nWidth           - ͼ�����ݿ��
 *   int nHeight          - ͼ�����ݸ߶�
 *   double dSigma        - ��˹�����ı�׼��
 *   unsigned char* pUnchSmthdImg - ָ�򾭹�ƽ��֮���ͼ������
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   Ϊ����ֹ���������ø�˹�˲���ͼ������˲����˲��ȶ�x������У�Ȼ���
 *   y������С�
 *
 *************************************************************************
 */
void GaussianSmooth(const unsigned char* pUnchImg, int nWidth, int nHeight,
    double sigma, unsigned char* pUnchSmthdImg)
{
  // ѭ�����Ʊ���
  int y;
  int x;

  int i;

  // ��˹�˲��������鳤��
  int nWindowSize;

  //  ���ڳ��ȵ�1/2
  int nHalfLen;

  // һά��˹�����˲���
  double* pdKernel;

  // ��˹ϵ����ͼ�����ݵĵ��
  double dDotMul;

  // ��˹�˲�ϵ�����ܺ�
  double dWeightSum;

  // �м����
  double* pdTmp;

  // �����ڴ�
  pdTmp = MALLOC(double, nWidth * nHeight);
  assert(pdTmp);

  // ����һά��˹�����˲���
  MakeGauss(sigma, &pdKernel, &nWindowSize);

  // MakeGauss���ش��ڵĳ��ȣ����ô˱������㴰�ڵİ볤
  nHalfLen = nWindowSize / 2;

  // x��������˲�
  for (y = 0; y < nHeight; y++) {
    for (x = 0; x < nWidth; x++) {
      dDotMul = 0;
      dWeightSum = 0;

      for (i = (-nHalfLen); i <= nHalfLen; i++) {
        // �ж��Ƿ���ͼ���ڲ�
        if ((i + x) >= 0 && (i + x) < nWidth) {
          dDotMul += (double) pUnchImg[ y * nWidth + (i + x) ] * pdKernel[ nHalfLen + i ];
          dWeightSum += pdKernel[ nHalfLen + i ];
        }
      }

      pdTmp[ y * nWidth + x ] = dDotMul / dWeightSum;
    }
  }

  // y��������˲�
  for (x = 0; x < nWidth; x++) {
    for (y = 0; y < nHeight; y++) {
      dDotMul = 0;
      dWeightSum = 0;

      for (i = (-nHalfLen); i <= nHalfLen; i++) {
        // �ж��Ƿ���ͼ���ڲ�
        if ((i + y) >= 0 && (i + y) < nHeight) {
          dDotMul += (double) pdTmp[(y + i) * nWidth + x ] * pdKernel[ nHalfLen + i ];
          dWeightSum += pdKernel[ nHalfLen + i ];
        }
      }

      pUnchSmthdImg[ y * nWidth + x ] = (unsigned char)(int)(dDotMul / dWeightSum);
    }
  }

  // �ͷ��ڴ�
  pfree(pdKernel);
  pdKernel = NULL;

  pfree(pdTmp);
  pdTmp = NULL;
}

/*************************************************************************
* \�������:
*   unsigned char* pSrc      - ������˹�˲����ͼ��
*   int nWidht       - ͼ����
*   int nHeight      - ͼ��߶�
*   int *pnMag       - �ݶȷ���
*   int *pnGradX     - x����ķ�����
*   int *pnGradY     - y����ķ�����
*
* \����ֵ:
*   ��
*
* \˵��:
*   ����������㷽�����ݶȷ��ȣ����õ�΢��������(-1 0 1) �� (-1 0 1)'(ת��)
*   �����ʱ��Ա߽����ز��������⴦��
*
*************************************************************************
*/
void DirGradMagnitude(const unsigned char* pSrc, int nWidth, int nHeight, int* pnMag, int* pnGradX, int* pnGradY)
{
  // ѭ�����Ʊ���
  int x, y;

  for (y = 0; y < nHeight; y++) {
    int pos = y * nWidth;
    const unsigned char* _pUnch = pSrc + pos;
    const unsigned char* _pUnchPrev = pSrc + MAX(0, y - 1) * nWidth;
    const unsigned char* _pUnchNext = pSrc + MIN(nHeight - 1, y + 1) * nWidth;
    int* _pnGradX = pnGradX + pos;
    int* _pnGradY = pnGradY + pos;
    int* _pnMag = pnMag + pos;

    for (x = 0; x < nWidth; x++) {
      int nGradX, nGradY;
      int xPrev = MAX(0, x - 1);
      int xNext = MIN(nWidth - 1, x + 1);
      // ����x��y����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
#if 0

      nGradX = (int)(_pUnch[ xNext ] - _pUnch[ xPrev ]);
      nGradY = (int)(_pUnchNext[ x ] - _pUnchPrev[ x ]);
      _pnGradX[ x ] = nGradX;
      _pnGradY[ x ] = nGradY;
      _pnMag[ x ] = (int)(sqrt((double)(nGradX * nGradX + nGradY * nGradY)) + 0.5);
#else

      nGradX = (int)(_pUnch[ xNext ] - _pUnch[ xPrev ]);
      nGradX += nGradX + (_pUnchNext[ xNext ] - _pUnchNext[ xPrev ]) + (_pUnchPrev[ xNext ] - _pUnchPrev[ xPrev ]);
      nGradX >>= 2;
      nGradY = (int)(_pUnchNext[ x ] - _pUnchPrev[ x ]);
      nGradY += nGradY + (_pUnchNext[ xNext ] - _pUnchPrev[ xNext ]) + (_pUnchNext[ xPrev ] - _pUnchPrev[ xPrev ]);
      nGradY >>= 2;
      _pnGradX[ x ] = nGradX;
      _pnGradY[ x ] = nGradY;
      _pnMag[ x ] = (nGradX + nGradY);
#endif

    }
  }
}

/*************************************************************************
 * \�������:
 *   int *pnMag         - �ݶ�ͼ
 *   int *pnGradX       - x����ķ�����
 *   int *pnGradY       - y����ķ�����
 *   int nWidth         - ͼ�����ݿ��
 *   int nHeight        - ͼ�����ݸ߶�
 *   unsigned char* pUnchRst    - ����NonmaxSuppress�����Ľ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ��ֹ�ݶ�ͼ�зǾֲ���ֵ������ء�
 *
 *************************************************************************
 */
void NonmaxSuppress(const int* pnMag, const int* pnGradX, const int* pnGradY, int nWidth,
    int nHeight, unsigned char* pUnchRst)
{
  // ѭ�����Ʊ���
  int x, y;
  int nPos;

  // x�����ݶȷ���
  int gx, gy;

  // ��ʱ����
  int g1, g2, g3, g4;
  double weight, dTmp1, dTmp2, dTmp;

  for (y = 1; y < nHeight - 1; y++) {
    for (x = 1; x < nWidth - 1; x++) {
      nPos = y * nWidth + x;

      // �����ǰ���ص��ݶȷ���Ϊ0�����Ǳ߽��
      if (pnMag[ nPos ] == 0) {
        pUnchRst[ nPos ] = 1;
      }
      else {
        int nPos1 = nPos - nWidth;
        int nPos2 = nPos + nWidth;
        // ��ǰ���ص��ݶȷ���
        dTmp = pnMag[ nPos ];

        // x��y������
        gx = pnGradX[ nPos ];
        gy = pnGradY[ nPos ];

        // ���������y������x������˵�������ķ�����ӡ�������y������
        if (abs(gy) > abs(gx)) {
          // �����ֵ�ı���
          weight = fabs((double) gx) / fabs((double) gy);

          g2 = pnMag[ nPos1 ];
          g4 = pnMag[ nPos2 ];

          // ���x��y��������ķ������ķ�����ͬ
          // C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
          // g1 g2
          //   C
          //   g4 g3
          if (gx * gy > 0) {
            g1 = pnMag[ nPos1 - 1 ];
            g3 = pnMag[ nPos2 + 1 ];
          }

          // ���x��y��������ķ������ķ����෴
          // C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
          //    g2 g1
          //   C
          // g3 g4
          else {
            g1 = pnMag[ nPos1 + 1 ];
            g3 = pnMag[ nPos2 - 1 ];
          }
        }

        // ���������x������y������˵�������ķ�����ӡ�������x����
        // ����ж���������x������y������ȵ����
        else {
          // �����ֵ�ı���
          weight = fabs((double) gy) / fabs((double) gx);

          g2 = pnMag[ nPos + 1 ];
          g4 = pnMag[ nPos - 1 ];

          // ���x��y��������ķ������ķ�����ͬ
          // C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
          // g3
          // g4 C g2
          //       g1
          if (gx * gy > 0) {
            g1 = pnMag[ nPos2 + 1 ];
            g3 = pnMag[ nPos1 - 1 ];
          }
          // ���x��y��������ķ������ķ����෴
          // C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
          //      g1
          // g4 C g2
          //  g3
          else {
            g1 = pnMag[ nPos1 + 1 ];
            g3 = pnMag[ nPos2 - 1 ];
          }
        }

        // ��������g1-g4���ݶȽ��в�ֵ
        {
          dTmp1 = weight * g1 + (1 - weight) * g2;
          dTmp2 = weight * g3 + (1 - weight) * g4;

          // ��ǰ���ص��ݶ��Ǿֲ������ֵ
          // �õ�����Ǹ��߽��
          if (dTmp >= dTmp1 && dTmp >= dTmp2) {
            pUnchRst[ nPos ] = 0;
          }
          else {
            // �������Ǳ߽��
            pUnchRst[ nPos ] = 1;
          }
        }
      } //else
    } // for

  }
}

/*************************************************************************
 * \�������:
 *   int *pnMag               - �ݶȷ���ͼ
 *  int nWidth               - ͼ�����ݿ��
 *  int nHeight              - ͼ�����ݸ߶�
 *   int *pnThdHigh           - ����ֵ
 *   int *pnThdLow            - ����ֵ
 *  double dRatioLow         - ����ֵ�͸���ֵ֮��ı���
 *  double dRatioHigh        - ����ֵռͼ�����������ı���
 *   unsigned char* pUnchEdge - ����non-maximum����������
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ����non-maximum����������pUnchEdge��ͳ��pnMag��ֱ��ͼ��ȷ����ֵ��
 *   ��������ֻ��ͳ��pUnchEdge�п���Ϊ�߽�����Щ���ء�Ȼ������ֱ��ͼ��
 *   ����dRatioHigh���ø���ֵ���洢��pnThdHigh������dRationLow�͸���ֵ��
 *   ���õ���ֵ���洢��*pnThdLow��dRatioHigh��һ�ֱ����������ݶ�С��
 *   *pnThdHigh��������Ŀռ��������Ŀ�ı�����dRationLow����*pnThdHigh
 *   ��*pnThdLow�ı��������������canny�㷨��ԭ������߸�����һ�����䡣
 *
 *************************************************************************
 */
void EstimateThreshold(const int* pnMag, int nWidth, int nHeight, int* pnThdHigh, int* pnThdLow,
    const unsigned char* pUnchEdge, double dRatioHigh, double dRationLow)
{
  // ѭ�����Ʊ���
  int x, y, k;

  // ������Ĵ�С���ݶ�ֵ�ķ�Χ�йأ�������ñ�������㷨����ô�ݶȵķ�Χ���ᳬ��pow(2,10)
  int nHist[ 1024 ];

  // ���ܵı߽���Ŀ
  int nEdgeNb;

  // ����ݶ�ֵ
  int nMaxMag;

  int nHighCount;

  nMaxMag = 0;

  // ��ʼ��
  for (k = 0; k < 1024; k++) {
    nHist[ k ] = 0;
  }

  // ͳ��ֱ��ͼ��Ȼ������ֱ��ͼ������ֵ
  for (y = 0; y < nHeight; y++) {
    for (x = 0; x < nWidth; x++) {
      // ֻ��ͳ����Щ�����Ǳ߽�㣬���һ�û�д����������
      if (!pUnchEdge[ y * nWidth + x ]) {
        nHist[ pnMag[ y * nWidth + x ] ] ++;
      }
    }
  }

  nEdgeNb = nHist[ 0 ];
  nMaxMag = 0;

  // ͳ�ƾ����������ֵ��ֹ(non-maximum suppression)�����ж�������
  for (k = 1; k < 1024; k++) {
    if (nHist[ k ] != 0) {
      // ����ݶ�ֵ
      nMaxMag = k;
    }

    // �ݶ�Ϊ0�ĵ��ǲ�����Ϊ�߽���
    // ����non-maximum suppression���ж�������
    nEdgeNb += nHist[ k ];
  }

  // �ݶȱȸ���ֵ*pnThdHighС�����ص�����Ŀ
  nHighCount = (int)(dRatioHigh * nEdgeNb + 0.5);

  k = 1;
  nEdgeNb = nHist[ 1 ];

  // �������ֵ
  while ((k < (nMaxMag - 1)) && (nEdgeNb < nHighCount)) {
    k++;
    nEdgeNb += nHist[ k ];
  }

  // ���ø���ֵ
  *pnThdHigh = k;

  // ���õ���ֵ
  *pnThdLow = (int)((*pnThdHigh) * dRationLow + 0.5);
}

/*************************************************************************
 * \�������:
 *   int    x         - ��������x����
 *   int    y         - ��������y����
 *   int nLowThd       - �ж�һ�����Ƿ�Ϊ�߽��ĵ���ֵ
 *   unsigned char* pUnchEdge - ��¼�߽��Ļ�����
 *   int nWidth               - ͼ�����ݿ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ݹ����
 *   ��(x,y)������������б߽��ĸ��٣�����ֻ����pUnchEdge��û�д�����
 *   �����Ǳ߽�����Щ����(=0),����ֵΪ1�����õ㲻�����Ǳ߽�㣬����ֵ
 *   Ϊ255�����õ��Ѿ�������Ϊ�߽�㣬�����ٿ���
 *
 *************************************************************************
 */
void TraceEdge(unsigned char* edges, int step)
{
  // ��8�������ؽ��в�ѯ
  if (*edges) {
    return ;
  }

  *edges = 2;
  TraceEdge(edges + 1, step);
  TraceEdge(edges - 1, step);
  TraceEdge(edges + step + 1, step);
  TraceEdge(edges + step , step);
  TraceEdge(edges + step - 1, step);
  TraceEdge(edges - step - 1, step);
  TraceEdge(edges - step , step);
  TraceEdge(edges - step + 1, step);
}

/*************************************************************************
 * \�������:
 *   int *pnMag              - �ݶȷ���ͼ
 *  int nWidth               - ͼ�����ݿ��
 *  int nHeight              - ͼ�����ݸ߶�
 *  double dRatioLow         - ����ֵ�͸���ֵ֮��ı���
 *  double dRatioHigh        - ����ֵռͼ�����������ı���
 *   unsigned char* pUnchEdge        - ��¼�߽��Ļ�����
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ������ʵ�����ơ��������󡱵�һ�����ܣ�Ҳ���ǣ��ȵ���EstimateThreshold
 *   �����Ծ���non-maximum����������pUnchSpr����һ������ֵ��Ȼ���ж�
 *   pUnchSpr�п��ܵı߽�����(=0)���ݶ��ǲ��Ǵ��ڸ���ֵnThdHigh�������
 *   ����ֵ�󣬸õ㽫��Ϊһ���߽����㣬����TraceEdge�������Ѷ�Ӧ�ñ߽�
 *   �����������ҳ�������󣬵������������ʱ�������������û�б���־��
 *   �߽�㣬��ô��һ�����Ǳ߽�㡣
 *
 *************************************************************************
 */
void Hysteresis(int* pnMag, int nWidth, int nHeight, double dRatioLow,
    double dRatioHigh, unsigned char* pUnchEdge)
{
  // ѭ�����Ʊ���
  int x, y, nPos;

  int nThdHigh, nThdLow;
  // ����TraceEdge��Ҫ�ĵ���ֵ���Լ�Hysteresis����ʹ�õĸ���ֵ
  EstimateThreshold(pnMag, nWidth, nHeight, &nThdHigh,
      &nThdLow, pUnchEdge, dRatioHigh, dRatioLow);

  // ���ѭ������Ѱ�Ҵ���nThdHigh�ĵ㣬��Щ�㱻���������߽�㣬Ȼ����
  // TraceEdge���������ٸõ��Ӧ�ı߽�
  // ����ͼ���Ե����Ϊ�����ܵı߽��
  memset(pUnchEdge, 1, nWidth);
  memset(pUnchEdge + nWidth * (nHeight - 1), 1, nWidth);

  for (y = 1; y < nHeight - 1; y++) {
    unsigned char* _pUnchEdge = pUnchEdge + y * nWidth;
    int* _pnMag = pnMag + y * nWidth;
    _pUnchEdge[ 0 ] = _pUnchEdge[ nWidth - 1 ] = 1;

    for (x = 1; x < nWidth - 1; x++) {
      nPos = y * nWidth + x;

      if (_pnMag[ x ] < nThdLow) {
        _pUnchEdge[ x ] = 1;
      }
    }
  }

  for (y = 1; y < nHeight - 1; y++) {
    unsigned char* _pUnchEdge = pUnchEdge + y * nWidth;
    int* _pnMag = pnMag + y * nWidth;

    for (x = 1; x < nWidth - 1; x++) {
      // ����������ǿ��ܵı߽�㣬�����ݶȴ��ڸ���ֵ����������Ϊ
      // һ���߽�����
      if ((!_pUnchEdge[ x ]) && (_pnMag[ x ] >= nThdHigh)) {
        // ���øõ�Ϊ�߽��
        TraceEdge(_pUnchEdge + x, nWidth);
      }
    }
  }

  // ��Щ��û�б�����Ϊ�߽��������Ѿ������ܳ�Ϊ�߽��
  for (y = 0; y < nHeight; y++) {
    unsigned char* _pUnchEdge = pUnchEdge + y * nWidth;

    for (x = 0; x < nWidth; x++) {
      // 1����Ϊ�Ǳ߽��, 2����Ϊ�߽��
      _pUnchEdge[ x ] = (unsigned char) - (_pUnchEdge[ x ] >> 1);
    }
  }
}

/*************************************************************************
* \�������:
*   unsigned char* pUnchImage- ͼ������
*  int nWidth               - ͼ�����ݿ��
*  int nHeight              - ͼ�����ݸ߶�
*  double sigma             - ��˹�˲��ı�׼����
*  double dRatioLow         - ����ֵ�͸���ֵ֮��ı��� 0.4
*  double dRatioHigh        - ����ֵռͼ�����������ı��� 0.79
*  unsigned char* pUnchEdge - canny���Ӽ����ķָ�ͼ
*
* \����ֵ:
*   ��
*
* \˵��:
*   canny�ָ����ӣ�����Ľ��������pUnchEdge�У��߼�1(255)��ʾ�õ�Ϊ
*   �߽�㣬�߼�0(0)��ʾ�õ�Ϊ�Ǳ߽�㡣�ú����Ĳ���sigma��dRatioLow
*   dRatioHigh������Ҫָ���ġ���Щ������Ӱ��ָ��߽����Ŀ�Ķ���
*************************************************************************
*/
void Canny(int nHeight, int nWidth, const unsigned char* pUnchImage, unsigned char* pUnchEdge,
    double sigma, double dRatioLow, double dRatioHigh)
{
  // ������˹�˲����ͼ������
  unsigned char* pUnchSmooth;

  // ָ��x��������ָ��
  int* pnGradX;

  // ָ��y��������ָ��
  int* pnGradY;

  // �ݶȵķ���
  int* pnGradMag;
  int bufsize = nWidth * nHeight;

  pUnchSmooth = MALLOC(unsigned char, bufsize);
  pnGradMag = MALLOC(int, 3 * bufsize);
  pnGradX = pnGradMag + bufsize;
  pnGradY = pnGradMag + 2 * bufsize;

  // ��ԭͼ������˲�
  GaussianSmooth(pUnchImage, nWidth, nHeight, sigma, pUnchSmooth);

  // ���㷽�������ݶȵķ���
  DirGradMagnitude(pUnchImage, nWidth, nHeight, pnGradMag, pnGradX, pnGradY);

  // Ӧ�� non-maximum ����
  NonmaxSuppress(pnGradMag, pnGradX, pnGradY, nWidth, nHeight, pUnchEdge);

  // Ӧ��Hysteresis���ҵ����еı߽�
  Hysteresis(pnGradMag, nWidth, nHeight, dRatioLow, dRatioHigh, pUnchEdge);

  // �ͷ��ڴ�
  pfree(pnGradMag);
  pnGradMag = NULL;
  pfree(pUnchSmooth);
  pUnchSmooth = NULL;
}
