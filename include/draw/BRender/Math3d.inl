//��ά�任����(���α任�͹۲�任)
//Ϊ�˷���ϰ��,���ǲ�����������������

//#define V3SET(C, X, Y, Z)  (C.x = X,C.y = Y,C.z = Z,C.w = 1.f)
#define V3OPT(C, A, B, OP)  (C.x = A.x OP B.x,C.y = A.y OP B.y,C.z = A.z OP B.z)
#define V3ADD(C, A, B)  V3OPT(C, A, B, +)
#define V3SUB(C, A, B)  V3OPT(C, A, B, -)
#define V3MUL(C, A, B)  V3OPT(C, A, B, *)
#define V3OPT1(C, A, OP)  (C.x OP A.x,C.y OP A.y,C.z OP A.z)
#define V3ADD1(C, A)  V3OPT1(C, A, +=)
#define V3SUB1(C, A)  V3OPT1(C, A, -=)
#define V3MUL1(C, A)  V3OPT1(C, A, *=)
#define V3OPTa(C, A, B, OP)  (C.x = A.x OP B,C.y = A.y OP B,C.z = A.z OP B)
#define V3ADDa(C, A, B)  V3OPTa(C, A, B, +)
#define V3SUBa(C, A, B)  V3OPTa(C, A, B, -)
#define V3MULa(C, A, B)  V3OPTa(C, A, B, *)
#define V3DIVa(C, A, B)  V3OPTa(C, A, B, /)
#define V3OPTa2(C, A, OP)  (C.x OP A,C.y OP A,C.z OP A)
#define V3ADDa2(C, A)  V3OPTa2(C, A, +=)
#define V3SUBa2(C, A)  V3OPTa2(C, A, -=)
#define V3MULa2(C, A)  V3OPTa2(C, A, *=)
#define V3DIVa2(C, A)  V3OPTa2(C, A, /=)
// �������
#define V3CROSS(C, A, B)  {VECTOR3D v; v.w=1.f; v.x = (A.y * B.z - A.z * B.y); v.y = (A.z * B.x - A.x * B.z); v.z = (A.x * B.y - A.y * B.x); C=v; }
//����������֮��
#define V3MULA(C, A, fs)    (C.x = A.x * fs,C.y = A.y * fs,C.z = A.z * fs)
//�����ڻ�
#define V3DOT(A, B)         (A.x * B.x + A.y * B.y + A.z * B.z)
#define V3DOT2(A, X, Y, Z)  (A.x * X + A.y * Y + A.z * Z)
#define V3MAGSQ(A)          (A.x * A.x + A.y * A.y + A.z * A.z)
#define V3MAG(A)            ((float)sqrt(V3MAGSQ(A)))
//������ʸ���˵�֮��ľ���
#define V3DIST(A, B)        (float)sqrt((A.x-B.x)*(A.x-B.x) + (A.y-B.y)*(A.y-B.y) + (A.z-B.z)*(A.z-B.z))
//������λ��������һ����λ����
#define V3UNIT(C, A)        {float fMag=V3MAG(A); if (fMag < 0.0001f) { fMag = 1.0f; } V3DIVa(C, A, fMag);}
//#define V3UNIT(A)           (float)sqrt((A.x-B.x)*(A.x-B.x) + (A.y-B.y)*(A.y-B.y) + (A.z-B.z)*(A.z-B.z))
//������λ��������һ����λ����
VECTOR3D Unit(VECTOR3D* pV)
{
  VECTOR3D vector;
  float fMag = (float)sqrt(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);

  if (fMag < 0.0001f) {
    fMag = 1.0f;
  }

  vector.x = pV->x / fMag;
  vector.y = pV->y / fMag;
  vector.z = pV->z / fMag;
  return vector;
}
//����λ��
void Unitize(VECTOR3D* pV)
{
  float fMag = (float)sqrt(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);

  if (fMag < 0.0001f) {
    fMag = 1.0f;
  }

  pV->x /= fMag;
  pV->y /= fMag;
  pV->z /= fMag;
}
//��this����������ת
//����x��ת, fThetaΪ�Ƕȵ�λ
void RotateX(VECTOR3D* pV, float fTheta)
{
  //���Ƕ�ת��Ϊ����
  float fRad = (float)((fTheta * PIE) / 180.0);
  float yy = (float)(pV->y * cos(fRad) - pV->z * sin(fRad));
  float zz = (float)(pV->y * sin(fRad) + pV->z * cos(fRad));
  pV->y = yy;
  pV->z = zz;
}
//��this����������ת
//����y��ת, fThetaΪ�Ƕȵ�λ
void RotateY(VECTOR3D* pV, float fTheta)
{
  //���Ƕ�ת��Ϊ����
  float fRad = (float)((fTheta * PIE) / 180.0);
  float zz = (float)(pV->z * cos(fRad) - pV->x * sin(fRad));
  float xx = (float)(pV->z * sin(fRad) + pV->x * cos(fRad));
  pV->x = xx;
  pV->z = zz;
}
//��this����������ת
//����y��ת, fThetaΪ�Ƕȵ�λ
void RotateZ(VECTOR3D* pV, float fTheta)
{
  //���Ƕ�ת��Ϊ����
  float fRad = (float)((fTheta * PIE) / 180.0);
  float xx = (float)(pV->x * cos(fRad) - pV->y * sin(fRad));
  float yy = (float)(pV->x * sin(fRad) + pV->y * cos(fRad));
  pV->x = xx;
  pV->y = yy;
}
//����������ת, fThetaΪ�Ƕ�,
//����ָ��, �������Ϊһ����������, �������������Դ�����ԭ�㻭��
void Rotate(VECTOR3D* pV, float fTheta, VECTOR3D axis)
{
  float fx, fy, fz, fRad, c, s, xx, yy, zz;
  //��λ��
  Unitize(&axis);
  //��ת��ķ�����
  fx = axis.x, fy = axis.y, fz = axis.z;
  //���Ƕ�ת��Ϊ����
  fRad = (float)((fTheta * PIE) / 180.0);
  //�Ƕȵ����Һ�����
  c = (float)cos(fRad);
  s = (float)sin(fRad);
  //��α任�ĺϳ�,�ο���Ӧ�ļ����ͼ��ѧ�鼮,
  //�任�ľ��������ο�:
  //<�����ͼ��ѧ>(������),��ҹ�,�廪��ѧ������
  //���������Ľ��
  xx = (fx * fx * (1.0f - c) + c)      * pV->x +
      (fx * fy * (1.0f - c) - fz * s) * pV->y +
      (fx * fz * (1.0f - c) + fy * s) * pV->z;
  yy = (fy * fx * (1.0f - c) + fz * s) * pV->x +
      (fy * fy * (1.0f - c) + c)      * pV->y +
      (fy * fz * (1.0f - c) - fx * s) * pV->z;
  zz = (fz * fx * (1.0f - c) - fy * s) * pV->x +
      (fz * fy * (1.0f - c) + fx * s) * pV->y +
      (fz * fz * (1.0f - c) + c)      * pV->z;
  //���������
  pV->x = xx;
  pV->y = yy;
  pV->z = zz;
}
//����������ת, fThetaΪ�Ƕ�,
//(x, y, z)Ϊ��ת������
void Rotate2(VECTOR3D* pV, float fTheta, float x, float y, float z)
{
  VECTOR3D vector;
  V3SET(vector, x, y, z);
  Rotate(pV, fTheta, vector);
}
//����С�淨��
VECTOR3D Normal(const VECTOR3D* v1, const VECTOR3D* v2, const VECTOR3D* v3)
{
  //���ֹ���
  VECTOR3D vector1 = {(v2->x - v1->x), (v2->y - v1->y), (v2->z - v1->z)};
  VECTOR3D vector2 = {(v3->x - v1->x), (v3->y - v1->y), (v3->z - v1->z)};
  VECTOR3D noraml;
  V3SET(noraml, 0, 0, 0);
  V3CROSS(noraml, vector1, vector2);
  V3UNIT(noraml, noraml);
  return noraml;
}
//����С�淨��
VECTOR3D Noraml2(const VECTOR3D* v1, const VECTOR3D* v2, const VECTOR3D* v3)
{
  //���һ���������а�������Զ��, �����������һ������������,
  float w1 = v1->w, w2 = v2->w, w3 = v3->w;

  if (ABS(w1) <= 0.00001f) {
    w1 = 1.0f;
  }

  if (ABS(w2) <= 0.00001f) {
    w2 = 1.0f;
  }

  if (ABS(w3) <= 0.00001f) {
    w3 = 1.0f;
  }

  {
    float x1 = v1->x / w1, y1 = v1->y / w1, z1 = v1->z / w1;
    float x2 = v2->x / w2, y2 = v2->y / w2, z2 = v2->z / w2;
    float x3 = v3->x / w3, y3 = v3->y / w3, z3 = v3->z / w3;
    VECTOR3D vector1 = {(x2 - x1), (y2 - y1), (z2 - z1)};
    VECTOR3D vector2 = {(x3 - x1), (y3 - y1), (z3 - z1)};
    VECTOR3D noraml;
    V3CROSS(noraml, vector1, vector2);
    return noraml;
  }
}
typedef struct _MATRIX2D {
  double//
  a00, a01, a02,
       a10, a11, a12,
       a20, a21, a22;
} MATRIX2D;
typedef struct _MATRIX2Di {
  int//
  a00, a01, a02,
       a10, a11, a12,
       a20, a21, a22;
} MATRIX2Di;
#define M2DSET(_C,A00,A01,A02, A10,A11,A12, A20,A21,A22) \
  ((_C).a00 = A00,(_C).a01 = A01,(_C).a02 = A02, \
      (_C).a10 = A10,(_C).a11 = A11,(_C).a12 = A12, \
      (_C).a20 = A20,(_C).a21 = A21,(_C).a22 = A22)

#define M2DF2FIX(_I, _F)  M2DSET(_I,F2FIX(_F.a00),F2FIX(_F.a01),F2FIX(_F.a02),F2FIX(_F. a10),F2FIX(_F.a11),F2FIX(_F.a12),F2FIX(_F. a20),F2FIX(_F.a21),F2FIX(_F.a22))

//�任��ʽ:��PΪ�任ǰ�ĵ�,P1Ϊ�任��ĵ�,AΪ�任����,��
//    P1 = P * A
//|P1.x|   |P.x|   |a00,a01,a02|
//|P1.y| = |P.y| * |a10,a11,a12|
//|P1.z|   |P.z|   |a20,a21,a22|
#define M2TRANSFORM_3X3_FIX(_I, X, Y, Z, PX, PY, PZ) \
  (PX = FIX2I(X * _I.a00 + Y * _I.a10 + Z * _I.a20), \
      PY = FIX2I(X * _I.a01 + Y * _I.a11 + Z * _I.a21), \
      PZ = FIX2I(X * _I.a02 + Y * _I.a12 + Z * _I.a22))

int M2Transform(MATRIX2D* This, double x, double y, double z, double* px, double* py, double* pz)
{
  //���ζ���
  *px = x * This->a00 + y * This->a10 + z * This->a20;
  *py = x * This->a01 + y * This->a11 + z * This->a21;
  *pz = x * This->a02 + y * This->a12 + z * This->a22;
  return 0;
}
// �������������任����
//    [pa pb pc] = [a b c] * A
// matlab ����
//syms xa ya za pxa pya pza xb yb zb pxb pyb pzb xc yc zc pxc pyc pzc m00 m01 m02 m10 m11 m12 m20 m21 m22;
//[m00,m01,m02,m10,m11,m12,m20,m21,m22]=solve('pxa = xa * m00 + ya * m10 + za * m20','pya = xa * m01 + ya * m11 + za * m21','pza = xa * m02 + ya * m12 + za * m22',    'pxb = xb * m00 + yb * m10 + zb * m20','pyb = xb * m01 + yb * m11 + zb * m21','pzb = xb * m02 + yb * m12 + zb * m22',    'pxc = xc * m00 + yc * m10 + zc * m20','pyc = xc * m01 + yc * m11 + zc * m21','pzc = xc * m02 + yc * m12 + zc * m22', 'm00,m01,m02,m10,m11,m12,m20,m21,m22')

int M2Slove(MATRIX2D* This,
    double xa, double ya, double za, double pxa, double pya, double pza,
    double xb, double yb, double zb, double pxb, double pyb, double pzb,
    double xc, double yc, double zc, double pxc, double pyc, double pzc)
{
  double _det = xa * yb * zc - xa * yc * zb + yc * xb * za + xc * ya * zb - yb * xc * za - xb * ya * zc;
  This->a00 = ((pxa * yb * zc - pxa * yc * zb + ya * zb * pxc - ya * pxb * zc - za * yb * pxc + za * yc * pxb) / (_det));
  This->a01 = ((pya * yb * zc - pya * yc * zb + ya * zb * pyc - ya * pyb * zc - za * yb * pyc + za * yc * pyb) / (_det));
  This->a02 = ((pza * yb * zc - pza * yc * zb + ya * zb * pzc - ya * pzb * zc - za * yb * pzc + za * yc * pzb) / (_det));
  This->a10 = ((xa * zb * pxc + xa * pxb * zc + za * xb * pxc + pxa * zb * xc - za * pxb * xc - pxa * xb * zc) / (_det));
  This->a11 = ((xa * zb * pyc + xa * pyb * zc + za * xb * pyc + pya * zb * xc - za * pyb * xc - pya * xb * zc) / (_det));
  This->a12 = ((xa * zb * pzc + xa * pzb * zc + za * xb * pzc + pza * zb * xc - za * pzb * xc - pza * xb * zc) / (_det));
  This->a20 = ((xa * yb * pxc + ya * pxb * xc - pxa * yb * xc - xa * pxb * yc - xb * ya * pxc + pxa * xb * yc) / (_det));
  This->a21 = ((xa * yb * pyc + ya * pyb * xc - pya * yb * xc - xa * pyb * yc - xb * ya * pyc + pya * xb * yc) / (_det));
  This->a22 = ((xa * yb * pzc + ya * pzb * xc - pza * yb * xc - xa * pzb * yc - xb * ya * pzc + pza * xb * yc) / (_det));
  return 0;
}
int affineTixin(img_t* im, int x1, int x2, int y1, int x3, int x4, int y2, HDDC hS, MATRIX2D* pm)
{
  int dy, x, y, x13, x24, t, px, py, pz;
  MATRIX2Di im;
  M2DF2FIX(im, (*pm));
  dy = y1 < y2 ? 1 : -1, y = y1;

  for (; y != y2; y += dy) {
    if (y >= 0 && y < im->h) {
      DWORD* yline = &_Pixel(hDC, 0, y);
      x13 = GET_LINE_X(x1, y1, x3, y2, y);
      x24 = GET_LINE_X(x2, y1, x4, y2, y);

      if (x13 > x24) {
        SWAP(x13, x24, t);
      }

      for (x = x13; x <= x24; ++x) {
        if (x >= 0 && x < hDC->w) {
          //float fx, fy, fz;
          //M2Transform(pm, x, y, 1., &fx, &fy, &fz);
          M2TRANSFORM_3X3_FIX(im, x, y, 1, px, py, pz);

          if (px >= 0 && px < hS->w && py >= 0 && py < hS->h) {
            yline[x] = _Pixel(hS, px, py);
          }
        }
      }
    }
  }

  return 0;
}
int affineTri(HDDC hDC, const point2type* ptD, HDDC hS, const point2type* ptS)
{
  MATRIX2D m;
  int x12, n, i1;
  point2type pt3[3], t;
  M2Slove(&m, ptD[0].x, ptD[0].y, 1.f, ptS[0].x, ptS[0].y, 1.f
      , ptD[1].x, ptD[1].y, 1.f, ptS[1].x, ptS[1].y, 1.f
      , ptD[2].x, ptD[2].y, 1.f, ptS[2].x, ptS[2].y, 1.f);
  memcpy(pt3, ptD, sizeof(pt3));
#define YLESS(a, b)  ((a).y<(b).y)
  SORT3(pt3, YLESS, t);
#undef YLESS
  TriSort(pt3, t);

  if (pt3[1].y == pt3[0].y) {
    affineTixin(hDC, pt3[0].x, pt3[1].x, pt3[0].y, pt3[2].x, pt3[2].x, pt3[2].y, hS, &m);
    return 0;
  }

  if (pt3[1].y == pt3[2].y) {
    affineTixin(hDC, pt3[0].x, pt3[0].x, pt3[0].y, pt3[1].x, pt3[2].x, pt3[2].y, hS, &m);
    return 0;
  }

  x12 = GET_LINE_X(pt3[0].x, pt3[0].y, pt3[2].x, pt3[2].y, pt3[1].y);
  n = pt3[2].y - pt3[0].y;
  i1 = pt3[1].y - pt3[0].y;
  affineTixin(hDC, pt3[0].x, pt3[0].x, pt3[0].y, pt3[1].x, x12, pt3[1].y, hS, &m);
  affineTixin(hDC, pt3[1].x, x12, pt3[1].y, pt3[2].x, pt3[2].x, pt3[2].y, hS, &m);
  return 0;
}
int test_affineTixin(HDDC hDC)
{
  enum {MAX_points = 40};
  int len = 4;
  point2type pt2[MAX_points] = {100, 10, 10, 200, 100, 50, 300, 300};
  point2type pt3[MAX_points] = {100, 210, 10, 100, 100, 150, 300, 10};
  point2type pt4[MAX_points] = {0, 0, 300, 300, 300, 0, 20, 100};
  Image im[1] = {0};
  TDrawDC hMemDC[1];
  RandPoints(pt3, MAX_points, hDC->w, hDC->h);
  len = MAX_points;
  //RandPoints(pt4, MAX_points, hDC->w, hDC->h);len=MAX_points;
  {
    double fx, fy, fz;
    int px, py, pz;
    MATRIX2D m;
    MATRIX2Di im;
    M2Slove(&m, pt3[0].x, pt3[0].y, 1.f, pt4[0].x, pt4[0].y, 1.f
        , pt3[1].x, pt3[1].y, 1.f, pt4[1].x, pt4[1].y, 1.f
        , pt3[2].x, pt3[2].y, 1.f, pt4[2].x, pt4[2].y, 1.f);
    M2DF2FIX(im, m);
    //M2TRANSFORM_3X3_FIX(im, pt3[0].x, pt3[0].y, 1, px, py, pz);
    //M2TRANSFORM_3X3_FIX(im, pt3[1].x, pt3[1].y, 1, px, py, pz);
    //M2TRANSFORM_3X3_FIX(im, pt3[2].x, pt3[2].y, 1, px, py, pz);
    M2Transform(&m, pt3[0].x, pt3[0].y, 1., &fx, &fy, &fz);
    M2Transform(&m, pt3[1].x, pt3[1].y, 1., &fx, &fy, &fz);
    M2Transform(&m, pt3[2].x, pt3[2].y, 1., &fx, &fy, &fz);
  }

  if (1) {
    DWORD clrs[] = {_RGB(255, 0, 0), _RGB(0, 255, 0), _RGB(0, 0, 255), _RGB(255, 255, 255)};
    newImage(hDC->w, hDC->h, 4, im);
    HDDCSET_IMAGE(hMemDC, im);
#define randf()  (rand()*1./RAND_MAX)
    FillChessBoard(hDC, 50, 50, randf() + 0.5, randf() + 0.5, randf() + 0.5, randf() + 0.5, clrs, 3);
    FillChessBoard(hMemDC, 30, 30, 1, 0, 0, 1, clrs, 4);
  }
  else {
    loadbmpfile("D:/pub/bin/feat/lena.BMP", 32, 1, im);
    HDDCSET_IMAGE(hMemDC, im);
  }

  if (0) {
    utime_start(_start_time);
    int mi01 = MIN(pt4[0].x, pt4[1].x);
    int mx01 = MAX(pt4[0].x, pt4[1].x);
    int mi23 = MIN(pt4[2].x, pt4[3].x);
    int mx23 = MAX(pt4[2].x, pt4[3].x);
    FillTiXin(hMemDC, mi01, mx01, pt4[1].y, mi23, mx23, pt4[3].y, _RGB(255, 0, 0));
    FillTiXin(hMemDC, mi01, mi01, pt4[1].y, mi23, mx23, pt4[3].y, _RGB(0, 255, 0));
    FillTiXin(hMemDC, mi01, mx01, pt4[1].y, mx23, mx23, pt4[3].y, _RGB(0, 0, 255));
    printf("%f\n", utime_elapsed(_start_time));
  }

  if (1) {
    utime_start(_start_time);
    affineTri(hDC, pt3, hMemDC, pt4);
    //affineTixin(hDC, mi01, mi01, pt4[1].y, mi23, mx23, pt4[3].y, _RGB(0, 255, 0));
    //affineTixin(hDC, mi01, mx01, pt4[1].y, mx23, mx23, pt4[3].y, _RGB(0, 0, 255));
    printf("%f\n", utime_elapsed(_start_time));
  }

  DrawPoly(hDC, pt3, 3, _RGB(0, 255, 0), 1);
  //cvWaitKey(-1);
  free(im);
  return 0;
}

//��λ����
#define M2IDENTITY(_C)   M2DSET(_C, 1.0f,0.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f)

// ����˷�
#define M2MUL_3X3(C, A, B) \
  C.a00 = A.a00 * B.a00 + A.a01 * B.a10 + A.a02 * B.a20; \
  C.a01 = A.a00 * B.a01 + A.a01 * B.a11 + A.a02 * B.a21; \
  C.a02 = A.a00 * B.a02 + A.a01 * B.a12 + A.a02 * B.a22; \
  C.a10 = A.a10 * B.a00 + A.a11 * B.a10 + A.a12 * B.a20; \
  C.a11 = A.a10 * B.a01 + A.a11 * B.a11 + A.a12 * B.a21; \
  C.a12 = A.a10 * B.a02 + A.a11 * B.a12 + A.a12 * B.a22; \
  C.a20 = A.a20 * B.a00 + A.a21 * B.a10 + A.a22 * B.a20; \
  C.a21 = A.a20 * B.a01 + A.a21 * B.a11 + A.a22 * B.a21; \
  C.a22 = A.a20 * B.a02 + A.a21 * B.a12 + A.a22 * B.a22

//����ϵ��(4��4��)
//��������:
//    a00, a01, a02, a03
//    a10, a11, a12, a13
//    a20, a21, a22, a23
//    a30, a31, a32, a33
//�任��ʽ:��PΪ�任ǰ�ĵ�,P1Ϊ�任��ĵ�,AΪ�任����,��
//    P1 = P * A
//|P1.x|    |P.x|   |a00,a01,a02,a03|
//|P1.y|  = |P.y| * |a10,a11,a12,a13|
//|P1.z|    |P.z|   |a20,a21,a22,a23|
//|P1.w|    |P.w|   |a30,a31,a32,a33|
typedef struct _MATRIX3D {
  float//
  a00, a01, a02, a03,
       a10, a11, a12, a13,
       a20, a21, a22, a23,
       a30, a31, a32, a33;
} MATRIX3D;

#define M3DSET(_C,A00,A01,A02,A03,A10,A11,A12,A13,A20,A21,A22,A23,A30,A31,A32,A33) \
  ((_C).a00 = A00,(_C).a01 = A01,(_C).a02 = A02,(_C).a03 = A03, \
      (_C).a10 = A10,(_C).a11 = A11,(_C).a12 = A12,(_C).a13 = A13, \
      (_C).a20 = A20,(_C).a21 = A21,(_C).a22 = A22,(_C).a23 = A23, \
      (_C).a30 = A30,(_C).a31 = A31,(_C).a32 = A32,(_C).a33 = A33)

//��λ����
#define M3IDENTITY(_C)   M3DSET(_C, 1.0f,0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f,1.0f)

// ����˷�
#define MATRIX_MULTIPLY_4X4_v1(C, A, B)  { int i, j, k; for(i = 0; i < 4; i++) { for(j = 0; j < 4; j++) { C.aij = 0.0f; for(k = 0; k < 4; k++) { C.aij += (A.aik * B.akj);  } } } }
//չ������, ����ѭ��, �Լ��ټӷ��������
#define MATRIX_MULTIPLY_4X4(C, A, B)  \
  C.a00 = A.a00 * B.a00 + A.a01 * B.a10 + A.a02 * B.a20 + A.a03 * B.a30; \
  C.a01 = A.a00 * B.a01 + A.a01 * B.a11 + A.a02 * B.a21 + A.a03 * B.a31; \
  C.a02 = A.a00 * B.a02 + A.a01 * B.a12 + A.a02 * B.a22 + A.a03 * B.a32; \
  C.a03 = A.a00 * B.a03 + A.a01 * B.a13 + A.a02 * B.a23 + A.a03 * B.a33; \
  C.a10 = A.a10 * B.a00 + A.a11 * B.a10 + A.a12 * B.a20 + A.a13 * B.a30; \
  C.a11 = A.a10 * B.a01 + A.a11 * B.a11 + A.a12 * B.a21 + A.a13 * B.a31; \
  C.a12 = A.a10 * B.a02 + A.a11 * B.a12 + A.a12 * B.a22 + A.a13 * B.a32; \
  C.a13 = A.a10 * B.a03 + A.a11 * B.a13 + A.a12 * B.a23 + A.a13 * B.a33; \
  C.a20 = A.a20 * B.a00 + A.a21 * B.a10 + A.a22 * B.a20 + A.a23 * B.a30; \
  C.a21 = A.a20 * B.a01 + A.a21 * B.a11 + A.a22 * B.a21 + A.a23 * B.a31; \
  C.a22 = A.a20 * B.a02 + A.a21 * B.a12 + A.a22 * B.a22 + A.a23 * B.a32; \
  C.a23 = A.a20 * B.a03 + A.a21 * B.a13 + A.a22 * B.a23 + A.a23 * B.a33; \
  C.a30 = A.a30 * B.a00 + A.a31 * B.a10 + A.a32 * B.a20 + A.a33 * B.a30; \
  C.a31 = A.a30 * B.a01 + A.a31 * B.a11 + A.a32 * B.a21 + A.a33 * B.a31; \
  C.a32 = A.a30 * B.a02 + A.a31 * B.a12 + A.a32 * B.a22 + A.a33 * B.a32; \
  C.a33 = A.a30 * B.a03 + A.a31 * B.a13 + A.a32 * B.a23 + A.a33 * B.a33

//����˷�
void matrix_multiply_4x4(MATRIX3D* c, MATRIX3D* a, MATRIX3D* b, DWORD dwMultiplier)
{
  MATRIX3D me;

  if (dwMultiplier == G3D_MATRIX_MULTIPLIER_PRECONCAT) {
    MATRIX_MULTIPLY_4X4(me, (*a), (*b));
  }
  else if (dwMultiplier == G3D_MATRIX_MULTIPLIER_POSTCONCAT) {
    MATRIX_MULTIPLY_4X4(me, (*b), (*a));
  }

  *c = me;
}
#define MultMatrixf(This, m, dwMultiplier)  matrix_multiply_4x4(This, This, m, dwMultiplier)

//����"=="�����
BOOL M3eq(MATRIX3D* This, const MATRIX3D* m)
{
  int i;
  const float* a1 = (const float*)(This);
  const float* a2 = (const float*)(m);

  for (i = 0; i < 16; i++) {
    if (ABS(a1[i] - a2[i]) > 0.0001f) {
      return FALSE;
    }
  }

  return TRUE;
}

// �����ͼ��ѧ�������Ա任
//�������任����
//Homogeneous matrices have the following advantages:
//(1) they provide simple explicit expressions for many familiar transformations including rotation,
//(2) these expressions are n-dimensional,
//(3) there is no need for auxillary matrices, as in vector methods for rotation,
//(4) more general transformations can be represented (e.g. projections, translations, rotations that move the origin),
//(5) directions (ideal points) can be used as parameters of the transformation, or as points to be transformed,
//(6) duality between points and hyperplanes applies to matters of incidence and invariant subspaces
//ƽ�Ʊ任, ƽ����(x, y, z)
#define M3TRANSLATE(_C, X, Y, Z)  ((_C).a30 += X,(_C).a31 += Y,(_C).a32 += Z)

//���ű任
//fsx, fsy, fsz----Ϊ��������
//��Ӧ��ʱ, Ҫ�ر�ע��, ��ֻ�ı����Խ����ϵ�Ԫ��
//���, �����ڵ�һ�α任
#define M3SCALE(_C, FSX, FSY, FSZ)  ((_C).a00 *= FSX,(_C).a11 *= FSY,(_C).a22 *= FSZ)
//���ű任
//fsx, fsy, fsz----Ϊ��������
//x, y, zΪ���ű任�Ĳο���, ȱʡ����Ϊ��������ϵ������ԭ��
void Scale2(MATRIX3D* This, float fsx, float fsy, float fsz, float x, float y, float z)
{
  MATRIX3D m;
  M3IDENTITY(m);
  m.a00 = fsx;
  m.a11 = fsy;
  m.a22 = fsz;
  m.a30 = (1 - fsx) * x;
  m.a31 = (1 - fsy) * y;
  m.a32 = (1 - fsz) * z;
  //�����ҳ�
  MultMatrixf(This, &m, G3D_MATRIX_MULTIPLIER_POSTCONCAT);
}
//�� x ����ת
//fThetaΪ�Ƕȵ�λ, �ο���Ϊ����ԭ��
void M3RotateX(MATRIX3D* This, float fTheta)
{
  //���Ƕ�ת��Ϊ����
  float fRad = (float)((fTheta * PIE) / 180.0);
  MATRIX3D m;
  M3IDENTITY(m);
  M3IDENTITY(m);
  m.a11 = (float)cos(fRad);
  m.a12 = (float)sin(fRad);
  m.a21 = -m.a12;
  m.a22 = m.a11;
  //�����ҳ�
  MultMatrixf(This, &m, G3D_MATRIX_MULTIPLIER_POSTCONCAT);
}
//�� x ����ת
//fThetaΪ�Ƕȵ�λ, �ο���Ϊ����(x, y, z)
void M3RotateX1(MATRIX3D* This, float fTheta, float x, float y, float z)
{
  //����˷���������
  M3TRANSLATE(*This, -x, -y, -z);
  M3RotateX(This, fTheta);
  M3TRANSLATE(*This, x, y, z);
}
//�� y ����ת
//fThetaΪ�Ƕȵ�λ, �ο���Ϊ����ԭ��
void M3RotateY(MATRIX3D* This, float fTheta)
{
  //���Ƕ�ת��Ϊ����
  float fRad = (float)((fTheta * PIE) / 180.0);
  MATRIX3D m;
  M3IDENTITY(m);
  m.a00 = (float)cos(fRad);
  m.a20 = (float)sin(fRad);
  m.a02 = -m.a20;
  m.a22 = m.a00;
  //�����ҳ�
  MultMatrixf(This, &m, G3D_MATRIX_MULTIPLIER_POSTCONCAT);
}
//�� y ����ת
//fThetaΪ�Ƕȵ�λ, �ο���Ϊ����(x, y, z)
void M3RotateY1(MATRIX3D* This, float fTheta, float x, float y, float z)
{
  M3TRANSLATE(*This, -x, -y, -z);
  M3RotateY(This, fTheta);
  M3TRANSLATE(*This, x, y, z);
}
//�� z ����ת
//fThetaΪ�Ƕȵ�λ, �ο���Ϊ����ԭ��
void M3RotateZ(MATRIX3D* This, float fTheta)
{
  //���Ƕ�ת��Ϊ����
  float fRad = (float)((fTheta * PIE) / 180.0);
  MATRIX3D m;
  M3IDENTITY(m);
  m.a00 = (float)cos(fRad);
  m.a01 = (float)sin(fRad);
  m.a10 = -m.a01;
  m.a11 = m.a00;
  //�����ҳ�
  MultMatrixf(This, &m, G3D_MATRIX_MULTIPLIER_POSTCONCAT);
}
//�� z ����ת
//fThetaΪ�Ƕȵ�λ, �ο���Ϊ����(x, y, z)
void M3RotateZ1(MATRIX3D* This, float fTheta, float x, float y, float z)
{
  M3TRANSLATE(*This, -x, -y, -z);
  M3RotateZ(This, fTheta);
  M3TRANSLATE(*This, x, y, z);
}
//��������(x, y, z)��ת�Ƕ�fTheta, �������ԭ��
void M3Rotate(MATRIX3D* This, float fTheta, float x, float y, float z)
{
  //���ᵥλ��
  float fMag = (float)sqrt(x * x + y * y + z * z);

  if (fMag < 0.0001f) {
    //��������ԭ��, ����, �ôα任��������ļ���λ�ò���Ӱ��
    return;
  }

  {
    //��ת��ķ�����,��λ����
    float fx = (x / fMag), fy = (y / fMag), fz = (z / fMag);
    //���Ƕ�ת��Ϊ����
    float fRad = (float)((fTheta * PIE) / 180.0);
    //�Ƕȵ����Һ�����
    float c = (float)cos(fRad);
    float s = (float)sin(fRad);
    //�任����m
    MATRIX3D m;
    M3IDENTITY(m);
    //��α任�ĺϳ�,�ο���Ӧ�ļ����ͼ��ѧ�鼮
    m.a00 = fx * fx * (1.0f - c) + c;
    m.a01 = fx * fy * (1.0f - c) - fz * s;
    m.a02 = fx * fz * (1.0f - c) + fy * s;
    m.a10 = fy * fx * (1.0f - c) + fz * s;
    m.a11 = fy * fy * (1.0f - c) + c;
    m.a12 = fy * fz * (1.0f - c) - fx * s;
    m.a20 = fz * fx * (1.0f - c) - fy * s;
    m.a21 = fz * fy * (1.0f - c) + fx * s;
    m.a22 = fz * fz * (1.0f - c) + c;
    //��ԭ�б任���кϳ�
    MultMatrixf(This, &m, G3D_MATRIX_MULTIPLIER_POSTCONCAT);
  }
}
//��ת, ���ɷ���axis�͵�ptOn����
void M3Rotate1(MATRIX3D* This, float fTheta, VECTOR3D axis, VECTOR3D ptOn)
{
  M3TRANSLATE(*This, -ptOn.x, -ptOn.y, -ptOn.z);
  M3Rotate(This, fTheta, axis.x, axis.y, axis.z);
  M3TRANSLATE(*This, ptOn.x, ptOn.y, ptOn.z);
}
//��� x �����, ��Ե�Ϊ����ԭ��
//ʹ�ù�ʽ: x1 = x0 , y1 = y0 + a * x0, z1 = z0 + b * x0
void SkewX(MATRIX3D* This, float fsy, float fsz)
{
  MATRIX3D m;
  M3IDENTITY(m);
  m.a01 = fsy;
  m.a02 = fsz;
  MultMatrixf(This, &m, G3D_MATRIX_MULTIPLIER_POSTCONCAT);
}
//��� x �����, ��Ե�Ϊ(x, y, z)
void M3SkewX(MATRIX3D* This, float fsy, float fsz, float x, float y, float z)
{
  M3TRANSLATE(*This, -x, -y, -z);
  SkewX(This, fsy, fsz);
  M3TRANSLATE(*This, x, y, z);
}
//��� y �����, ��Ե�Ϊ����ԭ��
//ʹ�ù�ʽ: x1 = x0 + a * y0, y1 = y0, z1 = z0 + b * y0
void M3SkewY(MATRIX3D* This, float fsx, float fsz)
{
  MATRIX3D m;
  M3IDENTITY(m);
  m.a10 = fsx;
  m.a12 = fsz;
  MultMatrixf(This, &m, G3D_MATRIX_MULTIPLIER_POSTCONCAT);
}
//��� y �����, ��Ե�Ϊ(x, y, z)
void M3SkewY1(MATRIX3D* This, float fsx, float fsz, float x, float y, float z)
{
  M3TRANSLATE(*This, -x, -y, -z);
  M3SkewY(This, fsx, fsz);
  M3TRANSLATE(*This, x, y, z);
}
//��� z �����, ��Ե�Ϊ����ԭ��
//ʹ�ù�ʽ: x1 = x0 + a * z0, y1 = y0 + b * z0, z1 = z0
void M3SkewZ(MATRIX3D* This, float fsx, float fsy)
{
  MATRIX3D m;
  M3IDENTITY(m);
  m.a20 = fsx;
  m.a21 = fsy;
  MultMatrixf(This, &m, G3D_MATRIX_MULTIPLIER_POSTCONCAT);
}
//��� z �����, ��Ե�Ϊ(x, y, z)
void SkewZ(MATRIX3D* This, float fsx, float fsy, float x, float y, float z)
{
  M3TRANSLATE(*This, -x, -y, -z);
  M3SkewZ(This, fsx, fsy);
  M3TRANSLATE(*This, x, y, z);
}
//��xoyƽ�����ԳƱ任(������任)
void ReflexXOY(MATRIX3D* This)
{
  This->a02 = -This->a02;
  This->a12 = -This->a12;
  This->a22 = -This->a22;
  This->a32 = -This->a32;
}
//��yozƽ�����ԳƱ任(������任)
void ReflexYOZ(MATRIX3D* This)
{
  This->a00 = -This->a00;
  This->a10 = -This->a10;
  This->a20 = -This->a20;
  This->a30 = -This->a30;
}
//��zoxƽ�����ԳƱ任(������任)
void ReflexZOX(MATRIX3D* This)
{
  This->a01 = -This->a01;
  This->a11 = -This->a11;
  This->a21 = -This->a21;
  This->a31 = -This->a31;
}
//�� x �ᷴ��
void ReflexX(MATRIX3D* This)
{
  //�ڶ���
  This->a01 = -This->a01;
  This->a11 = -This->a11;
  This->a21 = -This->a21;
  This->a31 = -This->a31;
  //������
  This->a02 = -This->a02;
  This->a12 = -This->a12;
  This->a22 = -This->a22;
  This->a32 = -This->a32;
}
//�� y �ᷴ��
void ReflexY(MATRIX3D* This)
{
  //��һ��
  This->a00 = -This->a00;
  This->a10 = -This->a10;
  This->a20 = -This->a20;
  This->a30 = -This->a30;
  //������
  This->a02 = -This->a02;
  This->a12 = -This->a12;
  This->a22 = -This->a22;
  This->a32 = -This->a32;
}
//�� z �ᷴ��
void ReflexZ(MATRIX3D* This)
{
  //��һ��
  This->a00 = -This->a00;
  This->a10 = -This->a10;
  This->a20 = -This->a20;
  This->a30 = -This->a30;
  //�ڶ���
  This->a01 = -This->a01;
  This->a11 = -This->a11;
  This->a21 = -This->a21;
  This->a31 = -This->a31;
}
//��ԭ�㷴��
void ReflexO(MATRIX3D* This)
{
  //��һ��
  This->a00 = -This->a00;
  This->a10 = -This->a10;
  This->a20 = -This->a20;
  This->a30 = -This->a30;
  //�ڶ���
  This->a01 = -This->a01;
  This->a11 = -This->a11;
  This->a21 = -This->a21;
  This->a31 = -This->a31;
  //������
  This->a02 = -This->a02;
  This->a12 = -This->a12;
  This->a22 = -This->a22;
  This->a32 = -This->a32;
}
//ִ�б任
//ִ�б任, ����Ϊ��ά����:�������
//����ָ��, ����һ�� w = 1.0f ����ʵ����б任ʱ, �����п���ת��Ϊ w != 1.0f �ĵ�.
//��Ӧ�õ�ʱ��, ��һ�����Ҫ���.
//���˵���, ���Ǵ󲿷�ʱ�����ڼ��α任�д���Transform()����,
//����, ��Щ�������������ʾΪ (z, y, z, 1.0f).
//|out_x|    |x|   |a00,a01,a02,a03|
//|out_y|  = |y| * |a10,a11,a12,a13|
//|out_z|    |z|   |a20,a21,a22,a23|
//|out_w|    |w|   |a30,a31,a32,a33|
VECTOR3D M3Transform(MATRIX3D* This, float x, float y, float z, float w)
{
  //���ζ���
  VECTOR3D hc;
  hc.x = x * This->a00 + y * This->a10 + z * This->a20 + w * This->a30;
  hc.y = x * This->a01 + y * This->a11 + z * This->a21 + w * This->a31;
  hc.z = x * This->a02 + y * This->a12 + z * This->a22 + w * This->a32;
  hc.w = x * This->a03 + y * This->a13 + z * This->a23 + w * This->a33;
  return hc;
}
//ִ�б任, ����Ϊ��ά���� VECTOR3D
VECTOR3D M3TransformH(MATRIX3D* This, VECTOR3D vertex)
{
  //���ζ���
  VECTOR3D hc;
  hc.x = vertex.x * This->a00 + vertex.y * This->a10 + vertex.z * This->a20 + vertex.w * This->a30;
  hc.y = vertex.x * This->a01 + vertex.y * This->a11 + vertex.z * This->a21 + vertex.w * This->a31;
  hc.z = vertex.x * This->a02 + vertex.y * This->a12 + vertex.z * This->a22 + vertex.w * This->a32;
  hc.w = vertex.x * This->a03 + vertex.y * This->a13 + vertex.z * This->a23 + vertex.w * This->a33;
  return hc;
}
//�任����, ��ǰ�����������߷�λ����
VECTOR3D M3TransformV(MATRIX3D* This, VECTOR3D vector)
{
  //����
  VECTOR3D v;
  v.x = vector.x * This->a00 + vector.y * This->a10 + vector.z * This->a20;
  v.y = vector.x * This->a01 + vector.y * This->a11 + vector.z * This->a21;
  v.z = vector.x * This->a02 + vector.y * This->a12 + vector.z * This->a22;
  v.w = 1.0f;
  return v;
}
// �������굽�۲�����ı任����
//(x, y, z)Ϊ��������ϵ�¶�����ӵ�
//�۲�����Ϊ����ԭ��,������Ϊ(0,1,0)
void Watch(MATRIX3D* This, float x, float y, float z)
{
  MATRIX3D mv;
  float fv, fu;
  //����ϵ�ı任���̷������岽:
  //��һ, ���û�����ϵƽ�����ӵ�;
  //�ڶ�, ��ƽ�ƺ��������ϵ�� X' ����ת90��;
  //����, �ٽ�������ϵ�� Y' ����ת�Ƕ� theta . sin(theta) = -x / sqrt( x * x + y * y);
  //����, �ٽ�������ϵ�� X' ����ת�Ƕ� alpha . sin(alpha) = c / mag(a, b, c)
  //����, ����������ϵ��Ϊ��������ϵ, Z �ᷴ��
  //���������ؼ���
  fv = (float)sqrt(x * x + y * y);
  fu = (float)sqrt(x * x + y * y + z * z);

  //�ӵ��Զ�����ƶ�������λ(����ȱʡ����ϵͳ֮��);
  if (fu < 0.00001) {
    fu = 2.0f;
  }

  //�۲�任�ڼ��α任���֮�����
  //�۲�任����, ����ֱ��д���۲�任�Ľ��, �����ɼ�������
  M3IDENTITY(mv);

  //����ӵ�λ�� z ����
  if (fv <= 0.00001f) {
    //��ʱ, ͶӰƽ��ƽ���� xoy ƽ��, ͶӰ����Ϊ�� z ������ָ����(��������ϵ)
    mv.a22 = -1;
    mv.a32 = fu;
    //����:
    //��������������Ĵν�Ҫ���������ǲ�������
    //�����������(fv > 0.00001f)
    /*
    mv.a00 = -1.0f;   mv.a01 = 0.0;       mv.a02 = -x / fu;
    mv.a10 = 0.0f;    mv.a11 = -1.0f;     mv.a12 = -y / fu;
    mv.a21 = fv / fu; mv.a22 = -z / fu;
    mv.a32 = fu;
    */
  }
  else {
    //����һ������
    mv.a00 = -y / fv;
    mv.a01 = -(x * z) / (fu * fv);
    mv.a02 = -x / fu;
    mv.a10 = x / fv;
    mv.a11 = -(y * z) / (fu * fv);
    mv.a12 = -y / fu;
    mv.a21 = fv / fu;
    mv.a22 = -z / fu;
    mv.a32 = fu;
    //����:
    //����һ������µ�͸�ӱ任, �ñ任���ܻ�����λ�÷����ı�,
    //����, һ�����ŵ�����, ���ܻᱻ�ŵ�.
  }

  //����:
  //�ڱ�̷�����,���ǲ����˾����ۻ��˷�����.
  //���Ƿ������������������,
  //��ʵ��,Ҳ���Բ���һ���任����,������Լ���ɡ�
  MultMatrixf(This, &mv, G3D_MATRIX_MULTIPLIER_PRECONCAT);
}

MATRIX3D M3transpose(const MATRIX3D* matIn)
{
  MATRIX3D result;

  result.a00 = matIn->a00;
  result.a01 = matIn->a10;
  result.a02 = matIn->a20;
  result.a03 = matIn->a30;

  result.a10 = matIn->a01;
  result.a11 = matIn->a11;
  result.a12 = matIn->a21;
  result.a13 = matIn->a31;

  result.a20 = matIn->a02;
  result.a21 = matIn->a12;
  result.a22 = matIn->a22;
  result.a23 = matIn->a32;

  result.a30 = matIn->a03;
  result.a31 = matIn->a13;
  result.a32 = matIn->a23;
  result.a33 = matIn->a33;

  return result;
}


MATRIX3D invertMatrix(const MATRIX3D* matIn)
{
  MATRIX3D result;

  // OpenGL matrices are column major and can be quite confusing to access
  // when stored in the typical, one-dimensional array often used by the API.
  // Here are some shorthand conversion macros, which convert a row/column
  // combination into an array index.

  float m11 = matIn->a00;
  float m12 = matIn->a01;
  float m13 = matIn->a02;
  float m14 = matIn->a03;
  float m21 = matIn->a10;
  float m22 = matIn->a11;
  float m23 = matIn->a12;
  float m24 = matIn->a13;
  float m31 = matIn->a20;
  float m32 = matIn->a21;
  float m33 = matIn->a22;
  float m34 = matIn->a23;
  float m41 = matIn->a30;
  float m42 = matIn->a31;
  float m43 = matIn->a32;
  float m44 = matIn->a33;

  // Inverse = adjoint / det. (See linear algebra texts.)

  // pre-compute 2x2 dets for last two rows when computing
  // cofactors of first two rows.
  float d12 = (m31 * m42 - m41 * m32);
  float d13 = (m31 * m43 - m41 * m33);
  float d23 = (m32 * m43 - m42 * m33);
  float d24 = (m32 * m44 - m42 * m34);
  float d34 = (m33 * m44 - m43 * m34);
  float d41 = (m34 * m41 - m44 * m31);

  float tmp[16], det;

  tmp[0] = (m22 * d34 - m23 * d24 + m24 * d23);
  tmp[1] = -(m21 * d34 + m23 * d41 + m24 * d13);
  tmp[2] = (m21 * d24 + m22 * d41 + m24 * d12);
  tmp[3] = -(m21 * d23 - m22 * d13 + m23 * d12);

  // Compute determinant as early as possible using these cofactors.
  det = m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2] + m14 * tmp[3];

  // Run singularity test.
  if (det == 0.0) {
    // Warning: Call to invertMatrix produced a Singular matrix.;

    float identity[16] = {
      1.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 1.0
    };

    memcpy(&result, identity, 16 * sizeof(float));
  }
  else {
    float invDet = 1.0f / det;

    // Compute rest of inverse.
    tmp[0] *= invDet;
    tmp[1] *= invDet;
    tmp[2] *= invDet;
    tmp[3] *= invDet;

    tmp[4] = -(m12 * d34 - m13 * d24 + m14 * d23) * invDet;
    tmp[5] = (m11 * d34 + m13 * d41 + m14 * d13) * invDet;
    tmp[6] = -(m11 * d24 + m12 * d41 + m14 * d12) * invDet;
    tmp[7] = (m11 * d23 - m12 * d13 + m13 * d12) * invDet;

    // Pre-compute 2x2 dets for first two rows when computing cofactors
    // of last two rows.
    d12 = m11 * m22 - m21 * m12;
    d13 = m11 * m23 - m21 * m13;
    d23 = m12 * m23 - m22 * m13;
    d24 = m12 * m24 - m22 * m14;
    d34 = m13 * m24 - m23 * m14;
    d41 = m14 * m21 - m24 * m11;

    tmp[8] = (m42 * d34 - m43 * d24 + m44 * d23) * invDet;
    tmp[9] = -(m41 * d34 + m43 * d41 + m44 * d13) * invDet;
    tmp[10] = (m41 * d24 + m42 * d41 + m44 * d12) * invDet;
    tmp[11] = -(m41 * d23 - m42 * d13 + m43 * d12) * invDet;
    tmp[12] = -(m32 * d34 - m33 * d24 + m34 * d23) * invDet;
    tmp[13] = (m31 * d34 + m33 * d41 + m34 * d13) * invDet;
    tmp[14] = -(m31 * d24 + m32 * d41 + m34 * d12) * invDet;
    tmp[15] = (m31 * d23 - m32 * d13 + m33 * d12) * invDet;

    memcpy(&result, tmp, 16 * sizeof(float));
  }

  return result;
}
