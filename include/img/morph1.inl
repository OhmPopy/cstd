// ************************************************************************
//  �ļ�����morph.cpp
//
//  ͼ����̬ѧ�任API�����⣺
//
//  ErosionBlock()  - ͼ��ʴ
//  DilationBlock() - ͼ������
//  OpenBlock()     - ͼ������
//  Closelock()    - ͼ�������
//  Thining()  - ͼ��ϸ��
//
// ************************************************************************

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

/*************************************************************************
* �������ƣ�
*   ErosionBlock()
* ����:
*   LPBYTE lpDIBBits       - ָ��ԴDIBͼ��ָ��
*   int lWidth           - Դͼ���ȣ���������������4�ı�����
*   int lHeight          - Դͼ��߶ȣ���������
*   int  nMode            - ��ʴ��ʽ��0��ʾˮƽ����1��ֱ����2�Զ���ṹԪ�ء�
*   int  structure[3][3]  - �Զ����3��3�ṹԪ�ء�
* ����ֵ:
*   BOOL                  - ��ʴ�ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶ�ͼ����и�ʴ���㡣�ṹԪ��Ϊˮƽ�����ֱ����������㣬
* �м��λ��ԭ�㣻�������û��Լ�����3��3�ĽṹԪ�ء�
* Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL ErosionBlock(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nMode , int structure[ 3 ][ 3 ])
{
  LPBYTE lpSrc;   // ָ��Դͼ���ָ��
  LPBYTE lpDst;   // ָ�򻺴�ͼ���ָ��
  LPBYTE lpNewDIBBits; // ָ�򻺴�DIBͼ���ָ��
  int i, j, n, m;   //ѭ������
  BYTE pixel; //����ֵ

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  if (nMode == 0) {
    //ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
    for (j = 0; j < lHeight; j++) {
      for (i = 1; i < lWidth - 1; i++) {
        // ����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұ�
        // ����������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = lpDIBBits + lWidth * j + i;
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = lpNewDIBBits + lWidth * j + i;
        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixel = (BYTE) * lpSrc;

        //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
        *lpDst = (BYTE) 0;

        //���Դͼ���е�ǰ���������������һ���㲻�Ǻ�ɫ��
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + n - 1);

          if (pixel == 255) {
            *lpDst = (BYTE) 255;
            break;
          }
        }
      }
    }
  }
  else if (nMode == 1) {
    //ʹ�ô�ֱ����ĽṹԪ�ؽ��и�ʴ
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // ����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±�
        // ����������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = lpDIBBits + lWidth * j + i;
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = lpNewDIBBits + lWidth * j + i;
        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixel = (BYTE) * lpSrc;

        //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
        *lpDst = (BYTE) 0;

        //���Դͼ���е�ǰ���������������һ���㲻�Ǻ�ɫ��
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + (n - 1) * lWidth);

          if (pixel == 255) {
            *lpDst = (BYTE) 255;
            break;
          }
        }
      }
    }
  }
  else {
    //ʹ���Զ���ĽṹԪ�ؽ��и�ʴ
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // ����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұ�
        // ���������غ����ϱߺ����±ߵ���������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = lpDIBBits + lWidth * j + i;
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = lpNewDIBBits + lWidth * j + i;
        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixel = (BYTE) * lpSrc;

        //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
        *lpDst = (BYTE) 0;

        //���ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ������һ�����Ǻ�ɫ��
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
        //ע����DIBͼ�������������µ��õ�
        for (m = 0; m < 3; m++) {
          for (n = 0; n < 3; n++) {
            if (structure[ m ][ n ] == -1) {
              continue;
            }

            pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));

            if (pixel == 255) {
              *lpDst = (BYTE) 255;
              break;
            }
          }
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
*   DilationBlock()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
*   int  nMode  - ���ͷ�ʽ��0��ʾˮƽ����1��ֱ����2�Զ���ṹԪ�ء�
*  int  structure[3][3] - �Զ����3��3�ṹԪ�ء�
* ����ֵ:
*   BOOL              - ���ͳɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶ�ͼ������������㡣�ṹԪ��Ϊˮƽ�����ֱ����������㣬
* �м��λ��ԭ�㣻�������û��Լ�����3��3�ĽṹԪ�ء�
* Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL DilationBlock(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nMode, int structure[ 3 ][ 3 ])
{
  LPBYTE lpSrc;   // ָ��Դͼ���ָ��
  LPBYTE lpDst;   // ָ�򻺴�ͼ���ָ��
  LPBYTE lpNewDIBBits; // ָ�򻺴�DIBͼ���ָ��
  int i, j, m, n;   // ѭ������
  BYTE pixel; // ����ֵ

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  if (nMode == 0) {
    //ʹ��ˮƽ����ĽṹԪ�ؽ�������
    for (j = 0; j < lHeight; j++) {
      for (i = 1; i < lWidth - 1; i++) {
        // ����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұ�
        // ����������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = lpDIBBits + lWidth * j + i;
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = lpNewDIBBits + lWidth * j + i;
        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixel = (BYTE) * lpSrc;

        //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
        if (pixel != 255 && pixel != 0) {
          return FALSE;
        }

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
        *lpDst = (BYTE) 255;

        //Դͼ���е�ǰ�������������ֻҪ��һ�����Ǻ�ɫ��
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + n - 1);

          if (pixel == 0) {
            *lpDst = (BYTE) 0;
            break;
          }
        }
      }
    }
  }
  else if (nMode == 1) {
    //ʹ�ô�ֱ����ĽṹԪ�ؽ�������
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // ����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±�
        // ����������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = lpDIBBits + lWidth * j + i;
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = lpNewDIBBits + lWidth * j + i;
        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixel = (BYTE) * lpSrc;

        //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
        *lpDst = (BYTE) 255;

        //Դͼ���е�ǰ�������������ֻҪ��һ�����Ǻ�ɫ��
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + (n - 1) * lWidth);

          if (pixel == 0) {
            *lpDst = (BYTE) 0;
            break;
          }
        }
      }
    }
  }
  else {
    //ʹ���Զ���ĽṹԪ�ؽ�������
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // ����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұ�
        // ���������غ����ϱߺ����±ߵ���������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = lpDIBBits + lWidth * j + i;
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = lpNewDIBBits + lWidth * j + i;
        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixel = (BYTE) * lpSrc;

        //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
        *lpDst = (BYTE) 255;

        //ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ����ֻҪ��һ���Ǻ�ɫ��
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
        //ע����DIBͼ�������������µ��õ�
        for (m = 0; m < 3; m++) {
          for (n = 0; n < 3; n++) {
            if (structure[ m ][ n ] == -1) {
              continue;
            }

            pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));

            if (pixel == 0) {
              *lpDst = (BYTE) 0;
              break;
            }
          }
        }
      }
    }
  }

  // �������ͺ��ͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits);

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   OpenBlock()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
*   int  nMode  - �����㷽ʽ��0��ʾˮƽ����1��ֱ����2�Զ���ṹԪ�ء�
*  int  structure[3][3] - �Զ����3��3�ṹԪ�ء�
* ����ֵ:
*   BOOL              - ������ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶ�ͼ����п����㡣�ṹԪ��Ϊˮƽ�����ֱ����������㣬
* �м��λ��ԭ�㣻�������û��Լ�����3��3�ĽṹԪ�ء�
* Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL OpenBlock(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nMode, int structure[ 3 ][ 3 ])
{
  LPBYTE lpSrc;   // ָ��Դͼ���ָ��
  LPBYTE lpDst;   // ָ�򻺴�ͼ���ָ��
  LPBYTE lpNewDIBBits; // ָ�򻺴�DIBͼ���ָ��
  int i, j, m, n;   //ѭ������
  BYTE pixel; //����ֵ

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  if (nMode == 0) {
    //ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
    for (j = 0; j < lHeight; j++) {
      for (i = 1; i < lWidth - 1; i++) {
        // ����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұ�
        // ����������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = lpDIBBits + lWidth * j + i;
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = lpNewDIBBits + lWidth * j + i;
        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixel = (BYTE) * lpSrc;

        //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
        *lpDst = (BYTE) 0;

        //���Դͼ���е�ǰ���������������һ���㲻�Ǻ�ɫ��
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + n - 1);

          if (pixel == 255) {
            *lpDst = (BYTE) 255;
            break;
          }
        }
      }
    }
  }
  else if (nMode == 1) {
    //ʹ�ô�ֱ����ĽṹԪ�ؽ��и�ʴ
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // ����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±�
        // ����������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = lpDIBBits + lWidth * j + i;
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = lpNewDIBBits + lWidth * j + i;
        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixel = (BYTE) * lpSrc;

        //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
        *lpDst = (BYTE) 0;

        //���Դͼ���е�ǰ���������������һ���㲻�Ǻ�ɫ��
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + (n - 1) * lWidth);

          if (pixel == 255) {
            *lpDst = (BYTE) 255;
            break;
          }
        }
      }
    }
  }
  else {
    //ʹ���Զ���ĽṹԪ�ؽ��и�ʴ
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // ����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұ�
        // ���������غ����ϱߺ����±ߵ���������
        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = lpDIBBits + lWidth * j + i;
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = lpNewDIBBits + lWidth * j + i;
        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixel = (BYTE) * lpSrc;

        //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
        *lpDst = (BYTE) 0;

        //���ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ������һ�����Ǻ�ɫ��
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
        //ע����DIBͼ�������������µ��õ�
        for (m = 0; m < 3; m++) {
          for (n = 0; n < 3; n++) {
            if (structure[ m ][ n ] == -1) {
              continue;
            }

            pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));

            if (pixel == 255) {
              *lpDst = (BYTE) 255;
              break;
            }
          }
        }
      }
    }
  }

  // ���Ƹ�ʴ���ͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  // ���³�ʼ���·�����ڴ棬�趨��ʼֵΪ255
  //lpDst = lpNewDIBBits;
  //memset(lpDst, (BYTE)255, lWidth* lHeight);

  if (nMode == 0) {
    //ʹ��ˮƽ����ĽṹԪ�ؽ�������
    for (j = 0; j < lHeight; j++) {
      for (i = 1; i < lWidth - 1; i++) {
        // ����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұ�
        // ����������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = lpDIBBits + lWidth * j + i;
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = lpNewDIBBits + lWidth * j + i;
        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixel = (BYTE) * lpSrc;

        //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
        *lpDst = (BYTE) 255;

        //Դͼ���е�ǰ�������������ֻҪ��һ�����Ǻ�ɫ��
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + n - 1);

          if (pixel == 0) {
            *lpDst = (BYTE) 0;
            break;
          }
        }
      }
    }
  }
  else if (nMode == 1) {
    //ʹ�ô�ֱ����ĽṹԪ�ؽ�������
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // ����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±�
        // ����������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = lpDIBBits + lWidth * j + i;
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = lpNewDIBBits + lWidth * j + i;
        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixel = (BYTE) * lpSrc;

        //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
        *lpDst = (BYTE) 255;

        //Դͼ���е�ǰ�������������ֻҪ��һ�����Ǻ�ɫ��
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + (n - 1) * lWidth);

          if (pixel == 0) {
            *lpDst = (BYTE) 0;
            break;
          }
        }
      }
    }
  }
  else {
    //ʹ���Զ���ĽṹԪ�ؽ�������
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // ����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұ�
        // ���������غ����ϱߺ����±ߵ���������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = lpDIBBits + lWidth * j + i;
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = lpNewDIBBits + lWidth * j + i;
        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixel = (BYTE) * lpSrc;

        //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
        *lpDst = (BYTE) 255;

        //ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ����ֻҪ��һ���Ǻ�ɫ��
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
        //ע����DIBͼ�������������µ��õ�
        for (m = 0; m < 3; m++) {
          for (n = 0; n < 3; n++) {
            if (structure[ m ][ n ] == -1) {
              continue;
            }

            pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));

            if (pixel == 0) {
              *lpDst = (BYTE) 0;
              break;
            }
          }
        }
      }
    }
  }

  // �������ͺ��ͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits);

  return TRUE;
}

/*************************************************************************
* �������ƣ�
*   CloseBlock()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
*   int  nMode  - �����㷽ʽ��0��ʾˮƽ����1��ֱ����2�Զ���ṹԪ�ء�
*  int  structure[3][3] - �Զ����3��3�ṹԪ�ء�
* ����ֵ:
*   BOOL              - ������ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶ�ͼ����п����㡣�ṹԪ��Ϊˮƽ�����ֱ����������㣬
* �м��λ��ԭ�㣻�������û��Լ�����3��3�ĽṹԪ�ء�
* Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL CloseBlock(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nMode, int structure[ 3 ][ 3 ])
{
  if (DilationBlock(lHeight, lWidth, lpDIBBits, lLineBytes, nMode , structure)) {
    if (ErosionBlock(lHeight, lWidth, lpDIBBits, lLineBytes, nMode , structure)) {
      return TRUE;
    }
  }

  return FALSE;
}

/*************************************************************************
* �������ƣ�
*   Thining()
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth       - Դͼ���ȣ���������������4�ı�����
*   int lHeight      - Դͼ��߶ȣ���������
* ����ֵ:
*   BOOL              - ������ɹ�����TRUE�����򷵻�FALSE��
* ˵��:
* �ú������ڶ�ͼ�����ϸ�����㡣Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL Thining(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;   // ָ��Դͼ���ָ��
  LPBYTE lpDst;   // ָ�򻺴�ͼ���ָ��
  LPBYTE lpNewDIBBits; // ָ�򻺴�DIBͼ���ָ��
  BOOL bModified;   //����
  int i, j, m, n;   //ѭ������

  //�ĸ�����
  BOOL bCondition1;
  BOOL bCondition2;
  BOOL bCondition3;
  BOOL bCondition4;

  BYTE nCount; //������
  BYTE pixel; //����ֵ
  BYTE neighbour[ 5 ][ 5 ]; //5��5������������ֵ

  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  bModified = TRUE;

  while (bModified) {
    bModified = FALSE;
    // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
    lpDst = lpNewDIBBits;
    memset(lpDst, (BYTE) 255, lWidth * lHeight);

    for (j = 2; j < lHeight - 2; j++) {
      for (i = 2; i < lWidth - 2; i++) {
        bCondition1 = FALSE;
        bCondition2 = FALSE;
        bCondition3 = FALSE;
        bCondition4 = FALSE;

        //����ʹ��5��5�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ�������Χ�ļ��кͼ�������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = lpDIBBits + lWidth * j + i;
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = lpNewDIBBits + lWidth * j + i;
        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixel = (BYTE) * lpSrc;

        //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
        if (pixel != 255 && *lpSrc != 0) {
          continue;
        }
        //���Դͼ���е�ǰ��Ϊ��ɫ��������
        else if (pixel == 255) {
          continue;
        }

        //��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
        for (m = 0; m < 5; m++) {
          for (n = 0; n < 5; n++) {
            neighbour[ m ][ n ] = (255 - (BYTE) * (lpSrc +
                ((4 - m) - 2) * lWidth + n - 2)) / 255;
          }
        }

        //����ж�������
        //�ж�2<=NZ(P1)<=6
        nCount = neighbour[ 1 ][ 1 ] + neighbour[ 1 ][ 2 ] + neighbour[ 1 ][ 3 ] \
            + neighbour[ 2 ][ 1 ] + neighbour[ 2 ][ 3 ] + \
            + neighbour[ 3 ][ 1 ] + neighbour[ 3 ][ 2 ] + neighbour[ 3 ][ 3 ];

        if (nCount >= 2 && nCount <= 6) {
          bCondition1 = TRUE;
        }

        //�ж�Z0(P1)=1
        nCount = 0;

        if (neighbour[ 1 ][ 2 ] == 0 && neighbour[ 1 ][ 1 ] == 1) {
          nCount++;
        }

        if (neighbour[ 1 ][ 1 ] == 0 && neighbour[ 2 ][ 1 ] == 1) {
          nCount++;
        }

        if (neighbour[ 2 ][ 1 ] == 0 && neighbour[ 3 ][ 1 ] == 1) {
          nCount++;
        }

        if (neighbour[ 3 ][ 1 ] == 0 && neighbour[ 3 ][ 2 ] == 1) {
          nCount++;
        }

        if (neighbour[ 3 ][ 2 ] == 0 && neighbour[ 3 ][ 3 ] == 1) {
          nCount++;
        }

        if (neighbour[ 3 ][ 3 ] == 0 && neighbour[ 2 ][ 3 ] == 1) {
          nCount++;
        }

        if (neighbour[ 2 ][ 3 ] == 0 && neighbour[ 1 ][ 3 ] == 1) {
          nCount++;
        }

        if (neighbour[ 1 ][ 3 ] == 0 && neighbour[ 1 ][ 2 ] == 1) {
          nCount++;
        }

        if (nCount == 1) {
          bCondition2 = TRUE;
        }

        //�ж�P2*P4*P8=0 or Z0(p2)!=1
        if (neighbour[ 1 ][ 2 ] * neighbour[ 2 ][ 1 ] * neighbour[ 2 ][ 3 ] == 0) {
          bCondition3 = TRUE;
        }
        else {
          nCount = 0;

          if (neighbour[ 0 ][ 2 ] == 0 && neighbour[ 0 ][ 1 ] == 1) {
            nCount++;
          }

          if (neighbour[ 0 ][ 1 ] == 0 && neighbour[ 1 ][ 1 ] == 1) {
            nCount++;
          }

          if (neighbour[ 1 ][ 1 ] == 0 && neighbour[ 2 ][ 1 ] == 1) {
            nCount++;
          }

          if (neighbour[ 2 ][ 1 ] == 0 && neighbour[ 2 ][ 2 ] == 1) {
            nCount++;
          }

          if (neighbour[ 2 ][ 2 ] == 0 && neighbour[ 2 ][ 3 ] == 1) {
            nCount++;
          }

          if (neighbour[ 2 ][ 3 ] == 0 && neighbour[ 1 ][ 3 ] == 1) {
            nCount++;
          }

          if (neighbour[ 1 ][ 3 ] == 0 && neighbour[ 0 ][ 3 ] == 1) {
            nCount++;
          }

          if (neighbour[ 0 ][ 3 ] == 0 && neighbour[ 0 ][ 2 ] == 1) {
            nCount++;
          }

          if (nCount != 1) {
            bCondition3 = TRUE;
          }
        }

        //�ж�P2*P4*P6=0 or Z0(p4)!=1
        if (neighbour[ 1 ][ 2 ] * neighbour[ 2 ][ 1 ] * neighbour[ 3 ][ 2 ] == 0) {
          bCondition4 = TRUE;
        }
        else {
          nCount = 0;

          if (neighbour[ 1 ][ 1 ] == 0 && neighbour[ 1 ][ 0 ] == 1) {
            nCount++;
          }

          if (neighbour[ 1 ][ 0 ] == 0 && neighbour[ 2 ][ 0 ] == 1) {
            nCount++;
          }

          if (neighbour[ 2 ][ 0 ] == 0 && neighbour[ 3 ][ 0 ] == 1) {
            nCount++;
          }

          if (neighbour[ 3 ][ 0 ] == 0 && neighbour[ 3 ][ 1 ] == 1) {
            nCount++;
          }

          if (neighbour[ 3 ][ 1 ] == 0 && neighbour[ 3 ][ 2 ] == 1) {
            nCount++;
          }

          if (neighbour[ 3 ][ 2 ] == 0 && neighbour[ 2 ][ 2 ] == 1) {
            nCount++;
          }

          if (neighbour[ 2 ][ 2 ] == 0 && neighbour[ 1 ][ 2 ] == 1) {
            nCount++;
          }

          if (neighbour[ 1 ][ 2 ] == 0 && neighbour[ 1 ][ 1 ] == 1) {
            nCount++;
          }

          if (nCount != 1) {
            bCondition4 = TRUE;
          }
        }

        if (bCondition1 && bCondition2 && bCondition3 && bCondition4) {
          *lpDst = (BYTE) 255;
          bModified = TRUE;
        }
        else {
          *lpDst = (BYTE) 0;
        }
      }
    }

    // ���Ƹ�ʴ���ͼ��
    memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
  }

  // ���Ƹ�ʴ���ͼ��
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits);
  return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   ErosionLine()
 *
 * ����:
 *   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   int lWidth          - Դͼ���ȣ���������������4�ı�����
 *   int lHeight         - Դͼ��߶ȣ���������
 *   int type            - ��ʴ��ʽ��0��ʾˮƽ����1��ʾ��ֱ����
 *   int num             - �ṹԪ�صĸ�����ˮƽ����ʹ�ֱ���򣩡�
 *
 * ����ֵ:
 *   BOOL                - ��ʴ�ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ����и�ʴ���㡣�ṹԪ��Ϊˮƽ�����ֱ���򣬸������û�ѡ��
 * Ϊ3��5��7���ṹԭ��λ�������ġ�
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ����������㣬��ǿ��ת����
 ************************************************************************/
BOOL ErosionLine(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int type, int num)
{
  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;
  // ָ��Ҫ���������ָ��
  LPBYTE lpDst;
  // ָ����ͼ���ָ��
  LPBYTE lpImage;
  // ѭ������
  int i, j, k;
  //����ֵ
  BYTE pixeltemp;
  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // �ж��Ƿ��ڴ����ʧ��
  if (lpImage == NULL) {
    // �����ڴ�ʧ��
    return FALSE;
  }

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


  if (type == 0) {
    //ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
    for (j = 0; j < lHeight; j++) {
      for (i = (num - 1) / 2; i < lWidth - (num - 1) / 2; i++) {
        //����ʹ��1��num�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ�(num-1)/2������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = (LPBYTE) lpImage + lWidth * j + i;

        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixeltemp = (BYTE) * lpSrc;

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
        *lpDst = (BYTE) 0;

        //���Դͼ���е�ǰ���������������(num-1)/2���㲻�Ǻ�ɫ��
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
        for (k = 0; k < num; k++) {
          //��ȡ���ػҶ�ֵ
          pixeltemp = *(lpSrc + k - (num - 1) / 2);

          //����һ���׵�
          if (pixeltemp == 255) {
            //�õ�����Ϊ�׵�
            *lpDst = (BYTE) 255;
            break;
          }
        }

      }
    }

  }
  else if (type == 1) {
    //ʹ�ô�ֱ����ĽṹԪ�ؽ��и�ʴ
    for (j = (num - 1) / 2; j < lHeight - (num - 1) / 2; j++) {
      for (i = 0; i < lWidth; i++) {
        //����ʹ��1��num�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ�(num-1)/2������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = (LPBYTE) lpImage + lWidth * j + i;

        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixeltemp = (BYTE) * lpSrc;

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
        *lpDst = (BYTE) 0;

        //���Դͼ���е�ǰ���������������(num-1)/2���㲻�Ǻ�ɫ����
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
        for (k = 0; k < num; k++) {
          //��ȡ���ػҶ�ֵ
          pixeltemp = *(lpSrc + (k - (num - 1) / 2) * lWidth);

          //����һ���׵�
          if (pixeltemp == 255) {
            //�õ�����Ϊ�׵�
            *lpDst = (BYTE) 255;
            break;
          }
        }

      }
    }

  }

  // ���Ƹ�ʴ���ͼ��
  memcpy(lpDIBBits, lpImage, lWidth * lHeight);

  // �ͷ��ڴ�
  SAFEFREE(lpImage);

  // ����
  return TRUE;
}



/*************************************************************************
 *
* �������ƣ�
*   DilationLine()
 *
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth   - Դͼ���ȣ���������������4�ı�����
*   int lHeight  - Դͼ��߶ȣ���������
*   int  type            - ���ͷ�ʽ��0��ʾˮƽ����1��ʾ��ֱ����
*     int  num            - �ṹԪ�صĸ�����ˮƽ����ʹ�ֱ���򣩡�
 *
* ����ֵ:
*   BOOL              - ���ͳɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
* �ú������ڶ�ͼ������������㡣�ṹԪ��Ϊˮƽ�����ֱ���򣬸������û�ѡ��
* Ϊ3��5��7���ṹԭ��λ�������ġ�
* Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ����������㣬��ǿ��ת����
 ************************************************************************/
BOOL DilationLine(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int type , int num)
{
  LPBYTE lpSrc;  // ָ��Դͼ���ָ��
  LPBYTE lpDst;  // ָ��Ҫ���������ָ��
  LPBYTE lpImage; // ָ����ͼ���ָ��
  // ѭ������
  int i, j, k;
  //����ֵ
  BYTE pixeltemp;
  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // �ж��Ƿ��ڴ����ʧ��
  if (lpImage == NULL) {
    // �����ڴ�ʧ��
    return FALSE;
  }

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


  if (type == 0) {
    //ʹ��ˮƽ����ĽṹԪ�ؽ�������
    for (j = 0; j < lHeight; j++) {
      for (i = (num - 1) / 2; i < lWidth - (num - 1) / 2; i++) {
        //����ʹ��1��num�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ�(num-1)/2������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = (LPBYTE) lpImage + lWidth * j + i;

        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixeltemp = (BYTE) * lpSrc;

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
        *lpDst = (BYTE) 255;

        //���Դͼ���е�ǰ������������Ҹ�(num-1)/2ֻҪ�и����Ǻ�ɫ��
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
        for (k = 0; k < num; k++) {
          //��ȡ���ػҶ�ֵ
          pixeltemp = *(lpSrc + k - (num - 1) / 2);

          //����һ���ڵ�
          if (pixeltemp == 0) {
            //�õ�����Ϊ�ڵ�
            *lpDst = (BYTE) 0;
            break;
          }
        }

      }
    }

  }
  else if (type == 1) {
    //ʹ�ô�ֱ����ĽṹԪ�ؽ�������
    for (j = (num - 1) / 2; j < lHeight - (num - 1) / 2; j++) {
      for (i = 0; i < lWidth; i++) {
        //����ʹ��1��num�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ�(num-1)/2������

        // ָ��Դͼ������j�У���i�����ص�ָ��
        lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
        lpDst = (LPBYTE) lpImage + lWidth * j + i;

        //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE��
        pixeltemp = (BYTE) * lpSrc;

        //Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
        *lpDst = (BYTE) 255;

        //���Դͼ���е�ǰ������������¸�(num-1)/2ֻҪ�и����Ǻ�ɫ��
        //��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
        for (k = 0; k < num; k++) {
          //��ȡ���ػҶ�ֵ
          pixeltemp = *(lpSrc + (k - (num - 1) / 2) * lWidth);

          //����һ���׵�
          if (pixeltemp == 0) {
            //�õ�����Ϊ�׵�
            *lpDst = (BYTE) 0;
            break;
          }
        }

      }
    }

  }

  // �������ͺ��ͼ��
  memcpy(lpDIBBits, lpImage, lWidth * lHeight);

  // �ͷ��ڴ�
  SAFEFREE(lpImage);

  // ����
  return TRUE;
}



/*************************************************************************
 *
* �������ƣ�
*   ThiningLine()
 *
* ����:
*   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   int lWidth   - Դͼ���ȣ���������������4�ı�����
*   int lHeight  - Դͼ��߶ȣ���������
 *
* ����ֵ:
*   BOOL              - ���ͳɹ�����TRUE�����򷵻�FALSE��
 *
* ˵��:
* ����8����ϸ���ķ������ֱ����ˮƽ����ʹ�ֱ�����ϸ��
 ************************************************************************/

BOOL ThiningLine(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  //���������������
  static int erasetable[ 256 ] = {
    0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0
  };

  // ָ��Դͼ���ָ��
  LPBYTE lpSrc;
  // ָ��Ҫ���������ָ��
  LPBYTE lpDst;
  // ָ����ͼ���ָ��
  LPBYTE lpImage;
  // ѭ������
  int i, j, num;
  //����ֵ
  BYTE pixeltemp;

  //��ʱ����
  int nw, n, ne, w, e, sw, s, se;
  // ��ʱ�����ڴ棬�Ա�����ͼ��
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // �ж��Ƿ��ڴ����ʧ��
  if (lpImage == NULL) {
    // �����ڴ�ʧ��
    return FALSE;
  }

  lpDst = (LPBYTE) lpImage;
  //��Ŀ��ͼ��ֵ��ȫ�ף�
  //memset(lpDst, (BYTE)255, lWidth* lHeight);

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

  // ��ʼ��ͼ��Ϊԭʼͼ��
  memcpy(lpImage, lpDIBBits, lWidth * lHeight);

  //�Ƚ���ˮƽ�����ϸ��
  for (j = 1; j < lHeight - 1; j++) {
    for (i = 1; i < lWidth - 1; i++) {
      //Ϊ��ֹԽ�磬��ȣ��߶��������Ҹ�����һ��

      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

      // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
      lpDst = (LPBYTE) lpImage + lWidth * j + i;

      //�Ǻڵ��������
      if (*(lpSrc) == 0) {
        w = (BYTE) * (lpSrc - 1);      //���ڵ�
        e = (BYTE) * (lpSrc + 1);      //���ڵ�

        //������������ھ���������һ���ǰ׵�Ŵ���
        if ((w == 255) || (e == 255)) {
          nw = (BYTE) * (lpSrc + lWidth - 1);     //�����ڵ�
          n = (BYTE) * (lpSrc + lWidth);     //���ڵ�
          ne = (BYTE) * (lpSrc + lWidth + 1);     //�����ڵ�
          sw = (BYTE) * (lpSrc - lWidth - 1);     //�����ڵ�
          s = (BYTE) * (lpSrc - lWidth);     //���ڵ�
          se = (BYTE) * (lpSrc - lWidth + 1);     //�����ڵ�
          //��������
          num = nw / 255 + n / 255 * 2 + ne / 255 * 4 + w / 255 * 8 + e / 255 * 16 + sw / 255 * 32 + s / 255 * 64 + se / 255 * 128;

          if (erasetable[ num ] == 1) {   //���������ɾ��
            //��ԭͼ�������н��úڵ�ɾ��
            *(lpSrc) = (BYTE) 255;
            //���ͼ�иúڵ�Ҳɾ��
            *(lpDst) = (BYTE) 255;
          }
        }
      }
    }
  }

  //�ٽ��д�ֱ�����ϸ��

  for (i = 1; i < lWidth - 1; i++) {
    for (j = 1; j < lHeight - 1; j++) {
      // ָ��Դͼ������j�У���i�����ص�ָ��
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
      lpDst = (LPBYTE) lpImage + lWidth * j + i;

      //�Ǻڵ��������
      if (*(lpSrc) == 0) {
        n = (BYTE) * (lpSrc + lWidth);
        s = (BYTE) * (lpSrc - lWidth);

        //������������ھ���������һ���ǰ׵�Ŵ���
        if ((n == 255) || (s == 255)) {
          nw = (BYTE) * (lpSrc + lWidth - 1);
          ne = (BYTE) * (lpSrc + lWidth + 1);
          w = (BYTE) * (lpSrc - 1);
          e = (BYTE) * (lpSrc + 1);
          sw = (BYTE) * (lpSrc - lWidth - 1);
          se = (BYTE) * (lpSrc - lWidth + 1);
          //��������
          num = nw / 255 + n / 255 * 2 + ne / 255 * 4 + w / 255 * 8 + e / 255 * 16 + sw / 255 * 32 + s / 255 * 64 + se / 255 * 128;

          if (erasetable[ num ] == 1) {
            //���������ɾ��
            //��ԭͼ�������н��úڵ�ɾ��
            *(lpSrc) = (BYTE) 255;
            //���ͼ�иúڵ�Ҳɾ��
            *(lpDst) = (BYTE) 255;
          }
        }
      }
    }
  }

  // ����ϸ�����ͼ��
  memcpy(lpDIBBits, lpImage, lWidth * lHeight);
  // �ͷ��ڴ�
  SAFEFREE(lpImage);
  // ����
  return TRUE;
}

