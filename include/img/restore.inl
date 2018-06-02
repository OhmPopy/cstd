// ************************************************************************
//  �ļ�����restore.cpp
//
//  ͼ��ԭAPI�����⣺
//
// BlurDIB()               - ͼ��ģ��
// InverseDIB()            - ͼ�����˲�
// NoiseBlurDIB()          - ͼ��ģ������
// WienerDIB()             - ͼ��ά���˲�
// RandomNoiseDIB()        - ͼ���м����������
// SaltNoiseDIB()          - ͼ���м��뽷������
// Fourn()                 - nάFFT
// NoRestriction()      - ͼ��ģ��
// InverseFilter()      - ͼ�����˲���ԭ
// NoiseDegeneration()  - ͼ��ģ������
// WinnerFilter()       - ͼ��ά���˲�
// MotionDegeneration() - ͼ���˶�ģ��
// MotionRestore()      - ͼ���˶�ģ����ԭ
//
// *************************************************************************

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

#define SWAP(a, b) tempr=(a);(a)=(b);(b)=tempr

/*************************************************************************
* �������ƣ�
*   BlurDIB()
 *
* ����:
*   LPBYTE lpDIBBits   - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������
*   int lHeight      - Դͼ��߶ȣ���������
*   int lLineBytes   - ͼ��ÿ�е��ֽ���
 *
* ����ֵ:
*   BOOL               - ƽ�Ƴɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
*   �ú���������DIBͼ�����ģ��������
 ************************************************************************/
BOOL BlurDIB(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  //ѭ������
  int i;
  int j;

  //����ֵ
  BYTE pixel;

  //������FFT������
  Float* fftSrc, *fftKernel;
  //��άFFT�ĳ��ȺͿ��
  unsigned int nn[ 3 ];
  //ͼ���һ������
  Float MaxNum;

  Float dPower = log((Float) lLineBytes) / log(2.0);

  if (dPower != (int) dPower) {
    return FALSE;
  }

  dPower = log((Float) lHeight) / log(2.0);

  if (dPower != (int) dPower) {
    return FALSE;
  }

  fftSrc = MALLOC(Float, lHeight * lLineBytes * 2 + 1);
  fftKernel = MALLOC(Float, lHeight * lLineBytes * 2 + 1);

  nn[ 1 ] = lHeight;
  nn[ 2 ] = lLineBytes;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] = (Float) pixel;
      fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;

      if (i < 5 && j < 5) {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 1 / 25.0;
      }
      else {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 0.0;
      }

      fftKernel[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;
    }
  }

  //��Դͼ�����FFT
  Fourn(fftSrc, nn, 2, 1);
  //�Ծ����ͼ�����FFT
  Fourn(fftKernel, nn, 2, 1);

  //Ƶ�����
  for (i = 1; i < lHeight * lLineBytes * 2; i += 2) {
    fftSrc[ i ] = fftSrc[ i ] * fftKernel[ i ] - fftSrc[ i + 1 ] * fftKernel[ i + 1 ];
    fftSrc[ i + 1 ] = fftSrc[ i ] * fftKernel[ i + 1 ] + fftSrc[ i + 1 ] * fftKernel[ i ];
  }

  //�Խ��ͼ����з�FFT
  Fourn(fftSrc, nn, 2, -1);

  //ȷ����һ������
  MaxNum = 0.0;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] =
          sqrt(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] \
              + fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ]);

      if (MaxNum < fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ]) {
        MaxNum = fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ];
      }
    }
  }

  //ת��Ϊͼ��
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      *lpSrc = (BYTE)(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * 255.0 / MaxNum);
    }
  }

  SAFEFREE(fftSrc);
  SAFEFREE(fftKernel);
  // ����
  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   Restore()
 *
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
 *
* ����ֵ:
*   BOOL               - ƽ�Ƴɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
*   �ú���������BlurDIB()���ɵ�DIBͼ����и�ԭ������
 *
 ************************************************************************/
BOOL Restore(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  //ѭ������
  int i;
  int j;

  //����ֵ
  BYTE pixel;

  //������FFT������
  Float* fftSrc, *fftKernel;
  Float a, b, c, d;
  //��άFFT�ĳ��ȺͿ��
  unsigned int nn[ 3 ];
  //ͼ���һ������
  Float MaxNum;

  Float dPower = log((Float) lLineBytes) / log(2.0);

  if (dPower != (int) dPower) {
    return FALSE;
  }

  dPower = log((Float) lHeight) / log(2.0);

  if (dPower != (int) dPower) {
    return FALSE;
  }

  fftSrc = MALLOC(Float, lHeight * lLineBytes * 2 + 1);
  fftKernel = MALLOC(Float, lHeight * lLineBytes * 2 + 1);

  nn[ 1 ] = lHeight;
  nn[ 2 ] = lLineBytes;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] = (Float) pixel;
      fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;

      if (i < 5 && j == 0) {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 1 / 5.0;
      }
      else {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 0.0;
      }

      fftKernel[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;
    }
  }

  //��Դͼ�����FFT
  Fourn(fftSrc, nn, 2, 1);
  //�Ծ����ͼ�����FFT
  Fourn(fftKernel, nn, 2, 1);

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      a = fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ];
      b = fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ];
      c = fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ];
      d = fftKernel[(2 * lLineBytes) * j + 2 * i + 2 ];

      if (c * c + d * d > 1e-3) {
        fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] = (a * c + b * d) / (c * c + d * d);
        fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] = (b * c - a * d) / (c * c + d * d);
      }
    }
  }

  //�Խ��ͼ����з�FFT
  Fourn(fftSrc, nn, 2, -1);

  //ȷ����һ������
  MaxNum = 0.0;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] =
          sqrt(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] \
              + fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ]);

      if (MaxNum < fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ]) {
        MaxNum = fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ];
      }
    }
  }


  //ת��Ϊͼ��
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      *lpSrc = (BYTE)(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * 255.0 / MaxNum);
    }
  }

  SAFEFREE(fftSrc);
  SAFEFREE(fftKernel);
  // ����
  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   NoiseBlur()
 *
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������
*   int lHeight      - Դͼ��߶ȣ���������
 *
* ����ֵ:
*   BOOL               - ģ����������ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
*   �ú���������DIBͼ�����ģ�����������
 *
 ************************************************************************/
BOOL NoiseBlur(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  //ѭ������
  int i;
  int j;

  //����ֵ
  BYTE pixel;

  //������FFT������
  Float* fftSrc, *fftKernel;
  //��άFFT�ĳ��ȺͿ��
  unsigned int nn[ 3 ];
  //ͼ���һ������
  Float MaxNum;

  Float dPower = log((Float) lLineBytes) / log(2.0);

  //ת��Ϊͼ�񣬼���
  char point;

  if (dPower != (int) dPower) {
    return FALSE;
  }

  dPower = log((Float) lHeight) / log(2.0);

  if (dPower != (int) dPower) {
    return FALSE;
  }

  fftSrc = MALLOC(Float, lHeight * lLineBytes * 2 + 1);
  fftKernel = MALLOC(Float, lHeight * lLineBytes * 2 + 1);

  nn[ 1 ] = lHeight;
  nn[ 2 ] = lLineBytes;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] = (Float) pixel;
      fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;

      if (i < 5 && j == 0) {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 1 / 5.0;
      }
      else {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 0.0;
      }

      fftKernel[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;
    }
  }

  //��Դͼ�����FFT
  Fourn(fftSrc, nn, 2, 1);
  //�Ծ����ͼ�����FFT
  Fourn(fftKernel, nn, 2, 1);

  //Ƶ�����
  for (i = 1; i < lHeight * lLineBytes * 2; i += 2) {
    fftSrc[ i ] = fftSrc[ i ] * fftKernel[ i ] - fftSrc[ i + 1 ] * fftKernel[ i + 1 ];
    fftSrc[ i + 1 ] = fftSrc[ i ] * fftKernel[ i + 1 ] + fftSrc[ i + 1 ] * fftKernel[ i ];
  }

  //�Խ��ͼ����з�FFT
  Fourn(fftSrc, nn, 2, -1);

  //ȷ����һ������
  MaxNum = 0.0;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] =
          sqrt(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] \
              + fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ]);

      if (MaxNum < fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ]) {
        MaxNum = fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ];
      }
    }
  }

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      if (i + j == ((int)((i + j) / 8)) * 8) {
        point = -16;
      }
      else {
        point = 0;
      }

      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      *lpSrc = (BYTE)(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * 255.0 / MaxNum + point);
    }
  }

  SAFEFREE(fftSrc);
  SAFEFREE(fftKernel);
  // ����
  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   Wiener()
 *
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������
*   int lHeight      - Դͼ��߶ȣ���������
 *
* ����ֵ:
*   BOOL               - ά���˲���ԭ�����ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
*   �ú���������DIBͼ�����ά���˲���ԭ������
 *
 ************************************************************************/

BOOL Wiener(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  //ѭ������
  int i;
  int j;

  //����ֵ
  BYTE pixel;

  //������FFT������
  Float* fftSrc, *fftKernel, *fftNoise;
  Float a, b, c, d, e, f, multi;
  //��άFFT�ĳ��ȺͿ��
  unsigned int nn[ 3 ];
  //ͼ���һ������
  Float MaxNum;

  Float dPower = log((Float) lLineBytes) / log(2.0);

  if (dPower != (int) dPower) {
    return FALSE;
  }

  dPower = log((Float) lHeight) / log(2.0);

  if (dPower != (int) dPower) {
    return FALSE;
  }

  fftSrc = MALLOC(Float, lHeight * lLineBytes * 2 + 1);
  fftKernel = MALLOC(Float, lHeight * lLineBytes * 2 + 1);
  fftNoise = MALLOC(Float, lHeight * lLineBytes * 2 + 1);

  nn[ 1 ] = lHeight;
  nn[ 2 ] = lLineBytes;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] = (Float) pixel;
      fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;

      if (i < 5 && j == 0) {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 1 / 5.0;
      }
      else {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 0.0;
      }

      fftKernel[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;

      if (i + j == ((int)((i + j) / 8)) * 8) {
        fftNoise [(2 * lLineBytes) * j + 2 * i + 1 ] = -16.0;
      }
      else {
        fftNoise [(2 * lLineBytes) * j + 2 * i + 1 ] = 0.0;
      }

      fftNoise[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;

    }
  }

  srand((unsigned) time(NULL));

  //��Դͼ�����FFT
  Fourn(fftSrc, nn, 2, 1);
  //�Ծ����ͼ�����FFT
  Fourn(fftKernel, nn, 2, 1);
  //������ͼ�����FFT
  Fourn(fftNoise, nn, 2, 1);

  for (i = 1; i < lHeight * lLineBytes * 2; i += 2) {
    a = fftSrc[ i ];
    b = fftSrc[ i + 1 ];
    c = fftKernel[ i ];
    d = fftKernel[ i + 1 ];
    e = fftNoise[ i ];
    f = fftNoise[ i + 1 ];
    multi = (a * a + b * b) / (a * a + b * b - e * e - f * f);

    if (c * c + d * d > 1e-3) {
      fftSrc[ i ] = (a * c + b * d) / (c * c + d * d) / multi;
      fftSrc[ i + 1 ] = (b * c - a * d) / (c * c + d * d) / multi;
    }
  }

  //�Խ��ͼ����з�FFT
  Fourn(fftSrc, nn, 2, -1);

  //ȷ����һ������
  MaxNum = 0.0;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] =
          sqrt(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] \
              + fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ]);

      if (MaxNum < fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ]) {
        MaxNum = fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ];
      }
    }
  }

  //ת��Ϊͼ��
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      *lpSrc = (BYTE)(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * 255.0 / MaxNum);
    }
  }

  SAFEFREE(fftSrc);
  SAFEFREE(fftKernel);
  SAFEFREE(fftNoise);
  // ����
  return TRUE;
}


BOOL Fourn(Float* data, unsigned int nn[], int ndim, int isign)
{
  int idim;
  unsigned int i1, i2, i3, i2rev, i3rev, ip1, ip2, ip3, ifp1, ifp2;
  unsigned int ibit, k1, k2, n, nprev, nrem, ntot;
  Float tempi, tempr;
  Float theta, wi, wpi, wpr, wr, wtemp;

  for (ntot = 1, idim = 1; idim <= ndim; idim++) {
    ntot *= nn[ idim ];
  }

  nprev = 1;

  for (idim = ndim; idim >= 1; idim--) {
    n = nn[ idim ];
    nrem = ntot / (n * nprev);
    ip1 = nprev << 1;
    ip2 = ip1 * n;
    ip3 = ip2 * nrem;
    i2rev = 1;

    for (i2 = 1; i2 <= ip2; i2 += ip1) {
      if (i2 < i2rev) {
        for (i1 = i2; i1 <= i2 + ip1 - 2; i1 += 2) {
          for (i3 = i1; i3 <= ip3; i3 += ip2) {
            i3rev = i2rev + i3 - i2;
            SWAP(data[ i3 ], data[ i3rev ]);
            SWAP(data[ i3 + 1 ], data[ i3rev + 1 ]);
          }
        }
      }

      ibit = ip2 >> 1;

      while (ibit >= ip1 && i2rev > ibit) {
        i2rev -= ibit;
        ibit >>= 1;
      }

      i2rev += ibit;
    }

    ifp1 = ip1;

    while (ifp1 < ip2) {
      ifp2 = ifp1 << 1;
      theta = isign * 6.28318530717959 / (ifp2 / ip1);
      wtemp = sin(0.5 * theta);
      wpr = -2.0 * wtemp * wtemp;
      wpi = sin(theta);
      wr = 1.0;
      wi = 0.0;

      for (i3 = 1; i3 <= ifp1; i3 += ip1) {
        for (i1 = i3; i1 <= i3 + ip1 - 2; i1 += 2) {
          for (i2 = i1; i2 <= ip3; i2 += ifp2) {
            k1 = i2;
            k2 = k1 + ifp1;
            tempr = wr * data[ k2 ] - wi * data[ k2 + 1 ];
            tempi = wr * data[ k2 + 1 ] + wi * data[ k2 ];
            data[ k2 ] = data[ k1 ] - tempr;
            data[ k2 + 1 ] = data[ k1 + 1 ] - tempi;
            data[ k1 ] += tempr;
            data[ k1 + 1 ] += tempi;
          }
        }

        wr = (wtemp = wr) * wpr - wi * wpi + wr;
        wi = wi * wpr + wtemp * wpi + wi;
      }

      ifp1 = ifp2;
    }

    nprev *= n;
  }

  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   RandomNoise()
 *
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
 *
* ����ֵ:
*   BOOL               - ģ�������ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
*   �ú���������DIBͼ�����ģ��������
 *
 ************************************************************************/

BOOL RandomNoise(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  //ѭ������
  int i;
  int j;

  //����ֵ
  BYTE pixel;

  //����
  BYTE NoisePoint;

  //����α�������
  srand((unsigned) time(NULL));

  //��ͼ���м���
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      NoisePoint = rand() / 1024;

      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      //ȡ������ֵ
      pixel = (BYTE) * lpSrc;

      *lpSrc = (BYTE)(pixel * 224 / 256 + NoisePoint);
    }
  }

  // ����
  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   SaltNoise()
 *
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
 *
* ����ֵ:
*   BOOL               - ģ�������ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
*   �ú���������DIBͼ�����ģ��������
 *
 ************************************************************************/

BOOL SaltNoise(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  //ѭ������
  int i;
  int j;

  //����α�������
  srand((unsigned) time(NULL));

  //��ͼ���м���
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      if (rand() > 31500) {
        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

        //ͼ���е�ǰ����Ϊ��
        *lpSrc = 0;
      }
    }
  }

  // ����
  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   NoRestriction()
 *
* ����:
*   CDib  *pDib       - ָ��CDib���ָ��
 *
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
*   �ú���������DIBͼ�����ģ��������
 *
 ************************************************************************/
// int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes
BOOL NoRestriction(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  //ѭ������
  int i;
  int j;

  //��ʱ����
  Float temp;

  // ʵ�ʽ��и���Ҷ�任�Ŀ�Ⱥ͸߶�
  int lW = 1;
  int lH = 1;

  int wp = 0;
  int hp = 0;

  //�����洢Դͼ��ͱ任�˵�ʱ������
  COMPLEX* pCTSrc, *pCTH;

  //�����洢Դͼ��ͱ任�˵�Ƶ������
  COMPLEX* pCFSrc, *pCFH;

  //ͼ���һ������
  Float MaxNum;

  // ��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2�������η�
  while (lW * 2 <= lLineBytes) {
    lW = lW * 2;
    wp++;
  }

  while (lH * 2 <= lHeight) {
    lH = lH * 2;
    hp++;
  }

  //����ͼ��ĳ��Ϳ����Ϊ2��������
  if (lW != (int) lLineBytes) {
    return FALSE;
  }

  if (lH != (int) lHeight) {
    return FALSE;
  }

  // Ϊʱ���Ƶ����������ռ�
  pCTSrc = MALLOC(COMPLEX, lHeight * lLineBytes);
  pCTH = MALLOC(COMPLEX, lHeight * lLineBytes);

  pCFSrc = MALLOC(COMPLEX, lHeight * lLineBytes);
  pCFH = MALLOC(COMPLEX, lHeight * lLineBytes);

  // �����ݴ���ʱ������
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      CplexSet((Float) * lpSrc , 0, pCTSrc[ lLineBytes * j + i ]);
      CplexSet(0.0 , 0.0, pCFSrc[ lLineBytes * j + i ]);

      if (i < 5 && j < 5) {
        CplexSet(0.04 , 0.0, pCTH[ lLineBytes * j + i ]);
      }
      else {
        CplexSet(0.0 , 0.0, pCTH[ lLineBytes * j + i ]);
      }

      CplexSet(0.0 , 0.0, pCFH[ lLineBytes * j + i ]);
    }
  }

  //��Դͼ�����FFT
  Fft2D(pCTSrc, lLineBytes, lHeight, pCFSrc);

  //�Ա任��ͼ�����FFT
  Fft2D(pCTH, lLineBytes, lHeight, pCFH);

  //Ƶ�����
  for (i = 0; i < lHeight * lLineBytes; i++) {
    CplexMul(pCFSrc[ i ], pCFH[ i ], pCFSrc[ i ]);
  }

  //�Խ��ͼ����з�FFT
  IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

  //ȷ����һ������
  MaxNum = 0.0;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      temp = sqrt(CREAL(pCTSrc[ lLineBytes * j + i ]) * CREAL(pCTSrc[ lLineBytes * j + i ]) +
          CIMAG(pCTSrc[ lLineBytes * j + i ]) * CIMAG(pCTSrc[ lLineBytes * j + i ]));

      //ѡ���һ������
      if (MaxNum < temp) {
        MaxNum = temp;
      }
    }
  }

  //ת��Ϊͼ��
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;
      *lpSrc = (BYTE)(CREAL(pCTSrc[(lLineBytes) * j + i ]) * 255.0 / MaxNum);
    }
  }

  //�ͷŴ洢�ռ�
  SAFEFREE(pCTSrc);
  SAFEFREE(pCTH);

  SAFEFREE(pCFSrc);
  SAFEFREE(pCFH);

  // ����
  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   NoiseDegeneration()
 *
* ����:
*   CDib  *pDib       - ָ��CDib���ָ��
 *
* ����ֵ:
*   BOOL               - ģ����������ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
*   �ú���������DIBͼ�����ģ�����������
 *
 ************************************************************************/

BOOL NoiseDegeneration(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  //ѭ������
  int i;
  int j;

  //ת��Ϊͼ�񣬼���
  BYTE NoisePoint;

  //��ʱ����
  Float temp;

  //ͼ���һ������
  Float MaxNum;

  // ʵ�ʽ��и���Ҷ�任�Ŀ�Ⱥ͸߶�
  int lW = 1;
  int lH = 1;

  int wp = 0;
  int hp = 0;

  //�����洢Դͼ��ͱ任�˵�ʱ������
  COMPLEX* pCTSrc, *pCTH;

  //�����洢Դͼ��ͱ任�˵�Ƶ������
  COMPLEX* pCFSrc, *pCFH;

  // ��֤��ɢ����Ҷ�任�Ŀ�Ⱥ͸߶�Ϊ2�������η�
  while (lW * 2 <= lLineBytes) {
    lW = lW * 2;
    wp++;
  }

  while (lH * 2 <= lHeight) {
    lH = lH * 2;
    hp++;
  }

  // Ϊʱ���Ƶ����������ռ�
  pCTSrc = MALLOC(COMPLEX, lHeight * lLineBytes);
  pCTH = MALLOC(COMPLEX, lHeight * lLineBytes);

  pCFSrc = MALLOC(COMPLEX, lHeight * lLineBytes);
  pCFH = MALLOC(COMPLEX, lHeight * lLineBytes);

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // ������ֵ�洢��ʱ��������
      CplexSet((Float) * lpSrc , 0, pCTSrc[ lLineBytes * j + i ]);

      // Ƶ����ֵ
      CplexSet(0.0 , 0.0, pCFSrc[ lLineBytes * j + i ]);

      // ������ͼ�����˻���ϵͳ
      if (i < 5 && j < 5) {
        CplexSet(0.04 , 0.0, pCTH[ lLineBytes * j + i ]);
      }
      else {
        CplexSet(0.0 , 0.0, pCTH[ lLineBytes * j + i ]);
      }

      // Ƶ����ֵ
      CplexSet(0.0 , 0.0, pCFH[ lLineBytes * j + i ]);
    }
  }

  //��Դͼ�����FFT
  Fft2D(pCTSrc, lLineBytes, lHeight, pCFSrc);

  //�Ա任��ͼ�����FFT
  Fft2D(pCTH, lLineBytes, lHeight, pCFH);

  //Ƶ�����
  for (i = 0; i < lHeight * lLineBytes; i++) {
    CplexMul(pCFSrc[ i ], pCFH[ i ], pCFSrc[ i ]);
  }

  //�Խ��ͼ����з�FFT
  IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

  //ȷ����һ������
  MaxNum = 0.0;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      temp = sqrt(CREAL(pCTSrc[ lLineBytes * j + i ]) * CREAL(pCTSrc[ lLineBytes * j + i ]) +
          CIMAG(pCTSrc[ lLineBytes * j + i ]) * CIMAG(pCTSrc[ lLineBytes * j + i ]));

      //ѡ���һ������
      if (MaxNum < temp) {
        MaxNum = temp;
      }
    }
  }

  //����α���������
  srand((unsigned) time(NULL));

  //ת��Ϊͼ�񣬲�����α�������
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // ����������
      NoisePoint = rand() / 2048 - 8;

      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // ʱ����룬�洢����ֵ
      *lpSrc = (BYTE)(CREAL(pCTSrc[(lLineBytes) * j + i ]) * 255.0 / MaxNum + NoisePoint);

      //�������ֵ����ֱ�Ӹ�ֵ255
      if (*lpSrc > 255) {
        * lpSrc = 255;
      }
    }
  }

  //�ͷŴ洢�ռ�
  SAFEFREE(pCTSrc);
  SAFEFREE(pCTH);

  SAFEFREE(pCFSrc);
  SAFEFREE(pCFH);

  // ����
  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   MotionDegeneration()
 *
* ����:
*   CDib  *pDib       - ָ��CDib���ָ��
 *
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
*   �ú���������DIBͼ��ģ��������ֱ���˶���ɵ�ģ��
 *
 ************************************************************************/
BOOL MotionDegeneration(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  //ѭ������
  int iColumn;
  int jRow;

  //��ʱ����
  int temp, m;

  // ��ʱ����
  Float p, q;

  int nTotTime, nTotLen, nTime;

  // ������ı���ϵ��
  Float B = 0.1;

  //�����洢Դͼ��ͱ任�˵�ʱ������
  int* nImageDegener;

  //�ܵ��˶�ʱ��10s
  nTotTime = 10;

  // �ܵ��˶�����10�����ص�
  nTotLen = 10;

  // Ϊʱ���Ƶ����������ռ�
  nImageDegener = MALLOC(int, lHeight * lLineBytes);

  // �����ݴ���ʱ������
  for (jRow = 0; jRow < lHeight; jRow++) {
    for (iColumn = 0; iColumn < lLineBytes; iColumn++) {
      temp = 0;

      // ָ��Դͼ������jRow�У���iColumn�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + iColumn;

      // ���ص������ֵ����
      for (nTime = 0; nTime < nTotTime; nTime++) {
        p = (Float) iColumn - (Float)(nTotLen) * nTime / nTotTime;

        if (p > 0) {
          q = p - floor((Float) p);

          if (q >= 0.5) {
            m = (int) ceil((Float) p);
          }
          else {
            m = (int) floor((Float) p);
          }

          // �ۼ�
          lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + m;
          temp = temp + *lpSrc;
        }
      }

      // ����������ı���ϵ��
      temp = (int)(B * temp);

      temp = (int) ceil((Float) temp);

      // ʹ��temp��ȡֵ����ȡֵ��Χ
      if (temp < 0) {
        temp = 0;
      }

      if (temp > 255) {
        temp = 255;
      }

      nImageDegener[ lLineBytes * jRow + iColumn ] = temp;
    }
  }

  //ת��Ϊͼ��
  for (jRow = 0; jRow < lHeight; jRow++) {
    for (iColumn = 0; iColumn < lLineBytes; iColumn++) {
      // ָ��Դͼ������jRow�У���iColumn�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + iColumn;

      *lpSrc = nImageDegener[ lLineBytes * jRow + iColumn ];
    }
  }

  //�ͷŴ洢�ռ�
  SAFEFREE(nImageDegener);

  // ����
  return TRUE;
}

/*************************************************************************
 *
* �������ƣ�
*   MotionRestore()
 *
* ����:
*   CDib  *pDib       - ָ��CDib���ָ��
 *
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
*   �ú�����������������ֱ���˶���ɵ�ģ��ͼ����и�ԭ
 *
 ************************************************************************/
BOOL MotionRestore(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  //ѭ������
  int iColumn;
  int jRow;

  int i, n, m;

  //��ʱ����
  int temp1, temp2,
      totalq, q1, q2, z;

  Float p, q;

  // ����A��ֵ
  int A = 80;

  //����B��ֵ
  int B = 10;

  //�ܵ��ƶ�����
  int nTotLen = 10;

  // ͼ���Ȱ������ٸ�ntotlen
  int K = lLineBytes / nTotLen;

  int error[ 10 ];

  //�����洢Դͼ��ʱ������
  int* nImageDegener;

  // Ϊʱ���������ռ�
  nImageDegener = MALLOC(int, lHeight * lLineBytes);

  // �����ش���������
  for (jRow = 0; jRow < lHeight; jRow++) {
    for (iColumn = 0; iColumn < lLineBytes; iColumn++) {
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + iColumn;

      nImageDegener[ lLineBytes * jRow + iColumn ] = (*lpSrc);
    }
  }

  for (jRow = 0; jRow < lHeight; jRow++) {
    // ����error[i]
    for (i = 0; i < 10; i++) {

      error[ i ] = 0;

      for (n = 0; n < K; n++)
        for (m = 0; m <= n; m++) {
          // �����Ƿ�Ϊһ�еĿ�ʼ��
          if (i == 0 && m == 0) {
            temp1 = temp2 = 0;
          }

          // ���в������
          else {
            lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + m * 10 + i;
            temp1 = *lpSrc;

            lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + m * 10 + i - 1;
            temp2 = *lpSrc;
          }

          error[ i ] = error[ i ] + temp1 - temp2;

        }

      error[ i ] = B * error[ i ] / K;
    }

    for (iColumn = 0; iColumn < lLineBytes; iColumn++) {
      // ����m���Լ�z
      m = iColumn / nTotLen;
      z = iColumn - m * nTotLen;

      // ��ʼ��
      totalq = 0;
      q = 0;

      for (n = 0; n <= m; n++) {
        q1 = iColumn - nTotLen * n;

        if (q1 == 0) {
          q = 0;
        }

        // ���в������
        else {
          q2 = q1 - 1;

          lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + q1;
          temp1 = *lpSrc;

          lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + q2;
          temp2 = *lpSrc;

          q = (temp1 - temp2) * B;
        }

        totalq += (int) q;
      }

      p = error[ z ];

      // �õ�f(x, y)��ֵ
      temp1 = (int)(totalq + A - p);

      // ʹ�����ص�ȡֵ����ȡֵ��Χ
      if (temp1 < 0) {
        temp1 = 0;
      }

      if (temp1 > 255) {
        temp1 = 255;
      }

      nImageDegener[ lLineBytes * jRow + iColumn ] = temp1;
    }
  }

  //ת��Ϊͼ��
  for (jRow = 0; jRow < lHeight; jRow++) {
    for (iColumn = 0; iColumn < lLineBytes; iColumn++) {
      // ָ��Դͼ������jRow�У���iColumn�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + iColumn;

      // �洢����ֵ
      *lpSrc = nImageDegener[ lLineBytes * jRow + iColumn ];
    }
  }

  //�ͷŴ洢�ռ�
  SAFEFREE(nImageDegener);

  // ����
  return TRUE;
}

#undef SWAP

