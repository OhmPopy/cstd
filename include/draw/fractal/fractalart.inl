
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "macro.h"
#include "xtypes.h"
#include "ColorMover.h"
//#pragma warning( disable: 4244 )


#define Int(x) (int)(x)
#define Rnd    (rand()*1./RAND_MAX)
#define SQR(_X)  ((_X)*(_X))

//"Mandlbrot��"
//"Zn+1=Zn^" & CStr(N)
//"Julia��"
//"Zn+1 = Zn^" & CStr(N - 100) & " + C"
void MandlbrotJuliaDraw(int wid, int hgh, unsigned char* dst, int step, int cn,
    double x0, double y0, double x1, double y1,
    int red, int grn, int blu, int nn, double crr, double cii,
    int jd, int t_max, int c_modal)
{
  //��ͼ����Draw
  //����˵��:
  //wid ͼƬ���ȣ�hgh ͼƬ���
  //��x0��y0��ͼƬ���Ͻ���ʼ��
  //��x1��y1��ͼƬ���½���ֹ��
  //red,grn,blu Ϊ��ͼ��ɫ
  //nn ΪN,crr ΪCʵ��,cii ΪC�鲿
  //jd Ϊ��ͼ����,t_max Ϊ������������
  //pic_num ָʾҪ�洢������λ��



  //-------------------------------------------------------------------------
  //��������
  //�������Ʊ���
  long Timer;                           //�����������
  int h, i, j, k;  //ѭ�����Ʊ���
  //Dim MyTime0, MyTime1                        //����ϵͳʱ��


  //��ɫ���Ʊ���
  color_t Color;                           //���������ɫֵ
  int di, Kn2;             //��ɫ���Ʋ���

  //��ֵ����
  double a ;                             //����ʵ��ֵ
  double p ;                             //�����鲿ֵ
  double a0;                             //����ʵ����ֵ
  double p0;                             //�����鲿��ֵ
  double aa;                             //����ʵ��ֵƽ��
  double pp;                             //�����鲿ֵƽ��

  //��ֵ��ʱ����
  double ta;                             //����ʵ����ʱ����
  double tp;                             //�����鲿��ʱ����

  double Max;                           //����ģ��������

  //��ͼƬ̫Сʱֹͣ�Ŵ�
  if (x1 == x0 || y1 == y0) {
    printf("   ѡ��̫С��������ѡ�� ��   ");
    return ;
  }

  //-------------------------------------------------------------------------
  //������ʼ��

  Kn2 = 4.5;
  Max = crr * crr + cii * cii;

  if (Max < 4) {
    Max = 4;
  }

  i = wid / 2;

  //-------------------------------------------------------------------------
  //��ͼѭ����ʼ
  //���ѭ���غ�����仯
  for (k = 0; k < wid; k += jd) {

    //�������м���������չ��Ч��
    i = i - ((k / jd % 2) * 2 - 1) * k;

    //�ڲ�ѭ����������仯
    for (j = 0; j < hgh; j += jd) {

      //��ʼ��������ʵ�����鲿
      a0 = (x1 - x0) * i / wid + x0;
      p0 = (y1 - y0) * j / hgh + y0;
      a = a0;
      p = p0;

      //-------------------------------------------------------------------------
      //������ʼ
      //���ݹ�ʽ�еĴη�ֵNѡ������㷨
      switch (nn) {
        //M������
      case 2:
        for (Timer = 1 ; Timer < t_max; ++Timer) {
          aa = a * a;
          pp = p * p;

          if ((aa + pp) > 4) {
            di = aa + pp;
            p = 2 * a * p + p0;
            a = aa - pp + a0;
            aa = a * a;
            pp = p * p;
            di = (di - 4) / (aa + pp - di);
            break;
          }

          p = 2 * a * p + p0;
          a = aa - pp + a0;
        } // Next Timer

        break;

      case 3:
        for (Timer = 1 ; Timer < t_max; ++Timer) {
          aa = a * a;
          pp = p * p;

          if (aa + pp > 4) {
            break;
          }

          a = (aa - 3 * pp) * a + a0;
          p = (3 * aa - pp) * p + p0;
        } // Next Timer

        break;

      case 4:
        for (Timer = 1 ; Timer < t_max; ++Timer) {
          aa = a * a;
          pp = p * p;

          if (aa + pp > 4) {
            break;
          }

          ta = SQR(aa - pp) - aa * pp * 4 + a0;
          tp = a * p * (aa - pp) * 4 + p0;
          a = ta;
          p = tp;

        } // Next Timer

        break;

      case 5:
        for (Timer = 1 ; Timer < t_max; ++Timer) {
          aa = a * a;
          pp = p * p;

          if (aa + pp > 4) {
            break;
          }

          ta = a * (SQR(aa - pp * 5) - pp * pp * 20) + a0;
          tp = p * (SQR(aa * 5 - pp) - aa * aa * 20) + p0;
          a = ta;
          p = tp;
        } // Next Timer

        break;

      case 6:
        for (Timer = 1 ; Timer < t_max; ++Timer) {
          aa = a * a;
          pp = p * p;

          if (aa + pp > 4) {
            break;
          }

          ta = aa * aa * aa - pp * pp * pp - aa * pp * (aa - pp) * 15 + a0;
          tp = a * p * (SQR(aa - pp) * 6 - aa * pp * 8) + p0;
          a = ta;
          p = tp;
        } // Next Timer

        break;

        //-------------------------------------------------------------------------

        //J������
      case 102:
        for (Timer = 1 ; Timer < t_max; ++Timer) {
          aa = a * a;
          pp = p * p;

          if (aa + pp > Max) {
            break;
          }

          p = 2 * a * p + cii;
          a = aa - pp + crr;
        } // Next Timer

        break;

      case 103:
        for (Timer = 1 ; Timer < t_max; ++Timer) {
          aa = a * a;
          pp = p * p;

          if (aa + pp > Max) {
            break;
          }

          a = (aa - 3 * pp) * a + crr;
          p = (3 * aa - pp) * p + cii;
        } // Next Timer

        break;

      case 104:
        for (Timer = 1 ; Timer < t_max; ++Timer) {
          aa = a * a;
          pp = p * p;

          if (aa + pp > Max) {
            break;
          }

          ta = SQR(aa - pp) - aa * pp * 4 + crr;
          tp = a * p * (aa - pp) * 4 + cii;
          a = ta;
          p = tp;

        } // Next Timer

        break;

      case 105:
        for (Timer = 1 ; Timer < t_max; ++Timer) {
          aa = a * a;
          pp = p * p;

          if (aa + pp > Max) {
            break;
          }

          ta = a * (SQR(aa - pp * 5) - pp * pp * 20) + crr;
          tp = p * (SQR(aa * 5 - pp) - aa * aa * 20) + cii;
          a = ta;
          p = tp;
        } // Next Timer

        break;

      case 106:
        for (Timer = 1 ; Timer < t_max; ++Timer) {
          aa = a * a;
          pp = p * p;

          if (aa + pp > Max) {
            break;
          }

          ta = aa * aa * aa - pp * pp * pp - aa * pp * (aa - pp) * 15 + crr;
          tp = a * p * (SQR(aa - pp) * 6 - aa * pp * 8) + cii;
          a = ta;
          p = tp;
        } // Next Timer

        break;
      }

      //������ɫ������ɫ
      switch (c_modal) {
      case 1:

        //��ɫ����Ϊ˫ɫ
        if (Timer > 10) {
          Color = _RGB(red, grn, blu);
        }
        else {
          Color = _RGB(255 - red, 255 - grn, 255 - blu);
        }

        break;

      case 2:
        //��ɫ����Ϊ����
        Color = _RGB(((Timer % 13) / (t_max % 11 + 11) * 256 + red) % 256,
            ((Timer % 13) / (t_max % 11 + 11) * 256 + grn) % 255,
            ((Timer % 13) / (t_max % 11 + 13) * 255 + blu) % 255);
        break;

      case 3:
        //��ɫ����Ϊ����
        Color = _RGB((256 - abs(abs(red - (2 * grn - 256) * (t_max - Timer - di) * (int) pow(2, Kn2) / t_max) % 512 - 256)),
            (256 - abs(abs(grn - (2 * blu - 256) * (t_max - Timer - di) * (int) pow(2, Kn2) / t_max) % 512 - 256)),
            (256 - abs(abs(blu - (2 * red - 256) * (t_max - Timer - di) * (int) pow(2, Kn2) / t_max) % 512 - 256)));
        break;

      default:
        Color = _RGB(256 - abs(abs(red - (2 * red - 256) * (t_max - Timer) * (int) pow(2, Kn2) / t_max) % 512 - 256),
            256 - abs(abs(grn - (2 * grn - 256) * (t_max - Timer) * (int) pow(2, Kn2) / t_max) % 512 - 256),
            256 - abs(abs(blu - (2 * blu - 256) * (t_max - Timer) * (int) pow(2, Kn2) / t_max) % 512 - 256));
        break;

      }

      if (jd == 1) {

        if (Timer == t_max + 1) {
          if (c_modal == 3) {
            //Frm_Imag.Pic.PSet (i, j), Color
            //Pic(pic_num, i, j) = Color;
            PUTPIX3C(dst + i * step + j * cn, Color);
          }
          else {
            //Frm_Imag.Pic.PSet (i, j), 0xFF;
            //Pic(pic_num, i, j) = 0xFF;
            //PUTPIX1C(dst+i*step+j*cn, Color);
            PUTPIX(dst + i * step + j * cn, 0xFF);

          }
        }
        else {
          //Frm_Imag.Pic.PSet (i, j), Color
          //Pic(pic_num, i, j) = Color
          PUTPIX3C(dst + i * step + j * cn, Color);
        }
      }
      else {
        if (Timer = t_max + 1) {
          if (c_modal = 3) {
            //Frm_Imag.Pic.Line (i, j)-(i + jd - 1, j + jd - 1), Color, BF
            for (h = 0 ; h < jd - 1; ++h) {
              //Pic(pic_num, i + h, j + h) = Color
              PUTPIX3C(dst + (i + h) * step + (j + h) * cn, Color);
            } // Next h
          }
          else {
            //Frm_Imag.Pic.Line (i, j)-(i + jd - 1, j + jd - 1), 0xFF;, BF
            for (h = 0 ; h < jd - 1; ++h) {
              //Pic(pic_num, i + h, j + h) = 0xFF;
              PUTPIX(dst + (i + h) * step + (j + h) * cn, 0xFF);
            } // Next h
          }
        }
        else {
          //Frm_Imag.Pic.Line (i, j)-(i + jd - 1, j + jd - 1), Color, BF
          for (h = 0 ; h < jd - 1; ++h) {
            //Pic(pic_num, i + h, j + h) = 0xFF;
            PUTPIX(dst + (i + h) * step + (j + h) * cn, 0xFF);
          } // Next h
        }
      }


    } // Next j
  } // Next k
}



void MandlbrotJuliaDraw2(int wid, int hgh, unsigned char* dst, int step, int cn, int Index)
{
  double t_M_J = 1 ;
  double t_Nx0 = -2 ;
  double t_Ny0 = 2 ;
  double t_Nx1 = 2 ;
  double t_Ny1 = -2 ;
  double t_JingDu = 1;
  double t_N = 102;
  double t_Timer_Max = 800 ;
  double t_Color_Modal = 3 ;
  double t_Cr_Ci[][ 2 ] = {
    -0.74543, 0.11301,
    0, 1,
    -1, 0, 0.11,
    0.66, -0.10281,
    0.95723, -0.12256,
    0.7486, -1.16,
    -0.25, -1.25,
    -0.01, -0.2,
    0.75, -0.48176,
    -0.53163, 0,
    -1, 0.25,
    0.52, 0.32,
    0.04, 0.5,
    0.55, 0, 0.66
  };

  int Br, Bg, Bb;
  double t_Cr, t_Ci;
  Br = rand() % 255;
  Bg = rand() % 255;
  Bb = rand() % 255;
  Index %= sizeof(t_Cr_Ci) / (2 * sizeof(double));
  t_Cr = t_Cr_Ci[ Index ][ 0 ];
  t_Ci = t_Cr_Ci[ Index ][ 1 ];
  MandlbrotJuliaDraw(wid, hgh, dst, step, cn,
      t_Nx0, t_Ny0, t_Nx1, t_Ny1,
      Br, Bg, Bb,
      t_N, t_Cr, t_Ci, t_JingDu,
      t_Timer_Max, t_Color_Modal);
}
