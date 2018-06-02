
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

/*************************************************************************
* \�������ƣ�
*   RegionSegFixThreshold()
 *
* \�������:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes     - ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
*   int nThreshold     - ����ָ����ֵ
 *
* \����ֵ:
*   ��
 *
* \˵��:
*   1���߼�����ʾ��Ӧ����Ϊǰ������0��ʾ������ֵ�ָ�Ĺؼ�����
*   ������ֵ��ѡȡ����ֵ��ѡȡһ��Ӧ����ʵ�ʵ�Ӧ�ö�
*   ����趨��
 **************************************************************************/
void RegionSegFixThreshold(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nThreshold)
{
  int y; //����ͼ���������
  int x; //����ͼ��ĺ�����

  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      if (*(lpDIBBits + y * lLineBytes + x) < nThreshold) {
        * (lpDIBBits + y * lLineBytes + x) = 0;
      }
    }
  }
}

/*************************************************************************
* \�������ƣ�
*   RegionSegAdaptive()
 *
* \�������:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes     - ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
 *
* \����ֵ:
*   ��
 *
* \˵��:
*   1���߼�����ʾ��Ӧ����Ϊǰ������0��ʾ����
*   ��ֵ�ָ�Ĺؼ�����������ֵ��ѡȡ����ֵ��ѡȡһ��Ӧ����ʵ�ʵ�Ӧ�ö�
*   ����趨���������У���ֵ���ǹ̶��ģ����Ǹ���ͼ�����ص�ʵ�����ʶ��趨�ġ�
*   ���������ͼ��ֳ��ĸ���ͼ��Ȼ�����ÿ����ͼ��ľ�ֵ�����ݾ�ֵ������ֵ
*   ��ֵֻ��Ӧ���ڶ�Ӧ����ͼ��
 **************************************************************************/
void RegionSegAdaptive(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  int y; //����ͼ���������
  int x; //����ͼ��ĺ�����

  // �ֲ���ֵ
  int nThd[ 2 ][ 2 ];

  // ��ͼ���ƽ��ֵ
  int nLocAvg;

  // ������ͼ�����ɨ�裺
  nLocAvg = 0;

  // y����
  for (y = 0; y < lHeight / 2; y++) {
    // x����
    for (x = 0; x < lWidth / 2; x++) {
      nLocAvg += lpDIBBits[ y * lLineBytes + x ];
    }
  }

  // �����ֵ
  nLocAvg /= ((lHeight / 2) * (lWidth / 2));

  // ������ֵΪ��ͼ���ƽ��ֵ
  nThd[ 0 ][ 0 ] = nLocAvg;

  // ������ͼ�����ɨ����зָ
  // y����
  for (y = 0; y < lHeight / 2; y++) {
    // x����
    for (x = 0; x < lWidth / 2; x++) {
      if (lpDIBBits[ y * lLineBytes + x ] < nThd[ 0 ][ 0 ]) {
        lpDIBBits[ y * lLineBytes + x ] = 255;
      }
      else {
        lpDIBBits[ y * lLineBytes + x ] = 0;
      }

    }
  }

  // =============================================
  // ������ͼ�����ɨ�裺
  nLocAvg = 0;

  // y����
  for (y = lHeight / 2; y < lHeight; y++) {
    // x����
    for (x = 0; x < lWidth / 2; x++) {
      nLocAvg += lpDIBBits[ y * lLineBytes + x ];
    }
  }

  // �����ֵ
  nLocAvg /= ((lHeight - lHeight / 2) * (lWidth / 2));

  // ������ֵΪ��ͼ���ƽ��ֵ
  nThd[ 1 ][ 0 ] = nLocAvg;

  // ������ͼ�����ɨ����зָ
  // y����
  for (y = lHeight / 2; y < lHeight; y++) {
    // x����
    for (x = 0; x < lWidth / 2; x++) {
      if (lpDIBBits[ y * lLineBytes + x ] < nThd[ 1 ][ 0 ]) {
        lpDIBBits[ y * lLineBytes + x ] = 255;
      }
      else {
        lpDIBBits[ y * lLineBytes + x ] = 0;
      }

    }
  }

  // =============================================
  // ������ͼ�����ɨ�裺
  nLocAvg = 0;

  // y����
  for (y = 0; y < lHeight / 2; y++) {
    // x����
    for (x = lWidth / 2; x < lWidth; x++) {
      nLocAvg += lpDIBBits[ y * lLineBytes + x ];
    }
  }

  // �����ֵ
  nLocAvg /= ((lHeight / 2) * (lWidth - lWidth / 2));

  // ������ֵΪ��ͼ���ƽ��ֵ
  nThd[ 0 ][ 1 ] = nLocAvg;

  // ������ͼ�����ɨ����зָ
  // y����
  for (y = 0; y < lHeight / 2; y++) {
    // x����
    for (x = lWidth / 2; x < lWidth; x++) {
      if (lpDIBBits[ y * lLineBytes + x ] < nThd[ 0 ][ 1 ]) {
        lpDIBBits[ y * lLineBytes + x ] = 255;
      }
      else {
        lpDIBBits[ y * lLineBytes + x ] = 0;
      }

    }
  }

  // =============================================
  // ������ͼ�����ɨ�裺
  nLocAvg = 0;

  // y����
  for (y = lHeight / 2; y < lHeight; y++) {
    // x����
    for (x = lWidth / 2; x < lWidth; x++) {
      nLocAvg += lpDIBBits[ y * lLineBytes + x ];
    }
  }

  // �����ֵ
  nLocAvg /= ((lHeight - lHeight / 2) * (lWidth - lWidth / 2));

  // ������ֵΪ��ͼ���ƽ��ֵ
  nThd[ 1 ][ 1 ] = nLocAvg;

  // ������ͼ�����ɨ����зָ
  // y����
  for (y = lHeight / 2; y < lHeight; y++) {
    // x����
    for (x = lWidth / 2; x < lWidth; x++) {
      if (lpDIBBits[ y * lLineBytes + x ] < nThd[ 1 ][ 1 ]) {
        lpDIBBits[ y * lLineBytes + x ] = 255;
      }
      else {
        lpDIBBits[ y * lLineBytes + x ] = 0;
      }
    }
  }

  // Ϊ����ʾ������ʾ���߼�1�ú�ɫ��ʾ���߼�0�ð�ɫ��ʾ
  for (y = 0; y < lHeight; y++) {
    // x����
    for (x = 0; x < lWidth; x++) {
      lpDIBBits[ y * lLineBytes + x ] = 255 - lpDIBBits[ y * lLineBytes + x ];
    }
  }
}

/*************************************************************************
 *
* \�������ƣ�
*   Roberts()
 *
* \�������:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes  - ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
*   Float* pdGrad - ָ���ݶ����ݵ�ָ�룬����ͼ����ݶ���Ϣ
 *
* \����ֵ:
*   ��
 *
* \˵��:
*   Roberts����
 *
 *************************************************************************
 */
void Roberts(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pdGrad)
{
  // ����ͼ���������
  int y;

  // ����ͼ��ĺ�����
  int x;

  //����������������ʾRoberts���ӵ�һ��ģ�����������ֵ
  int nUpLeft;
  int nDownRight;

  // ����������������ʾRoberts���ӵڶ���ģ�����������ֵ
  int nUpRight;
  int nDownLeft;

  // ����������������ʾRoberts���Ӽ���Ľ��
  int nValueOne;
  int nValueTwo;

  // ��ʱ����
  Float dGrad;

  // ��ʼ��
  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      *(pdGrad + y * lWidth + x) = 0;
    }
  }

  // ���濪ʼ����Roberts���ӽ��м��㣬Ϊ�˱�֤��������Ҫ��
  // ������λ��ͼ�����ݵ��ڲ������������ѭ����������
  // y<lHeight-1 ������y<lHeight����Ӧ��x����Ҳ��x<lWidth-1
  // ������x<lWidth

  for (y = 0; y < lHeight - 1; y++)
    for (x = 0; x < lWidth - 1; x++) {
      // Roberts���ӵ�һ��ģ����Ҫ������ֵ
      nUpLeft = *(lpDIBBits + y * lLineBytes + x);
      nDownRight = *(lpDIBBits + (y + 1) * lLineBytes + x + 1);
      nDownRight *= -1;

      //Roberts���ӵĵ�һ��ģ�������
      nValueOne = nUpLeft + nDownRight;

      // Roberts���ӵڶ���ģ����Ҫ������ֵ
      nUpRight = *(lpDIBBits + y * lLineBytes + x + 1);
      nDownLeft = *(lpDIBBits + (y + 1) * lLineBytes + x);
      nDownLeft *= -1;

      // Roberts���ӵĵڶ���ģ�������
      nValueTwo = nUpRight + nDownLeft;

      // ��������ƫ������ƽ����
      dGrad = nValueOne * nValueOne + nValueTwo * nValueTwo;

      // ����
      dGrad = pow(dGrad, 0.5);

      // ��������ŷʽ����
      *(pdGrad + y * lWidth + x) = dGrad;
    }
}
/*************************************************************************
 *
* \�������ƣ�
*   Laplacian1()
 *
* \�������:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes  - ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
*   Float* pdGrad - ָ���ݶ����ݵ�ָ�룬����ͼ����ݶ���Ϣ
 *
* \����ֵ:
*   ��
 *
* \˵��:
*   Laplacian����, �Ƕ������ӣ�����Roberts����������Ҫ����ģ�����
*   �ݶȣ�Laplacian����ֻҪһ�����ӾͿ��Լ����ݶȡ�������Ϊ������
*   ������Ϣ���������Ƚ�����
 *
 *************************************************************************
 */
void Laplacian1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pdGrad)
{
  int y; // ����ͼ���������
  int x; // ����ͼ��ĺ�����

  // ��ʱ����
  Float dGrad;

  // ģ��ѭ�����Ʊ���
  int yy;
  int xx;

  //�������������ʾLaplacian��������ֵ
  int nTmp[ 3 ][ 3 ];

  // ����ģ��ϵ��
  static int nWeight[ 3 ][ 3 ];

  // ��ʼ��
  for (y = 0; y < lHeight; y++)
    for (x = 0; x < lWidth; x++) {
      *(pdGrad + y * lWidth + x) = 0;
    }

  nWeight[ 0 ][ 0 ] = -1;
  nWeight[ 0 ][ 1 ] = -1;
  nWeight[ 0 ][ 2 ] = -1;
  nWeight[ 1 ][ 0 ] = -1;
  nWeight[ 1 ][ 1 ] = 8;
  nWeight[ 1 ][ 2 ] = -1;
  nWeight[ 2 ][ 0 ] = -1;
  nWeight[ 2 ][ 1 ] = -1;
  nWeight[ 2 ][ 2 ] = -1;


  // ���濪ʼ����Laplacian���ӽ��м��㣬Ϊ�˱�֤��������Ҫ��
  // ������λ��ͼ�����ݵ��ڲ������������ѭ����������
  // y<lHeight-2 ������y<lHeight����Ӧ��x����Ҳ��x<lWidth-2
  // ������x<lWidth
  for (y = 1; y < lHeight - 2; y++)
    for (x = 1; x < lWidth - 2; x++) {
      dGrad = 0;
      // Laplacian������Ҫ�ĸ�������ֵ

      // ģ���һ��
      nTmp[ 0 ][ 0 ] = lpDIBBits[(y - 1) * lLineBytes + x - 1 ];
      nTmp[ 0 ][ 1 ] = lpDIBBits[(y - 1) * lLineBytes + x ];
      nTmp[ 0 ][ 2 ] = lpDIBBits[(y - 1) * lLineBytes + x + 1 ];

      // ģ��ڶ���
      nTmp[ 1 ][ 0 ] = lpDIBBits[ y * lLineBytes + x - 1 ];
      nTmp[ 1 ][ 1 ] = lpDIBBits[ y * lLineBytes + x ];
      nTmp[ 1 ][ 2 ] = lpDIBBits[ y * lLineBytes + x + 1 ];

      // ģ�������
      nTmp[ 2 ][ 0 ] = lpDIBBits[(y + 1) * lLineBytes + x - 1 ];
      nTmp[ 2 ][ 1 ] = lpDIBBits[(y + 1) * lLineBytes + x ];
      nTmp[ 2 ][ 2 ] = lpDIBBits[(y + 1) * lLineBytes + x + 1 ];

      // �����ݶ�
      for (yy = 0; yy < 3; yy++)
        for (xx = 0; xx < 3; xx++) {
          dGrad += nTmp[ yy ][ xx ] * nWeight[ yy ][ xx ];
        }

      // �ݶ�ֵд���ڴ�
      *(pdGrad + y * lWidth + x) = dGrad;
    }

}

/*************************************************************************
 *
 * \�������ƣ�
 *   RegionGrow()
 *
 * \�������:
 *   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes     - ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
 *   LPBYTE pUnRegion  - ָ���������������ָ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   pUnRegionָ��ָ����������洢�����������Ľ��������1���߼�����ʾ
 *  ��Ӧ����Ϊ��������0��ʾΪ����������
 *   ��������һ����������Ƚ���Ҫ�����⣺
 *  1. ���ӵ��ѡȡ
 *  2. ����׼��
 *  3. ��ֹ����
 *  ������Ϊ��������������Ҫ�������������ÿ���������ĺû�ֱ�ӹ�ϵ��
 *  ���������Ľ����
 *  �����������ӵ�ѡȡΪͼ������ģ�����׼�����������ص�����ֵС��
 *  nThreshold, ��ֹ������һֱ���е���û����������׼����Ҫ������ʱΪֹ
 **************************************************************************/
void RegionGrow(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, LPBYTE pUnRegion, int nThreshold)
{
  static int nDx[] = { -1, 0, 1, 0};
  static int nDy[] = {0, 1, 0, -1};

  // �����ջ���洢����
  int* pnGrowQueX;
  int* pnGrowQueY;

  // ���ӵ�
  int nSeedX, nSeedY;

  // �����ջ�������յ�
  // ��nStart=nEnd, ��ʾ��ջ��ֻ��һ����
  int nStart;
  int nEnd;

  // ����ͼ���������
  // int y;

  // ����ͼ��ĺ�����
  // int x;

  // ��ǰ���ڴ��������
  int nCurrX;
  int nCurrY;

  // ѭ�����Ʊ���
  int k;

  // ͼ��ĺ�������, �����Ե�ǰ���ص�4������б���
  int xx;
  int yy;

  // ��ʼ��
  memset(pUnRegion, 0, sizeof(BYTE) * lWidth * lHeight);


  // �������ӵ�Ϊͼ�������
  nSeedX = lWidth / 2;
  nSeedY = lHeight / 2;
  // ����ռ�
  pnGrowQueX = MALLOC(int, lWidth * lHeight);
  pnGrowQueY = MALLOC(int, lWidth * lHeight);

  //��ʼ��
  nStart = 0;
  nEnd = 0;

  // �����ӵ������ѹ��ջ
  pnGrowQueX[ nEnd ] = nSeedX;
  pnGrowQueY[ nEnd ] = nSeedY;

  while (nStart <= nEnd) {
    // ��ǰ���ӵ������
    nCurrX = pnGrowQueX[ nStart ];
    nCurrY = pnGrowQueY[ nStart ];

    // �Ե�ǰ���4������б���
    for (k = 0; k < 4; k++) {
      // 4�������ص�����
      xx = nCurrX + nDx[ k ];
      yy = nCurrY + nDy[ k ];

      // �ж�����(xx��yy) �Ƿ���ͼ���ڲ�
      // �ж�����(xx��yy) �Ƿ��Ѿ������
      // pUnRegion[yy*lWidth+xx]==0 ��ʾ��û�д���

      // �����������ж�����(xx��yy)�͵�ǰ����(nCurrX, nCurrY) ����ֵ��ľ���ֵ
      if ((xx < lWidth) && (xx >= 0) && (yy < lHeight) && (yy >= 0)
          && (pUnRegion[ yy * lWidth + xx ] == 0)
          && abs(lpDIBBits[ yy * lLineBytes + xx ] - lpDIBBits[ nCurrY * lLineBytes + nCurrX ]) < nThreshold) {
        // ��ջ��β��ָ�����һλ
        nEnd++;

        // ����(xx��yy) ѹ��ջ
        pnGrowQueX[ nEnd ] = xx;
        pnGrowQueY[ nEnd ] = yy;

        // ������(xx��yy)���ó��߼�1��255��
        // ͬʱҲ���������ش����
        pUnRegion[ yy * lWidth + xx ] = 255;
      }
    }

    nStart++;
  }

  // �ͷ��ڴ�
  SAFEFREE(pnGrowQueX);
  SAFEFREE(pnGrowQueY);
}

void Dft2D(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pTrRstRpart, Float* pTrRstIpart)
{
  //����ͼ���������
  int y;

  //����ͼ��ĺ�����
  int x;

  //Ƶ��ĺ�����
  int m;

  //Ƶ���������
  int n;

  int nPxValue;
  Float fCosTable;
  Float fSinTable;
  Float fTmpRstR;
  Float fTmpRstI;

  //��ʼ���������
  for (n = 0; n < lHeight; n++) {
    for (m = 0; m < lWidth; m++) {
      *(pTrRstRpart + n * lWidth + m) = 0;
      *(pTrRstIpart + n * lWidth + m) = 0;
    }
  }

  fCosTable = 0;
  nPxValue = 0;

  for (n = 0; n < lHeight; n++)
    for (m = 0; m < lWidth; m++) {
      fTmpRstR = 0;
      fTmpRstI = 0;

      for (y = 0; y < lHeight; y++)
        for (x = 0; x < lWidth; x++) {
          fCosTable = cos(2 * PI * (((Float) m * x) / lWidth + ((Float) n * y) / lHeight));
          fSinTable = sin(-2 * PI * (((Float) m * x) / lWidth + ((Float) n * y) / lHeight));
          nPxValue = *(lpDIBBits + y * lLineBytes + x);

          fTmpRstR += fCosTable * nPxValue;
          fTmpRstI += fSinTable * nPxValue;
        }

      *(pTrRstRpart + lWidth * n + m) = fTmpRstR;
      *(pTrRstIpart + lWidth * n + m) = fTmpRstI;
    }
}

/*************************************************************************
 *
* \�������ƣ�
*   Sobel1()
 *
* \�������:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes    - ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
*   Float* pdGrad - ָ���ݶ����ݵ�ָ�룬����ͼ����ݶ���Ϣ
 *
* \����ֵ:
*   ��
 *
* \˵��:
*   Sobe����
 *
*   ���б߽�ָ�
 *
 *************************************************************************
 */
void Sobel1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pdGrad)
{
  // ����ͼ���������
  int y;

  // ����ͼ��ĺ�����
  int x;

  // ����ģ��ϵ��
  static int nWeight[ 2 ][ 3 ][ 3 ];

  //�������������ʾLaplacian��������ֵ
  int nTmp[ 3 ][ 3 ];

  // ��ʱ����
  Float dGrad;
  Float dGradOne;
  Float dGradTwo;

  // ģ��ѭ�����Ʊ���
  int yy;
  int xx;


  // ��ʼ��
  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      *(pdGrad + y * lWidth + x) = 0;
    }
  }

  nWeight[ 0 ][ 0 ][ 0 ] = -1;
  nWeight[ 0 ][ 0 ][ 1 ] = 0;
  nWeight[ 0 ][ 0 ][ 2 ] = 1;
  nWeight[ 0 ][ 1 ][ 0 ] = -2;
  nWeight[ 0 ][ 1 ][ 1 ] = 0;
  nWeight[ 0 ][ 1 ][ 2 ] = 2;
  nWeight[ 0 ][ 2 ][ 0 ] = -1;
  nWeight[ 0 ][ 2 ][ 1 ] = 0;
  nWeight[ 0 ][ 2 ][ 2 ] = 1;

  nWeight[ 1 ][ 0 ][ 0 ] = 1;
  nWeight[ 1 ][ 0 ][ 1 ] = 2;
  nWeight[ 1 ][ 0 ][ 2 ] = 1;
  nWeight[ 1 ][ 1 ][ 0 ] = 0;
  nWeight[ 1 ][ 1 ][ 1 ] = 0;
  nWeight[ 1 ][ 1 ][ 2 ] = 0;
  nWeight[ 1 ][ 2 ][ 0 ] = -1;
  nWeight[ 1 ][ 2 ][ 1 ] = -2;
  nWeight[ 1 ][ 2 ][ 2 ] = -1;

  // ���濪ʼ����Prewitt���ӽ��м��㣬Ϊ�˱�֤��������Ҫ��
  // ������λ��ͼ�����ݵ��ڲ������������ѭ����������
  // y<lHeight-1 ������y<lHeight����Ӧ��x����Ҳ��x<lWidth-1
  // ������x<lWidth
  for (y = 1; y < lHeight - 1; y++) {
    for (x = 1; x < lWidth - 1; x++) {
      dGrad = 0;
      dGradOne = 0;
      dGradTwo = 0;
      // Laplacian������Ҫ�ĸ�������ֵ

      // ģ���һ��
      nTmp[ 0 ][ 0 ] = lpDIBBits[(y - 1) * lLineBytes + x - 1 ];
      nTmp[ 0 ][ 1 ] = lpDIBBits[(y - 1) * lLineBytes + x ];
      nTmp[ 0 ][ 2 ] = lpDIBBits[(y - 1) * lLineBytes + x + 1 ];

      // ģ��ڶ���
      nTmp[ 1 ][ 0 ] = lpDIBBits[ y * lLineBytes + x - 1 ];
      nTmp[ 1 ][ 1 ] = lpDIBBits[ y * lLineBytes + x ];
      nTmp[ 1 ][ 2 ] = lpDIBBits[ y * lLineBytes + x + 1 ];

      // ģ�������
      nTmp[ 2 ][ 0 ] = lpDIBBits[(y + 1) * lLineBytes + x - 1 ];
      nTmp[ 2 ][ 1 ] = lpDIBBits[(y + 1) * lLineBytes + x ];
      nTmp[ 2 ][ 2 ] = lpDIBBits[(y + 1) * lLineBytes + x + 1 ];

      // �����ݶ�
      for (yy = 0; yy < 3; yy++)
        for (xx = 0; xx < 3; xx++) {
          dGradOne += nTmp[ yy ][ xx ] * nWeight[ 0 ][ yy ][ xx ];
          dGradTwo += nTmp[ yy ][ xx ] * nWeight[ 1 ][ yy ][ xx ];
        }

      dGrad = dGradOne * dGradOne + dGradTwo * dGradTwo;
      dGrad = sqrt(dGrad);
      // �ݶ�ֵд���ڴ�
      *(pdGrad + y * lWidth + x) = dGrad;
    }
  }
}

/*************************************************************************
 *
* \�������ƣ�
*   Prewitt1()
 *
* \�������:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes    - ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
*   Float* pdGrad - ָ���ݶ����ݵ�ָ�룬����ͼ����ݶ���Ϣ
 *
* \����ֵ:
*   ��
 *
* \˵��:
*   Prewitt����
 *
*   ���б߽�ָ�
 *
 *************************************************************************
 */
void Prewitt1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pdGrad)
{
  // ����ͼ���������
  int y;

  // ����ͼ��ĺ�����
  int x;

  // ����ģ��ϵ��
  static int nWeight[ 2 ][ 3 ][ 3 ];

  //�������������ʾLaplacian��������ֵ
  int nTmp[ 3 ][ 3 ];

  // ��ʱ����
  Float dGrad;
  Float dGradOne;
  Float dGradTwo;

  // ģ��ѭ�����Ʊ���
  int yy;
  int xx;


  // ��ʼ��
  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      *(pdGrad + y * lWidth + x) = 0;
    }
  }

  nWeight[ 0 ][ 0 ][ 0 ] = -1;
  nWeight[ 0 ][ 0 ][ 1 ] = 0;
  nWeight[ 0 ][ 0 ][ 2 ] = 1;
  nWeight[ 0 ][ 1 ][ 0 ] = -1;
  nWeight[ 0 ][ 1 ][ 1 ] = 0;
  nWeight[ 0 ][ 1 ][ 2 ] = 1;
  nWeight[ 0 ][ 2 ][ 0 ] = -1;
  nWeight[ 0 ][ 2 ][ 1 ] = 0;
  nWeight[ 0 ][ 2 ][ 2 ] = 1;

  nWeight[ 1 ][ 0 ][ 0 ] = 1;
  nWeight[ 1 ][ 0 ][ 1 ] = 1;
  nWeight[ 1 ][ 0 ][ 2 ] = 1;
  nWeight[ 1 ][ 1 ][ 0 ] = 0;
  nWeight[ 1 ][ 1 ][ 1 ] = 0;
  nWeight[ 1 ][ 1 ][ 2 ] = 0;
  nWeight[ 1 ][ 2 ][ 0 ] = -1;
  nWeight[ 1 ][ 2 ][ 1 ] = -1;
  nWeight[ 1 ][ 2 ][ 2 ] = -1;

  // ���濪ʼ����Prewitt���ӽ��м��㣬Ϊ�˱�֤��������Ҫ��
  // ������λ��ͼ�����ݵ��ڲ������������ѭ����������
  // y<lHeight-1 ������y<lHeight����Ӧ��x����Ҳ��x<lWidth-1
  // ������x<lWidth
  for (y = 1; y < lHeight - 1; y++)
    for (x = 1; x < lWidth - 1; x++) {
      dGrad = 0;
      dGradOne = 0;
      dGradTwo = 0;
      // Laplacian������Ҫ�ĸ�������ֵ

      // ģ���һ��
      nTmp[ 0 ][ 0 ] = lpDIBBits[(y - 1) * lLineBytes + x - 1 ];
      nTmp[ 0 ][ 1 ] = lpDIBBits[(y - 1) * lLineBytes + x ];
      nTmp[ 0 ][ 2 ] = lpDIBBits[(y - 1) * lLineBytes + x + 1 ];

      // ģ��ڶ���
      nTmp[ 1 ][ 0 ] = lpDIBBits[ y * lLineBytes + x - 1 ];
      nTmp[ 1 ][ 1 ] = lpDIBBits[ y * lLineBytes + x ];
      nTmp[ 1 ][ 2 ] = lpDIBBits[ y * lLineBytes + x + 1 ];

      // ģ�������
      nTmp[ 2 ][ 0 ] = lpDIBBits[(y + 1) * lLineBytes + x - 1 ];
      nTmp[ 2 ][ 1 ] = lpDIBBits[(y + 1) * lLineBytes + x ];
      nTmp[ 2 ][ 2 ] = lpDIBBits[(y + 1) * lLineBytes + x + 1 ];

      // �����ݶ�
      for (yy = 0; yy < 3; yy++)
        for (xx = 0; xx < 3; xx++) {
          dGradOne += nTmp[ yy ][ xx ] * nWeight[ 0 ][ yy ][ xx ];
          dGradTwo += nTmp[ yy ][ xx ] * nWeight[ 1 ][ yy ][ xx ];
        }

      dGrad = dGradOne * dGradOne + dGradTwo * dGradTwo;
      dGrad = sqrt(dGrad);
      // �ݶ�ֵд���ڴ�
      *(pdGrad + y * lWidth + x) = dGrad;
    }
}

/*************************************************************************
 *
* \�������ƣ�
*   EdgeTrack()
 *
* \�������:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes     - ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
*   LPBYTE pUnEdgeTrack - ָ��߽���ٽ����ָ��
 *
* \����ֵ:
*   ��
 *
* \˵��:
*   pUnEdgeTrackָ��ָ����������洢�˱߽���ٵĽ��������1���߼�����ʾ
*  ��Ӧ����Ϊ�߽�㣬0��ʾΪ�Ǳ߽��
 *
*   ���б߽�ָ�
 *
 *************************************************************************
 */
void EdgeTrack(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, LPBYTE pUnEdgeTrack)
{
  static int nDx[ 8 ] = { -1, -1, -1, 0, 0, 1, 1, 1};
  static int nDy[ 8 ] = { -1, 0, 1, -1, 1, -1, 0, 1};

  // ����ͼ���������
  int y;

  // ����ͼ��ĺ�����
  int x;

  // ָ���ݶ����ݵ�ָ��
  Float* pdGrad;

  // ���嵱ǰ�����ݶ�ֵ
  Float dCurrGrad = 0;

  // ��������ݶ�ֵ
  Float dMaxGrad;

  // ����ݶ�ֵ��Ӧ�����ص�����
  int nPx;
  int nPy;

  // ѭ��������������ǰ���ص�8����
  int i;

  int yy;
  int xx;

  int nDetX;
  int nDetY;

  // ����ͼ��Ĵ�С�����ڴ�ռ䣬�洢�ݶȼ���Ľ��
  pdGrad = MALLOC(Float, lHeight * lWidth);

  // ����Roberts�������ݶ�
  Roberts(lHeight, lWidth, lpDIBBits, lLineBytes, pdGrad);

  // ���ó�ֵ
  dMaxGrad = 0;

  nPx = 0;
  nPy = 0;

  // ���ݶ����ֵ���ڵ����ص�����
  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      dCurrGrad = pdGrad[ y * lWidth + x ];

      if (dMaxGrad < dCurrGrad) {
        dMaxGrad = dCurrGrad;
        nPx = x;
        nPy = y;
      }
    }
  }

  // ��ʼ��
  memset(pUnEdgeTrack, 0, sizeof(BYTE) * lWidth * lHeight);
  dCurrGrad = pdGrad[ nPy * lWidth + nPx ];

  // �ӣ�nPx��nPy���㿪ʼ���б߽����
  pUnEdgeTrack[ nPy * lWidth + nPx ] = 255;

  while (dCurrGrad > 10) {
    // ���õ�ǰ��Ϊ�߽��
    pUnEdgeTrack[ nPy * lWidth + nPx ] = 255;

    dMaxGrad = 0;

    for (i = 0; i < 8; i++) {
      nDetX = nDx[ i ];
      nDetY = nDy[ i ];
      y = nPy + nDetY;
      x = nPx + nDetX;

      // �ж��Ƿ���ͼ���ڲ�
      if (x >= 0 && x < lWidth && y >= 0 && y < lHeight) {
        if ((pdGrad[ y * lWidth + x ] > dMaxGrad) && (pUnEdgeTrack[ y * lWidth + x ] == 0)) {
          dMaxGrad = pdGrad[ y * lWidth + x ];
          yy = y;
          xx = x;
        }
      }
    }

    // ��һ���߽����ݶȣ���������
    dCurrGrad = dMaxGrad;
    nPy = yy;
    nPx = xx;
  }

  //�ͷ��ڴ�
  SAFEFREE(pdGrad);

}

/*************************************************************************
* \�������ƣ�
*   GradMagnitude()
 *
* \�������:
*   int *pnGradX                         - x����ķ�����
*   int *pnGradY                         - y����ķ�����
*   int nWidht              - ͼ����
*   int lHeight                - ͼ��߶�
*   int *pnMag                           - �ݶȷ���
 *
* \����ֵ:
*   ��
 *
* \˵��:
*   ����������÷����������ݶȷ��ȣ���������DirGrad��������Ľ��
*
 **************************************************************************/
void GradMagnitude(int* pnGradX, int* pnGradY, int lWidth, int lHeight, int* pnMag)
{

  // ѭ�����Ʊ���
  int y;
  int x;

  // �м����
  Float dSqtOne;
  Float dSqtTwo;

  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      dSqtOne = pnGradX[ y * lWidth + x ] * pnGradX[ y * lWidth + x ];
      dSqtTwo = pnGradY[ y * lWidth + x ] * pnGradY[ y * lWidth + x ];
      pnMag[ y * lWidth + x ] = (int)(sqrt(dSqtOne + dSqtTwo) + 0.5);
    }
  }
}

/*************************************************************************
* �������ƣ�
*   RegionDivideThreshold()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
*   int threshold        - �ָ�����
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶ�ͼ����й̶���ֵ�ָ
 ************************************************************************/
BOOL RegionDivideThreshold(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int threshold)
{
  int i;  //ѭ������
  int j;    //ѭ������
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;
  //����ֵ
  BYTE pixeltemp;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {

      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //ȡ�õ�ǰָ�봦������ֵ
      pixeltemp = (BYTE) * lpSrc;

      //Ŀ��ͼ���лҶ�ֵС������������Ϊ�ڵ�
      if (pixeltemp <= threshold) {
        *lpSrc = (BYTE) 0;
      }
      else {    //��������Ϊ�׵�
        *lpSrc = (BYTE) 255;
      }
    }
  }

  return TRUE;
}

/*************************************************************************
* \�������ƣ�
*   RegionDivideAdaptive()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶ�ͼ���������Ӧ��ֵ�ָ��ͼ��ֳ��ĸ���ͼ��Ȼ�����ÿ����ͼ��ľ�ֵ��
*  ���ݾ�ֵ������ֵ����ֵֻ��Ӧ���ڶ�Ӧ����ͼ��
 *************************************************************************
 */
BOOL RegionDivideAdaptive(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  int i;  //ѭ������
  int j;    //ѭ������
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;
  //����ֵ
  BYTE pixeltemp;
  // ��ͼ��Ҷȵ�ƽ��ֵ
  int nAvg;
  nAvg = 0; //��ʼ��Ϊ0

  // ������ͼ�����ɨ�裺
  for (j = 0; j < lHeight / 2; j++) {
    for (i = 0; i < lWidth / 2; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //ȡ�õ�ǰָ�봦������ֵ
      pixeltemp = (BYTE) * lpSrc;
      //�Ҷȵ��ۼ�
      nAvg = (int) pixeltemp + nAvg;
    }
  }

  // �����ֵ
  nAvg = nAvg / ((lHeight / 2) * (lWidth / 2));

  // ������ͼ�����ɨ�裺
  for (j = 0; j < lHeight / 2; j++) {
    for (i = 0; i < lWidth / 2; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //ȡ�õ�ǰָ�봦������ֵ
      pixeltemp = (BYTE) * lpSrc;

      //Ŀ��ͼ���лҶ�ֵС������������Ϊ�ڵ�
      if (pixeltemp <= nAvg) {
        *lpSrc = (BYTE) 0;
      }
      else {    //��������Ϊ�׵�
        *lpSrc = (BYTE) 255;
      }
    }
  }

  nAvg = 0;   //��ʼ��Ϊ0

  // ������ͼ�����ɨ�裺
  for (j = lHeight / 2; j < lHeight; j++) {
    for (i = 0; i < lWidth / 2; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //ȡ�õ�ǰָ�봦������ֵ
      pixeltemp = (BYTE) * lpSrc;
      //�Ҷȵ��ۼ�
      nAvg = (int) pixeltemp + nAvg;
    }
  }

  // �����ֵ
  nAvg = nAvg / ((lHeight / 2) * (lWidth / 2));

  // ������ͼ�����ɨ�裺
  for (j = lHeight / 2; j < lHeight; j++) {
    for (i = 0; i < lWidth / 2; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //ȡ�õ�ǰָ�봦������ֵ
      pixeltemp = (BYTE) * lpSrc;

      //Ŀ��ͼ���лҶ�ֵС������������Ϊ�ڵ�
      if (pixeltemp <= nAvg) {
        *lpSrc = (BYTE) 0;
      }
      else {    //��������Ϊ�׵�
        *lpSrc = (BYTE) 255;
      }
    }
  }

  nAvg = 0;   //��ʼ��Ϊ0

  // ������ͼ�����ɨ�裺
  for (j = lHeight / 2; j < lHeight; j++) {
    for (i = lWidth / 2; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //ȡ�õ�ǰָ�봦������ֵ
      pixeltemp = (BYTE) * lpSrc;
      //�Ҷȵ��ۼ�
      nAvg = (int) pixeltemp + nAvg;
    }
  }

  // �����ֵ
  nAvg = nAvg / ((lHeight / 2) * (lWidth / 2));

  // ������ͼ�����ɨ�裺
  for (j = lHeight / 2; j < lHeight; j++) {
    for (i = lWidth / 2; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //ȡ�õ�ǰָ�봦������ֵ
      pixeltemp = (BYTE) * lpSrc;

      //Ŀ��ͼ���лҶ�ֵС������������Ϊ�ڵ�
      if (pixeltemp <= nAvg) {
        *lpSrc = (BYTE) 0;
      }
      else {    //��������Ϊ�׵�
        *lpSrc = (BYTE) 255;
      }
    }
  }

  nAvg = 0;   //��ʼ��Ϊ0

  // ������ͼ�����ɨ�裺
  for (j = 0; j < lHeight / 2; j++) {
    for (i = lWidth / 2; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //ȡ�õ�ǰָ�봦������ֵ
      pixeltemp = (BYTE) * lpSrc;
      //�Ҷȵ��ۼ�
      nAvg = (int) pixeltemp + nAvg;
    }
  }

  // �����ֵ
  nAvg = nAvg / ((lHeight / 2) * (lWidth / 2));

  // ������ͼ�����ɨ�裺
  for (j = 0; j < lHeight / 2; j++) {
    for (i = lWidth / 2; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //ȡ�õ�ǰָ�봦������ֵ
      pixeltemp = (BYTE) * lpSrc;

      //Ŀ��ͼ���лҶ�ֵС������������Ϊ�ڵ�
      if (pixeltemp <= nAvg) {
        *lpSrc = (BYTE) 0;
      }
      else {    //��������Ϊ�׵�
        *lpSrc = (BYTE) 255;
      }
    }
  }

  return TRUE;
}


/*************************************************************************
* \�������ƣ�
*   RegionGrow()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
*     int nThreshold            - ������������
*     int seedx                - �����������ӵĺ�����
*     int seedy                - �����������ӵ�������
 *     int mode                - �����������ӵ�ѡȡ����
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
* \˵��:
*   �������Ĵ������ǽ�������������ʾΪ��ɫ���أ�δ���������ʾΪ��ɫ���ء�
*   ��������һ����������Ƚ���Ҫ�����⣺
*        1. ���ӵ��ѡȡ
*        2. ����׼��
*        3. ��ֹ����
*     ������Ϊ��������������Ҫ�������������ÿ���������ĺû�ֱ�ӹ�ϵ��
*     ���������Ľ����
 *************************************************************************/
BOOL RegionGrow1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nThreshold, int seedx, int seedy, int mode)
{
  //�������ڴ��������������
  static int Dx[] = { -1, 0, 1, 0};    //X��
  static int Dy[] = {0, 1, 0, -1};    //y��
  // ��ʱ���ͼ�����ݵ�ָ��
  LPBYTE lpTmpBits;
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;
  LPBYTE lpSrc1;
  // ָ��Ҫ���������ָ��
  LPBYTE lpDst;

  // ���ӵ�
  IPOINT seed;

  // �����ջ���ֱ����X Y����
  int* GrowX;
  int* GrowY;

  // �����ջ�������յ�
  // ��Start=End, ��ʾ��ջ��ֻ��һ����
  int Start;
  int End;

  // ��ǰ���ڴ��������
  IPOINT Current;
  // ѭ�����Ʊ���
  int k;
  // ͼ��ĺ�������, �����Ե�ǰ���ص�4������б���
  int xx;
  int yy;

  // �����ڴ�
  lpTmpBits = MALLOC(BYTE, lWidth * lHeight);

  // �ж��Ƿ��ڴ����ʧ��
  if (lpTmpBits == NULL) {
    // �����ڴ�ʧ��
    return FALSE;
  }

  // ��Ŀ��ͼ���������س�ʼ��Ϊ�׵�
  lpDst = (LPBYTE) lpDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  if (mode == 0) {
    // ���ѡ�����ӵ�Ϊͼ�������
    // �������ӵ�Ϊͼ�������
    seed.x = lWidth / 2;
    seed.y = lHeight / 2;
  }
  else {   //�Լ��������ӵ�
    // �������ӵ�Ϊ�����
    seed.x = seedx;
    seed.y = seedy;
  }

  // ����ռ䣬���Ϊͼ���������Ŀ
  GrowX = MALLOC(int, lWidth * lHeight);
  GrowY = MALLOC(int, lWidth * lHeight);

  //��ʼ��
  Start = 0;
  End = 0;
  // �����ӵ������ѹ��ջ
  GrowX[ End ] = seed.x;
  GrowY[ End ] = seed.y;

  while (Start <= End) {
    // ��ǰ���ӵ������
    Current.x = GrowX[ Start ];
    Current.y = GrowY[ Start ];

    // �Ե�ǰ���4������б���
    for (k = 0; k < 4; k++) {
      // 4�������ص�����
      xx = Current.x + Dx[ k ];
      yy = Current.y + Dy[ k ];

      // �ж�����(xx��yy) �Ƿ���ͼ���ڲ�
      // �ж�����(xx��yy) �Ƿ��Ѿ������
      // *lpDst==255 ��ʾ��û�д���
      // ָ��Դͼ��(xx��yy)���ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - yy) + xx;
      lpSrc1 = (LPBYTE) lpDIBBits + lWidth * (lHeight - Current.y) + Current.x;
      // ָ��Ŀ��ͼ���j�У���i�����ص�ָ��
      lpDst = (LPBYTE) lpTmpBits + lWidth * (lHeight - yy) + xx;

      // �����������ж�����(xx��yy)�͵�ǰ����(nCurrX, nCurrY) ����ֵ��ľ���ֵ
      if ((xx < lWidth) && (xx >= 0) && (yy < lHeight) && (yy >= 0) && (*lpDst == 255)
          && abs(*lpSrc - *lpSrc1) < nThreshold) {
        // ��ջ��β��ָ�����һλ
        End++;
        // ����(xx��yy) ѹ��ջ
        GrowX[ End ] = xx;
        GrowY[ End ] = yy;
        // Ŀ��ͼ��Ϊ�ڵ㣬ͬʱҲ���������ش����
        *lpDst = 0;
      }
    }

    Start++;
  }

  // ���������������ͼ��
  memcpy(lpDIBBits, lpTmpBits, lWidth * lHeight);
  // �ͷ��ڴ�
  SAFEFREE(lpDIBBits);
  SAFEFREE(GrowX);
  SAFEFREE(GrowY);
  // ����
  return TRUE;
}
