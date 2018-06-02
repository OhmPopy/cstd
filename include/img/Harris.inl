
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"

// ����ģ�����㺯��
// im������ͼ��
// tp��ģ�����
int mbys(double* im, int imW, int imH, const double* tp, int tpW, int tpH, double* out)
{
  int i, j, m, n;
#define im(ROW,COL) im[imW*(ROW)+(COL)]
#define tp(ROW,COL) tp[tpW*(ROW)+(COL)]
#define out(ROW,COL) out[imW*(ROW)+(COL)]

  memset(out, 0, imW * imH * sizeof(double));

  for (i = 0; i < imH; i++) {
    for (j = 0; j < imW; j++) {
      double a = 0;

      //ȥ�������߽����
      if (i > (tpH / 2) && i < imH - (tpH / 2) && j > (tpW / 2) && j < imW - (tpW / 2)) {
        for (m = 0; m < tpH; m++) {
          for (n = 0; n < tpW; n++) {
            a += im(i + m - (tpH / 2), j + n - (tpW / 2)) * tp(m, n);
          }
        }
      }

      out(i, j) = a;
    }
  }

  return 0;
}

// int cxDIB          ͼ����
// int cyDIB          ͼ��߶�
// long lLineBytes    ����Ҷ�ͼ��ÿ�е��ֽ���
// gausswidth =5      ��ά��˹���ڿ��
// sigma=0.8          ��˹�����ķ���
// size =5            �Ǽ���ֵ���Ƶ�������
// thresh=5000        ����ȷ���ǵ��������ֵ

int ImgHarris(int cyDIB, int cxDIB, const unsigned char* lpDIBBits, int lLineBytes, unsigned char* corner,
    int gausswidth, float sigma, int size, int thresh)
{
  int i, j, m, n;
  const unsigned char* lpSrc; //һ��ָ��Դ��Ŀ�����ص��ƶ�ָ��

  //����I��Ix��Ix2��Iy��Iy2��Ixy��cim��mx��corner����
  double* I = MALLOC(double, cxDIB * cyDIB);
  double* Ix = MALLOC(double, cxDIB * cyDIB);
  double* Ix2 = MALLOC(double, cxDIB * cyDIB);
  double* Iy = MALLOC(double, cxDIB * cyDIB);
  double* Iy2 = MALLOC(double, cxDIB * cyDIB);
  double* Ixy = MALLOC(double, cxDIB * cyDIB);
  double* cim = MALLOC(double, cxDIB * cyDIB);
  double* mx = MALLOC(double, cxDIB * cyDIB);

  // corner=MALLOC(unsigned char, cxDIB*cyDIB);
  // memset(corner, 0, cxDIB*cyDIB*sizeof(unsigned char));

  //������Է������Ԫ��
#define I(ROW,COL)      I[cxDIB*(ROW)+(COL)]
#define Ix(ROW,COL)     Ix[cxDIB*(ROW)+(COL)]
#define Ix2(ROW,COL)    Ix2[cxDIB*(ROW)+(COL)]
#define Iy(ROW,COL)     Iy[cxDIB*(ROW)+(COL)]
#define Iy2(ROW,COL)    Iy2[cxDIB*(ROW)+(COL)]
#define Ixy(ROW,COL)    Ixy[cxDIB*(ROW)+(COL)]
#define cim(ROW,COL)    cim[cxDIB*(ROW)+(COL)]
#define mx(ROW,COL)     mx[cxDIB*(ROW)+(COL)]
#define corner(ROW,COL) corner[cxDIB*(ROW)+(COL)]

  //��ͼ��Ҷ�ֵ���Ƶ�I�У��ⲽ����Ҫ������Ϊʲô��
  for (i = 0; i < cyDIB; i++) {
    for (j = 0; j < cxDIB; j++) {
      lpSrc = (unsigned char*) lpDIBBits + lLineBytes * (cyDIB - 1 - i) + j;
      //��256���Ҷ�ͼ��ת��Ϊdouble��
      I(i, j) = (double)(*lpSrc);
    }
  }

  //--------------------------------------------------------------------------
  //                     ��һ�������ò�����Ӷ�ͼ������˲�
  //--------------------------------------------------------------------------
  {
    //����ˮƽ���������Ӳ���Ix
    const double dx[ 9 ] = {
      -1, 0, 1, -1, 0, 1, -1, 0, 1
    };
    const double dy[ 9 ] = {
      -1, -1, -1, 0, 0, 0, 1, 1, 1
    };
    mbys(I, cxDIB, cyDIB, dx, 3, 3, Ix);

    //���崹ֱ���������Ӳ���Iy
    mbys(I, cxDIB, cyDIB, dy, 3, 3, Iy);

    //���м���Ixд�뵽�ı��ļ��Ա��������
    /*FILE *fp;
    fp=fopen("Ix.txt","w+");
    for(i = 0; i < cyDIB; i++) {
      for(j = 0; j < cxDIB; j++)
        fprintf(fp,"%f ",Ix(i,j));
      fprintf(fp,"\n");
    }
    */

    //����Ix2��Iy2��Ixy
    for (i = 0; i < cyDIB; i++) {
      for (j = 0; j < cxDIB; j++) {
        Ix2(i, j) = Ix(i, j) * Ix(i, j);
        Iy2(i, j) = Iy(i, j) * Iy(i, j);
        Ixy(i, j) = Ix(i, j) * Iy(i, j);
      }
    }

  }

  //--------------------------------------------------------------------------
  //                  �ڶ�������Ix2/Iy2/Ixy���и�˹ƽ������ȥ������
  //--------------------------------------------------------------------------
  {
    // ������ʹ��5��5�ĸ�˹ģ��
    // ����ģ�����
    double total = 0;
    double* g = MALLOC(double, gausswidth * gausswidth);

    for (i = 0; i < gausswidth; i++) {
      for (j = 0; j < gausswidth; j++) {
        g[ i * gausswidth + j ] = exp(-((i - (gausswidth / 2)) * (i - (gausswidth / 2)) + (j - (gausswidth / 2)) * (j - (gausswidth / 2))) / (2 * sigma));
      }
    }

    //��һ����ʹģ�����֮��Ϊ1����ʵ�˲�����ʡ�ԣ�
    for (i = 0; i < gausswidth * gausswidth; i++) {
      total += g[ i ];
    }

    for (i = 0; i < gausswidth; i++) {
      for (j = 0; j < gausswidth; j++) {
        g[ i * gausswidth + j ] /= total;
      }
    }

    // ���и�˹ƽ��
    mbys(Ix2, cxDIB, cyDIB, g, gausswidth, gausswidth, Ix2);
    mbys(Iy2, cxDIB, cyDIB, g, gausswidth, gausswidth, Iy2);
    mbys(Ixy, cxDIB, cyDIB, g, gausswidth, gausswidth, Ixy);
    SAFEFREE(g);
  }

  //--------------------------------------------------------------------------
  //                        ������������ǵ���
  //--------------------------------------------------------------------------
  //����cim����cornerness of image�����ǰ����������ǵ�����
  for (i = 0; i < cyDIB; i++) {
    for (j = 0; j < cxDIB; j++) {
      //ע�⣺Ҫ�ڷ�ĸ�м���һ����С���Է�ֹ����Ϊ�����
      cim(i, j) = (Ix2(i, j) * Iy2(i, j) - Ixy(i, j) * Ixy(i, j)) / (Ix2(i, j) + Iy2(i, j) + 0.000001);
    }
  }

  //--------------------------------------------------------------------------
  //                 ���Ĳ������оֲ��Ǽ���ֵ�����Ի�����սǵ�
  //--------------------------------------------------------------------------
  //ע����оֲ�����ֵ���Ƶ�˼·
  //const double size=7;
  //��ÿ������������������ֵ�˲��������õ��ֵ��Ϊ�����������Ǹ�ֵ������ֵ�˲��е����ƣ�
  for (i = 0; i < cyDIB; i++) {
    for (j = 0; j < cxDIB; j++) {
      double maxpix = -1000000;

      if (i > (size / 2) && i < cyDIB - (size / 2) && j > (size / 2) && j < cxDIB - (size / 2)) {
        for (m = 0; m < size; m++) {
          for (n = 0; n < size; n++) {
            if (cim(i + m - (size / 2), j + n - (size / 2)) > maxpix) {
              maxpix = cim(i + m - (size / 2), j + n - (size / 2));
            }
          }
        }
      }

      mx(i, j) = maxpix > 0 ? maxpix : 0;
    }
  }

  // ����ȷ���ǵ�
  // const double thresh=4500;
  for (i = 0; i < cyDIB; i++) {
    for (j = 0; j < cxDIB; j++) {
      if (cim(i, j) == mx(i, j))         // ����ȡ�þֲ�����ֵ
        if (mx(i, j) > thresh) {      // Ȼ����������ֵ
          corner(i, j) = 1;  // �������������������ǽǵ㣡
        }
    }
  }

  SAFEFREE(I);
  SAFEFREE(Ix);
  SAFEFREE(Ix2);
  SAFEFREE(Iy);
  SAFEFREE(Iy2);
  SAFEFREE(Ixy);
  SAFEFREE(cim);
  SAFEFREE(mx);
  return 0;
}
