
#include <math.h>
#include <memory.h>
#include <time.h>
#include <stdlib.h>
#include "macro.h"
#include "fractal.h"

/*�����Ǹ�˹�����������*/

#define GAUSS_TABLE_LENGTH 256

double Gauss_F[ GAUSS_TABLE_LENGTH ];
double Gauss_f[ GAUSS_TABLE_LENGTH ];
DWORD RandomKey, RandomKey32;

DWORD MCoef_32[ 2 ] = {0xE7BD2160, 0xDA3A2A9C};
WORD MCode_16[ 4 ] = {0xD31C, 0xC36C, 0xD0A2, 0xD228} ;
// two m sequence

#define INVALID_BLOCK 0xFFFF
#define BLOCK_SIZE 512

DWORD m_Seq_32(DWORD Key)
{
  int i;

  for (i = 0; i < 32; i++) {
    _asm {
      MOV EBX, Key;
      SHL EBX, 1
      MOV EAX, Key;
      MOV EDX, MCoef_32[ 0 ];
      AND EAX, EDX; //select the bit for xor
      MOV EDX, EAX;
      SHR EAX, 16;
      XOR AX, DX;
      XOR AH, AL; // because P only judge one byte
      // so must XOR to judge the p of whole word
      JP NEXT //jp equals the xor
      INC EBX
      NEXT:
      MOV Key, EBX;

    }
  }

  return Key;
}

WORD RNG()
{
  DWORD A, B;
  _asm {
    _emit 0x0f
    _emit 0x31
    MOV A, EAX
    MOV B, EDX
  }

  RandomKey = m_Seq_32(RandomKey ^ A ^ B);
  return (WORD) RandomKey;
}

DWORD RNG32()
{
  DWORD A, B;
  _asm {
    _emit 0x0f
    _emit 0x31
    MOV A, EAX
    MOV B, EDX
  }

  RandomKey32 = m_Seq_32(RandomKey32 ^ A ^ B);
  return RandomKey32;
}

void NormalTable(double* FTable, double* fTable, int Length)
/*
������̬�ֲ���������̬�ֲ������ܶȺ���������Ϊ3������
��̬�ֲ�������ֵΪ0������Ϊ1
FTable: ��̬�ֲ�������
fTable: ��̬�ֲ������ܶȱ�
Length: ��ĳ���
*/

{
  int i;
  double h; /* ���� */
  double x, temp;
  double C;

  x = -3;
  h = 6.0 / Length / 2;
  FTable[ 0 ] = 0;
  C = 1 / sqrt(2 * 3.1415927);
  /* ��ʼ�������� */

  fTable[ 0 ] = exp(-x * x / 2) * C;
  /* ��ʼ��ĸ����ܶ� ��exp(-x*x/2)*CΪ�����ܶȺ��� */

  for (i = 1; i < Length; i++) {
    x += h;
    temp = exp(-x * x / 2) * C;
    x += h;
    fTable[ i ] = exp(-x * x / 2) * C;
    /* ������̬�ֲ������ܶȺ��� */

    FTable[ i ] = FTable[ i - 1 ] + (fTable[ i - 1 ] + 4 * temp + fTable[ i ]) * h / 3;
    /* ����ɭ��ֵ���ֹ�ʽ������̬�ֲ����� */
  }
}

double NormalRNG(double* Gauss_F, int Length)
/*
���ɾ�ֵΪ0,����Ϊ1�ĸ�˹�����
Gauss_F: ��̬�ֲ�������������ϣ�������NormalTable��������
Length: ��̬�ֲ�������ĳ���
����ֵ����ֵΪ0,����Ϊ1�ĸ�˹�ֲ������
*/

{
  double RandomNumber;
  double temp, h;
  int i;

  h = 6.0 / Length;
  /* ��̬�ֲ���Ĳ��� */


  temp = (double) RNG() / 65536;
  /*����һ��[0,1)�����ھ�������� */

  if (temp == 0) {
    return 0;
  }

  for (i = 0; i < Length; i++) {
    if (temp <= Gauss_F[ i ]) {    /*���������������̬�ֲ�����ĸ����� */
      RandomNumber = (-3 + h * i) + (temp - Gauss_F[ i - 1 ]) / (Gauss_F[ i ] - Gauss_F[ i - 1 ]) * h;
      /* �������Բ�ֵ */
      break;
    }
  }

  return RandomNumber;
}

#define ZEROMEMORY(ptr, len) memset(ptr, 0, len)

//#define s  0.1   //���α���ֲڶ�����

typedef struct CSceneRipple {
  short* buf1;    // ���θ߶����ݻ�����
  short* buf2;    // ���θ߶����ݻ�����
  int width;  // ����ͼ���
  int height;  // ����ͼ�߶�
  int rain;         // ����
  double s;         // ���α���ֲڶ�����
}
CSceneRipple;

//Ч��
BOOL rain = FALSE;  //����

//*******************************************************************
//������FreeObject
//���ܣ��ͷ������ù�������DirectDraw����
//*******************************************************************
void CSceneRippleFree(CSceneRipple* sr)
{
  SAFEFREE(sr->buf1);
  SAFEFREE(sr->buf2);
}

///////////////////////////////////////////////////////////////////////////
//����θ߶�
///////////////////////////////////////////////////////////////////////////
void CSceneRippleFracHeight(CSceneRipple* sr, int a, int b, int c, int d)
{
  int width = sr->w;
  double s = sr->s;
  int e, f, g, h, m;

  //�ݹ��������
  //2*2����
  if ((a + 1 == b) && (a + width == d)) {
    return ;
  }

  //�����˻���һ��ֱ��
  if ((a == b) || (a == d)) {
    return ;
  }

  //������е�λ����߶�

  e = (a + b) / 2;
  f = b + ((c - b) / width) / 2 * width;
  g = (c + d) / 2;
  h = a + ((d - a) / width) / 2 * width;
  m = (h + f) / 2;

  sr->buf1[ e ] = (short)((sr->buf1[ a ] + sr->buf1[ b ]) / 2 + s * NormalRNG(Gauss_F, GAUSS_TABLE_LENGTH) * (b - a));
  sr->buf1[ f ] = (short)((sr->buf1[ b ] + sr->buf1[ c ]) / 2 + s * NormalRNG(Gauss_F, GAUSS_TABLE_LENGTH) * (c - b) / width);
  sr->buf1[ g ] = (short)((sr->buf1[ c ] + sr->buf1[ d ]) / 2 + s * NormalRNG(Gauss_F, GAUSS_TABLE_LENGTH) * (c - d));
  sr->buf1[ h ] = (short)((sr->buf1[ d ] + sr->buf1[ a ]) / 2 + s * NormalRNG(Gauss_F, GAUSS_TABLE_LENGTH) * (d - a) / width);
  sr->buf1[ m ] = (short)((sr->buf1[ h ] + sr->buf1[ f ]) / 2 + s * NormalRNG(Gauss_F, GAUSS_TABLE_LENGTH) * (f - h));

  //�ݹ����������ĸ߶�
  CSceneRippleFracHeight(sr, a, e, m, h);
  CSceneRippleFracHeight(sr, e, b, f, m);
  CSceneRippleFracHeight(sr, m, f, c, g);
  CSceneRippleFracHeight(sr, h, m, g, d);
}


//******************************************************************
//������InitSurfaces()
//���ܣ���ʼ��ҳ��ͼ��
//******************************************************************
CSceneRipple* InitSurfaces(int height, int width)
{
  CSceneRipple* sr = MALLOC(CSceneRipple, 1);
  sr->s = 0.1;
  sr->w = width;
  sr->h = height;
  sr->buf1 = MALLOC(short, width * height);
  sr->buf2 = MALLOC(short, width * height);
  ZEROMEMORY(sr->buf1, width * height * sizeof(short));
  ZEROMEMORY(sr->buf2, width * height * sizeof(short));

  sr->buf1[ 0 ] = (short)(rand() / 32767.0 * 128);
  sr->buf1[ width - 1 ] = (short)(rand() / 32767.0 * 128);
  sr->buf1[ width * (height - 1) ] = (short)(rand() / 32767.0 * 128);
  sr->buf1[ width * height - 1 ] = (short)(rand() / 32767.0 * 128);
  CSceneRippleFracHeight(sr, 0, width - 1, width * height - 1, width * (height - 1));

  NormalTable(Gauss_F, Gauss_f, GAUSS_TABLE_LENGTH);
  return sr;
}

//*******************************************************
//������θ߶����ݻ�����
//*******************************************************
void RippleSpread(CSceneRipple* sr)
{
  int i;
  int width = sr->w;
  int height = sr->h;
  short* buf1 = sr->buf1;
  short* buf2 = sr->buf2;

  for (i = width; i < width * height - width; i++) {
    //��ɢ
    buf2[ i ] = ((buf1[ i - 1 ] +
        buf1[ i + 1 ] +
        buf1[ i - width ] +
        buf1[ i + width ])
        >> 1)
        - buf2[ i ];
    //˥��
    buf2[ i ] -= buf2[ i ] >> 6;
  }

  //�������θ߶����ݻ�����
  {
    short* ptmp = buf1;
    buf1 = buf2;
    buf2 = ptmp;
  }
}

//������ˮ��
// pBack����
void DrawRipple(IDC* pDC, CSceneRipple* sr, unsigned char* pBack, int lPitchBack)
{
  int depth;
  int lPitch;
  BYTE* Bitmap1;
  BYTE* Bitmap2;
  /*�����Ǵ������ɺͶ�������*/

  //�������ҳ����Ⱦ
  int xoff, yoff;
  int k;
  int i, j, d;
  int pos1, pos2;
  //ȡ��ҳ������λ��ȣ���ҳ���ڴ�ָ��
  int width = pDC->w;
  int height = pDC->h;
  short* buf1;
  short* buf2;
  buf1 = sr->buf1;
  buf2 = sr->buf2;
  Bitmap1 = pBack;
  Bitmap2 = pDC->data;
  lPitch = pDC->s;
  depth = pDC->c;
  k = width;

  for (i = 1; i < height - 1; i++) {
    k++;

    for (j = 1; j < width - 1; j++) {
      //����ƫ����
      xoff = buf1[ k - 1 ] - buf1[ k + 1 ];
      yoff = buf1[ k - width ] - buf1[ k + width ] ;

      //�ж������Ƿ��ڴ��ڷ�Χ��
      if ((i + yoff) < 0) {
        k++;
        continue;
      }

      if ((i + yoff) > height) {
        k++;
        continue;
      }

      if ((j + xoff) < 0) {
        k++;
        continue;
      }

      if ((j + xoff) > width) {
        k++;
        continue;
      }

      //�����ƫ�����غ�ԭʼ���ص��ڴ��ַƫ����
      pos1 = lPitchBack * (i + yoff) + depth * (j + xoff);
      pos2 = lPitch * i + depth * j;

      //��������
      for (d = 0; d < depth; d++) {
        Bitmap2[ pos2++ ] = Bitmap1[ pos1++ ];
      }

      k++;
    }

    k++;
  }

}

//*****************************************************
//���Ӳ�Դ
//*****************************************************
void DropStone(CSceneRipple* sr, int x,    //x����
    int y,   //y����
    int stonesize,   //��Դ�뾶
    int stoneweight)    //��Դ����
{
  int posx, posy;
  int width = sr->w;
  int height = sr->h;
  short* buf1 = sr->buf1;
  short* buf2 = sr->buf2;

  //�ж������Ƿ�����Ļ��Χ��
  if ((x + stonesize) > width ||
      (y + stonesize) > height ||
      (x - stonesize) < 0 ||
      (y - stonesize) < 0) {
    return ;
  }

  for (posx = x - stonesize; posx < x + stonesize; posx++)
    for (posy = y - stonesize; posy < y + stonesize; posy++)
      if ((posx - x) * (posx - x) + (posy - y) * (posy - y) < stonesize * stonesize) {
        buf1[ width * posy + posx ] = -stoneweight;
      }
}
