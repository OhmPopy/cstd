//��ά�ռ��еķ��β�ֵ�㷨
//�� ��ƽ���ϻ�������������n��n�������A��B��C��D��4���ǵ����ɫ��
//�� ����4���ǵ����ɫֵ��ƽ��ֵ�������е�M����ɫ��
//�� ����A��B��M���������һ����㣨��ɫֵΪ0����ƽ�����õ����е�E����ɫ������B��C��M���������һ����㣨��ɫֵΪ0����ƽ�����õ����е�F����ɫ������C��D��M���������һ����㣨��ɫֵΪ0����ƽ�����õ����е�G����ɫ������A��D��M���������һ����㣨��ɫֵΪ0����ƽ�����õ����е�H����ɫ��
//�� ����С������EBFM��4�ǵ���ɫ��ƽ��ֵ�����С�������е��Լ�С�����εı��е����ɫ��Ȼ�������������������С������MFCG���е㼰���е����ɫ���Դ����ƣ�����С������HMGD��AEMH���е�ͱ��е����ɫ�� �ݹ鲽��
//��ʹ�����������񲻶�ϸ����ֱ���ﵽԤ�ڵĵݹ���� -Two-dimensional space fractal interpolation algorithm

#include <time.h>
#include <stdlib.h>
#include "macro.h"
#include "fractal.h"

#define MAX_LOADSTRING 100

double random()
{
  return rand() * 1. / RAND_MAX;
}
typedef color_t Color;
//��������������λ�õ����λ����ɫֵ
double Displace(double height, double width, double num)
{
  return ((double) random() - 0.5f) * num / (width + height) * 3;
}

//����һ��������ɫֵ����ɫc
Color ComputeColor(double c)
{
  double Red = 0;
  double Green = 0;
  double Blue = 0;

  if (c < 0.5f) {
    Red = c * 2;
  }
  else {
    Red = (1.0f - c) * 2;
  }

  if (c >= 0.3f && c < 0.8f) {
    Green = (c - 0.3f) * 2;
  }
  else if (c < 0.3f) {
    Green = (0.3f - c) * 2;
  }
  else {
    Green = (1.3f - c) * 2;
  }

  if (c >= 0.5f) {
    Blue = (c - 0.5f) * 2;
  }
  else {
    Blue = (0.5f - c) * 2;
  }

  return _RGB(255 * Red, 255 * Green, 255 * Blue);
}


//����һ��ִ���е�λ���㷨�ĵݹ麯���������Լ���ΪС�����ص��������Ƭ
void Midpoint(IDC* pDC, double x, double y,
    double width, double height,
    double r1, double r2, double r3, double r4)
{
  double M1, M2, M3, M4, Middle;
  double newWidth = width / 2;
  double newHeight = height / 2;

  if (width > 2. || height > 2.) {
    //����е�λ��
    Middle = ((r1 + r2 + r3 + r4) / 4. + Displace(height, width, newWidth + newHeight));
    M1 = (r1 + r2) / 2;   //�������������ǵ��м��ƽ��ֵ
    M2 = (r2 + r3) / 2;
    M3 = (r3 + r4) / 2;
    M4 = (r4 + r1) / 2;

    //��֤�е㲻���ڡ����λ�ơ�ȷ���ķ�Χ֮��
    if (Middle < 0) {
      Middle = 0;
    }
    else if (Middle > 1.0f) {
      Middle = 1.0f;
    }

    //�ٴβ���4���µ������
    Midpoint(pDC, x, y, newWidth, newHeight, r1, M1, Middle, M4);
    Midpoint(pDC, x + newWidth, y, newWidth, newHeight, M1, r2, M2, Middle);
    Midpoint(pDC, x + newWidth, y + newHeight, newWidth, newHeight, Middle, M2, r3, M3);
    Midpoint(pDC, x, y + newHeight, newWidth, newHeight, M4, Middle, M3, r4);
  }
  else {
    //���ǻ���״����һЩ������ƬС�����ص�ĳߴ�
    //������Ƭ��4���ǵ���ƽ����������������ĵ�
    double c = (r1 + r2 + r3 + r4) / 4.;

    Color col = ComputeColor(c);
    //Javaû�л�������ĺ�����������1��1�ľ��������Ƶ�
    SetPixelV(pDC, (int) x, (int) y, col) ;
  }
}

//����һ�������֡������������ڵݹ麯��֮ǰ����һ����ʼ����
void DrawCloud(IDC* pDC)
{
  double r1, r2, r3, r4;

  //�����ʼ����4���ǵ�������ɫֵ
  //�⽫����������applet�е�4���ǵ����ɫ
  r1 = (double) random();
  r2 = (double) random();
  r3 = (double) random();
  r4 = (double) random();

  Midpoint(pDC, 0, 0, pDC->w , pDC->h , r1, r2, r3, r4);
}
