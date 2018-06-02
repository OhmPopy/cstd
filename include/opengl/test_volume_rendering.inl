// �����RayCasting������ͶӰ�㷨��
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cstd.h"
#define EPSILON 0.000001
//��������������
//********************************************************************//
//x,y,z:���������½�����
//side:������߳�
//density:�������Ӧ�ı���ֵ
//Data:�������
//Dim:�����ݴ�С
//********************************************************************//
void GenCube(int x, int y, int z, int side, int density, uchar* Data, int* Dim)
{
  int max_x = x + side, max_y = y + side, max_z = z + side;
  int Dimxy = Dim[0] * Dim[1];
  for (int k = z; k < max_z; k++) {
    for (int j = y; j < max_y; j++) {
      for (int i = x; i < max_x; i++) {
        Data[k * Dimxy + j * Dim[0] + i] = density;
      }
    }
  }
}
//������������
//********************************************************************//
//x,y,z:��������
//radius:��뾶
//density:�����Ӧ�ı���ֵ
//Data:�������
//Dim:�����ݴ�С
//********************************************************************//
void GenSphere(int x, int y, int z, int radius, int density, uchar* Data, int* Dim)
{
  int radius2 = radius * radius;
  int Dimxy = Dim[0] * Dim[1];
  for (int k = 0; k < Dim[2]; k++) {
    for (int j = 0; j < Dim[1]; j++) {
      for (int i = 0; i < Dim[0]; i++) {
        if ((i - x) * (i - x) + (j - y) * (j - y) + (k - z) * (k - z) <= radius2) {
          Data[k * Dimxy + j * Dim[0] + i] = density;
        }
      }
    }
  }
}
//�����Բ�ֵ
//********************************************************************//
//rgba:��ֵ���
//pos:����������
//CData:�����������
//Dim:�����ݴ�С
//********************************************************************//
void TrInterpolation(float* rgba, float* pos, uchar* Data, uchar* pal, int* Dim) {
  int x0, y0, z0, x1, y1, z1;
  float fx, fy, fz;
  float v0, v1, v2, v3, v4, v5, v6;
  int Slicesize = Dim[0] * Dim[1];
  int Stepsize = Dim[0];
  x0 = (int)pos[0]; //��������
  y0 = (int)pos[1];
  z0 = (int)pos[2];
  int i0 = z0 * Slicesize + y0 * Stepsize + x0;
  pal += Data[i0]*4;
  rgba[0] = pal[0]/255.;
  rgba[1] = pal[1]/255.;
  rgba[2] = pal[2]/255.;
  rgba[3] = pal[3]/255.;
  return ;
}
void TrInterpolation1(float* rgba, float* pos, uchar* Data, uchar* pal, int* Dim)
{
  int x0, y0, z0, x1, y1, z1;
  float fx, fy, fz;
  float v0, v1, v2, v3, v4, v5, v6;
  int Slicesize = Dim[0] * Dim[1];
  int Stepsize = Dim[0];
  x0 = (int)pos[0]; //��������
  y0 = (int)pos[1];
  z0 = (int)pos[2];
  fx = pos[0] - x0; //С������
  fy = pos[1] - y0;
  fz = pos[2] - z0;
  x1 = x0 + 1;
  y1 = y0 + 1;
  z1 = z0 + 1;
  if (x1 >= Dim[0]) {
    x1 = Dim[0] - 1; //��ֹԽ��
  }
  if (y1 >= Dim[1]) {
    y1 = Dim[1] - 1;
  }
  if (z1 >= Dim[2]) {
    z1 = Dim[2] - 1;
  }
  int i0 = z0 * Slicesize + y0 * Stepsize;
  int i1 = z0 * Slicesize + y1 * Stepsize;
  int i2 = z1 * Slicesize + y0 * Stepsize;
  int i3 = z1 * Slicesize + y1 * Stepsize;
  for (int i = 0; i < 4; i++) {
    //�����㴦��ֵ���ڽ���8�����ֵ���
    v0 = pal[Data[i0 + x0]*4 + i] * (1 - fx) + pal[Data[i0 + x1]*4 + i] * fx;
    v1 = pal[Data[i1 + x0]*4 + i] * (1 - fx) + pal[Data[i1 + x1]*4 + i] * fx;
    v2 = pal[Data[i2 + x0]*4 + i] * (1 - fx) + pal[Data[i2 + x1]*4 + i] * fx;
    v3 = pal[Data[i3 + x0]*4 + i] * (1 - fx) + pal[Data[i3 + x1]*4 + i] * fx;
    v4 = v0 * (1 - fy) + v1 * fy;
    v5 = v2 * (1 - fy) + v3 * fy;
    v6 = v4 * (1 - fz) + v5 * fz;
    v6/=255;
    if (v6 > 1) {
      v6 = 1; //��ֹԽ��
    }
    rgba[i] = v6;
  }
}
//�жϵ��Ƿ��ڰ�Χ����
//********************************************************************//
//point:������
//Dim:��Χ�����Ͻ����꣨���½�����Ϊ��0,0,0����
//********************************************************************//
bool CheckinBox(float* point, int* Dim)
{
  if (point[0] < 0 || point[0] >= Dim[0] || point[1] < 0 || point[1] >= Dim[1] || point[2] < 0 || point[2] >= Dim[2]) {
    return false;
  }
  else {
    return true;
  }
}
//�����������˻�
//********************************************************************//
//c=a*b
//c:�������
//a:�������
//b:��������
//********************************************************************//
void MatrixmulVec(float* c, float* a, float* b)
{
  float x, y, z;
  x = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
  y = a[3] * b[0] + a[4] * b[1] + a[5] * b[2];
  z = a[6] * b[0] + a[7] * b[1] + a[8] * b[2];
  c[0] = x;
  c[1] = y;
  c[2] = z;
}
//�������
//********************************************************************//
//c=a x b
//c:�������
//a:��������
//b:��������
//********************************************************************//
void CrossProd(float* c, float* a, float* b)
{
  float x, y, z;
  x = a[1] * b[2] - b[1] * a[2];
  y = a[2] * b[0] - b[2] * a[0];
  z = a[0] * b[1] - b[0] * a[1];
  c[0] = x;
  c[1] = y;
  c[2] = z;
}
//������һ��
//********************************************************************//
//norm:��һ�����
//a:��������
//********************************************************************//
void Normalize(float* norm, float* a)
{
  float len = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
  norm[0] = a[0] / len;
  norm[1] = a[1] / len;
  norm[2] = a[2] / len;
}
//��ȡ��ͼ��ռ䵽����ռ�任����ת����
//********************************************************************//
//����������OpenGL�е�gluLookAt����
//�ο���http://blog.csdn.net/popy007/article/details/5120158
//R:��ת����
//eye:�ӵ�λ��
//center:����ο���λ��
//up:������ϵķ���
//********************************************************************//
void RotationMatrix(float* R, float* eye, float* center, float* up)
{
  float XX[3], YY[3], ZZ[3]; //ͼ��ռ�Ļ�����
  ZZ[0] = eye[0] - center[0];
  ZZ[1] = eye[1] - center[1];
  ZZ[2] = eye[2] - center[2];
  CrossProd(XX, up, ZZ);
  CrossProd(YY, ZZ, XX);
  Normalize(XX, XX);
  Normalize(YY, YY);
  Normalize(ZZ, ZZ);
  //��ͼ��ռ������������ת����
  R[0] = XX[0];
  R[1] = YY[0];
  R[2] = ZZ[0];
  R[3] = XX[1];
  R[4] = YY[1];
  R[5] = ZZ[1];
  R[6] = XX[2];
  R[7] = YY[2];
  R[8] = ZZ[2];
}
//�ж�Ͷ��������Χ���Ƿ��ཻ�����ཻ���󿿽��ӵ㴦�Ľ������꣩
//********************************************************************//
//˼·������Χ��6����������չ�����ֳ�3�飬��ƽ����XOY,YOZ,ZOXƽ��ĸ���2����
//�������6��ƽ��Ľ��㣬��ÿ���2��������ѡ�������ӵ�Ͻ��ߣ������õ�3����
//ѡ���㣻����3����ѡ������ѡ�������ӵ���Զ���Ǹ�������ж�������Ƿ����ڰ�
//Χ���ڣ����ڣ���Ϊ�������Χ�еĿ����ӵ㴦�Ľ��㡣
//stratpos:�����ӵ㴦�Ľ�������
//pos:�����������
//dir:���߷�������
//Dim:��Χ�����Ͻ����꣨���½�����Ϊ��0,0,0����
//********************************************************************//
bool Intersection(float* startpos, float* pos, float* dir, int* Dim)
{
  float nearscale = -1000000;
  float scale1, scale2;
  //�������Χ��ƽ����YOZ��2��ƽ�潻��
  if ((dir[0] <= -EPSILON) || (dir[0] >= EPSILON)) {
    scale1 = (0 - pos[0]) / dir[0];
    scale2 = (Dim[0] - 1 - pos[0]) / dir[0];
    //ѡ�������ӵ�Ľ��㣬���뵱ǰ��ѡ��Ƚϣ�������Զ��
    if (scale1 < scale2) {
      if (scale1 > nearscale) {
        nearscale = scale1;
      }
    }
    else {
      if (scale2 > nearscale) {
        nearscale = scale2;
      }
    }
  }
  //�������Χ��ƽ����ZOX��2��ƽ�潻��
  if ((dir[1] <= -EPSILON) || (dir[1] >= EPSILON)) {
    scale1 = (0 - pos[1]) / dir[1];
    scale2 = (Dim[1] - 1 - pos[1]) / dir[1];
    //ѡ�������ӵ�Ľ��㣬���뵱ǰ��ѡ��Ƚϣ�������Զ��
    if (scale1 < scale2) {
      if (scale1 > nearscale) {
        nearscale = scale1;
      }
    }
    else {
      if (scale2 > nearscale) {
        nearscale = scale2;
      }
    }
  }
  //�������Χ��ƽ����XOY��2��ƽ�潻��
  if ((dir[2] <= -EPSILON) || (dir[2] >= EPSILON)) {
    scale1 = (0 - pos[2]) / dir[2];
    scale2 = (Dim[2] - 1 - pos[2]) / dir[2];
    //ѡ�������ӵ�Ľ��㣬���뵱ǰ��ѡ��Ƚϣ�������Զ��
    if (scale1 < scale2) {
      if (scale1 > nearscale) {
        nearscale = scale1;
      }
    }
    else {
      if (scale2 > nearscale) {
        nearscale = scale2;
      }
    }
  }
  startpos[0] = pos[0] + nearscale * dir[0] ;
  startpos[1] = pos[1] + nearscale * dir[1] ;
  startpos[2] = pos[2] + nearscale * dir[2] ;
  return CheckinBox(startpos, Dim); //�жϸõ��Ƿ��ڰ�Χ����
}
//�ϳ�����ֵ
//********************************************************************//
//rgba:�ϳ���ɫֵ
//x0,y0:��άͼ����������
//CData:�����������
//Dim:�����ݴ�С
//R:��ת���󣨻�����ͼ��ռ䵽����ռ��ת����
//T:ƽ��������ͬ�ϣ�
//********************************************************************//
void Composite(uchar* rgba, int x0, int y0, uchar* Data, uchar* pal, int* Dim, float* R, float* T)
{
  int stepsize = 1; //��������
  float cumcolor[4];//�ۼ���ɫֵ
  cumcolor[0] = cumcolor[1] = cumcolor[2] = cumcolor[3] = 0.0;
  float pos[3], dir[3]; //Ͷ�������㡢����
  float startpos[3];//�������Χ�н��ӵ㴦�Ľ�������
  float samplepos[3];//����������
  float samplecolor[4];//��������ɫ
  //����ƽ��ͶӰ������ͼ��ռ���Ͷ����ߵķ���(0,0,-1),���(x0,y0,0)
  pos[0] = x0;
  pos[1] = y0;
  pos[2] = 0;
  //����������ת��������ռ�
  //*********************************//
  dir[0] = -R[2];
  dir[1] = -R[5];
  dir[2] = -R[8]; //���߷���������ռ�ı��
  MatrixmulVec(pos, R, pos); //��ת
  pos[0] += T[0]; //ƽ��
  pos[1] += T[1];
  pos[2] += T[2];
  //*********************************//
  if (Intersection(startpos, pos, dir, Dim)) { //�жϹ������Χ���Ƿ��ཻ
    samplepos[0] = startpos[0];
    samplepos[1] = startpos[1];
    samplepos[2] = startpos[2];
    while (CheckinBox(samplepos, Dim) && cumcolor[3] < 1) { //�����������Χ�л��ۼƲ�͸���ȳ���1ʱ��ֹ�ϳ�
      TrInterpolation(samplecolor, samplepos, Data, pal, Dim); //�����Բ�ֵ��ò����㴦����ɫ����͸����
      //�ϳ���ɫ����͸����,���õ��Ǵ�ǰ����ĺϳɹ�ʽ
      cumcolor[0] += samplecolor[0] * samplecolor[3] * (1 - cumcolor[3]); //R
      cumcolor[1] += samplecolor[1] * samplecolor[3] * (1 - cumcolor[3]); //G
      cumcolor[2] += samplecolor[2] * samplecolor[3] * (1 - cumcolor[3]); //B
      cumcolor[3] += samplecolor[3] * (1 - cumcolor[3]); //A
      //��һ��������
      samplepos[0] += dir[0] * stepsize;
      samplepos[1] += dir[1] * stepsize;
      samplepos[2] += dir[2] * stepsize;
    }
    rgba[0] = cumcolor[0]*255;
    rgba[1] = cumcolor[1]*255;
    rgba[2] = cumcolor[2]*255;
    rgba[3] = cumcolor[3]*255;
    return;
  }
  rgba[0] = rgba[1] = rgba[2] = rgba[3] = 0; //���������Χ�в��ཻ������ɫ
}


//���ݷ���
//********************************************************************//
//��ԭʼ�����ݵı���ֵӳ��Ϊ��ɫ�Ͳ�͸����
//���ﴦ��ıȽϼ򵥣�ֱ�ӽ�֮ǰ���ɵ����ݷ����ࣺ���������ɫ�������ɫ��С�������ɫ
//CData:�����������
//Data:ԭʼ������
//Dim:�����ݴ�С
//********************************************************************//
void Classify(uchar* pal)
{
  int n = 255;
  int a = 2;
  for (int i = 0; i < 256; i++) {
    uchar* LinePD = pal+i*4;
    if (i <= 70) {
      //��ɫ
      LinePD[0] = 1*n;
      LinePD[1] = 1*n;
      LinePD[2] = 1*n;
      LinePD[3] = a;
    }
    else if (i <= 150) {
      //��ɫ
      LinePD[0] = 0;
      LinePD[1] = 0;
      LinePD[2] = 1*n;
      LinePD[3] = a;
    }
    else {
      //��ɫ
      LinePD[0] = 0;
      LinePD[1] = 1*n;
      LinePD[2] = 1*n;
      LinePD[3] = a;
    }
  }
}

int slice(uchar* Data, int* Dim, uchar* pal, int w, int h, uchar* img, int step, float16 T) {
  int i, j;
  float x, y;
  int Slicesize = Dim[0] * Dim[1];
  int Stepsize = Dim[0];
  for (i=0; i<h; ++i) {
    for (j=0; j<w; ++j) {
      float x = j+0.5-w*0.5;
      float y = i+0.5-h*0.5;
      float x1 = T[0*4+0]*x + T[0*4+1]*y + T[0*4+3] + Dim[0]*0.5;
      float y1 = T[1*4+0]*x + T[1*4+1]*y + T[1*4+3] + Dim[1]*0.5;
      float z1 = T[2*4+0]*x + T[2*4+1]*y + T[2*4+3] + Dim[2]*0.5;
      if (x1>=0 && x1<Dim[0] && y1>=0 && y1<Dim[1] && z1>=0 && z1<Dim[2]) {
        int x0 = x1;
        int y0 = y1;
        int z0 = z1;
        int i0 = z0 * Slicesize + y0 * Stepsize + x0;
        *(uint*)(img+i*step+j*4) = *(uint*)(pal+Data[i0]*4);
      }
    }
  }
  return 0;
}

#include "opengl/vtk/math.inl"
#include "opengl/vtk/math3.inl"

void RayCasting(int h, int w) {}

int test_RotateAxis4() {
  float4 v1 = {1, 0, 0}, v2;
  float4 axis = {1, 1, 1};
  float16 rot;
  RotateAxis4(rot, axis, DEGTORAD*120);
  MultiplyPoint16(rot, v1, v2);
  return 0;
}

int test_volume_rendering(int argc, char** argv)
{
  test_RotateAxis4();
  enum { WIDTH = 400, HEIGTH = 500};
  uchar Image[WIDTH* HEIGTH*4];
  int Dim[3] = {200, 200, 200}; //�����ݴ�С
  uchar* Data = (uchar*)malloc( Dim[0] * Dim[1] * Dim[2]);
  uchar pal[1024];
  float T[3] = {0, 0, 450}; //ƽ��������Ҫ����R�������Ա�֤���������ȫò��
  float R[9];//��ת����
  float eye[3] = {0.5, 0.5, 1}; //�ӵ�λ��
  float center[3] = {0, 0, 0}; //����ο���λ��
  float up[3] = {0, 1, 0}; //������ϵķ���
  RotationMatrix(R, eye, center, up); //�����ת����
  //����һ���������壬�ڲ�����һ�����壬�����м��ְ���һ��С������
  GenCube(0, 0, 0, 200, 70, Data, Dim); //��������
  GenSphere(100, 100, 100, 80, 150, Data, Dim); //����
  GenCube(70, 70, 70, 60, 200, Data, Dim); //С������

  Classify(pal); //�������ݷ���

  if (0) {
    int w=400, h=400;
    img_t im[1] = {0};
    float16 T;
    int angle = 60;
    int x = 0;
    int y = 0;
    int z = 0;
    imsetsize(im, h, w, 4, 1);
    
    // ��ת��
    float4 axis = {1, 1, 0};
    cvNamedWindow("im", 0);
    cvCreateTrackbar("angle", "im", &angle, 360, NULL);
    cvCreateTrackbar("x", "im", &x, 200, NULL);
    cvCreateTrackbar("y", "im", &y, 200, NULL);
    cvCreateTrackbar("z", "im", &z, 200, NULL);
    
    for (;1;) {
      angle%=360;
      Identity16(T);
      memset(im->tt.data, 0, im->s*im->h);
      RotateAxis4(T, axis, DEGTORAD*angle);
      T[0*4+3] = x;
      T[1*4+3] = y;
      T[2*4+3] = z;
      slice(Data, Dim, pal, w, h, im->tt.data, im->s, T);
      imshow(im);
      cvWaitKey(10);
      //angle += 1;
    }
    cvShowImagePal("slice", h, w, im->tt.data, im->s, im->c, PF_32bppPARGB, NULL);
    cvWaitKey(100);
    imfree(im);
  }
  for (;;) {
    uchar* LinePS = Image;
    {utime_start(_start_time);
    for (int j = 0; j < HEIGTH; j++) { //����ϳ�����ֵ
      for (int i = 0; i < WIDTH; i++) {
        Composite(LinePS, i, j, Data, pal, Dim, R, T);
        LinePS+=4;
      }
    }
printf("%f\n", utime_elapsed(_start_time));}
    cvShowImagePal("asdf", HEIGTH, WIDTH, (uchar*)Image, WIDTH * 4, 4, PF_32bppPARGB, NULL);
    cvWaitKey(100);
  }
  free(Data);
  //ʹ��OpenGL��ʾ�˶�άͼ��
  return 0;
}

#include "sys/sys_win32.inl"
