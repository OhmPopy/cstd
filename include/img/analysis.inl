
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

/*************************************************************************
 *
 * �������ƣ�
 *   HoleNumber()
 *
 * ����:
 *   CDib* pDib        - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�False��
 *
 * ˵��:
 *   �ú�������ȥͼ�������С����ֵ��С����
 *
 *************************************************************************/
BOOL HoleNumber(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  // ѭ������
  int i, j, s, n;

  // ��Ѩ����Ŀ�Լ������ֵ
  int nHoleNum, nMinArea;

  int nBlackPix, temp;

  // ����ͷ��촫����־
  int nDir1, nDir2;

  // �����洢��һλ����
  int* pnBinary;

  pnBinary = MALLOC(int, lHeight * lLineBytes);

  // С�������ֵ���Ϊ20�����ص�
  nMinArea = 50;

  // ��ͼ���ֵ��
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // ��ɫ����Ϊ���������0
      if (*lpSrc > 200) {
        pnBinary[ lLineBytes * j + i ] = 0;
      }
      // �����ش��-1
      else {
        pnBinary[ lLineBytes * j + i ] = -1;
      }
    }
  }

  // ��Ѩ������ֵ
  nHoleNum = 1;

  do {
    s = 0;

    // Ѱ��ÿ����Ѩ�ĳ�ʼ����ֵ
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 1; i < lWidth - 1; i++) {
        // �ҵ���ʼ����
        if (pnBinary[ lLineBytes * j + i ] == -1) {
          s = 1;
          // ������ֵ�ĳɵ�ǰ�Ŀ�Ѩ��ֵ
          pnBinary[ lLineBytes * j + i ] = nHoleNum;

          // ����ѭ��
          j = lHeight;
          i = lLineBytes;
        }
      }
    }

    //û�г�ʼ���أ�����ѭ��
    if (s == 0) {
      break;
    }
    else {
      do {
        // ����ͷ��촫��ϵ������ֵ0
        nDir1 = 0;
        nDir2 = 0;

        // ����ɨ��
        for (j = 1; j < lHeight - 1; j++) {
          for (i = 1; i < lWidth - 1; i++) {
            nBlackPix = pnBinary[ lLineBytes * j + i ];

            // ��������Ѿ���ɨ�裬�����Ǳ���ɫ��������һ��ѭ��
            if (nBlackPix != -1) {
              continue;
            }

            // ����ϲ������������ֵ�Ѿ���ɨ�裬�����ڵ�ǰ�Ŀ�Ѩ����ǰ������ֵ
            // �ĳɿ�Ѩ����ֵ
            nBlackPix = pnBinary[ lLineBytes * (j - 1) + i ];

            if (nBlackPix == nHoleNum) {
              pnBinary[ lLineBytes * j + i ] = nHoleNum;
              nDir1 = 1;

              continue;
            }

            nBlackPix = pnBinary[ lLineBytes * j + i - 1 ];

            if (nBlackPix == nHoleNum) {
              pnBinary[ lLineBytes * j + i ] = nHoleNum;
              nDir1 = 1;
            }

          }
        }

        // ��������ȫ����ɨ�裬����ѭ��
        if (nDir1 == 0) {
          break;
        }

        // ����ɨ��
        for (j = lHeight - 2; j >= 1; j--) {
          for (i = lWidth - 2; i >= 1; i--) {
            nBlackPix = pnBinary[ lLineBytes * j + i ];

            // ��������Ѿ���ɨ�裬�����Ǳ���ɫ��������һ��ѭ��
            if (nBlackPix != -1) {
              continue;
            }

            // ����²�����Ҳ������ֵ�Ѿ���ɨ�裬�����ڵ�ǰ�Ŀ�Ѩ����ǰ������ֵ
            // �ĳɿ�Ѩ����ֵ
            nBlackPix = pnBinary[ lLineBytes * (j + 1) + i ];

            if (nBlackPix == nHoleNum) {
              pnBinary[ lLineBytes * j + i ] = nHoleNum;
              nDir2 = 1;

              continue;
            }

            nBlackPix = pnBinary[ lLineBytes * j + i + 1 ];

            if (nBlackPix == nHoleNum) {
              pnBinary[ lLineBytes * j + i ] = nHoleNum;
              nDir2 = 1;
            }
          }
        }

        if (nDir2 == 0) {
          break;
        }
      }
      while (1);
    }

    // ��Ѩ������
    nHoleNum++;
  }
  while (1);

  nHoleNum --;

  // Ѱ�����С����ֵ�Ŀ�Ѩ����
  for (n = 1; n <= nHoleNum; n++) {
    s = 0;

    for (j = 0; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth - 1; i++) {
        nBlackPix = pnBinary[ lLineBytes * j + i ];

        if (nBlackPix == n) {
          s++;
        }

        // �����������Ѿ�������ֵ������ѭ��
        if (s > nMinArea) {
          break;
        }
      }

      if (s > nMinArea) {
        break;
      }
    }

    // С����ֵ�����򣬸����뱳��һ������ɫ��������ȥ
    if (s <= nMinArea) {
      for (j = 0; j < lHeight - 1; j++) {
        for (i = 0; i < lWidth - 1; i++) {
          nBlackPix = pnBinary[ lLineBytes * j + i + 1 ];

          if (nBlackPix == n) {
            pnBinary[ lLineBytes * j + i + 1 ] = 0;
          }

        }
      }

    }

  }

  // �洢����ֵ�����
  for (j = 0; j < lHeight; j++) {
    // ��
    for (i = 0; i < lWidth; i++) {
      // ��ֵͼ��
      temp = pnBinary[ j * lLineBytes + i ];

      // ָ��λͼi��j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // ����Դͼ��
      if (temp != 0) {
        * (lpSrc) = 0;
      }
      else {
        * (lpSrc) = 255;
      }
    }
  }

  SAFEFREE(pnBinary);

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   Moment()
 *
* ����:
*   CDib* pDib        - ָ��CDib���ָ��
 *
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�False��
 *
* ˵��:
*   �ú�������ͼ������ء�
 *************************************************************************/
int Moment(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  // ͼ��ľ�
  int nImageMoment;

  // ѭ������
  int i, j;

  int ip, jq;

  // ��ʱ����
  Float temp;

  nImageMoment = 0;

  // ����һ�׾�
  ip = jq = 1;

  // ���صļ���
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      temp = pow((Float) i, ip) * pow((Float)(j), jq);
      temp = temp * (*lpSrc);

      nImageMoment = nImageMoment + (int) temp;
    }
  }

  return nImageMoment;
}

/*************************************************************************
 *
* �������ƣ�
*   BaryCenterMoment()
 *
* ����:
*   CDib* pDib        - ָ��CDib���ָ��
 *
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�False��
 *
* ˵��:
*   �ú�������ͼ������ľء�
 *
 *************************************************************************/

BOOL BaryCenterMoment(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int* pBarycenterX, int* pBarycenterY, int* pBarycenterMoment)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  // ͼ������ֵ
  int nPixelValue;

  // ͼ�����ľ�
  int nBarycenterMoment;

  // 0�ξ�m00, x�����һ�ξ�m01��y�����һ�ξ�m10
  int m00, m10, m01;

  // ����x��y����ֵ
  int nBarycenterX, nBarycenterY;

  // ѭ������
  int i, j;

  // ��ʱ����
  Float temp;

  // ����ֵΪ��
  m00 = 0;
  m01 = 0;
  m10 = 0;
  nBarycenterMoment = 0;

  // ��0�ξ�m00, x�����һ�ξ�m01��y�����һ�ξ�m10
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;
      nPixelValue = *lpSrc;

      m00 = m00 + nPixelValue;

      temp = i * nPixelValue;
      m01 = (int)(m01 + temp);

      temp = j * nPixelValue;
      m10 = (int)(m10 + temp);
    }
  }

  // ����x��y����ֵ
  nBarycenterX = (int)(m01 / m00 + 0.5);
  nBarycenterY = (int)(m10 / m00 + 0.5);

  // �������ľ�
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;
      nPixelValue = *lpSrc;

      temp = (i - nBarycenterX) * (j - nBarycenterY);

      temp = temp * nPixelValue;

      nBarycenterMoment = nBarycenterMoment + (int) temp;
    }
  }

  *pBarycenterX = nBarycenterX;
  *pBarycenterY = nBarycenterY;
  *pBarycenterMoment = nBarycenterMoment;

  return TRUE;

}

/*************************************************************************
 *
* �������ƣ�
*   StreetDis()
 *
* ����:
*   CDib* pDib        - ָ��CDib���ָ��
 *
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�False��
 *
* ˵��:
*   �ú����������̾����ͼ����б任��
 *
 *************************************************************************/
BOOL StreetDis(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  // ѭ������ʱ����
  int i, j;
  int temp, s;

  // �洢����ֵ������
  int* pnBinary, *pnStore;

  int nImageValue;

  pnBinary = MALLOC(int, lHeight * lLineBytes);
  pnStore = MALLOC(int, lHeight * lLineBytes);

  // ��ͼ���ֵ��
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // ��ɫ����Ϊ���������0
      if (*lpSrc > 200) {
        pnBinary[ lLineBytes * j + i ] = 0;
        pnStore [ lLineBytes * j + i ] = 0;
      }
      // �����ش��1
      else {
        pnBinary[ lLineBytes * j + i ] = 1;
        pnStore [ lLineBytes * j + i ] = 1;
      }
    }
  }

  s = 1;

  while (s == 1) {
    s = 0;

    // ���о�����ۼ�
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 1; i < lWidth - 1; i++) {
        nImageValue = pnBinary[ lLineBytes * j + i ];

        // ����Ǳ�����������һ��ѭ��
        if (nImageValue == 0) {
          continue;
        }

        // �����ǰ����ֵ�����ܵ�ֵһ��������ֵ����һ
        if (nImageValue == pnBinary[ lLineBytes * (j - 1) + i ] && nImageValue == pnBinary[ lLineBytes * (j + 1) + i ])
          if (nImageValue == pnBinary[ lLineBytes * j + i - 1 ] && nImageValue == pnBinary[ lLineBytes * j + i + 1 ]) {
            pnStore[ lLineBytes * j + i ] ++;
            s = 1;
          }
      }
    }

    // �ڽ�����һ��ѭ��ǰ����ǰ�Ľ������
    for (j = 0; j < lHeight; j++)
      for (i = 1; i < lWidth; i++) {
        pnBinary[ lLineBytes * j + i ] = pnStore[ lLineBytes * j + i ];
      }
  }

  // �洢����ֵ�����
  for (j = 0; j < lHeight; j++) {
    // ��
    for (i = 0; i < lWidth; i++) {
      // �Ǽܵ�����ֵ
      temp = pnStore[ j * lLineBytes + i ];

      // ָ��λͼi��j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // ����Դͼ��
      * (lpSrc) = (BYTE)((25 - temp) * 10 + 5);
    }
  }

  SAFEFREE(pnStore);
  SAFEFREE(pnBinary);

  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   FreameWork()
 *
* ����:
*   CDib* pDib        - ָ��CDib���ָ��
 *
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�False��
 *
* ˵��:
*   �ú����������̾��룬����ͼ��ĹǼܡ�
 *
 *************************************************************************/

BOOL FreameWork(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  // ѭ������ʱ����
  int i, j;
  int temp, s;

  // �洢����ֵ������
  int* pnBinary, *pnStore;

  int nImageValue;

  pnBinary = MALLOC(int, lHeight * lLineBytes);
  pnStore = MALLOC(int, lHeight * lLineBytes);

  // ��ͼ���ֵ��
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // ��ɫ����Ϊ���������0
      if (*lpSrc > 200) {
        pnBinary[ lLineBytes * j + i ] = 0;
        pnStore [ lLineBytes * j + i ] = 0;
      }
      // �����ش��1
      else {
        pnBinary[ lLineBytes * j + i ] = 1;
        pnStore [ lLineBytes * j + i ] = 1;
      }
    }
  }

  s = 1;

  while (s == 1) {
    s = 0;

    // ���о�����ۼ�
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 1; i < lWidth - 1; i++) {
        nImageValue = pnBinary[ lLineBytes * j + i ];

        // ����Ǳ�����������һ��ѭ��
        if (nImageValue == 0) {
          continue;
        }

        // �����ǰ����ֵ�����ܵ�ֵһ��������ֵ����һ
        if (nImageValue == pnBinary[ lLineBytes * (j - 1) + i ] && nImageValue == pnBinary[ lLineBytes * (j + 1) + i ])
          if (nImageValue == pnBinary[ lLineBytes * j + i - 1 ] && nImageValue == pnBinary[ lLineBytes * j + i + 1 ]) {
            pnStore[ lLineBytes * j + i ] ++;
            s = 1;
          }
      }
    }

    // �ڽ�����һ��ѭ��ǰ����ǰ�Ľ������
    for (j = 0; j < lHeight; j++)
      for (i = 1; i < lWidth; i++) {
        pnBinary[ lLineBytes * j + i ] = pnStore[ lLineBytes * j + i ];
      }
  }

  for (j = 0; j < lHeight; j++)
    for (i = 0; i < lWidth; i++) {
      pnStore[ lLineBytes * j + i ] = 0;
    }

  // �����ǰ������ֵ����Χ�Ķ�Ҫ�ߣ���Ϊ�Ǽ�
  for (j = 1; j < lHeight - 1; j++)
    for (i = 1; i < lWidth - 1; i++) {
      nImageValue = pnBinary[ lLineBytes * j + i ] + 1;

      // ������ͨ����бȽ�
      if (nImageValue != pnBinary[ lLineBytes * (j - 1) + i ] && nImageValue != pnBinary[ lLineBytes * (j + 1) + i ])
        if (nImageValue != pnBinary[ lLineBytes * j + i - 1 ] && nImageValue != pnBinary[ lLineBytes * j + i + 1 ]) {
          pnStore[ lLineBytes * j + i ] = pnBinary[ lLineBytes * j + i ];
        }
    }


  // �洢����ֵ�����
  for (j = 0; j < lHeight; j++) {
    // ��
    for (i = 0; i < lWidth; i++) {
      // �Ǽܵ�����ֵ
      temp = pnStore[ j * lLineBytes + i ];

      // ָ��λͼi��j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // ����Դͼ�񣬲�������ֵ���б任���Ա����
      if (temp != 0) {
        * (lpSrc) = temp;
      }
      else {
        * (lpSrc) = 255;
      }
    }
  }

  SAFEFREE(pnStore);
  SAFEFREE(pnBinary);

  return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   ChessBoarddisrestore()
 *
 * ����:
 *   CDib* pDib        - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�False��
 *
 * ˵��:
 *   �ú������ùǼܶ�ԭͼ����лָ���
 *
 *************************************************************************/
BOOL ChessBoarddisreStore(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  // ѭ������
  int i, j, n, m;

  // ��ʱ����
  int temp, s;

  // �����������ֵ������
  int* pnBinary, *pnStore;

  int nImageValue;

  pnBinary = MALLOC(int, lHeight * lLineBytes);
  pnStore = MALLOC(int, lHeight * lLineBytes);

  // ���鸳ֵ
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pnBinary[ lLineBytes * j + i ] = 0;

      if ((*lpSrc) != 255) {
        pnStore [ lLineBytes * j + i ] = (*lpSrc);
      }
      else {
        pnStore [ lLineBytes * j + i ] = 0;
      }
    }
  }


  // ����ͼ��Ļָ�
  for (j = 1; j < lHeight - 1; j++) {
    for (i = 1; i < lWidth - 1; i++) {
      nImageValue = pnStore[ lLineBytes * j + i ];

      if (nImageValue == 0) {
        continue;
      }

      if (nImageValue == 1) {
        pnBinary[ lLineBytes * j + i ] = 1;
        continue;
      }

      // �������ֵ���ڵ���2�����ԣ�2��nImageValue����1�����η�Χ������ֵ��1
      s = nImageValue;

      // ���ε�����
      for (m = -s; m <= s; m++) {
        pnBinary[ lLineBytes * j + i + m ] = 1;
      }

      // ���ε��ϰ벿��
      for (n = -s; n < 0; n++)
        for (m = -s - n; m <= s + n; m++) {
          pnBinary[ lLineBytes * (j + n) + i + m ] = 1;
        }

      // ���ε��°벿��
      for (n = 1; n <= s; n++)
        for (m = -s + n; m <= s - n; m++) {
          pnBinary[ lLineBytes * (j + n) + i + m ] = 1;
        }

    }
  }

  // �洢����ֵ�����
  for (j = 0; j < lHeight; j++) {
    // ��
    for (i = 0; i < lWidth; i++) {
      // �Ǽܵ�����ֵ
      temp = pnBinary[ j * lLineBytes + i ];

      // ָ��λͼi��j�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // ����Դͼ��
      if (temp != 0)
        // ��ɫ�������
      {
        * (lpSrc) = 0;
      }
      else
        // ��ɫ�������
      {
        * (lpSrc) = 255;
      }
    }
  }

  SAFEFREE(pnStore);
  SAFEFREE(pnBinary);

  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   OutLine()
 *
* ����:
*   CDib* pDib        - ָ��CDib���ָ��
 *
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�False��
 *
* ˵��:
*   �ú����Զ�ֵͼ��������������
 *
 **************************************************************************/
BOOL OutLine(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{

  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  // ѭ������
  int i, j;

  int nPixelValue;

  int n1, n2, n3, n4, n5, n6, n7, n8;

  // �����������ֵ������
  int* pnBinary;

  pnBinary = MALLOC(int, lHeight * lLineBytes);

  // ��ͼ���ֵ��
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // ��ɫ����Ϊ���������0
      if (*lpSrc > 200) {
        pnBinary[ lLineBytes * j + i ] = 0;
      }
      // �����ش��1
      else {
        pnBinary[ lLineBytes * j + i ] = 1;
      }
    }
  }

  for (j = 1; j < lHeight - 1; j++) {
    for (i = 1; i < lWidth - 1; i++) {
      nPixelValue = pnBinary[ lLineBytes * j + i ];
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // �����ǰ�����ǰ�ɫ��������һ��ѭ��
      if (nPixelValue == 0) {
        // ����Ӧ������ֵ�ĳ���

        *lpSrc = 255;
        continue;
      }

      // �����ǰ�����Ǻ�ɫ
      else {
        // ����ܱߵ�8��ͨ��
        n1 = pnBinary[ lLineBytes * (j + 1) + i + 1 ];
        n2 = pnBinary[ lLineBytes * (j + 1) + i - 1 ];
        n3 = pnBinary[ lLineBytes * (j + 1) + i ];
        n4 = pnBinary[ lLineBytes * (j - 1) + i + 1 ];
        n5 = pnBinary[ lLineBytes * (j - 1) + i - 1 ];
        n6 = pnBinary[ lLineBytes * (j - 1) + i ];
        n7 = pnBinary[ lLineBytes * j + i + 1 ];
        n8 = pnBinary[ lLineBytes * j + i - 1 ];

        //������ڵİ˸��㶼�Ǻڵ�
        if (n1 && n2 && n3 && n4 && n5 && n6 && n7 && n8 == 1) {
          *lpSrc = (BYTE) 255;
        }
      }
    }
  }

  SAFEFREE(pnBinary);

  // ����
  return TRUE;
}
