
#include <math.h>
#include <stdio.h>
#include "_MILTrack.h"
#include "direct.h"
#include "utime.h"

// ��ӳ���m��f ӳ�䵽 p
#define MAPPING(_N, _A, _B, _M) {int _I=0; for (; _I<_N; ++_I) { _B[_I] = _M[(int)(_A[_I])]; } }

#define HIST(_N, _A, _H) {int _I=0; for (; _I<_N; ++_I) { ++_H[(int)(_A[_I])]; } }

void histeq( int _N, const unsigned char* f, unsigned char* p ) {
  enum { BIT = 8, SCAL = 14, };
  enum { LV = 1 << BIT, };
  const int MN = ( LV << SCAL ) / ( _N );
  int t, i; // ѭ������
  unsigned char bMap[ LV ] = {0}; // �Ҷ�ӳ���
  int nCnt[ LV ] = {0}; // �ۻ�ֱ��ͼ
  HIST( _N, f, nCnt );
  bMap[ 0 ] = ( nCnt[ 0 ] * MN );
  for ( i = 1; i < LV; ++i ) {
    nCnt[ i ] += nCnt[ i - 1 ];
    t = ( nCnt[ i ] * MN ) >> SCAL;
    bMap[ i ] = ( t >= LV ) ? ( LV - 1 ) : ( t ); // �����Ӧ���»Ҷ�ֵ
  }
  MAPPING( _N, f, p, bMap );
  return ;
}

//�������ܣ����ӻҶ�ͼ��ĶԱȶȣ���Ч��matlab�Դ�����imadjust,����Ч��OutImage = imadjust(InImage);
//InImageΪ����Ҷȵ�ͼ��
//OutImageΪ���ӶԱȶȺ�ĻҶ�ͼ��
void my_imadjust( int _N, const unsigned char* f, unsigned char* p ) {
  enum { BIT = 8, SCAL = 14, };
  enum { LV = 1 << BIT, };
  int t, i; // ѭ������
  unsigned char bMap[ LV ] = {0}; // �Ҷ�ӳ���
  int nCnt[ LV ] = {0}; // �ۻ�ֱ��ͼ
  int low_in;
  int high_in;
  int tol_low = ( int ) ( 0.01 * _N );
  int tol_high = ( int ) ( 0.01 * _N );
  HIST( _N, f, nCnt );
  for ( t = 0, low_in = 0; low_in < LV; ++low_in ) {
    t += nCnt[ low_in ];
    if ( t > tol_low ) {
      break;
    }
  }
  for ( t = 0, high_in = LV; high_in-- > 0; ) {
    t += nCnt[ high_in ];
    if ( t > tol_high ) {
      break;
    }
  }
  {
    int len = ( high_in - low_in );
    for ( i = 0; i < LV; ++i ) {
      if ( i < low_in ) {
        bMap[ i ] = 0;
      } else if ( i >= high_in ) {
        bMap[ i ] = LV - 1;
      } else {
        //���ұ����õ��ı���lut = uint8(round(temp2.*255));
        bMap[ i ] = ( LV * ( i - low_in ) ) / len; // �����Ӧ���»Ҷ�ֵ
      }
    }
  }
  //��InImage�е�100��λ�õ�Ԫ��ֵΪa,��OutImage��ͬλ�õ�Ԫ��ֵΪlut(a+1)
  MAPPING( _N, f, p, bMap );
  return ;
}
