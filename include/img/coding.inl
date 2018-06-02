
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

// FOURBYTES��������������4�����������
#define FOURBYTES(bits)    (((bits) + 31) / 32* 4)

#define FWRITE(file, _PTR, _N) fwrite( _PTR, _N, 1, file)
#define FWRITEHUGE(file, _PTR, _N) fwrite( _PTR, _N, 1, file)
#define FREAD(file, _PTR, _N) fread( _PTR, _N, 1, file)
#define FREADHUGE(file, _PTR, _N) fread( _PTR, _N, 1, file)

/**************************************************************************
 *  �ļ�����ImageCoding.cpp
 *
 *  �����任API�����⣺
 *
 *  BITPLANE()            - ͼ��λƽ��ֽ�
 *  LimbPatternBayer()       - ��BAYER������ʾͼ��
 *  DitherFloydSteinberg()   - ��Floyd��Steinberg�㷨��������ͼ��
 *
 *************************************************************************/

/*************************************************************************
 *
* �������ƣ�
*   BITPLANE()
 *
* ����:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes        - ָ��CDib���ָ��
*   FILE* file        - Ҫ��ȡ���ļ�
 *
* ����ֵ:
*   BOOL               - �ɹ�����TRUE
 *
* ˵��:
*   �ú������ƶ���ͼ�����λƽ��ֽ�
 *
 *************************************************************************/
BOOL BitPlane(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, BYTE bBitNum)
{
  LPBYTE lpSrc; // ָ��Դͼ���ָ��
  int i, j; // ѭ������
  BYTE bTemp, bA; // �м����

  // �����Ƶ�iλ��Ӧ��ʮ����ֵ
  BYTE bCount;

  // ����������������Ҫ�󣬲����зֽ�
  if (bBitNum < 1 || bBitNum > 8) {
    return FALSE;
  }

  bCount = (BYTE)(1 << (bBitNum - 1));

  for (i = 0; i < lHeight; i++) {
    for (j = 0; j < lWidth; j++) {
      // ָ��λͼi��j�е�ָ��
      lpSrc = (LPBYTE) lpDIBBits + j + lLineBytes * (lHeight - 1 - i);

      // ��λͼ���Ҷ�����зֽ�
      if (bBitNum == 8) {
        // ��������λͼ8��ֱ��ȡ��ֵ�ĵ�8λ
        bTemp = (BYTE)((*(lpSrc) & bCount) / bCount);
        bTemp = (BYTE)(bTemp * 255);
      }
      else {
        // ������������ûҶ���
        bTemp = (BYTE)((*(lpSrc) & bCount) / bCount);
        bA = (BYTE)(bCount * 2);

        // ��i��1λͼ������ֵ
        bA = (BYTE)((*(lpSrc) & bA) / bA);

        // ���
        bTemp = (BYTE)((bTemp ^ bA) * 255);
      }

      // ����Դͼ��
      *(lpSrc) = bTemp;
    }
  }

  // ����ֵ
  return TRUE;
}

/*************************************************************************
 * �������ƣ�
 *   LimbPatternBayer()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�������BAYER������ʾͼ��
 ************************************************************************/
BOOL LimbPatternBayer(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // Bayer��Ķ���
  BYTE BayerPattern[ 8 ][ 8 ] = { //
    0, 32, 8, 40, 2, 34, 10, 42,
    48, 16, 56, 24, 50, 18, 58, 26,
    12, 44, 4, 36, 14, 46, 6, 38,
    60, 28, 52, 20, 62, 30, 54, 22,
    3, 35, 11, 43, 1, 33, 9, 41,
    51, 19, 59, 27, 49, 17, 57, 25,
    15, 47, 7, 39, 13, 45, 5, 37,
    63, 31, 55, 23, 61, 29, 53, 21
  };

  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  // ѭ������
  int i, j;

  // ���ص�ֵ
  int nPixelValue;

  // ��ͼ���ֵ��������BAYER������ʾͼ��
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      nPixelValue = (*lpSrc);

      nPixelValue = nPixelValue;

      // ������λ�����Ƚ�
      if ((nPixelValue >> 2) > BayerPattern[ j & 7 ][ i & 7 ])
        //��׵�
      {
        *(lpSrc) = (BYTE) 255;
      }

      else
        //��ڵ�
      {
        *(lpSrc) = (BYTE) 0;
      }
    }
  }

  return TRUE;
}

/*************************************************************************
 * �������ƣ�
 *   DitherFloydSteinberg()
 *
 * ����:
 *   CDib  *pDib       - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������Floyd��Steinberg�㷨��������ͼ��
 ************************************************************************/
BOOL DitherFloydSteinberg(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;

  // ѭ������
  int i, j;

  // ����ϵ��
  Float temp, error;

  // ����ֵ
  int nPixelValue;

  // ��ͼ���ֵ��������Floyd��Steinberg�㷨��������ͼ��
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      nPixelValue = *lpSrc;

      //128����ֵ
      if (nPixelValue > 128) {
        //��׵�
        *lpSrc = 255;

        //�������
        error = (Float)(nPixelValue - 255.0);
      }
      else {
        //��ڵ�
        *lpSrc = 0;

        //�������
        error = (Float) nPixelValue;
      }

      // ������Ǳ߽�
      if (i < lLineBytes - 1) {
        //���Ҵ���
        temp = (Float) * (lpSrc + 1);

        temp = temp + error * (1.5 / 8.0);

        if (temp > 255.0) {
          temp = 255.0;
        }

        *(lpSrc + 1) = (BYTE) temp;

      }

      // ������Ǳ߽�
      if (j < lHeight - 1) {
        // ���´���
        temp = (Float) * (lpSrc + lLineBytes);

        temp = temp + error * (1.5 / 8.0);

        *(lpSrc + lLineBytes) = (BYTE) temp;

        if (i < lLineBytes - 1) {
          // �����´���
          temp = (Float) * (lpSrc + lLineBytes + 1);

          temp = temp + error * (2.0 / 16.0);

          *(lpSrc + lLineBytes + 1) = (BYTE) temp;
        }
      }

    }

  }

  return TRUE;
}
